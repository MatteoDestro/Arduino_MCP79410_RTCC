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
#  The code for this demo is splitted into three files:
#
#  - MCP79410_SetAlarm -> Main file project
#  - RTCC_Settings     -> Contains code to manage RTCC settings (Alarms 0 and 1)
#  - TimerInt          -> Contains code to manage Timer1 of ATMEGA328P. Timer1 generates an interrupt every 2 mSec.
#                         With this resolution (2 mSec) it is possible to manage the timer variables used in this demo.
#                         For example the TimeOut used to filter the unwanted variations on the digital inputs (debouncing)
#
#  Arduino Uno Rev. 3
#   
#  =============================================================================                    
#  
#  INSTALLATION
#  The 3 library files (MCP79410.cpp, MCP79410.h and keywords.txt) in the MCP79410 folder should be placed in your Arduino Library folder.
#  Run the MCP79410_SetAlarm.ino file from your Arduino IDE.
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

const char Alarm0_Conf[]         PROGMEM = "# Alarm 0 is            -> ";
const char Alarm1_Conf[]         PROGMEM = "# Alarm 1 is            -> ";
const char AlarmOutputPol[]      PROGMEM = "# Alarm Output Polarity -> ";
const char AlarmIntFlag[]        PROGMEM = "# Alarm Interrupt Flag  -> ";
const char AlarmTimeConf[]       PROGMEM = "# Alarm time configured -> ";
const char AlarmDateConf[]       PROGMEM = "# Date time configured  -> ";
const char AlarmxMask[]          PROGMEM = "# Alarm Mask Set        -> ";

const char Enable[]              PROGMEM = "Enabled";
const char Disable[]             PROGMEM = "Disabled";

const char AlarmxMaskError[]     PROGMEM = "# Alarm Mask config Error";

const char MFP_LHL[]             PROGMEM = "MFP is a logic high level";
const char MFP_LLL[]             PROGMEM = "MFP is a logic low level";

const char SecondsMatch[]        PROGMEM = "SecondsMatch";
const char MinutesMatch[]        PROGMEM = "MinutesMatch";
const char HoursMatch[]          PROGMEM = "HoursMatch";
const char DayOfWeekMatch[]      PROGMEM = "DayOfWeekMatch";
const char DateMatch[]           PROGMEM = "DateMatch";
const char AllMatch[]            PROGMEM = "AllMatch";

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
  
  RTCC_TimeKeeperSettings();
  RTCC_AlarmSettings();
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

