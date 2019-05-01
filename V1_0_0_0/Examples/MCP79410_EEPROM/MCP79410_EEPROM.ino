/*************************************************************************************************************
#  HISTORY
#  v1.0  -  First Release
#
#  =============================================================================
#  This is an example that shows how to manage the memory EEPROM of MCP79410 device (RTCC - Real Time Clock Calendar)
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
#  - MCP79410_EEPROM -> Main file project
#  - TimerInt        -> Contains code to manage Timer1 of ATMEGA328P. Timer1 generates an interrupt every 2 mSec.
#                       With this resolution (2 mSec) it is possible to manage the timer variables used in this demo.
#                       For example the TimeOut used to filter the unwanted variations on the digital inputs (debouncing)
#
#  Arduino Uno Rev. 3
#   
#  =============================================================================
#                        
#  INSTALLATION
#  The 3 library files (MCP79410.cpp, MCP79410.h and keywords.txt) in the MCP79410 folder should be placed in your Arduino Library folder.
#  Run the MCP79410_EEPROM.ino file from your Arduino IDE.
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

uint16_t TimeOutReadEEPROM;           //  TimeOut read EEPROM
uint16_t TimeOutReadProtectedEEPROM;  //  TimeOut read protected EEPROM

//======================================================================

//======================================================================
//  String constants
const char Sep1[]        PROGMEM = "#=================================================================";
const char DataRead[]    PROGMEM = "# Data read from EEPROM is: ";
const char MAC_AddRead[] PROGMEM = "# The MAC Address read from protected EEPROM is: ";
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
  uint8_t Data;
  
  SetInputPin();
  SetOutputPin(); 
  SetupTimer();
  
  mcp79410.Begin();
  Serial.begin(19200);
  
  Set_EEPROM();
  delay(250);
  Set_Protected_EEPROM();
}
//======================================================================

//======================================================================
void loop() {
  Read_EEPROM();
  Read_Protected_EEPROM();
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
//  Program EEPROM: Start Address -> 0x00
//                  End   Address -> 0x7F
//  This function writes into EEPROM the string "ElettronicaIn"
void Set_EEPROM(void) {
  uint8_t Counter;
  
  mcp79410.DataArray[0]  = 'E';
  mcp79410.DataArray[1]  = 'l';
  mcp79410.DataArray[2]  = 'e';
  mcp79410.DataArray[3]  = 't';
  mcp79410.DataArray[4]  = 't';
  mcp79410.DataArray[5]  = 'r';
  mcp79410.DataArray[6]  = 'o';
  mcp79410.DataArray[7]  = 'n';
  mcp79410.DataArray[8]  = 'i';
  mcp79410.DataArray[9]  = 'c';
  mcp79410.DataArray[10] = 'a';
  mcp79410.DataArray[11] = 'I';
  mcp79410.DataArray[12] = 'n';

  Counter = 0;
  do {
    mcp79410.WriteSingleReg(EEPROM_HW_ADD, Counter, mcp79410.DataArray[Counter]);
    mcp79410.DataArray[Counter] = 0x00;
    delay(10);
  } while (++Counter < 14);
}
//======================================================================

//======================================================================
//  Program EEPROM: Start Address -> 0x00
//                  End   Address -> 0x7F
//  This function reads the EEPROM memory and prints the result of data read
//  The read of memory is performed every 10 seconds
void Read_EEPROM(void) {
  uint8_t Counter;
  char    c;
  
  if (TimeOutReadEEPROM == 0) {
    TimeOutReadEEPROM = T_10SEC;
    digitalWrite(Led, LOW);        // Led ON
    
    mcp79410.ReadArray(EEPROM_HW_ADD, EEPROM_START_ADD, 13);
    ReadStringFLASH((uint8_t *)Sep1, strlen(Sep1), TRUE, FALSE);
    ReadStringFLASH((uint8_t *)DataRead, strlen(DataRead), TRUE, FALSE);
  
    Counter = 0;
    do {
      c = char(mcp79410.DataArray[Counter]);
      mcp79410.DataArray[Counter] = 0x00;
      Serial.print(c);
    } while (++Counter < 14);
    Serial.println("");
    ReadStringFLASH((uint8_t *)Sep1, strlen(Sep1), TRUE, FALSE);
    Serial.println("");
    digitalWrite(Led, HIGH);        // Led OFF
  }
}
//======================================================================

//======================================================================
//  Program EEPROM: Start Address -> 0xF0
//                  End   Address -> 0xF7
//  This function writes into protected EEPROM the MAC Address "D8:80:39:52:22:15"
//  This is an example only, the MAC address used is not available because is already used by Microchip
void Set_Protected_EEPROM(void) {
  uint8_t Counter;
  
  mcp79410.DataArray[0]  = 0xD8;
  mcp79410.DataArray[1]  = 0x80;
  mcp79410.DataArray[2]  = 0x39;
  mcp79410.DataArray[3]  = 0x52;
  mcp79410.DataArray[4]  = 0x22;
  mcp79410.DataArray[5]  = 0x15;  

  Counter = 0;
  do {
    mcp79410.WriteProtected_EEPROM((PROTECTED_EEPROM_START_ADD + Counter), mcp79410.DataArray[Counter]);
    mcp79410.DataArray[Counter] = 0x00;
    delay(10);
  } while (++Counter < 6);
}
//======================================================================

//======================================================================
void Read_Protected_EEPROM(void) {
  uint8_t Counter;
  
  if (TimeOutReadProtectedEEPROM == 0) {
    TimeOutReadProtectedEEPROM = T_10SEC;
    mcp79410.ReadArray(EEPROM_HW_ADD, PROTECTED_EEPROM_START_ADD, 6);
    ReadStringFLASH((uint8_t *)Sep1, strlen(Sep1), TRUE, FALSE);
    ReadStringFLASH((uint8_t *)MAC_AddRead, strlen(MAC_AddRead), TRUE, FALSE);
    
    Counter = 0;
    do {
      Serial.print(mcp79410.DataArray[Counter], HEX);
      mcp79410.DataArray[Counter] = 0x00;
      if (Counter <5) {
        Serial.print(":");
      }
    } while (++Counter < 6);
    Serial.println("");
    ReadStringFLASH((uint8_t *)Sep1, strlen(Sep1), TRUE, FALSE);
    Serial.println("");    
  }
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
