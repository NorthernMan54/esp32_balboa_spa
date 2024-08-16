/* Analytics library code is placed under the MIT license
 * Copyright (c) 2018 Stefan Staub
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "Analytics.h"

// Store data in ESP32 RTC memory, it will survive restarts but not power cycles
// RTC_NOINIT_ATTR AnalyticsData data;

Analytics::Analytics(AnalyticsData *data)
{
  analyticsInstance = data;
  esp_reset_reason_t reason = esp_reset_reason();

  if (reason == ESP_RST_POWERON)
  {
    analyticsInstance->onTimeToday = 0;
    analyticsInstance->onTimeYesterday = 0;
    analyticsInstance->previousReading = millis();
  }
  analyticsInstance->previousHour = getHour();
}

Analytics::~Analytics() {}

void Analytics::on()
{
  rollover();
  analyticsInstance->onTimeToday += millis() - analyticsInstance->previousReading;
  analyticsInstance->previousReading = millis();
}

void Analytics::off()
{
  rollover();
  analyticsInstance->previousReading = millis();
}

unsigned long Analytics::today()
{
  return analyticsInstance->onTimeToday / 1000;
}

unsigned long Analytics::yesterday()
{
  return analyticsInstance->onTimeYesterday / 1000;
}

int Analytics::getHour()
{
  time_t now;
  struct tm *now_tm;
  int hour;

  now = time(NULL);
  now_tm = localtime(&now);
  hour = now_tm->tm_hour;

  return hour;
}

void Analytics::rollover()
{
  if (getHour() < analyticsInstance->previousHour)
  {
    analyticsInstance->previousHour = getHour();
    analyticsInstance->onTimeYesterday = analyticsInstance->onTimeToday;
    analyticsInstance->onTimeToday = 0;
  }
}
