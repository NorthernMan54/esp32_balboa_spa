#include "cacheRead.h"
#include <Arduino.h>
#include <CircularBuffer.hpp>
#include <ArduinoLog.h>

#include <utilities.h>
#include <bridge.h>

#include "balboa.h"
#include "spaMessage.h"

// Local Functions

void processFragment(uint8_t *data, size_t length);

// 7e 08 0a bf 22 00 00 01 58 7e 7e 08 0a bf 22 04 00 00 f4 7e 7e 08 0a bf 22 01 00 00 34 7e 7e 08 0a bf 22 02 00 00 89 7e
// config request (2e), followed by 04 (25), Filter Cycles Message (23), and Information Response ( 24 )

void cacheRead(uint8_t *data, size_t length)
{
  size_t pos = 0;
  while (pos < length)
  {
    // Look for the start delimiter (0x7E)
    if (data[pos] == 0x7E)
    {
      // Check if there are enough bytes to determine the length
      if (pos + 1 < length)
      {
        // Get the length of the fragment from the second byte
        uint8_t fragmentLength = data[pos + 1];

        // Ensure there are enough bytes for the complete fragment
        if (pos + fragmentLength + 1 < length)
        {
          // Extract the fragment
          processFragment(&data[pos], fragmentLength + 2);

          // Move to the next fragment
          pos += fragmentLength + 2;
        }
        else
        {
          // Not enough bytes for the complete fragment, break
          break;
        }
      }
    }
    else
    {
      // Skip invalid data
      pos++;
    }
  }
}

#define cacheRecent(structure)                                                                                                                               \
  {                                                                                                                                                          \
    unsigned long currentTime = getTime();                                                                                                                   \
    ((currentTime - (structure).lastUpdate < STALE_TIME * 2) ? bridgeSend((structure).rawData, (structure).rawDataLength) : sendMessageToSpa(data, length)); \
  }

void processFragment(uint8_t *data, size_t length)
{
  // Check if the fragment is a valid message
  if (data[4] == Settings_Request_Type)
  {
    // Check the message type
    switch (data[5])
    {
    case Configuration_Req_Type:
      Log.verbose(F("[Cache]: Configuration Message" CR));
      cacheRecent(spaConfigurationData);
      break;
    case Settings_0x04_Req_Type:
      Log.verbose(F("[Cache]: Settings 0x04 Message" CR));
      cacheRecent(spaSettings0x04Data);
      break;
    case Filter_Cycles_Req_Type:
      Log.verbose(F("[Cache]: Filter Cycles Message" CR));
      cacheRecent(spaFilterSettingsData);
      break;
    case Information_Req_Type:
      Log.verbose(F("[Cache]: Information Request" CR));
      cacheRecent(spaInformationData);
      break;
    default:
      Log.verbose(F("[Cache]: Unknown Request %x" CR), data[5]);
      sendMessageToSpa(data, length);
      break;
    }
  }
  else
  {
    sendMessageToSpa(data, length);
  }
}