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

#ifndef ANALYTICS_H
#define ANALYTICS_H

#include "Arduino.h"

#define ANALYTICS_VERSION "0.1.0"
#define ANALYTICS_MAGIC_NUMBER 0x123456789  // Magic number to check if data is initialized

typedef struct
{
  unsigned long long int initialized;
  int previousHour;
  unsigned long previousReading;
  unsigned long onTimeToday;
  unsigned long onTimeYesterday;
} AnalyticsData;

class Analytics
{

public:
  /** create a Analytics object
   *
   *
   */
  Analytics(AnalyticsData *data);

  /** destructor for the Analytics object
   *
   */
  ~Analytics();

  /** start the Analytics
   *
   */
  void on();

  /** resume the Analytics. If not started, it will start it.
   *
   */
  void off();

  /*

  */
  unsigned long today();

  unsigned long yesterday();

private:
  AnalyticsData *analyticsInstance;
  void rollover();
  void reset();
  int getHour();
};

#endif