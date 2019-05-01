 /*  ===========================================================================
#  This is the library for MCP79410 I2C I/O expander. 
#  
#  Written by Matteo Destro for Futura Group srl
#  www.Futurashop.it
#  www.open-electronics.org
#
#  BSD license, all text above must be included in any redistribution
#  ===========================================================================
#
#	REVISION	1.0.0	02/10/2015
#
#  ===========================================================================
#
#  INSTALLATION
#  The 3 library files (MCP79410.cpp, MCP73410.h and keywords.txt) in the MCP79410
#  folder should be placed in your Arduino Library folder.
#
#  SUPPORT
#
#  info@open-electronics.org
#
#  ===========================================================================*/

#ifndef _MCP79410_H_
#define _MCP79410_H_

#include "Arduino.h"

#define RTCC_HW_ADD		0x6F	//	This address is got shifting the address 0xDE by one position on the right
#define EEPROM_HW_ADD	0x57	//	This address is got shifting the address 0xAE by one position on the right

//------------------------------------------------------
//	MCP79410 RTCC Register Address
#define RTCSEC_ADD   	0x00	//	Address TimeKeeping Seconds value register
#define RTCMIN_ADD   	0x01	//	Address TimeKeeping Minutes value register
#define RTCHOUR_ADD		0x02	//	Address TimeKeeping Hours value register
#define RTCWKDAY_ADD	0x03	//	Address TimeKeeping WeekDay value register
#define RTCDATE_ADD		0x04	//	Address TimeKeeping Date value register
#define RTCMTH_ADD 		0x05	//	Address TimeKeeping Month value register
#define RTCYEAR_ADD		0x06	//	Address TimeKeeping Year value register
#define CONTROL_ADD		0x07	//	Address RTCC control register
#define OSCTRIM_ADD		0x08	//	Address Oscillator digital trim register
#define EEUNLOCK_ADD	0x09	//	Address Not a physical register
#define ALM0SEC_ADD		0x0A	//	Address Alarm 0 Seconds value register
#define ALM0MIN_ADD		0x0B	//	Address Alarm 0 Minutes value register
#define ALM0HOUR_ADD	0x0C	//	Address Alarm 0 Hours value register
#define ALM0WKDAY_ADD	0x0D	//	Address Alarm 0 WeekDay value register
#define ALM0DATE_ADD	0x0E	//	Address Alarm 0 Date value register
#define ALM0MTH_ADD		0x0F	//	Address Alarm 0 Month value register
#define RESERVED1_ADD	0x10	//	Address Reserved
#define ALM1SEC_ADD		0x11	//	Address Alarm 1 Seconds value register
#define ALM1MIN_ADD		0x12	//	Address Alarm 1 Minutes value register
#define ALM1HOUR_ADD	0x13	//	Address Alarm 1 Hours value register
#define ALM1WKDAY_ADD	0x14	//	Address Alarm 1 WeekDay value register
#define ALM1DATE_ADD	0x15	//	Address Alarm 1 Date value register
#define ALM1MTH_ADD		0x16	//	Address Alarm 1 Month value register
#define RESERVED2_ADD	0x17	//	Address Reserved
#define PWRDWMIN_ADD	0x18	//	Address Power-Down TimeStamp Minutes value register
#define PWRDWHOUR_ADD	0x19	//	Address Power-Down TimeStamp Hours value register
#define PWRDWDATE_ADD	0x1A	//	Address Power-Down TimeStamp Date value register
#define PWRDWMTH_ADD	0x1B	//	Address Power-Down TimeStamp Month value register
#define PWRUPMIN_ADD	0x1C	//	Address Power-Up TimeStamp Minutes value register
#define PWRUPHOUR_ADD	0x1D	//	Address Power-Up TimeStamp Hours value register
#define PWRUPDATE_ADD	0x1E	//	Address Power-Up TimeStamp Date value register
#define PWRUPMTH_ADD	0x1F	//	Address Power-Up TimeStamp Month value register
//------------------------------------------------------

//------------------------------------------------------
//	Control Bits const. Examples of use
#define	OSCILLATOR_BIT_ON		0x80	//	Start Oscillator Bit (RTCSEC   | OSCILLATOR_BIT_ON)
#define	OSCILLATOR_BIT_OFF		0x7F 	//	Stop  Oscillator Bit (RTCSEC   & OSCILLATOR_BIT_OFF)
#define	_12_HOUR_ON				0x40 	//	12 Hour Format       (RTCHOUR  | _12_HOUR_ON)
#define	_24_HOUR_ON				0xBF	//	24 Hour Format       (RTCHOUR  & _24_HOUR_ON)
#define	VBAT_ON					0x08	//	Enable Vbat input    (RTCWKDAY | VBAT_ON) 
#define	VBAT_OFF				0xBF	//	Disable Vbat input   (RTCWKDAY & VBAT_OFF) 

#define OUT_LOGIC_LEVEL_HIGH	0x80	//	MFP signal level is logic HIGH        (CONTROL | OUT_LOGIC_LEVEL_HIGH)
#define OUT_LOGIC_LEVEL_LOW 	0x7F	//	MFP signal level is logic LOW         (CONTROL | OUT_LOGIC_LEVEL_LOW)
#define	SQUARE_WAVE_OUTPUT_ON	0x40	//	Enable Square Wave Clock Output Mode  (CONTROL | SQUARE_WAVE_OUTPUT_ON)
#define	SQUARE_WAVE_OUTPUT_OFF	0xBF	//	Disable Square Wave Clock Output Mode (CONTROL & SQUARE_WAVE_OUTPUT_OFF)
#define	ALARM1_ON				0x20	//	Alarm 1 Enabled                       (CONTROL | ALARM1_ON)
#define	ALARM1_OFF				0xDF	//	Alarm 1 Disabled                      (CONTROL | ALARM1_OFF)
#define	ALARM0_ON				0x10	//	Alarm 0 Enabled                       (CONTROL | ALARM0_ON)
#define	ALARM0_OFF				0xEF	//	Alarm 0 Disabled                      (CONTROL | ALARM0_OFF)
#define X1_EXTOSC_ON			0x08	//	Enable X1 pin         				  (CONTROL | X1_EXTOSC_ON)
#define X1_EXTOSC_OFF		 	0xF7	//	Disable X1 pin         				  (CONTROL | X1_EXTOSC_OFF)
#define CRSTRIM_ON				0x04	//	Enable Coarse Trim mode (if SQWEN=1)  (CONTROL | CRSTRIM_ON)
#define CRSTRIM_OFF			 	0xFB	//	Disable Coarse Trim mode         	  (CONTROL | CRSTRIM_OFF)
#define SQWFS_1Hz				0xFC	//	If SQWEN=1 and CSTRIM=0 F=1Hz         (CONTROL & SQWFS_1Hz)
#define SQWFS_4096Hz			0x01	//	If SQWEN=1 and CSTRIM=0 F=4096Hz     ((CONTROL & SQWFS_1Hz) | SQWFS_4096Hz)
#define SQWFS_8192Hz			0x02	//	If SQWEN=1 and CSTRIM=0 F=8192Hz     ((CONTROL & SQWFS_1Hz) | SQWFS_8192Hz)
#define SQWFS_32768Hz			0x03	//	If SQWEN=1 and CSTRIM=0 F=32768Hz    ((CONTROL & SQWFS_1Hz) | SQWFS_32768Hz)

#define	TEST_POWER_FAIL			0x10	//	TEST POWER FAIL STATUS (RTCWKDAY & TEST_POWER_FAIL) 
#define	TEST_OSCRUN				0x20	//	TEST OSCRUN STATUS     (RTCWKDAY & TEST_OSCRUN) 
#define	TEST_LEAPYEAR			0x20	//	TEST LEAPYEAR STATUS   (RTCMTH   & TEST_OSCRUN) 
//------------------------------------------------------

//------------------------------------------------------
//	TimeKeeper data registers (CONSTANT Example)
#define RTCSEC_REG		0x80	//	TimeKeeping Seconds value register
								//	Bxxxxxxxx
								//	 ||||||||
								//	 ||||++++--->	SECONE -> Bynary-Coded Decimal Value of Second's Ones Digit (0-9)
								//	 |+++------->	SECTEN -> Bynary-Coded Decimal Value of Second's Tens Digit (0-5)
								//	 +---------->	ST     -> Start oscillator bit (1->Enabled; 0->Disabled)
#define RTCMIN_REG      0x00	//	TimeKeeping Minutes value register
								//	Bxxxxxxxx
								//	 ||||||||
								//	 ||||++++--->	MINONE -> Bynary-Coded Decimal Value of Minute's Ones Digit (0-9)
								//	 |+++------->	MINTEN -> Bynary-Coded Decimal Value of Minute's Tens Digit (0-5)
								//	 +---------->	Unimplemented
#define RTCHOUR_REG		0x00	//	TimeKeeping Hours value register
								//	Bxxxxxxxx	(12-Hour Format)
								//	 ||||||||
								//	 ||||++++--->	HRONE  -> Bynary-Coded Decimal Value of Hour's Ones Digit (0-9)
								//	 |||+------->	HRTEN  -> Bynary-Coded Decimal Value of Hour's Tens Digit (0-1)
								//	 ||+-------->	~AM/PM -> AM/PM Indicator bit (1->PM; 0->AM)
								//	 |+--------->	12/~24 -> 12 or 24 Hour Time Format bit (1->12 Hour Format; 0->24 Hour Format)										
								//	 +---------->	Unimplemented
								//	Bxxxxxxxx	(24-Hour Format)
								//	 ||||||||
								//	 ||||++++--->	HRONE  -> Bynary-Coded Decimal Value of Hour's Ones Digit (0-9)
								//	 ||++------->	HRTEN  -> Bynary-Coded Decimal Value of Hour's Tens Digit (0-2)
								//	 |+--------->	12/~24 -> 12 or 24 Hour Time Format bit (1->12 Hour Format; 0->24 Hour Format)										
								//	 +---------->	Unimplemented										
#define RTCWKDAY_REG	0x08	//	TimeKeeping WeekDay value register										
								//	Bxxxxxxxx
								//	 ||||||||
								//	 |||||+++--->	WKDAY   -> Bynary-Coded Decimal Value of Day of Week Digit (1-7)
								//	 ||||+------>	VBATEN  -> Externally Battery Backup Supply Enable Bit (1->Vbat input is Enabled;
								//	 ||||																	0->Vbat input is Disabled)
								//	 |||+------->	PWRFAIL -> Power Failure Status Bit (1->Primary power was lost and the power-fail
								//	 |||													time-stamp registers have been loaded (Must be cleared
								//	 |||													in software). Clearing this bit resets the power-fail
								//	 |||													time-stamp register to 0);
								//	 ||| 												 0-> Primary power has not been lost
								//	 ||+--------->	OSCRUN  -> Oscillator status bit (Read Only) (1-> Oscillator is enabled and running;
								//	 ||															  0-> Oscillator has stopped or has been disabled)										
								//	 ++---------->	Unimplemented
#define RTCDATE_REG		0x00	//	TimeKeeping Date value register
								//	Bxxxxxxxx
								//	 ||||||||
								//	 ||||++++--->	DATEONE -> Bynary-Coded Decimal Value of Date's Ones Digit (0-9)
								//	 ||++------->	DATETEN -> Bynary-Coded Decimal Value of Date's Tens Digit (0-3)
								//	 ++--------->	Unimplemented
#define RTCMTH_REG	 	0x00	//	TimeKeeping Month value register
								//	Bxxxxxxxx
								//	 ||||||||
								//	 ||||++++--->	MTHONE -> Bynary-Coded Decimal Value of Months's Ones Digit (0-9)
								//	 |||+------->	MTHTEN -> Bynary-Coded Decimal Value of Month's Tens Digit (0-1)
								//	 ||+-------->	LPYR   -> Leap Year Bit (1->Year is leap year; 0->Year is not a leap year)
								//	 ++--------->	Unimplemented
#define RTCYEAR_REG		0x00	//	TimeKeeping Year value register
								//	Bxxxxxxxx
								//	 ||||||||
								//	 ||||++++--->	YRONE -> Bynary-Coded Decimal Value of Year's Ones Digit (0-9)
								//	 ++++------->	YRTEN -> Bynary-Coded Decimal Value of Year's Tens Digit (0-9)
//------------------------------------------------------

//------------------------------------------------------
#define CONTROL_REG		0x00	//	RTCC control register
								//	Bxxxxxxxx
								//	 ||||||||
								//	 ||||||++--->	SQWFS   -> Square Wave Clock Output Frequency:
								//	 ||||||                    if SQWEN=1 And CRSTRIM=0:
								//	 ||||||					      00 -> 1 Hz
								//	 ||||||						  01 -> 4096 Hz
								//	 ||||||						  10 -> 8192 Hz
								//	 ||||||						  11 -> 32768 Hz
								//	 ||||||                    if SQWEN=0 And CRSTRIM=1 (Unused)
								//	 |||||+----->	CRSTRIM -> Coarse Trim Mode Enable bit (1->Enabled Coarse Trim mode; 0->Disabled Coarse Trim mode)
								//	 ||||+------>	EXTOSC  -> External Oscillator Input bit (1->Enable X1 pin to be driven by external 32768 Hz source;
								//	 ||||													  0->Disable esternal 32768 Hz input)
								//   |||+------->   ALM0EN  -> Alarm 0 Modeule Enable bit (1->Alarm 0 Enabled; 0->Alarm 0 Disabled)
								//   ||+-------->   ALM1EN  -> Alarm 1 Modeule Enable bit (1->Alarm 1 Enabled; 0->Alarm 1 Disabled)
								//   |+--------->   SQWEN   -> Square Wave Output Enable Bit (1->Enable Square Wave Clock Output mode;
								//   |                                                        0->Disable Square Wave Clock Output mode)
								//   +---------->   OUT		-> Logic Level for General Purpose Output bit (SQWEN=0; ALM0EN=0; ALM1EN=0)
								//							   	1->MFP Signal level is logic High
								//							   	0->MFP Signal level is logic Low
//------------------------------------------------------
										
//------------------------------------------------------
//	Alarm 0 data registers (CONSTANT)
#define ALM0SEC_REG		0x00	//	Alarm 0 Seconds value register
								//	Bxxxxxxxx
								//	 ||||||||
								//	 ||||++++--->	SECONE -> Bynary-Coded Decimal Value of Second's Ones Digit (0-9)
								//	 |+++------->	SECTEN -> Bynary-Coded Decimal Value of Second's Tens Digit (0-5)
								//	 +---------->	Unimplemented
#define ALM0MIN_REG		0x00	//	Alarm 0 Minutes value register
								//	Bxxxxxxxx
								//	 ||||||||
								//	 ||||++++--->	MINONE -> Bynary-Coded Decimal Value of Minute's Ones Digit (0-9)
								//	 |+++------->	MINTEN -> Bynary-Coded Decimal Value of Minute's Tens Digit (0-5)
								//	 +---------->	Unimplemented
#define ALM0HOUR_REG	0x00	//	Alarm 0 Hours value register
								//	Bxxxxxxxx	(12-Hour Format)
								//	 ||||||||
								//	 ||||++++--->	HRONE  -> Bynary-Coded Decimal Value of Hour's Ones Digit (0-9)
								//	 |||+------->	HRTEN  -> Bynary-Coded Decimal Value of Hour's Tens Digit (0-1)
								//	 ||+-------->	~AM/PM -> AM/PM Indicator bit (1->PM; 0->AM)
								//	 |+--------->	12/~24 -> 12 or 24 Hour Time Format bit (1->12 Hour Format; 0->24 Hour Format)										
								//	 +---------->	Unimplemented
								//	Bxxxxxxxx	(24-Hour Format)
								//	 ||||||||
								//	 ||||++++--->	HRONE  -> Bynary-Coded Decimal Value of Hour's Ones Digit (0-9)
								//	 ||++------->	HRTEN  -> Bynary-Coded Decimal Value of Hour's Tens Digit (0-2)
								//	 |+--------->	12/~24 -> 12 or 24 Hour Time Format bit (1->12 Hour Format; 0->24 Hour Format)										
								//	 +---------->	Unimplemented
#define ALM0WKDAY_REG	0x00	//	Alarm 0 WeekDay value register
								//	Bxxxxxxxx
								//	 ||||||||
								//	 |||||+++--->	WKDAY   -> Bynary-Coded Decimal Value of Day bits (1-7)
								//	 ||||+------>	ALM0IF  -> Alarm interrupt Flag bit (1->Alarm match occurred (Must be cleared in software);
								//	 |||| 											 	 0->Alarm match did not occur)
								//	 |+++------->	ALM0MSK -> Alarm Mask bits 000->Seconds match
								//   |                                         001->Minutes match
								//   |										   010->Hours match
								//   |										   011->Day of week match
								//   |										   100->Date match
								//   |										   101->Reserved
								//   |										   110->Reserved
								//   |										   111->Seconds, Minutes, Hour, Day of Week, Date and Month match
								//	 +---------->	ALMPOL  -> Alarm Interrupt Output Polarity bit (1->Asserted output state of MFP is a logic high level;
								//																	0->Asserted output state of MFP is a logic low level)
#define ALM0DATE_REG	0x00	//	Alarm 0 Date value register
								//	Bxxxxxxxx
								//	 ||||||||
								//	 ||||++++--->	DATEONE -> Bynary-Coded Decimal Value of Date's Ones Digit (0-9)
								//	 ||++------->	DATETEN -> Bynary-Coded Decimal Value of Date's Tens Digit (0-3)
								//	 ++--------->	Unimplemented
#define ALM0MTH_REG		0x00	//	Alarm 0 Month value register
								//	Bxxxxxxxx
								//	 ||||||||
								//	 ||||++++--->	MTHONE -> Bynary-Coded Decimal Value of Months's Ones Digit (0-9)
								//	 |||+------->	MTHTEN -> Bynary-Coded Decimal Value of Month's Tens Digit (0-1)
								//	 ||+-------->	LPYR   -> Leap Year Bit (1->Year is leap year; 0->Year is not a leap year)
								//	 ++--------->	Unimplemented																		
//------------------------------------------------------

//------------------------------------------------------
//	Alarm 1 data registers (CONSTANT)
#define ALM1SEC_REG		0x00	//	Alarm 1 Seconds value register
								//	Bxxxxxxxx
								//	 ||||||||
								//	 ||||++++--->	SECONE -> Bynary-Coded Decimal Value of Second's Ones Digit (0-9)
								//	 |+++------->	SECTEN -> Bynary-Coded Decimal Value of Second's Tens Digit (0-5)
								//	 +---------->	Unimplemented
#define ALM1MIN_REG		0x00	//	Alarm 1 Minutes value register
								//	Bxxxxxxxx
								//	 ||||||||
								//	 ||||++++--->	MINONE -> Bynary-Coded Decimal Value of Minute's Ones Digit (0-9)
								//	 |+++------->	MINTEN -> Bynary-Coded Decimal Value of Minute's Tens Digit (0-5)
								//	 +---------->	Unimplemented
#define ALM1HOUR_REG	0x00	//	Alarm 1 Hours value register
								//	Bxxxxxxxx	(12-Hour Format)
								//	 ||||||||
								//	 ||||++++--->	HRONE  -> Bynary-Coded Decimal Value of Hour's Ones Digit (0-9)
								//	 |||+------->	HRTEN  -> Bynary-Coded Decimal Value of Hour's Tens Digit (0-1)
								//	 ||+-------->	~AM/PM -> AM/PM Indicator bit (1->PM; 0->AM)
								//	 |+--------->	12/~24 -> 12 or 24 Hour Time Format bit (1->12 Hour Format; 0->24 Hour Format)										
								//	 +---------->	Unimplemented
								//	Bxxxxxxxx	(24-Hour Format)
								//	 ||||||||
								//	 ||||++++--->	HRONE  -> Bynary-Coded Decimal Value of Hour's Ones Digit (0-9)
								//	 ||++------->	HRTEN  -> Bynary-Coded Decimal Value of Hour's Tens Digit (0-2)
								//	 |+--------->	12/~24 -> 12 or 24 Hour Time Format bit (1->12 Hour Format; 0->24 Hour Format)										
								//	 +---------->	Unimplemented
#define ALM1WKDAY_REG	0x00	//	Alarm 1 WeekDay value register
								//	Bxxxxxxxx
								//	 ||||||||
								//	 |||||+++--->	WKDAY   -> Bynary-Coded Decimal Value of Day bits (1-7)
								//	 ||||+------>	ALM0IF  -> Alarm interrupt Flag bit (1->Alarm match occurred (Must be cleared in software);
								//	 |||| 												0->Alarm match did not occur)
								//	 |+++------->	ALM0MSK -> Alarm Mask bits 000->Seconds match
								//   |                                         001->Minutes match
								//   |										   010->Hours match
								//   |										   011->Day of week match
								//   |										   100->Date match
								//   |										   101->Reserved
								//   |										   110->Reserved
								//   |										   111->Seconds, Minutes, Hour, Day of Week, Date and Month match
								//	 +---------->	ALMPOL  -> Alarm Interrupt Output Polarity bit (1->Asserted output state of MFP is a logic high level;
								//																	0->Asserted output state of MFP is a logic low level)
#define ALM1DATE_REG	0x00	//	Alarm 1 Date value register
								//	Bxxxxxxxx
								//	 ||||||||
								//	 ||||++++--->	DATEONE -> Bynary-Coded Decimal Value of Date's Ones Digit (0-9)
								//	 ||++------->	DATETEN -> Bynary-Coded Decimal Value of Date's Tens Digit (0-3)
								//	 ++--------->	Unimplemented
#define ALM1MTH_REG		0x00	//	Alarm 1 Month value register
								//	Bxxxxxxxx
								//	 ||||||||
								//	 ||||++++--->	MTHONE -> Bynary-Coded Decimal Value of Months's Ones Digit (0-9)
								//	 |||+------->	MTHTEN -> Bynary-Coded Decimal Value of Month's Tens Digit (0-1)
								//	 ||+-------->	LPYR   -> Leap Year Bit (1->Year is leap year; 0->Year is not a leap year)
								//	 ++--------->	Unimplemented																		
//------------------------------------------------------

//------------------------------------------------------
//	PowerDown Time-Stamp data registers (CONSTANT)
#define PWRDWMIN_REG	0x00	//	Power-Down TimeStamp Minutes value register
								//	Bxxxxxxxx
								//	 ||||||||
								//	 ||||++++--->	MINONE -> Bynary-Coded Decimal Value of Minute's Ones Digit (0-9)
								//	 |+++------->	MINTEN -> Bynary-Coded Decimal Value of Minute's Tens Digit (0-5)
								//	 +---------->	Unimplemented
#define PWRDWHOUR_REG	0x00	//	Power-Down TimeStamp Hours value register
								//	Bxxxxxxxx	(12-Hour Format)
								//	 ||||||||
								//	 ||||++++--->	HRONE  -> Bynary-Coded Decimal Value of Hour's Ones Digit (0-9)
								//	 |||+------->	HRTEN  -> Bynary-Coded Decimal Value of Hour's Tens Digit (0-1)
								//	 ||+-------->	~AM/PM -> AM/PM Indicator bit (1->PM; 0->AM)
								//	 |+--------->	12/~24 -> 12 or 24 Hour Time Format bit (1->12 Hour Format; 0->24 Hour Format)										
								//	 +---------->	Unimplemented
								//	Bxxxxxxxx	(24-Hour Format)
								//	 ||||||||
								//	 ||||++++--->	HRONE  -> Bynary-Coded Decimal Value of Hour's Ones Digit (0-9)
								//	 ||++------->	HRTEN  -> Bynary-Coded Decimal Value of Hour's Tens Digit (0-2)
								//	 |+--------->	12/~24 -> 12 or 24 Hour Time Format bit (1->12 Hour Format; 0->24 Hour Format)										
								//	 +---------->	Unimplemented
#define PWRDWDATE_REG	0x00	//	Power-Down TimeStamp Date value register
								//	Bxxxxxxxx
								//	 ||||||||
								//	 ||||++++--->	DATEONE -> Bynary-Coded Decimal Value of Date's Ones Digit (0-9)
								//	 ||++------->	DATETEN -> Bynary-Coded Decimal Value of Date's Tens Digit (0-3)
								//	 ++--------->	Unimplemented
#define PWRDWMTH_REG	0x00	//	Power-Down TimeStamp Month value register
								//	Bxxxxxxxx
								//	 ||||||||
								//	 ||||++++--->	MTHONE -> Bynary-Coded Decimal Value of Months's Ones Digit (0-9)
								//	 |||+------->	MTHTEN -> Bynary-Coded Decimal Value of Month's Tens Digit (0-1)
								//	 +++-------->	WKDAY  -> Bynary-Coded Decimal Value of Day bits (1-7)
//------------------------------------------------------

//------------------------------------------------------
//	PowerUp Time-Stamp data registers (CONSTANT)										
#define PWRUPMIN_REG	0x00	//	Power-Down TimeStamp Minutes value register
								//	Bxxxxxxxx
								//	 ||||||||
								//	 ||||++++--->	MINONE -> Bynary-Coded Decimal Value of Minute's Ones Digit (0-9)
								//	 |+++------->	MINTEN -> Bynary-Coded Decimal Value of Minute's Tens Digit (0-5)
								//	 +---------->	Unimplemented
#define PWRUPHOUR_REG	0x00	//	Power-Down TimeStamp Hours value register
								//	Bxxxxxxxx	(12-Hour Format)
								//	 ||||||||
								//	 ||||++++--->	HRONE  -> Bynary-Coded Decimal Value of Hour's Ones Digit (0-9)
								//	 |||+------->	HRTEN  -> Bynary-Coded Decimal Value of Hour's Tens Digit (0-1)
								//	 ||+-------->	~AM/PM -> AM/PM Indicator bit (1->PM; 0->AM)
								//	 |+--------->	12/~24 -> 12 or 24 Hour Time Format bit (1->12 Hour Format; 0->24 Hour Format)										
								//	 +---------->	Unimplemented
								//	Bxxxxxxxx	(24-Hour Format)
								//	 ||||||||
								//	 ||||++++--->	HRONE  -> Bynary-Coded Decimal Value of Hour's Ones Digit (0-9)
								//	 ||++------->	HRTEN  -> Bynary-Coded Decimal Value of Hour's Tens Digit (0-2)
								//	 |+--------->	12/~24 -> 12 or 24 Hour Time Format bit (1->12 Hour Format; 0->24 Hour Format)										
								//	 +---------->	Unimplemented
#define PWRUPDATE_REG	0x00	//	Power-Down TimeStamp Date value register
								//	Bxxxxxxxx
								//	 ||||||||
								//	 ||||++++--->	DATEONE -> Bynary-Coded Decimal Value of Date's Ones Digit (0-9)
								//	 ||++------->	DATETEN -> Bynary-Coded Decimal Value of Date's Tens Digit (0-3)
								//	 ++--------->	Unimplemented
#define PWRUPMTH_REG	0x00	//	Power-Down TimeStamp Month value register
								//	Bxxxxxxxx
								//	 ||||||||
								//	 ||||++++--->	MTHONE -> Bynary-Coded Decimal Value of Months's Ones Digit (0-9)
								//	 |||+------->	MTHTEN -> Bynary-Coded Decimal Value of Month's Tens Digit (0-1)
								//	 +++-------->	WKDAY  -> Bynary-Coded Decimal Value of Day bits (1-7)
//------------------------------------------------------										
						
//------------------------------------------------------										
//	MCP79410 EEPROM Register Address

#define EEPROM_START_ADD			0x00
#define EEPROM_STOP_ADD				0x7F
#define EEPROM_BLOCK_PROTECTION_ADD	0xFF

#define PROTECTED_EEPROM_START_ADD	0xF0
#define PROTECTED_EEPROM_STOP_ADD	0xF7

//------------------------------------------------------										

class MCP79410 {
public:
	void    Begin(void);
	void    ToggleSingleBit(uint8_t ControlByte, uint8_t RegAdd, uint8_t Bit);
	void    SetSingleBit(uint8_t ControlByte, uint8_t RegAdd, uint8_t Bit);
	void    ResetSingleBit(uint8_t ControlByte, uint8_t RegAdd, uint8_t Bit);
	void    WriteSingleReg(uint8_t ControlByte, uint8_t RegAdd, uint8_t RegData);
	void    WriteArray(uint8_t ControlByte, uint8_t StartAdd, uint8_t Lenght);
	void    ClearReg(uint8_t ControlByte, uint8_t RegAdd);
	uint8_t ReadSingleReg(uint8_t ControlByte, uint8_t RegAdd);
	void    ReadArray(uint8_t ControlByte, uint8_t StartAdd, uint8_t Lenght);
	
	void	StartOscillatorBit(uint8_t EnableDisable);
	void	Hour12or24TimeFormatBit(uint8_t SetHourType);
	void 	AmPmBit(uint8_t SetAmPm);
	void	VbatEnBit(uint8_t EnableDisable);
	void    ResetPwFailBit(void);
	
	void 	GeneralPurposeOutputBit(uint8_t SetReset);
	void 	SquareWaveOutputBit(uint8_t EnableDisable);
	void 	Alarm1Bit(uint8_t EnableDisable);
	void 	Alarm0Bit(uint8_t EnableDisable);
	void 	ExternalOscillatorBit(uint8_t EnableDisable);
	void 	CoarseTrimModeBit(uint8_t EnableDisable);
	void	SetOutputFrequencyBit(uint8_t OutputFreq);
	
	void	AlarmHour12or24TimeFormatBit(uint8_t SetHourType, uint8_t Alarm0_1);
	void	AlarmAmPmBit(uint8_t SetAmPm, uint8_t Alarm0_1);
	void	AlarmIntOutputPolarityBit(uint8_t SetReset, uint8_t Alarm0_1);
	void	AlarmMaskBit(uint8_t Alarm0_1, uint8_t Mask);
	void	ResetAlarmIntFlagBit(uint8_t Alarm0_1);
	
	void	PowerHour12or24TimeFormatBit(uint8_t SetHourType, uint8_t PowerDownUp);
	void	PowerAmPmBit(uint8_t SetAmPm, uint8_t PowerDownUp);
	
	void	WriteTimeKeeping(uint8_t Hour12or24Format);
	void	WriteAlarmRegister(uint8_t Alarm0_1, uint8_t Hour12or24Format);
	void	WritePowerDownUpRegister(uint8_t PowerDownUp, uint8_t Hour12or24Format);
	void	ReadTimeKeeping(void);
	void	ReadAlarmRegister(uint8_t Alarm0_1);
	void	ReadPowerDownUpRegister(uint8_t PowerDownUp);
	
	void 	Set_EEPROM_WriteProtection(uint8_t Section);
	void	WriteProtected_EEPROM(uint8_t RegAdd, uint8_t RegData);
	
	uint8_t DataArray[16];
	
	//------------------------------------------------------	
	union ControlReg {
		uint8_t  ControlByte;
		struct {
			uint8_t SquareWaveFreqOutput	:2;
			uint8_t CoarseTrimEnable		:1;
			uint8_t ExtOscInput				:1;
			uint8_t	Alarm0_Enable			:1;
			uint8_t Alarm1_Enable			:1;
			uint8_t SquareWaveOutputEnable	:1;
			uint8_t LogicLevelOutput		:1;
		} Bit;	
	} ControlReg;

	union OSC_TrimReg {
		uint8_t  OSC_TrimByte;
		struct {
			uint8_t TrimSignBit	:1;
			uint8_t TrimValue	:7;
		} Bit;	
	} OSC_TrimReg;
	//------------------------------------------------------

	//------------------------------------------------------
	//	TimeKeeper data registers
	typedef union TimeKeeperSecond {
		uint8_t  SecByte;
		struct {
			uint8_t SecOne		:4;
			uint8_t SecTen		:3;
			uint8_t StartOsc	:1;
		} SecBit;		
	} TimeKeeperSeconds;
	typedef union TimeKeeperMinute {
		uint8_t  MinByte;
		struct {
			uint8_t MinOne		:4;
			uint8_t MinTen		:3;
			uint8_t Free		:1;
		} MinBit;		
	} TimeKeeperMinute;
	typedef union TimeKeeperHour12 {	
		uint8_t  Hour_12Byte;
		struct {
			uint8_t HrOne		:4;
			uint8_t HrTen		:1;
			uint8_t AmPm		:1;
			uint8_t _12_24		:1;
			uint8_t Free		:1;
		} Hour_12Bit;	
	} TimeKeeperHour12;
	typedef union TimeKeeperHour24 {		
		uint8_t  Hour_24Byte;
		struct {
			uint8_t HrOne		:4;
			uint8_t HrTen		:2;
			uint8_t _12_24		:1;
			uint8_t Free		:1;
		} Hour_24Bit;	
	} TimeKeeperHour24;
	typedef union TimeKeeperWeekDay {	
		uint8_t  WkDayByte;
		struct {
			uint8_t WkDay		:3;
			uint8_t VbatEn		:1;
			uint8_t PwrFail		:1;
			uint8_t OSCrun		:1;
			uint8_t Free		:2;
		} WkDayBit;	
	} TimeKeeperWeekDay;
	typedef union TimeKeeperDate {		
		uint8_t  DateByte;
		struct {
			uint8_t DateOne		:4;
			uint8_t DateTen		:2;
			uint8_t Free		:2;
		} DateBit;
	} TimeKeeperDate;
	typedef union TimeKeeperMonth {			
		uint8_t  MonthByte;
		struct {
			uint8_t MonthOne	:4;
			uint8_t MonthTen	:1;
			uint8_t LeapYear	:1;
			uint8_t Free		:2;
		} MonthBit;
	} TimeKeeperMonth;
	typedef union TimeKeeperYear {				
		uint8_t  YearByte;
		struct {
			uint8_t YearOne		:4;
			uint8_t YearTen		:4;
		} YearBit;
	} TimeKeeperYear;
	
	struct {
		TimeKeeperSeconds 	Second;
		TimeKeeperMinute	Minute;
		TimeKeeperHour12	Hour12;
		TimeKeeperHour24	Hour24;
		TimeKeeperWeekDay	WeekDay;
		TimeKeeperDate		Date;
		TimeKeeperMonth		Month;
		TimeKeeperYear		Year;
	} TimeKeeper;
	//------------------------------------------------------
	
	//------------------------------------------------------
	//	Alarm data registers
	typedef union AlarmSecond {	
		uint8_t  SecByte;
		struct {
			uint8_t SecOne		:4;
			uint8_t SecTen		:3;
			uint8_t Free		:1;
		} SecBit;
	} AlarmSecond;
	typedef union AlarmMinute {	
		uint8_t  MinByte;
		struct {
			uint8_t MinOne		:4;
			uint8_t MinTen		:3;
			uint8_t Free		:1;
		} MinBit;
	} AlarmMinute;
	typedef union AlarmHour12 {		
		uint8_t  Hour_12Byte;
		struct {
			uint8_t HrOne		:4;
			uint8_t HrTen		:1;
			uint8_t AmPm		:1;
			uint8_t _12_24		:1;
			uint8_t Free		:1;
		} Hour_12Bit;
	} AlarmHour12;
	typedef union AlarmHour24 {		
		uint8_t  Hour_24Byte;
		struct {
			uint8_t HrOne		:4;
			uint8_t HrTen		:2;
			uint8_t _12_24		:1;
			uint8_t Free		:1;
		} Hour_24Bit;
	} AlarmHour24;	
	typedef union AlarmWeekDay {	
		uint8_t  WkDayByte;
		struct {
			uint8_t WkDay		:3;
			uint8_t AlarmIF		:1;
			uint8_t AlarmMask	:3;
			uint8_t AlarmPol	:1;
		} WkDayBit;	
	} AlarmWeekDay;
	typedef union AlarmDate {		
		uint8_t  DateByte;
		struct {
			uint8_t DateOne		:4;
			uint8_t DateTen		:2;
			uint8_t Free		:2;
		} DateBit;	
	} AlarmDate;	
	typedef union AlarmMonth {	
		uint8_t  MonthByte;
		struct {
			uint8_t MonthOne	:4;
			uint8_t MonthTen	:1;
			uint8_t Free		:3;
		} MonthBit;	
	} AlarmMonth;
	
	struct {
		AlarmSecond 	Second;
		AlarmMinute		Minute;
		AlarmHour12		Hour12;
		AlarmHour24		Hour24;
		AlarmWeekDay	WeekDay;
		AlarmDate		Date;
		AlarmMonth		Month;
	} Alarm[2];
	//------------------------------------------------------

	//------------------------------------------------------
	//	PowerDown/PowerUp TimeStamp data registers
	//	PowerTimeStamp[0] Stores PowerDown TimeStamp
	//	PowerTimeStamp[1] Stores PowerUp TimeStamp
	typedef union PowerMinute {	
		uint8_t  MinByte;
		struct {
			uint8_t MinOne		:4;
			uint8_t MinTen		:3;
			uint8_t Free		:1;
		} MinBit;
	} PowerMinute;	
	typedef union PowerHour12 {		
		uint8_t  Hour_12Byte;
		struct {
			uint8_t HrOne		:4;
			uint8_t HrTen		:1;
			uint8_t AmPm		:1;
			uint8_t _12_24		:1;
			uint8_t Free		:1;
		} Hour_12Bit;
	} PowerHour12;
	typedef union PowerHour24 {		
		uint8_t  Hour_24Byte;
		struct {
			uint8_t HrOne		:4;
			uint8_t HrTen		:2;
			uint8_t _12_24		:1;
			uint8_t Free		:1;
		} Hour_24Bit;
	} PowerHour24;
	typedef union PowerDate {		
		uint8_t  DateByte;
		struct {
			uint8_t DateOne		:4;
			uint8_t DateTen		:2;
			uint8_t Free		:2;
		} DateBit;	
	} PowerDate;	
	typedef union PowerMonth {	
		uint8_t  MonthByte;
		struct {
			uint8_t MonthOne	:4;
			uint8_t MonthTen	:1;
			uint8_t WkDay		:3;
		} MonthBit;	
	} PowerMonth;
	
	struct {
		PowerMinute		Minute;
		PowerHour12		Hour12;
		PowerHour24		Hour24;
		PowerDate		Date;
		PowerMonth		Month;
	} Power[2];	
	//------------------------------------------------------
		
private:

};

#endif
