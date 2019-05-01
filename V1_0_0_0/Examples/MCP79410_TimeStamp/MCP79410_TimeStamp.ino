/*************************************************************************************************************
#  HISTORY
#  v1.0  -  First Release
#
#  =============================================================================
#  This is an example that shows how to set the alarms 0 and 1 of the MCP79410 device (RTCC - Real Time Clock Calendar)
#  The hardware used is referred to "shield RTCC MCP79410 Rev. 1.1" or higher developed by Futura Elettronica srl
#
#  Developed by Matteo Destro for Futura Group srl
#
#  www.Futurashop.it
#  www.open-electronics.org
#
#  BSD license, all text above must be included in any redistribution
#
#  The code for this demo is splitted into two files:
#
#  - MCP79410_TimeStamp -> Main file project
#  - TimerInt           -> Contains code to manage Timer1 of ATMEGA328P. Timer1 generates an interrupt every 2 mSec.
#                          With this resolution (2 mSec) it is possible to manage the timer variables used in this demo.
#                          For example the TimeOut used to filter the unwanted variations on the digital inputs (debouncing)
#
#  Arduino Uno Rev. 3
#   
#  =============================================================================                    
#  
#  INSTALLATION
#  The 3 library files (MCP79410.cpp, MCP79410.h and keywords.txt) in the MCP79410 folder should be placed in your Arduino Library folder.
#  Run the MCP79410_TimeStamp.ino file from your Arduino IDE.
#
#  SUPPORT
#  
#  info@open-electronics.org
#
*********************************************************************************************************/

#include <Wire.h>
#include "MCP79410.h"

MCP79410 mcp79410 = MCP79410();

#define TRUE  0
#define FALSE 1

#define TIMEKEEPER_CASE  0
#define ALARM_CASE       1

//======================================================================
//  Define timer Constants and Variables
#define	TIMEBASE      2                //  Time Base: 2mSec

#define T_10MSEC      (10/TIMEBASE)    //  Defines a constant for a timeout of 10 mSec
#define T_20MSEC      (20/TIMEBASE)    //  Defines a constant for a timeout of 20 mSec
#define T_50MSEC      (50/TIMEBASE)    //  Defines a constant for a timeout of 50 mSec

#define T_5SEC	      (5000/TIMEBASE)  //  Defines a constant for a timeout of 5,000 Sec
#define T_10SEC	      (10000/TIMEBASE) //  Defines a constant for a timeout of 10,000 Sec

uint16_t TimeOutReadTimeKeeper;  //  TimeOut read TimeKeeper Registers
//======================================================================

//======================================================================
//  String constants
const char Sep1[]                PROGMEM = "#=================================================================";
const char OscillatorEnable[]    PROGMEM = "# Oscillator is enabled and running";
const char OscillatorDisable[]   PROGMEM = "# Oscillator has stopped or has been disabled";
const char TimeReaded[]          PROGMEM = "# Time Readed: ";
const char DateReaded[]          PROGMEM = "# Date Readed: ";

const char TimeStampPowerUp[]    PROGMEM = "# TimeStamp PowerUp: ";
const char TimeStampPowerDown[]  PROGMEM = "# TimeStamp PowerDown: ";


const char Monday[]              PROGMEM = "MON";
const char Tuesday[]             PROGMEM = "TUE";
const char Wednesday[]           PROGMEM = "WED";
const char Thursday[]            PROGMEM = "THU";
const char Friday[]              PROGMEM = "FRI";
const char Saturday[]            PROGMEM = "SAT";
const char Sunday[]              PROGMEM = "SUN";
//======================================================================

//======================================================================
//  Output pin define
uint8_t ForceOn = 8;
uint8_t Led     = 7;         //  Digital Output. Led LD7
//======================================================================

//======================================================================
//  Input pin define
uint8_t P1_Button = 4;         //  Digital input. Button P1. See schematic
uint8_t P2_Button = 5;         //  Digital input. Button P2. See schematic
uint8_t P3_Button = 6;         //  Digital input. Button P3. See schematic
//======================================================================

//======================================================================
//  Sketch Setup
void setup() {
  SetInputPin();
  SetOutputPin(); 
  SetupTimer();
  
  mcp79410.Begin();
  Serial.begin(19200);
  
  TimeOutReadTimeKeeper = T_10SEC;
}
//======================================================================

//======================================================================
void loop() {
  RTCC_Read_Registers();     //  Read Settings of RTCC
}
//======================================================================

//======================================================================
// This function is used to read the string data by flash and print the data read on the screen
// It is useful to save SRAM memory instead that used the "println" function with a string type parameter
// The string type parameters are saved into FLASH memory using the "PROGMEM" function
String ReadStringFLASH(uint8_t *FlashPointer, uint8_t Lenght, boolean PrintCR, boolean NoPrint) {
  uint8_t k;
  char    myChar;
  String  TempString;
  
  for (k = 0; k < Lenght; k++) {
    myChar = pgm_read_byte_near(FlashPointer + k);
    if (NoPrint == FALSE) { 
      Serial.print(myChar);
    }
    TempString += myChar;
  }
  if (NoPrint == FALSE) { 
    if (PrintCR == TRUE) {
      Serial.print("\n");
    }
  } else {
    return(TempString); 
  }
  return("");
}
//======================================================================

//======================================================================
//  Sets digital inputs
void SetInputPin(void) {
  pinMode(P1_Button, INPUT);          // set pin to input
  pinMode(P2_Button, INPUT);          // set pin to input
  pinMode(P3_Button, INPUT);          // set pin to input
  digitalWrite(P1_Button, HIGH);      // turn on pullup resistors
  digitalWrite(P2_Button, HIGH);      // turn on pullup resistors
  digitalWrite(P3_Button, HIGH);      // turn on pullup resistors 
}
//======================================================================

//======================================================================
//  Sets digital Output
void SetOutputPin(void) {
  pinMode(ForceOn, OUTPUT);
  digitalWrite(ForceOn, LOW);     // ForceOn OFF
  
  pinMode(Led, OUTPUT);           // Set pin to Output
  digitalWrite(Led, HIGH);        // Led OFF
}
//======================================================================

//======================================================================
//  This function, every 15 seconds, reads the configuration of TimeKeeper registers and
//  Alarm 0/1 configurations. Then prints on the screen (Using println function) the read data
void RTCC_Read_Registers(void) {
  if (TimeOutReadTimeKeeper == 0) {
    digitalWrite(Led, LOW);        // Led ON
    TimeOutReadTimeKeeper = T_10SEC;
    mcp79410.ReadTimeKeeping();     //  Reads TimeKeeping settings
    mcp79410.ControlReg.ControlByte = mcp79410.ReadSingleReg(RTCC_HW_ADD, CONTROL_ADD);

    if (mcp79410.TimeKeeper.WeekDay.WkDayBit.PwrFail == 1) {
      mcp79410.ReadPowerDownUpRegister(0);
      mcp79410.ReadPowerDownUpRegister(1);
      mcp79410.ResetPwFailBit();        
    }
      
    if (mcp79410.TimeKeeper.WeekDay.WkDayBit.OSCrun != 0) {
      //  Oscillator is enabled and running
      ReadStringFLASH((uint8_t *)Sep1, strlen(Sep1), TRUE, FALSE);
      //---------------------------------------------
      //  Print TimeKeeper
      ReadStringFLASH((uint8_t *)OscillatorEnable, strlen(OscillatorEnable), TRUE, FALSE);
      ReadStringFLASH((uint8_t *)TimeReaded, strlen(TimeReaded), FALSE, FALSE);
      PrintTime();
      ReadStringFLASH((uint8_t *)DateReaded, strlen(DateReaded), FALSE, FALSE);
      PrintDate();
      //---------------------------------------------
     
      //---------------------------------------------
      //  Print TimeStamp       
      ReadStringFLASH((uint8_t *)TimeStampPowerDown, strlen(TimeStampPowerDown), FALSE, FALSE);
      PrintTimeTimeStamp(1);
      PrintDateTimeStamp(1);
      ReadStringFLASH((uint8_t *)TimeStampPowerUp, strlen(TimeStampPowerUp), FALSE, FALSE);
      PrintTimeTimeStamp(0);
      PrintDateTimeStamp(0);
      //---------------------------------------------
      
      ReadStringFLASH((uint8_t *)Sep1, strlen(Sep1), TRUE, FALSE);
      Serial.println("");
    } else {
      //  Oscillator has stopped or has been disabled
      ReadStringFLASH((uint8_t *)Sep1, strlen(Sep1), TRUE, FALSE);
      ReadStringFLASH((uint8_t *)OscillatorDisable, strlen(OscillatorDisable), TRUE, FALSE);
      ReadStringFLASH((uint8_t *)Sep1, strlen(Sep1), TRUE, FALSE);
    }
    digitalWrite(Led, HIGH);        // Led OFF
  }
}
//======================================================================

//======================================================================
//  This function prints the informations read by RTCC device
void PrintTime(void) {
  Serial.print(mcp79410.TimeKeeper.Hour24.Hour_24Bit.HrTen, DEC);
  Serial.print(mcp79410.TimeKeeper.Hour24.Hour_24Bit.HrOne, DEC);
  Serial.print(":");
  Serial.print(mcp79410.TimeKeeper.Minute.MinBit.MinTen, DEC);
  Serial.print(mcp79410.TimeKeeper.Minute.MinBit.MinOne, DEC);
  Serial.print(":");
  Serial.print(mcp79410.TimeKeeper.Second.SecBit.SecTen, DEC);
  Serial.print(mcp79410.TimeKeeper.Second.SecBit.SecOne, DEC);
  if (mcp79410.TimeKeeper.Hour24.Hour_24Bit._12_24 == 0) {
    //  24H format 
    Serial.println("");
  } else {
    //  12H format
    if (mcp79410.TimeKeeper.Hour12.Hour_12Bit.AmPm == 0) {
      //  AM
      Serial.println("AM");            
    } else {
      //  PM
      Serial.println("PM");
    }
  }
}
//======================================================================

//======================================================================
//  This function prints the informations read by RTCC device
void PrintTimeTimeStamp(uint8_t PowerUpDown) {
  Serial.print(mcp79410.Power[PowerUpDown].Hour24.Hour_24Bit.HrTen, DEC);
  Serial.print(mcp79410.Power[PowerUpDown].Hour24.Hour_24Bit.HrOne, DEC);
  Serial.print(":");
  Serial.print(mcp79410.Power[PowerUpDown].Minute.MinBit.MinTen, DEC);
  Serial.print(mcp79410.Power[PowerUpDown].Minute.MinBit.MinOne, DEC);
  Serial.print(":");
  Serial.print("XX");
  if (mcp79410.Power[PowerUpDown].Hour24.Hour_24Bit._12_24 == 0) {
    //  24H format 
    Serial.print(" -> ");
  } else {
    //  12H format
    if (mcp79410.Power[PowerUpDown].Hour12.Hour_12Bit.AmPm == 0) {
      //  AM
      Serial.print("AM -> ");            
    } else {
      //  PM
      Serial.println("PM -> ");
    }
  }
}
//======================================================================

//======================================================================
//  This function prints the informations read by RTCC device
void PrintDate(void) {
  uint8_t TempData;
  
  TempData = mcp79410.TimeKeeper.WeekDay.WkDayBit.WkDay;
  Serial.print(mcp79410.TimeKeeper.Date.DateBit.DateTen, DEC);
  Serial.print(mcp79410.TimeKeeper.Date.DateBit.DateOne, DEC);
  Serial.print("/");
  Serial.print(mcp79410.TimeKeeper.Month.MonthBit.MonthTen, DEC);
  Serial.print(mcp79410.TimeKeeper.Month.MonthBit.MonthOne, DEC);
  Serial.print("/");
  Serial.print("20");
  Serial.print(mcp79410.TimeKeeper.Year.YearBit.YearTen, DEC);
  Serial.print(mcp79410.TimeKeeper.Year.YearBit.YearOne, DEC);      
  Serial.print(" - ");
 
  switch (TempData) {
    case 1:    //  Monday
      ReadStringFLASH((uint8_t *)Monday, strlen(Monday), TRUE, FALSE);
      break; 
    case 2:    //  Tuesday
      ReadStringFLASH((uint8_t *)Tuesday, strlen(Tuesday), TRUE, FALSE);
      break;
    case 3:    //  Wednesday
      ReadStringFLASH((uint8_t *)Wednesday, strlen(Wednesday), TRUE, FALSE);
      break;
    case 4:    //  Thursday
      ReadStringFLASH((uint8_t *)Thursday, strlen(Thursday), TRUE, FALSE);
      break;
    case 5:    //  Friday
      ReadStringFLASH((uint8_t *)Friday, strlen(Friday), TRUE, FALSE);
      break;
    case 6:    //  Saturday
      ReadStringFLASH((uint8_t *)Saturday, strlen(Saturday), TRUE, FALSE);
      break;
    case 7:    //  Sunday
      ReadStringFLASH((uint8_t *)Sunday, strlen(Sunday), TRUE, FALSE);
      break;
    default:
      Serial.println("***");
      break;
  }
}
//======================================================================

//======================================================================
//  This function prints the informations read by RTCC device
void PrintDateTimeStamp(uint8_t PowerUpDown) {
  uint8_t TempData;
  
  TempData = mcp79410.Power[PowerUpDown].Month.MonthBit.WkDay;
  Serial.print(mcp79410.Power[PowerUpDown].Date.DateBit.DateTen, DEC);
  Serial.print(mcp79410.Power[PowerUpDown].Date.DateBit.DateOne, DEC);
  Serial.print("/");
  Serial.print(mcp79410.Power[PowerUpDown].Month.MonthBit.MonthTen, DEC);
  Serial.print(mcp79410.Power[PowerUpDown].Month.MonthBit.MonthOne, DEC);
  Serial.print("/");
  Serial.print("YYYY");   
  Serial.print(" - ");
 
  switch (TempData) {
    case 1:    //  Monday
      ReadStringFLASH((uint8_t *)Monday, strlen(Monday), TRUE, FALSE);
      break; 
    case 2:    //  Tuesday
      ReadStringFLASH((uint8_t *)Tuesday, strlen(Tuesday), TRUE, FALSE);
      break;
    case 3:    //  Wednesday
      ReadStringFLASH((uint8_t *)Wednesday, strlen(Wednesday), TRUE, FALSE);
      break;
    case 4:    //  Thursday
      ReadStringFLASH((uint8_t *)Thursday, strlen(Thursday), TRUE, FALSE);
      break;
    case 5:    //  Friday
      ReadStringFLASH((uint8_t *)Friday, strlen(Friday), TRUE, FALSE);
      break;
    case 6:    //  Saturday
      ReadStringFLASH((uint8_t *)Saturday, strlen(Saturday), TRUE, FALSE);
      break;
    case 7:    //  Sunday
      ReadStringFLASH((uint8_t *)Sunday, strlen(Sunday), TRUE, FALSE);
      break;
    default:
      Serial.println("***");
      break;
  }
}
//======================================================================
