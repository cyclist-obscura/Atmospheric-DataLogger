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

 



//////////////////////////////////////
// Required Libraries
//////////////////////////////////////
#include <MinimumSerial.h>
#include <BlockDriver.h>
#include <sdios.h>
#include <SysCall.h>
#include <SdFatConfig.h>
#include <SdFat.h>
#include <FreeStack.h>
#include <SPI.h>
#include <Wire.h>
#include <RTClib.h>              
#include <Adafruit_BME280.h>

 


 
//////////////////////////////////////
// Class Instantiations
//////////////////////////////////////
SdFat SD;                                                                                                                                            
RTC_DS1307 RTC;
Adafruit_BME280 bme;                                                         // The BME280 will be an I2C device

 


 
//////////////////////////////////////
// Definitions and Global Variables
//////////////////////////////////////
float hgInches = 29.921;                                                     // Enter the sealevel barometric pressure here (xx.xx inches Hg)
char TmeStrng[] = "0000/00/00,00:00:00";                                     // 19 ascii characters
#define SEALEVELPRESSURE_HPA (hgInches/0.02952998751)                        // hPa=(inches Hg)/0.02952998751
#define MOSIpin 11
#define MISOpin 12
#define DS1307_I2C_ADDRESS 0x68
#define delay_ms 30000
 



 
//////////////////////////////////////
// SETUP FUNCTION
//////////////////////////////////////
void setup() { 

  Serial.begin(9600);                                                         // Open serial communications and wait for port to open:
  Wire.begin();                                                               // start the i2c interface
  RTC.begin();                                                                // start the RTC

  RTC.adjust(DateTime((__DATE__), (__TIME__)));                               //sets the RTC to the computer time when the sketch is loaded
                                                                              // NB COMMENT OUT AFTER FIRST USE

  Serial.println("Cycling Datalogger - v0.1 - 16/11/2018");

  
  // Test Card is Available
  Serial.print("...Find SD card: ");

  if (!SD.begin(10)) {
    Serial.println("......Card failed");
  }
  else  {
    Serial.println("......SD card OK");  
  }

  // Print a file to the SD Card
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {                                                             // if the file is available, write to it:
    dataFile.println("Nano Logger");
    dataFile.close();
  }
  else {
    Serial.println("......file error");                                       // if the file isn't open, pop up an error:
  }


  // Load BME280 Sensor
  bool status;     
  status = bme.begin(0x76);                                                   // 0x76 is the I2C address of the clone sensors I have
  if (status) {
    dataFile.println("...BME280 Sensor Init");
    Serial.println("...BME280 Sensor Init");
  }
  else  {
    Serial.println("......No BME280");
    while (1);
  }

  Serial.println();

}                                                                             // END OF SETUP FUNCTION

 


 
//////////////////////////////////////
// LOOP FUNCTION
//////////////////////////////////////
void loop() {
  
  // Read Data from BME Sensor - Temp Humidity Pressure Altitude
  float BMEt = (bme.readTemperature());
  float BMEh = (bme.readHumidity());
  float BMEp = (bme.readPressure() / 100.0F);
  float BMEa = (bme.readAltitude(SEALEVELPRESSURE_HPA));
 
  // Read time from realtime clock
  DateTime now = RTC.now();                                              //this reads the time from the RTC

  // Construct time string - format YYYY/MM/DD,HH:MM:SS
  sprintf(TmeStrng, "%04d/%02d/%02d,%02d:%02d:%02d", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());

  // Print data to terminal
  Serial.print("RTC utc Time: ");
  Serial.print(now.unixtime());
  Serial.println();
  Serial.print("RTC time: ");
  Serial.println(TmeStrng);
  Serial.print("BME280 temp: ");
  Serial.print(BMEt);
  Serial.println(" C");
  Serial.print("BME280 Humidity: ");
  Serial.print(BMEh);
  Serial.println(" %");
  Serial.print("Pressure: ");
  Serial.print(BMEp);
  Serial.println(" hPa");
  Serial.print("Elevation: ");
  Serial.print(BMEa);
  Serial.println(" m");
  Serial.println();
         

  //Construct a data string to write to SD card
  String dataString = "";                                           //this erases the previous string
  dataString += TmeStrng;
  dataString += ",";     
  dataString += String(BMEt);
  dataString += ",";  
  dataString += String(BMEh);
  dataString += ",";   
  dataString += String(BMEp);
  dataString += ",";   
  dataString += String(BMEa);
 
  // Write the data to the SD card
  File dataFile = SD.open("datalog.txt", FILE_WRITE);               // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
  }
  else {
    Serial.println("file error");                                   // if the file isn't open, pop up an error:
  }

  // Pause
  delay(delay_ms);                       
  
}                                                                   // END of the MAIN LOOP

 
