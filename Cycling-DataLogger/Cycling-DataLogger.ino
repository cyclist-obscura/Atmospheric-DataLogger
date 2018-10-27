/*   Cycling Data Logger
 *   Copyright (C) 2018  Cyclist Obscura
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 *    Version 0.1 - Basic setup to record Text to an SD Card
 */


// Require Libraries

#include <SPI.h>
#include <SD.h>



long record = 0;



// Setup the arduino board
void setup() {
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  printHeaderSerial();
  
}

// Repeating function
void loop() {
  // Variables

  float temp = 35.0;
  float pressure = 1012.0;
  
  Serial.print("Data Record: ");
  Serial.println(record);

  // Atmospheric Sensor
  Serial.println("...Atmospheric Sensor");
  Serial.print("......Temperature: ");
  Serial.println(temp, 1);
  Serial.print("......Pressure: ");
  Serial.println(pressure, 0);

  
  // Increment Record Number and Wait
  record++;
  delay(5000);
}



// Print Header Data to Serial
void printHeaderSerial()  {
  Serial.println("Cycling Data Logger");
  Serial.println("v1.0 27/10/18");
  Serial.println("");
}
