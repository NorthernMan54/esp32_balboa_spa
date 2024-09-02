#include "spaWebServer.h"

#include <ESPAsyncWebServer.h>
#include <ArduinoLog.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <base64.hpp>
#include "FS.h"
#include <LittleFS.h>
#define FORMAT_LITTLEFS_IF_FAILED true

// Internal libraries

#include <tinyxml2.h>
#include <spaMessage.h>
#include <utilities.h>
#include <restartReason.h>

// Local functions

void handleConfig(AsyncWebServerRequest *request);
void handleStatus(AsyncWebServerRequest *request);
void handleState(AsyncWebServerRequest *request);
void handleSlash(AsyncWebServerRequest *request);
void handleNotFound(AsyncWebServerRequest *request);
void handleBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
void handleData(AsyncWebServerRequest *request);
void handleLoginData(AsyncWebServerRequest *request);
void handleOptionsData(AsyncWebServerRequest *request);
void handleOptionsLoginData(AsyncWebServerRequest *request);
String parseBody(String body);
void listDir(fs::FS &fs, const char *dirname, uint8_t levels);

AsyncWebServer server(80);
bool serverSetup = false;

void spaWebServerSetup()
{
  if (!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED))
  {
    Log.error("[Web]: Error LittleFS Mount Failed");
  }
  else
  {
    Log.notice("[Web]: LittleFS Mounted" CR);
    listDir(LittleFS, "/", 3);
  }
  // put your setup code here, to run once:
  Log.verbose(F("[Web]: spaWebServerSetup()" CR));
}

void spaWebServerLoop()
{
  if (!serverSetup)
  {
    server.on("/", HTTP_GET, handleState);
    server.on("/state", HTTP_GET, handleState);
    server.on("/config", HTTP_GET, handleConfig);
    server.on("/status", HTTP_GET, handleStatus);

    // Balboa cloud emulation

    server.on("/devices/sci", HTTP_OPTIONS, handleOptionsData);
    server.on("/devices/sci", HTTP_POST, handleData, NULL, handleBody);

    server.on("/users/login", HTTP_OPTIONS, handleOptionsLoginData);
    server.on("/users/login", HTTP_POST, handleLoginData, NULL, handleBody);

    server.onNotFound(handleNotFound);

    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type, Authorization, X-Requested-With");

    server.begin();
    serverSetup = true;
    Log.notice(F("[Web]: Web server started at http://%p/" CR), WiFi.localIP());
  }
}

#define style String("<style>body{font-family:Arial,Helvetica,sans-serif;}h1{color:blue;}ul{list-style-type:none;}li{padding:5px;}button{  border: none;  color: white; padding: 15px 32px;  text-align: center;  text-decoration: none;  display: inline-block;  font-size: 16px;  margin: 4px 2px;  cursor: pointer;background-color: #04AA6D;} .active, .btn:hover { background-color: #666;  color: white;}</style>")

#define head String("<head><title>Spa Web Server State</title>") + style + String("</head>")

#define webMenuStatus String("<form><button class='active' formaction='/status'>SPA Status</button><button formaction='/config'>SPA Config</button><button formaction='/state'>ESP State</button><button formaction='/index.html'>SPA Website</button></form>")

#define webMenuConfig String("<form><button formaction='/status'>SPA Status</button><button class='active' formaction='/config'>SPA Config</button><button formaction='/state'>ESP State</button><button formaction='/index.html'>SPA Website</button></form>")

#define webMenuState String("<form><button formaction='/status'>SPA Status</button><button formaction='/config'>SPA Config</button><button class='active' formaction='/state'>ESP State</button><button formaction='/index.html'>SPA Website</button></form>")

void handleStatus(AsyncWebServerRequest *request)
{
  Log.verbose("[Web]: Request %s received from %p" CR, request->url().c_str(), request->client()->remoteIP());
  String html = "<html>" + head + "<body>" + webMenuStatus + "<h1>Spa Status</h1><ul>";
  html += "<li><b>lastUpdate:</b> " + formatNumberWithCommas(spaStatusData.lastUpdate) + "</li>";
  html += "<li><b>magicNumber: </b>" + String(spaStatusData.magicNumber) + "</li>";
  html += "<br><li><b>Free Heap: </b>" + formatNumberWithCommas(ESP.getFreeHeap()) + "</li>";
  html += "<li><b>Free Stack: </b>" + formatNumberWithCommas(uxTaskGetStackHighWaterMark(NULL)) + "</li>";

  html += "<br><li><b>Current Temp: </b>" + String(spaStatusData.currentTemp) + "°C</li>";
  html += "<li><b>Set Temp: </b>" + String(spaStatusData.setTemp) + "°C</li>";
  html += "<li><b>Heating Mode: </b>" + String(spaStatusData.heatingMode) + "</li>";
  html += "<li><b>Heating State: </b>" + String(spaStatusData.heatingState) + "</li>";
  html += "<li><b>Needs Heat: </b>" + String(spaStatusData.needsHeat) + "</li>";
  html += "<li><b>Temp Range: </b>" + String(spaStatusData.tempRange) + "</li>";
  html += "<li><b>Temp Scale: </b>" + String(spaStatusData.tempScale) + "</li>";
  html += "<li><b>Time: </b>" + String(spaStatusData.time) + "</li>";
  html += "<li><b>Clock Mode: </b>" + String(spaStatusData.clockMode) + "</li>";
  html += "<li><b>Filter Mode: </b>" + String(spaStatusData.filterMode) + "</li>";
  html += "<li><b>Pump 1: </b>" + String(spaStatusData.pump1) + "</li>";
  html += "<li><b>Pump 2: </b>" + String(spaStatusData.pump2) + "</li>";
  html += "<li><b>Pump 3: </b>" + String(spaStatusData.pump3) + "</li>";
  html += "<li><b>Pump 4: </b>" + String(spaStatusData.pump4) + "</li>";
  html += "<li><b>Pump 5: </b>" + String(spaStatusData.pump5) + "</li>";
  html += "<li><b>Pump 6: </b>" + String(spaStatusData.pump6) + "</li>";
  html += "<li><b>Circulation Pump: </b>" + String(spaStatusData.circ) + "</li>";
  html += "<li><b>Blower: </b>" + String(spaStatusData.blower) + "</li>";
  html += "<li><b>Light 1: </b>" + String(spaStatusData.light1) + "</li>";
  html += "<li><b>Light 2: </b>" + String(spaStatusData.light2) + "</li>";
  html += "<li><b>Mister: </b>" + String(spaStatusData.mister) + "</li>";
  html += "<li><b>Panel Locked: </b>" + String(spaStatusData.panelLocked) + "</li>";
  html += "<li><b>Settings Lock: </b>" + String(spaStatusData.settingsLock) + "</li>";
  html += "<li><b>M8 Cycle Time: </b>" + String(spaStatusData.m8CycleTime) + "</li>";
  html += "<li><b>Notification: </b>" + String(spaStatusData.notification) + "</li>";
  html += "<li><b>Flags 19: </b>" + String(spaStatusData.flags19) + "</li>";
  html += "</ul></body></html>";
  // Add more fields as needed
  request->send(200, "text/html", html);
  Log.verbose(F("[Web]: Response sent %s" CR), html.c_str());
}

void handleConfig(AsyncWebServerRequest *request)
{
  // Log.verbose("[Web]: Request %s received from %p" CR, request->url().c_str(), request->client()->remoteIP());

  String html = "<html>" + head + "<body>" + webMenuConfig + "<h1>Spa Configuration</h1><ul>";
  if (spaConfigurationData.lastUpdate == 0)
  {
    html += "<li><b>Spa Configuration not available</b></li>";
  }
  else
  {
    html += "<li><b>lastUpdate: </b>" + formatNumberWithCommas(spaConfigurationData.lastUpdate) + "</li>";
    html += "<li><b>magicNumber: </b>" + String(spaConfigurationData.magicNumber) + "</li>";
    html += "<li><b>Pump 1: </b>" + String(spaConfigurationData.pump1) + "</li>";
    html += "<li><b>Pump 2: </b>" + String(spaConfigurationData.pump2) + "</li>";
    html += "<li><b>Pump 3: </b>" + String(spaConfigurationData.pump3) + "</li>";
    html += "<li><b>Pump 4: </b>" + String(spaConfigurationData.pump4) + "</li>";
    html += "<li><b>Pump 5: </b>" + String(spaConfigurationData.pump5) + "</li>";
    html += "<li><b>Pump 6: </b>" + String(spaConfigurationData.pump6) + "</li>";
    html += "<li><b>Light 1: </b>" + String(spaConfigurationData.light1) + "</li>";
    html += "<li><b>Light 2: </b>" + String(spaConfigurationData.light2) + "</li>";
    html += "<li><b>Blower: </b>" + String(spaConfigurationData.blower) + "</li>";
    html += "<li><b>Circulation Pump: </b>" + String(spaConfigurationData.circulationPump) + "</li>";
    html += "<li><b>Aux 1: </b>" + String(spaConfigurationData.aux1) + "</li>";
    html += "<li><b>Aux 2: </b>" + String(spaConfigurationData.aux2) + "</li>";
    html += "<li><b>Mister: </b>" + String(spaConfigurationData.mister) + "</li>";
    html += "<li><b>temp_scale: </b>" + String(spaConfigurationData.temp_scale) + "</li>";

    // Add more fields as needed
    html += "</ul></body></html>";
  }
  request->send(200, "text/html", html);
  // Log.verbose(F("[Web]: Response sent %s" CR), html.c_str());
  Log.verbose("[Web]: handleConfig %p %s %s" CR, request->client()->remoteIP(), request->methodToString(), request->url().c_str());
}

void handleState(AsyncWebServerRequest *request)
{
  // Log.verbose(F("[Web]: handleStatus()" CR));
  String html = "<html>" + head + "<body>" + webMenuState + "<h1>ESP State</h1><ul>";
  html += "<li><b>Free Heap: </b>" + formatNumberWithCommas(ESP.getFreeHeap()) + "</li>";
  html += "<li><b>Free Stack: </b>" + formatNumberWithCommas(uxTaskGetStackHighWaterMark(NULL)) + "</li>";
  html += "<li><b>Uptime: </b>" + formatNumberWithCommas(millis() / 1000) + "</li>";
  html += "<li><b>Time: </b>" + formatNumberWithCommas(getTime()) + "</li>";
  html += "<li><b>Refresh Time: </b>" + formatNumberWithCommas(getTime() + 60 * 60) + "</li>";
  html += "<li><b>Restart Reason: </b>" + getLastRestartReason() + "</li>";
  String release = String(__DATE__) + " - " + String(__TIME__);
  html += "<li><b>Release: </b>" + release + "</li>";

  html += "</ul><h1>Spa Status</h1><ul>";
  html += "<li><b>lastUpdate: </b>" + formatNumberWithCommas(spaStatusData.lastUpdate) + "</li>";
  html += "<li><b>magicNumber: </b>" + String(spaStatusData.magicNumber) + "</li>";

  html += "</ul><h1>Configuration Status</h1><ul>";
  html += "<li><b>lastUpdate: </b>" + formatNumberWithCommas(spaConfigurationData.lastUpdate) + "</li>";
  html += "<li><b>lastRequest: </b>" + formatNumberWithCommas(spaConfigurationData.lastRequest) + "</li>";
  html += "<li><b>magicNumber: </b>" + String(spaConfigurationData.magicNumber) + "</li>";
  html += "<li><b>staleData: </b>" + String(staleData(spaConfigurationData)) + "</li>";
  html += "<li><b>retryRequest: </b>" + String(retryRequest(spaConfigurationData)) + "</li>";

  html += "</ul><h1>Preferences Status</h1><ul>";
  html += "<li><b>lastUpdate: </b>" + formatNumberWithCommas(spaPreferencesData.lastUpdate) + "</li>";
  html += "<li><b>lastRequest: </b>" + formatNumberWithCommas(spaPreferencesData.lastRequest) + "</li>";
  html += "<li><b>magicNumber: </b>" + String(spaPreferencesData.magicNumber) + "</li>";
  html += "<li><b>staleData: </b>" + String(staleData(spaPreferencesData)) + "</li>";
  html += "<li><b>retryRequest: </b>" + String(retryRequest(spaPreferencesData)) + "</li>";

  html += "</ul><h1>Filters Status</h1><ul>";
  html += "<li><b>lastUpdate: </b>" + formatNumberWithCommas(spaFilterSettingsData.lastUpdate) + "</li>";
  html += "<li><b>lastRequest: </b>" + formatNumberWithCommas(spaFilterSettingsData.lastRequest) + "</li>";
  html += "<li><b>magicNumber: </b>" + String(spaFilterSettingsData.magicNumber) + "</li>";
  html += "<li><b>staleData: </b>" + String(staleData(spaFilterSettingsData)) + "</li>";
  html += "<li><b>retryRequest: </b>" + String(retryRequest(spaFilterSettingsData)) + "</li>";

  html += "</ul><h1>Information Status</h1><ul>";
  html += "<li><b>lastUpdate: </b>" + formatNumberWithCommas(spaInformationData.lastUpdate) + "</li>";
  html += "<li><b>lastRequest: </b>" + formatNumberWithCommas(spaInformationData.lastRequest) + "</li>";
  html += "<li><b>magicNumber: </b>" + String(spaInformationData.magicNumber) + "</li>";
  html += "<li><b>staleData: </b>" + String(staleData(spaInformationData)) + "</li>";
  html += "<li><b>retryRequest: </b>" + String(retryRequest(spaInformationData)) + "</li>";

  html += "</ul><h1>Fault Status</h1><ul>";
  html += "<li><b>lastUpdate: </b>" + formatNumberWithCommas(spaFaultLogData.lastUpdate) + "</li>";
  html += "<li><b>lastRequest: </b>" + formatNumberWithCommas(spaFaultLogData.lastRequest) + "</li>";
  html += "<li><b>magicNumber: </b>" + String(spaFaultLogData.magicNumber) + "</li>";
  html += "<li><b>staleData: </b>" + String(staleData(spaFaultLogData)) + "</li>";
  html += "<li><b>retryRequest: </b>" + String(retryRequest(spaFaultLogData)) + "</li>";

  html += "</ul><h1>spaSettings0x04Data Status</h1><ul>";
  html += "<li><b>lastUpdate: </b>" + formatNumberWithCommas(spaSettings0x04Data.lastUpdate) + "</li>";
  html += "<li><b>lastRequest: </b>" + formatNumberWithCommas(spaSettings0x04Data.lastRequest) + "</li>";
  html += "<li><b>magicNumber: </b>" + String(spaSettings0x04Data.magicNumber) + "</li>";
  html += "<li><b>staleData: </b>" + String(staleData(spaSettings0x04Data)) + "</li>";
  html += "<li><b>retryRequest: </b>" + String(retryRequest(spaSettings0x04Data)) + "</li>";

  html += "</ul></body></html>";

  request->send(200, "text/html", html);
  Log.verbose("[Web]: handleStatus %p %s %s" CR, request->client()->remoteIP(), request->methodToString(), request->url().c_str());

  // Log.verbose(F("[Web]: Response sent %s" CR), html.c_str());
}

/*

This is the balboa cloud emulation

*/

String encodeResponse(uint8_t rawData[BALBOA_MESSAGE_SIZE], uint8_t length)
{
  if (length)
  {
    unsigned char message[BALBOA_MESSAGE_SIZE];
    for (int i = 0; i < length - 2 && i < BALBOA_MESSAGE_SIZE; i++)
    {
      message[i] = (char)rawData[i + 1];
    }
    message[length - 2] = '\0';

    //  Log.verbose("Encode: %s\n", message);
    // Base64 encode the string
    unsigned char encoded[BALBOA_MESSAGE_SIZE * 2 + 1];
    int encodedLength = encode_base64(message, length - 2, encoded);
    encoded[encodedLength] = '\0';
    char encodedString[BALBOA_MESSAGE_SIZE * 2];
    strncpy((char *)encodedString, (char *)encoded, encodedLength);
    encodedString[encodedLength] = '\0';
    //  Log.verbose("Encoded: %s\n", encodedString);
    return String(encodedString);
  }
  else
  {
    return "";
  }
}

void handleBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
  // Log.verbose("[Web]: handleBody Request %s %s %d received from %p" CR, request->methodToString(), request->url().c_str(), index, request->client()->remoteIP());

  if (index == 0)
  {
    // Allocate memory to store the body data
    request->_tempObject = malloc(total + 1);

    // Copy the data to the allocated buffer
    memcpy((char *)request->_tempObject + index, data, len);
    if (index + len == total)
    {
      // Null-terminate the buffer
      ((char *)request->_tempObject)[total] = '\0';
    }
  }
}

// <sci_request version="1.0"><file_system><targets><device id="00 11 22 33 44 55 66 77"/></targets><commands><get_file path="PanelUpdate.txt"/></commands></file_system></sci_request>
// <sci_request version="1.0"><file_system><targets><device id="00 11 22 33 44 55 66 77"/></targets><commands><get_file path="SystemInformation.txt"/></commands></file_system></sci_request>
// <sci_request version="1.0"><file_system cache="false"><targets><device id="00 11 22 33 44 55 66 77" /></targets><commands><get_file path="SetupParameters.txt" /></commands></file_system></sci_request>

// <sci_request version="1.0"><data_service><targets><device id="00 11 22 33 44 55 66 77"/></targets><requests><device_request target_name="Request">Filters</device_request></requests></data_service></sci_request>

// <sci_request version="1.0"><data_service><targets><device id="00 11 22 33 44 55 66 77"/></targets><requests><device_request target_name="TempUnits">F</device_request></requests></data_service></sci_request>

String parseBody(String body)
{
  String response = "";
  if (body.indexOf("PanelUpdate.txt") > 0)
  {
    response = encodeResponse(spaStatusData.rawData, spaStatusData.rawDataLength);
  }
  else if (body.indexOf("DeviceConfiguration.txt") > 0)
  {
    response = encodeResponse(spaConfigurationData.rawData, spaConfigurationData.rawDataLength);
  }
  else if (body.indexOf("SetupParameters.txt") > 0)
  {
    response = encodeResponse(spaPreferencesData.rawData, spaPreferencesData.rawDataLength);
  }
  else if (body.indexOf("SystemInformation.txt") > 0)
  {
    response = encodeResponse(spaInformationData.rawData, spaInformationData.rawDataLength);
  }
  else if (body.indexOf("Filters") > 0)
  {
    //<device_request target_name="Filters">${encodedValue}</device_request>
    response = "<device_request target_name='Filters'>" + encodeResponse(spaFilterSettingsData.rawData, spaFilterSettingsData.rawDataLength) + "</device_request>";
  }
  else if (body.indexOf("device_request") > 0)
  {
    using namespace tinyxml2;
    XMLDocument xmlDocument;
    xmlDocument.Parse(body.c_str());
    tinyxml2::XMLElement *deviceRequestElement = xmlDocument.FirstChildElement("sci_request")
                                                     ->FirstChildElement("data_service")
                                                     ->FirstChildElement("requests")
                                                     ->FirstChildElement("device_request");

    const char *targetName = deviceRequestElement->Attribute("target_name");

    // Get the value inside the <device_request> element
    const char *deviceRequestValue = deviceRequestElement->GetText();

    Log.verbose("[Web]: Button requested %s %s" CR, targetName, deviceRequestValue);
    // response = encodeResponse(spaFilterSettingsData.rawData, spaFilterSettingsData.rawDataLength);
  }
  else
  {
    // Log.verbose("[Web]: Error Unknown object requested %s" CR, body.c_str());
  }
  return response;
}

void handleData(AsyncWebServerRequest *request)
{
  // Log.verbose("[Web]: handleData Request %s %s received from %p" CR, request->methodToString(), request->url().c_str(), request->client()->remoteIP());

  if (request->_tempObject != nullptr)
  {
    // Log.verbose("[Web]: handleData _tempObject %s" CR, request->_tempObject);
    String body = String((char *)request->_tempObject);
    // Log.verbose("[Web]: handleData 1" CR);
    free(request->_tempObject);
    // Log.verbose("[Web]: handleData 2" CR);
    request->_tempObject = nullptr;
    // Log.verbose("[Web]: handleData body %s" CR, body.c_str());

    String response = parseBody(body);
    if (response.length() == 0)
    {
      Log.verbose("[Web]: ERROR: handleData no response for %s" CR, body.c_str());
      request->send(404, "text/plain", "Not found");
      return;
    }
    // Log.verbose("[Web]: handleData response %s" CR, response.c_str());
    Log.verbose("[Web]: handleData %p %s %s %s" CR, request->client()->remoteIP(), request->methodToString(), request->url().c_str(), response.c_str());
    request->send(200, "text/xml", "<response><data>" + response + "</data></response>");
  }
  else
  {
    Log.verbose("[Web]: handleData no body" CR);
    request->send(200, "text/xml", "<noresponse></noresponse>");
  }
}

void handleLoginData(AsyncWebServerRequest *request)
{
  // Log.verbose("[Web]: handleData Request %s %s received from %p" CR, request->methodToString(), request->url().c_str(), request->client()->remoteIP());

  if (request->_tempObject != nullptr)
  {
    // Log.verbose("[Web]: handleData _tempObject %s" CR, request->_tempObject);
    String body = String((char *)request->_tempObject);
    // Log.verbose("[Web]: handleData 1" CR);
    free(request->_tempObject);
    // Log.verbose("[Web]: handleData 2" CR);
    request->_tempObject = nullptr;
    // Log.verbose("[Web]: handleData body %s" CR, body.c_str());

    // data.device.device_id
    // data.token

    AsyncResponseStream *response = request->beginResponseStream("application/json");
    DynamicJsonDocument doc(128);

    doc["username"] = WiFi.getHostname();
    doc["token"] = WiFi.macAddress();
    doc["device"]["device_id"] = WiFi.macAddress();

    serializeJsonPretty(doc, *response);
    request->send(response);

    Log.verbose("[Web]: handleData %p %s %s %s" CR, request->client()->remoteIP(), request->methodToString(), request->url().c_str(), response);
  }
  else
  {
    Log.verbose("[Web]: handleData no body" CR);
    request->send(200, "text/xml", "<noresponse></noresponse>");
  }
}

void handleOptionsData(AsyncWebServerRequest *request)
{
  Log.verbose("[Web]: handleOptionsData %p %s %s" CR, request->client()->remoteIP(), request->methodToString(), request->url().c_str());
  request->send(200, "text/plain", "Data received");
}

void handleOptionsLoginData(AsyncWebServerRequest *request)
{
  Log.verbose("[Web]: handleOptionsLoginData %p %s %s" CR, request->client()->remoteIP(), request->methodToString(), request->url().c_str());
  request->send(200, "text/plain", "Data received");
}

void handleSlash(AsyncWebServerRequest *request)
{
  Log.verbose("[Web]: handleSlash %p %s %s" CR, request->client()->remoteIP(), request->methodToString(), request->url().c_str());
  AsyncWebServerResponse *response = request->beginResponse(302); // Sends 302 Weiterleitung
  response->addHeader("Location", "index.html");
  request->send(response);
}

void handleNotFound(AsyncWebServerRequest *request)
{
  if (LittleFS.exists(request->url()))
  {
    Log.verbose("[Web]: LFS %p %s %s" CR, request->client()->remoteIP(), request->methodToString(), request->url().c_str());
    request->send(LittleFS, request->url(), String(), false);
    return;
  }

  Log.verbose(F("[Web]: handleNotFound() %s %s" CR), request->methodToString(), request->url().c_str());
  int headers = request->headers();
  int i;
  for (i = 0; i < headers; i++)
  {
    AsyncWebHeader *h = request->getHeader(i);
    Log.verbose("HEADER[%s]: %s\n", h->name().c_str(), h->value().c_str());
  }

  int args = request->args();
  for (int i = 0; i < args; i++)
  {
    Log.verbose(F("ARG[%s]: %s" CR), request->argName(i).c_str(), request->arg(i).c_str());
  }

  request->send(404, "text/plain", "Not found");
}

void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
  File root = fs.open(dirname);
  if (!root)
  {
    Serial.println("- failed to open directory");
    return;
  }
  if (!root.isDirectory())
  {
    Serial.println(" - not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file)
  {
    if (file.isDirectory())
    {
      if (levels)
      {
        listDir(fs, file.path(), levels - 1);
      }
    }
    else
    {
      Serial.print("  FILE: ");
      Serial.print(dirname);
      if (strcmp(dirname, "/") != 0)
      {
        Serial.print("/");
      }
      Serial.print(file.name());
      Serial.print("\tSIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}
