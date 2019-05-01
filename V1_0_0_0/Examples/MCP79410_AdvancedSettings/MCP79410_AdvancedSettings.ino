/*************************************************************************************************************
#  HISTORY
#  v1.0  -  First Release
#
#  =============================================================================
#  This is an example that shows how to set TimeKeeper and the alarms 0 and 1 of the MCP79410 device (RTCC - Real Time Clock Calendar).
#  We implemented a system to set TimeKeepr and Alarms using the "Serial Monitor" of the IDE Arduino.
#  In the RTCC_Setting file there is the code to decode the user commands sent via serial monitor in string format.
#  At head of file there is a list of command supported
#  The hardware used is referred to "shield RTCC MCP79410 Rev. 1.1" or higher developed by Futura Elettronica srl
#
#  Developed by Matteo Destro for Futura Group srl
#
#  www.Futurashop.it
#  www.open-electronics.org
#
#  BSD license, all text above must be included in any redistribution
#
#  The code for this demo is splitted into six files:
#
#  - MCP79410_AdvancedSettings -> Main file project
#  - DigitalInput              -> Contains code to manage digital inputs and the respective functions to manage it
#  - DigitalOutput             -> Contains code to manage digital output and the respective functions to manage it
#  - RTCC_Management           -> Contains code to manage the functions that reads TimeKeeper and Alarms registers. Manages the Power ON and Power OFF of the electronics of the shield RTCC MCP79410 Rev. 1.1
#  - RTCC_Settings             -> Contains code to manage RTCC settings and decode user command (TimeKeeper, Alarms 0 and 1)
#  - TimerInt                  -> Contains code to manage Timer1 of ATMEGA328P. Timer1 generates an interrupt every 2 mSec.
#                                 With this resolution (2 mSec) it is possible to manage the timer variables used in this demo.
#                                 For example the TimeOut used to filter the unwanted variations on the digital inputs (debouncing)
#
#  Arduino Uno Rev. 3
#   
#  =============================================================================                    
#  
#  INSTALLATION
#  The 3 library files (MCP79410.cpp, MCP79410.h and keywords.txt) in the MCP79410 folder should be placed in your Arduino Library folder.
#  Run the MCP79410_AdvancedSettings.ino file from your Arduino IDE.
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

#define MAX_HOURS_12H    12
#define MAX_HOURS_24H    23
#define MAX_MINUTES      59
#define MAX_SECONDS      59

#define MAX_DAYS         31
#define MAX_MONTHS       12

#define TIMEKEEPER_CASE  0
#define ALARM_CASE       1
#define TIMESTAMP_CASE   2

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

const char cmdReceived[]         PROGMEM = "# Command received by user -> ";
const char cmdError[]            PROGMEM = "# Command syntax error. Retry";

const char HoursError[]          PROGMEM = "# Hours Syntax Error";
const char MinutesError[]        PROGMEM = "# Minutes Syntax Error";
const char SecondsError[]        PROGMEM = "# Seconds Syntax Error";
const char DaysError[]           PROGMEM = "# Days Syntax Error";
const char MonthsError[]         PROGMEM = "# Months Syntax Error";
const char YearsError[]          PROGMEM = "# Years Syntax Error";
const char WeekDayError[]        PROGMEM = "# Week Of Day Syntax Error";
const char AlarmMaskError[]      PROGMEM = "# Alarm Mask Syntax Error";
const char MfpPollError[]        PROGMEM = "# MFP Output Polarity Syntax Error";

const char AlarmxMaskError[]     PROGMEM = "# Alarm Mask config Error";

const char Enable[]              PROGMEM = "Enabled";
const char Disable[]             PROGMEM = "Disabled";

const char MFP_LHL[]             PROGMEM = "MFP is a logic high level";
const char MFP_LLL[]             PROGMEM = "MFP is a logic low level";

const char StartTimeKeeperSet[]  PROGMEM = "# TimeKeeper settings in progress. System wait a command to set date and time";
const char StopTimeKeeperSet[]   PROGMEM = "# TimeKeeper settings has been stopped";
const char StartAlarmSet[]       PROGMEM = "# Alarms settings in progress. System wait a command to set alarm 0/1";
const char StopAlarmSet[]        PROGMEM = "# Alarms settings has been stopped";

//------------------------------------
//  Commands string to set TimeKeeper and Alarm 0/1
const char TimeSet[]             PROGMEM = "TimeSet";
const char DateSet[]             PROGMEM = "DateSet";
const char TimeAlarmSet[]        PROGMEM = "TimeSetAlarm";
const char DateAlarmSet[]        PROGMEM = "DateSetAlarm";
const char ResetAlarm[]          PROGMEM = "ResetAlarm";
const char DisableAlarm[]        PROGMEM = "DisableAlarm";
const char EnableAlarm[]         PROGMEM = "EnableAlarm";

const char LHL_Alarm[]           PROGMEM = "LHL";
const char LLL_Alarm[]           PROGMEM = "LLL";

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
//------------------------------------
//======================================================================

//======================================================================
//  State Machine defines
typedef void State;
typedef State (*Pstate)();

Pstate Input_Management;        //  States machine used to manage the digital inputs (P1, P2, P3)
Pstate Led_Management;          //  States machine used to manage led LD4. This led is used to recognize the pressure on P1, P2 and P3 buttons
Pstate RTCC_Config_Management;  //  States machine used to config the RTCC
//======================================================================

//======================================================================
//  Define timer Constants and Variables
#define	TIMEBASE      2                //  Time Base: 2mSec

#define T_10MSEC      (10/TIMEBASE)    //  Defines a constant for a timeout of 10 mSec
#define T_20MSEC      (20/TIMEBASE)    //  Defines a constant for a timeout of 20 mSec
#define T_50MSEC      (50/TIMEBASE)    //  Defines a constant for a timeout of 50 mSec
#define T_100MSEC     (100/TIMEBASE)   //  Defines a constant for a timeout of 100 mSec
#define T_250MSEC     (250/TIMEBASE)   //  Defines a constant for a timeout of 250 mSec
#define T_750MSEC     (750/TIMEBASE)   //  Defines a constant for a timeout of 750 mSec

#define T_2SEC	      (2000/TIMEBASE)  //  Defines a constant for a timeout of 2,000  Sec
#define T_15SEC	      (15000/TIMEBASE) //  Defines a constant for a timeout of 15,000 Sec

uint8_t  DebouncingTimeOut;      //  Debouncing TimeOut for digital inputs
uint16_t TimeOutWait;            //  TimeOut to manage pulse pressing
uint16_t TimeOutLed;             //  TimeOut to manage led pulse
uint16_t TimeOutReadTimeKeeper;  //  TimeOut read TimeKeeper Registers
//======================================================================

//======================================================================
//  Output pin define
uint8_t ForceOn = 8;

//======================================================================

//======================================================================
//  Input pin define
uint8_t P1_Button = 4;         //  Digital input. Button P1. See schematic
uint8_t P2_Button = 5;         //  Digital input. Button P2. See schematic
uint8_t P3_Button = 6;         //  Digital input. Button P3. See schematic

uint8_t Led       = 7;         //  Digital Output. Led LD7
//======================================================================

//======================================================================
//  Variables to manage the input pin (Debouncing)
union  DigInputStatus {
  uint8_t  Input;
  struct {
    uint8_t  P1_Button  : 1; //  Bit 0
    uint8_t  P2_Button  : 1; //  Bit 1
    uint8_t  P3_Button  : 1; //  Bit 2
    uint8_t  Free       : 5;
  } In;
} DigInputStatus;

union  DigInputReaded {
  uint8_t  Input;
  struct {
    uint8_t  P1_Button  : 1; //  Bit 0
    uint8_t  P2_Button  : 1; //  Bit 1
    uint8_t  P3_Button  : 1; //  Bit 2
    uint8_t  Free       : 5;
  } In;
} DigInputReaded;

uint8_t DigInputVar;
//======================================================================

//======================================================================
//  Variables
String   SerialInput;
String   TempString;
uint8_t  TimeKeeper_Alarm_TimeStamp = 0;  //  0 -> TimeKeeper; 1 -> Alarm; 2 -> TimeStamp
uint8_t  StringIndex = 0;
uint8_t  AlarmIndex  = 0;
  
boolean  StartTimeKeeperSettings = false;
boolean  StartAlarmSettings      = false;
//======================================================================

//======================================================================
//  Sketch Setup
void setup() {
  SetInputPin();
  SetOutputPin();
  SetupTimer();
   
  Input_Management       = Input_Idle;     //  Initializes states machine used to manage the digital inputs (P1, P2, P3)
  Led_Management         = Led_Idle;       //  Initializes states machine used to manage led LD4. This led is used to recognize the pressure on P1, P2 and P3 buttons
  RTCC_Config_Management = RTCC_SetupIdle; //  Initializes states machine used to config the RTCC 
  
  mcp79410.Begin();
  Serial.begin(19200);
  
  TimeOutReadTimeKeeper = T_100MSEC;
}
//======================================================================

//======================================================================
void loop() {
  DebouncingInput();         //  Debouncing P1, P2 and P3 input
  Input_Management();        //  State Machine to manage Digital input
  Led_Management();          //  State Machine to manage Led
  RTCC_Config_Management();  //  State machine used to config the RTCC
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

