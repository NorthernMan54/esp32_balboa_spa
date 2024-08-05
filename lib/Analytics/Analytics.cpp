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

Analytics::Analytics()
{
  previousHour = getHour();
}

Analytics::~Analytics() {}

void Analytics::on()
{
  rollover();
  onTimeToday += millis() - previousReading;
  previousReading = millis();
}

void Analytics::off()
{
  rollover();
  previousReading = millis();
}

unsigned long Analytics::today()
{
  return onTimeToday / 1000;
}

unsigned long Analytics::yesterday()
{
  return onTimeYesterday / 1000;
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

void Analytics::rollover() {
  if (getHour() != previousHour) {
    previousHour = getHour();
    onTimeYesterday = onTimeToday;
    onTimeToday = 0;
  }
}

