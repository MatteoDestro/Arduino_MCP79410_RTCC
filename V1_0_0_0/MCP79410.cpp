/*  ===========================================================================
#  This is the library for MCP79410 RTCC (Real Time Clock Calendar). 
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
#  The 3 library files (MCP79410.cpp, MCP79410.h and keywords.txt) in the MCP79410
#  folder should be placed in your Arduino Library folder.
#
#  SUPPORT
#
#  info@open-electronics.org
#
#  ===========================================================================*/

#include <Wire.h>
#include "MCP79410.h"
#ifdef __AVR__
#include <avr/pgmspace.h>
#endif

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// minihelper to keep Arduino backward compatibility
static inline void wiresend(uint8_t x) {
#if ARDUINO >= 100
	Wire.write((uint8_t) x);
#else
	Wire.send(x);
#endif
}

static inline uint8_t wirerecv(void) {
#if ARDUINO >= 100
	return Wire.read();
#else
	return Wire.receive();
#endif
}

//-----------------------------------------------------
//	Public Function
void MCP79410::Begin(void) {
	Wire.begin();
}

//----------------------------------------------------------------------------------------------------------
//==============================
//	GENERAL PURPOSE FUNCTIONS
//==============================
//-----------------------------------------------------
//	This function toggles a single bit into selected register
//	ControlByte = Hardware address + W/R bit
//  RegAdd      = Register address
//	Bit         = Bit to set
void MCP79410::ToggleSingleBit(uint8_t ControlByte, uint8_t RegAdd, uint8_t Bit) {
	uint8_t RegData;
	if (Bit > 7) { Bit = 7; }
	
	Wire.beginTransmission(ControlByte);
	wiresend(RegAdd);
	Wire.endTransmission();
	Wire.requestFrom(ControlByte, 1);
	RegData = wirerecv();

	RegData ^= (0x01 << Bit);

	Wire.beginTransmission(ControlByte);
	wiresend(RegAdd);
	wiresend(RegData);
	Wire.endTransmission();	
}
//-----------------------------------------------------

//-----------------------------------------------------
//	This function sets a single bit into selected register
//	ControlByte = Hardware address + W/R bit
//  RegAdd      = Register address
//	Bit         = Bit to set
void MCP79410::SetSingleBit(uint8_t ControlByte, uint8_t RegAdd, uint8_t Bit) {
	uint8_t RegData;
	if (Bit > 7) { Bit = 7; }
	
	Wire.beginTransmission(ControlByte);
	wiresend(RegAdd);
	Wire.endTransmission();
	Wire.requestFrom(ControlByte, 1);
	RegData = wirerecv();

	RegData |= (0x01 << Bit);

	Wire.beginTransmission(ControlByte);
	wiresend(RegAdd);
	wiresend(RegData);
	Wire.endTransmission();	
}
//-----------------------------------------------------

//-----------------------------------------------------
//	This function resets a single bit into selected register
//	ControlByte = Hardware address + W/R bit
//  RegAdd      = Register address
//	Bit         = Bit to reset
void MCP79410::ResetSingleBit(uint8_t ControlByte, uint8_t RegAdd, uint8_t Bit) {
	uint8_t RegData;
	if (Bit > 7) { Bit = 7; }
	
	Wire.beginTransmission(ControlByte);
	wiresend(RegAdd);
	Wire.endTransmission();
	Wire.requestFrom(ControlByte, 1);
	RegData = wirerecv();
	
	RegData &= ~(0x01 << Bit);
	
	Wire.beginTransmission(ControlByte);
	wiresend(RegAdd);
	wiresend(RegData);
	Wire.endTransmission();	
}
//-----------------------------------------------------

//-----------------------------------------------------
//	This function writes a single register
//	ControlByte = Hardware address + W/R bit
//  RegAdd      = Register address
//	RegData     = Data to write into register selected
void MCP79410::WriteSingleReg(uint8_t ControlByte, uint8_t RegAdd, uint8_t RegData) {
	Wire.beginTransmission(ControlByte);
	wiresend(RegAdd);
	wiresend(RegData);
	Wire.endTransmission();	
}
//-----------------------------------------------------

//-----------------------------------------------------
//	This function writes n bytes sequentially
//	For internal EEPROM max 8 Byte at once
//	ControlByte = Hardware address + W/R bit
//	StartAdd    = Start Address to write
//	Lenght      = n Byte to write
void MCP79410::WriteArray(uint8_t ControlByte, uint8_t StartAdd, uint8_t Lenght) {
	uint8_t	*Pointer;
	uint8_t	Counter;
	
	Wire.beginTransmission(ControlByte);
	wiresend(StartAdd);
	
 	Pointer = &DataArray[0];
	Counter = 0;
	do {
		wiresend(*(uint8_t *)Pointer++);
	} while (Counter++ < (Lenght - 1));
 	
	Wire.endTransmission();	
}
//-----------------------------------------------------

//-----------------------------------------------------
//	This function clear a single register
//	ControlByte = Hardware address + W/R bit
//  RegAdd      = Register address
void MCP79410::ClearReg(uint8_t ControlByte, uint8_t RegAdd) {
	Wire.beginTransmission(ControlByte);
	wiresend(RegAdd);
	wiresend(0x00);
	Wire.endTransmission();	
}
//-----------------------------------------------------

//-----------------------------------------------------
//	This function read a single register
//	ControlByte = Hardware address + W/R bit
//  RegAdd      = Register address
//	This function return the value of selected register
uint8_t MCP79410::ReadSingleReg(uint8_t ControlByte, uint8_t RegAdd) {
	Wire.beginTransmission(ControlByte);
	wiresend(RegAdd);
	Wire.endTransmission();
	Wire.requestFrom(ControlByte, 1);
	return wirerecv();	
}
//-----------------------------------------------------

//-----------------------------------------------------
//	This function read n bytes sequentially
//	ControlByte = Hardware address + W/R bit
//	StartAdd    = Start Address to read
//	Lenght      = n Byte to read
void MCP79410::ReadArray(uint8_t ControlByte, uint8_t StartAdd, uint8_t Lenght) {
	uint8_t	*Pointer;
		
	Pointer = &DataArray[0];
	
	Wire.beginTransmission(ControlByte);
	wiresend(StartAdd);
	Wire.endTransmission();
	
	Wire.requestFrom(ControlByte, Lenght);
    while(Wire.available())	{
		*(uint8_t *)Pointer++ = Wire.read();
    }		
}
//-----------------------------------------------------
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
//==============================
//	RTCC DEDICATED FUNCTIONS
//==============================

//=========================================================================
//-----------------------------------------------------
//	This function Set/Reset OUT Bit in CONTROL register
//	SetReset => "1" Set; "0" Reset
void MCP79410::GeneralPurposeOutputBit(uint8_t SetReset) {
	if (SetReset != 0) {
		SetSingleBit(RTCC_HW_ADD, CONTROL_ADD, 7);
	} else {
		ResetSingleBit(RTCC_HW_ADD, CONTROL_ADD, 7);
	}
}
//-----------------------------------------------------

//-----------------------------------------------------
//	This function Set/Reset SQWEN Bit in CONTROL register
//	EnableDisable => "1" Enable; "0" Disable
void MCP79410::SquareWaveOutputBit(uint8_t EnableDisable) {
	if (EnableDisable != 0) {
		SetSingleBit(RTCC_HW_ADD, CONTROL_ADD, 6);
	} else {
		ResetSingleBit(RTCC_HW_ADD, CONTROL_ADD, 6);
	}
}
//-----------------------------------------------------

//-----------------------------------------------------
//	This function Enable/Disable Alarm 1 in CONTROL register
//	EnableDisable => "1" Enable; "0" Disable
void MCP79410::Alarm1Bit(uint8_t EnableDisable) {
	if (EnableDisable != 0) {
		SetSingleBit(RTCC_HW_ADD, CONTROL_ADD, 5);
	} else {
		ResetSingleBit(RTCC_HW_ADD, CONTROL_ADD, 5);
	}
}
//-----------------------------------------------------

//-----------------------------------------------------
//	This function Enable/Disable Alarm 0 in CONTROL register
//	EnableDisable => "1" Enable; "0" Disable
void MCP79410::Alarm0Bit(uint8_t EnableDisable) {
	if (EnableDisable != 0) {
		SetSingleBit(RTCC_HW_ADD, CONTROL_ADD, 4);
	} else {
		ResetSingleBit(RTCC_HW_ADD, CONTROL_ADD, 4);
	}
}
//-----------------------------------------------------

//-----------------------------------------------------
//	This function Enable/Disable ExtOSC in CONTROL register
//	EnableDisable => "1" Enable; "0" Disable
void MCP79410::ExternalOscillatorBit(uint8_t EnableDisable) {
	if (EnableDisable != 0) {
		SetSingleBit(RTCC_HW_ADD, CONTROL_ADD, 3);
	} else {
		ResetSingleBit(RTCC_HW_ADD, CONTROL_ADD, 3);
	}
}
//-----------------------------------------------------

//-----------------------------------------------------
//	This function Enable/Disable CRSTRIM in CONTROL register
//	EnableDisable => "1" Enable; "0" Disable
void MCP79410::CoarseTrimModeBit(uint8_t EnableDisable) {
	if (EnableDisable != 0) {
		SetSingleBit(RTCC_HW_ADD, CONTROL_ADD, 2);
	} else {
		ResetSingleBit(RTCC_HW_ADD, CONTROL_ADD, 2);
	}
}
//-----------------------------------------------------

//-----------------------------------------------------
//	This function Set SQWFS in CONTROL register
//	OutputFreq => 00 (0) -> 1 Hz
//                01 (1) -> 4096 Hz
// 			      10 (2) -> 8192 Hz
//  		      11 (3) -> 32768 Hz
void MCP79410::SetOutputFrequencyBit(uint8_t OutputFreq) {
	uint8_t RegData;
	RegData = ReadSingleReg(RTCC_HW_ADD, CONTROL_ADD);
	RegData &= 0xFC;
	RegData |= OutputFreq;
	WriteSingleReg(RTCC_HW_ADD, CONTROL_ADD, RegData);
}
//-----------------------------------------------------
//=========================================================================

//=========================================================================
//-----------------------------------------------------
//	This function Set/Reset Start Oscillator Bit in RTCSEC register
//	EnableDisable => "1" Enable; "0" Disable
void MCP79410::StartOscillatorBit(uint8_t EnableDisable) {
	if (EnableDisable != 0) {
		SetSingleBit(RTCC_HW_ADD, RTCSEC_ADD, 7);
	} else {
		ResetSingleBit(RTCC_HW_ADD, RTCSEC_ADD, 7);
	}
}
//-----------------------------------------------------
//=========================================================================

//=========================================================================
//-----------------------------------------------------
//	This function Set 12 or 24 Hour Time Format Bit in RTCHOUR register
//	SetHourType => "1" 12 Hour; "0" 24 Hour
void MCP79410::Hour12or24TimeFormatBit(uint8_t SetHourType) {
	if (SetHourType != 0) {
		SetSingleBit(RTCC_HW_ADD, RTCHOUR_ADD, 6);
	} else {
		ResetSingleBit(RTCC_HW_ADD, RTCHOUR_ADD, 6);
	}
}
//-----------------------------------------------------

//-----------------------------------------------------
//	This function Set AM or PM Bit in RTCHOUR register
//	SetAmPm => "1" PM; "0" AM
void MCP79410::AmPmBit(uint8_t SetAmPm) {
	if (SetAmPm != 0) {
		SetSingleBit(RTCC_HW_ADD, RTCHOUR_ADD, 5);
	} else {
		ResetSingleBit(RTCC_HW_ADD, RTCHOUR_ADD, 5);
	}
}
//-----------------------------------------------------
//=========================================================================

//=========================================================================
//-----------------------------------------------------
//	This function Set/Reset Vbaten Bit in RTCWKDAY register
//	EnableDisable => "1" Enable; "0" Disable
void MCP79410::VbatEnBit(uint8_t EnableDisable) {
	if (EnableDisable != 0) {
		SetSingleBit(RTCC_HW_ADD, RTCWKDAY_ADD, 3);
	} else {
		ResetSingleBit(RTCC_HW_ADD, RTCWKDAY_ADD, 3);
	}
}
//-----------------------------------------------------

//-----------------------------------------------------
//	This function Reset PwFail Bit in RTCWKDAY register
void MCP79410::ResetPwFailBit(void) {
	ResetSingleBit(RTCC_HW_ADD, RTCWKDAY_ADD, 4);
}
//-----------------------------------------------------
//=========================================================================

//=========================================================================
//-----------------------------------------------------
//	This function Set 12 or 24 Hour Time Format Bit in ALARMxHOUR register
//	SetHourType => "1" 12 Hour; "0" 24 Hour
//	Alarm0_1 => "1" Alarm 1; "0" Alarm 0
void MCP79410::AlarmHour12or24TimeFormatBit(uint8_t SetHourType, uint8_t Alarm0_1) {
	if (SetHourType != 0) {
		if (Alarm0_1 != 0) {
			SetSingleBit(RTCC_HW_ADD, ALM1HOUR_ADD, 6);	
		} else {
			SetSingleBit(RTCC_HW_ADD, ALM0HOUR_ADD, 6);
		}
	} else {
		if (Alarm0_1 != 0) {
			ResetSingleBit(RTCC_HW_ADD, ALM1HOUR_ADD, 6);
		} else {
			ResetSingleBit(RTCC_HW_ADD, ALM0HOUR_ADD, 6);
		}
	}
}
//-----------------------------------------------------

//-----------------------------------------------------
//	This function Set AM or PM Bit in ALARMxHOUR register
//	SetAmPm => "1" PM; "0" AM
//	Alarm0_1 => "1" Alarm 1; "0" Alarm 0
void MCP79410::AlarmAmPmBit(uint8_t SetAmPm, uint8_t Alarm0_1) {
	if (SetAmPm != 0) {
		if (Alarm0_1 != 0) {
			SetSingleBit(RTCC_HW_ADD, ALM1HOUR_ADD, 5);
		} else {
			SetSingleBit(RTCC_HW_ADD, ALM0HOUR_ADD, 5);
		}
	} else {
		if (Alarm0_1 != 0) {
			ResetSingleBit(RTCC_HW_ADD, ALM1HOUR_ADD, 5);
		} else {
			ResetSingleBit(RTCC_HW_ADD, ALM0HOUR_ADD, 5);
		}
	}
}
//-----------------------------------------------------
//=========================================================================

//=========================================================================
//-----------------------------------------------------
//	This function Set/Reset ALMPOL Bit in ALARMxWKDAY register
//	SetReset => "1" Set; "0" Reset
//	Alarm0_1 => "1" Alarm 1; "0" Alarm 0
void MCP79410::AlarmIntOutputPolarityBit(uint8_t SetReset, uint8_t Alarm0_1) {
	if (SetReset != 0) {
		if (Alarm0_1 != 0) {
			SetSingleBit(RTCC_HW_ADD, ALM1WKDAY_ADD, 7);
		} else {
			SetSingleBit(RTCC_HW_ADD, ALM0WKDAY_ADD, 7);
		}
	} else {
		if (Alarm0_1 != 0) {
			ResetSingleBit(RTCC_HW_ADD, ALM1WKDAY_ADD, 7);
		} else {
			ResetSingleBit(RTCC_HW_ADD, ALM0WKDAY_ADD, 7);
		}
	}
}
//-----------------------------------------------------

//-----------------------------------------------------
//	This function Set ALMxMSK Bits in ALARMxWKDAY register
//	Alarm0_1 => "1" Alarm 1; "0" Alarm 0
//	Mask     => 000 (0) -> Seconds match
//              001 (1) -> Minutes match
// 			    010 (2) -> Hours match
//  		    011 (3) -> Day of week match
//              100 (4) -> Date match
//              101 (5) -> Reserved
//    		    110 (6) -> Reserved
// 			    111 (7) -> Seconds, Minutes, Hour, Day of Week, Date and Month match
void MCP79410::AlarmMaskBit(uint8_t Alarm0_1, uint8_t Mask) {
	uint8_t RegData;
	if (Alarm0_1 != 0) {
		RegData = ReadSingleReg(RTCC_HW_ADD, ALM1WKDAY_ADD);
		RegData &= 0x8F;
		RegData |= Mask;
		WriteSingleReg(RTCC_HW_ADD, ALM1WKDAY_ADD, RegData);
	} else {
		RegData = ReadSingleReg(RTCC_HW_ADD, ALM0WKDAY_ADD);
		RegData &= 0x8F;
		RegData |= Mask;
		WriteSingleReg(RTCC_HW_ADD, ALM0WKDAY_ADD, RegData);
	}
}
//-----------------------------------------------------

//-----------------------------------------------------
//	This function Reset ALMxIF Bit in ALARMxWKDAY register
//	Alarm0_1 => "1" Alarm 1; "0" Alarm 0
void MCP79410::ResetAlarmIntFlagBit(uint8_t Alarm0_1) {	
	if (Alarm0_1 != 0) {
		ResetSingleBit(RTCC_HW_ADD, ALM1WKDAY_ADD, 3);
	} else {
		ResetSingleBit(RTCC_HW_ADD, ALM0WKDAY_ADD, 3);
	}
}
//-----------------------------------------------------
//=========================================================================

//=========================================================================
//-----------------------------------------------------
//	This function Set 12 or 24 Hour Time Format Bit in PWRxxHOUR register
//	SetHourType => "1" 12 Hour; "0" 24 Hour
//	PowerDownUp => "1" PowerDown; "0" PowerUp
void MCP79410::PowerHour12or24TimeFormatBit(uint8_t SetHourType, uint8_t PowerDownUp) {
	if (SetHourType != 0) {
		if (PowerDownUp != 0) {
			SetSingleBit(RTCC_HW_ADD, PWRDWHOUR_ADD, 6);	
		} else {
			SetSingleBit(RTCC_HW_ADD, PWRUPHOUR_ADD, 6);
		}
	} else {
		if (PowerDownUp != 0) {
			ResetSingleBit(RTCC_HW_ADD, PWRDWHOUR_ADD, 6);
		} else {
			ResetSingleBit(RTCC_HW_ADD, PWRUPHOUR_ADD, 6);
		}
	}
}
//-----------------------------------------------------

//-----------------------------------------------------
//	This function Set AM or PM Bit in ALARMxHOUR register
//	SetAmPm => "1" PM; "0" AM
//	PowerDownUp => "1" PowerDown; "0" PowerUp
void MCP79410::PowerAmPmBit(uint8_t SetAmPm, uint8_t PowerDownUp) {
	if (SetAmPm != 0) {
		if (PowerDownUp != 0) {
			SetSingleBit(RTCC_HW_ADD, PWRDWHOUR_ADD, 5);
		} else {
			SetSingleBit(RTCC_HW_ADD, PWRUPHOUR_ADD, 5);
		}
	} else {
		if (PowerDownUp != 0) {
			ResetSingleBit(RTCC_HW_ADD, PWRDWHOUR_ADD, 5);
		} else {
			ResetSingleBit(RTCC_HW_ADD, PWRUPHOUR_ADD, 5);
		}
	}
}
//-----------------------------------------------------
//=========================================================================

//=========================================================================
//-----------------------------------------------------
//	This function writes TimeKeeping registers
//	Hour12or24Format => "1" 12-Hour Format; "0" 24-Hour Format
void MCP79410::WriteTimeKeeping(uint8_t Hour12or24Format) {
	WriteSingleReg(RTCC_HW_ADD, RTCSEC_ADD, TimeKeeper.Second.SecByte);
	WriteSingleReg(RTCC_HW_ADD, RTCMIN_ADD, TimeKeeper.Minute.MinByte);
	if (Hour12or24Format != 0) {
		WriteSingleReg(RTCC_HW_ADD, RTCHOUR_ADD, TimeKeeper.Hour12.Hour_12Byte);
	} else {
		WriteSingleReg(RTCC_HW_ADD, RTCHOUR_ADD, TimeKeeper.Hour24.Hour_24Byte);
	}
	WriteSingleReg(RTCC_HW_ADD, RTCWKDAY_ADD, TimeKeeper.WeekDay.WkDayByte);
	WriteSingleReg(RTCC_HW_ADD, RTCDATE_ADD,  TimeKeeper.Date.DateByte);
	WriteSingleReg(RTCC_HW_ADD, RTCMTH_ADD,   TimeKeeper.Month.MonthByte);
	WriteSingleReg(RTCC_HW_ADD, RTCYEAR_ADD,  TimeKeeper.Year.YearByte);
}
//-----------------------------------------------------

//-----------------------------------------------------
//	This function reads TimeKeeping registers
void MCP79410::ReadTimeKeeping(void) {
	 TimeKeeper.Second.SecByte     = ReadSingleReg(RTCC_HW_ADD, RTCSEC_ADD);
	 TimeKeeper.Minute.MinByte     = ReadSingleReg(RTCC_HW_ADD, RTCMIN_ADD);
	 TimeKeeper.Hour12.Hour_12Byte = ReadSingleReg(RTCC_HW_ADD, RTCHOUR_ADD);
	 TimeKeeper.Hour24.Hour_24Byte = TimeKeeper.Hour12.Hour_12Byte;
	 TimeKeeper.WeekDay.WkDayByte  = ReadSingleReg(RTCC_HW_ADD, RTCWKDAY_ADD);
	 TimeKeeper.Date.DateByte      = ReadSingleReg(RTCC_HW_ADD, RTCDATE_ADD);
	 TimeKeeper.Month.MonthByte    = ReadSingleReg(RTCC_HW_ADD, RTCMTH_ADD);
	 TimeKeeper.Year.YearByte      = ReadSingleReg(RTCC_HW_ADD, RTCYEAR_ADD);
}
//-----------------------------------------------------

//-----------------------------------------------------
//	This function writes Alarm 0-1 registers
//	Alarm0_1 => "1" Alarm 1; "0" Alarm 0
//	Hour12or24Format => "1" 12-Hour Format; "0" 24-Hour Format
void MCP79410::WriteAlarmRegister(uint8_t Alarm0_1, uint8_t Hour12or24Format) {
	if (Alarm0_1 != 0) {
		WriteSingleReg(RTCC_HW_ADD, ALM1SEC_ADD, Alarm[1].Second.SecByte);
		WriteSingleReg(RTCC_HW_ADD, ALM1MIN_ADD, Alarm[1].Minute.MinByte);
		if (Hour12or24Format != 0) {		
			WriteSingleReg(RTCC_HW_ADD, ALM1HOUR_ADD, Alarm[1].Hour12.Hour_12Byte);
		} else {
			WriteSingleReg(RTCC_HW_ADD, ALM1HOUR_ADD, Alarm[1].Hour24.Hour_24Byte);
		}
		WriteSingleReg(RTCC_HW_ADD, ALM1WKDAY_ADD, Alarm[1].WeekDay.WkDayByte);
		WriteSingleReg(RTCC_HW_ADD, ALM1DATE_ADD,  Alarm[1].Date.DateByte);
		WriteSingleReg(RTCC_HW_ADD, ALM1MTH_ADD,   Alarm[1].Month.MonthByte);
	} else {
		WriteSingleReg(RTCC_HW_ADD, ALM0SEC_ADD, Alarm[0].Second.SecByte);
		WriteSingleReg(RTCC_HW_ADD, ALM0MIN_ADD, Alarm[0].Minute.MinByte);
		if (Hour12or24Format != 0) {		
			WriteSingleReg(RTCC_HW_ADD, ALM0HOUR_ADD, Alarm[0].Hour12.Hour_12Byte);
		} else {
			WriteSingleReg(RTCC_HW_ADD, ALM0HOUR_ADD, Alarm[0].Hour24.Hour_24Byte);
		}
		WriteSingleReg(RTCC_HW_ADD, ALM0WKDAY_ADD, Alarm[0].WeekDay.WkDayByte);
		WriteSingleReg(RTCC_HW_ADD, ALM0DATE_ADD,  Alarm[0].Date.DateByte);
		WriteSingleReg(RTCC_HW_ADD, ALM0MTH_ADD,   Alarm[0].Month.MonthByte);		
	}
}
//-----------------------------------------------------

//-----------------------------------------------------
//	This function reads alarm 0-1 registers
//	Alarm0_1 => "1" Alarm 1; "0" Alarm 0
void MCP79410::ReadAlarmRegister(uint8_t Alarm0_1) {
	if (Alarm0_1 != 0) {
		Alarm[1].Second.SecByte     = ReadSingleReg(RTCC_HW_ADD, ALM1SEC_ADD);
		Alarm[1].Minute.MinByte     = ReadSingleReg(RTCC_HW_ADD, ALM1MIN_ADD);
		Alarm[1].Hour12.Hour_12Byte = ReadSingleReg(RTCC_HW_ADD, ALM1HOUR_ADD);
		Alarm[1].Hour24.Hour_24Byte = Alarm[1].Hour12.Hour_12Byte;
		Alarm[1].WeekDay.WkDayByte  = ReadSingleReg(RTCC_HW_ADD, ALM1WKDAY_ADD);
		Alarm[1].Date.DateByte      = ReadSingleReg(RTCC_HW_ADD, ALM1DATE_ADD);
		Alarm[1].Month.MonthByte    = ReadSingleReg(RTCC_HW_ADD, ALM1MTH_ADD);		
	} else {
		Alarm[0].Second.SecByte     = ReadSingleReg(RTCC_HW_ADD, ALM0SEC_ADD);
		Alarm[0].Minute.MinByte     = ReadSingleReg(RTCC_HW_ADD, ALM0MIN_ADD);
		Alarm[0].Hour12.Hour_12Byte = ReadSingleReg(RTCC_HW_ADD, ALM0HOUR_ADD);
		Alarm[0].Hour24.Hour_24Byte = Alarm[0].Hour12.Hour_12Byte;
		Alarm[0].WeekDay.WkDayByte  = ReadSingleReg(RTCC_HW_ADD, ALM0WKDAY_ADD);
		Alarm[0].Date.DateByte      = ReadSingleReg(RTCC_HW_ADD, ALM0DATE_ADD);
		Alarm[0].Month.MonthByte    = ReadSingleReg(RTCC_HW_ADD, ALM0MTH_ADD);		
	}
}
//-----------------------------------------------------

//-----------------------------------------------------
//	This function writes PowerDown-Up registers
//	PowerDownUp => "1" PowerDown; "0" PowerUp
//	Hour12or24Format => "1" 12-Hour Format; "0" 24-Hour Format
void MCP79410::WritePowerDownUpRegister(uint8_t PowerDownUp, uint8_t Hour12or24Format) {
	if (PowerDownUp != 0) {
		WriteSingleReg(RTCC_HW_ADD, PWRDWMIN_ADD, Power[1].Minute.MinByte);
		if (Hour12or24Format != 0) {
			WriteSingleReg(RTCC_HW_ADD, PWRDWHOUR_ADD, Power[1].Hour12.Hour_12Byte);
		} else {
			WriteSingleReg(RTCC_HW_ADD, PWRDWHOUR_ADD, Power[1].Hour24.Hour_24Byte);
		}
		WriteSingleReg(RTCC_HW_ADD, PWRDWDATE_ADD, Power[1].Date.DateByte);
		WriteSingleReg(RTCC_HW_ADD, PWRDWMTH_ADD,  Power[1].Month.MonthByte);
	} else {
		WriteSingleReg(RTCC_HW_ADD, PWRUPMIN_ADD, Power[0].Minute.MinByte);
		if (Hour12or24Format != 0) {
			WriteSingleReg(RTCC_HW_ADD, PWRUPHOUR_ADD, Power[0].Hour12.Hour_12Byte);
		} else {
			WriteSingleReg(RTCC_HW_ADD, PWRUPHOUR_ADD, Power[0].Hour24.Hour_24Byte);
		}
		WriteSingleReg(RTCC_HW_ADD, PWRUPDATE_ADD, Power[0].Date.DateByte);
		WriteSingleReg(RTCC_HW_ADD, PWRUPMTH_ADD,  Power[0].Month.MonthByte);		
	}
}
//-----------------------------------------------------

//-----------------------------------------------------
//	This function reads PowerDown-Up registers
//	PowerDownUp => "1" PowerDown; "0" PowerUp
void MCP79410::ReadPowerDownUpRegister(uint8_t PowerDownUp) {
	if (PowerDownUp != 0) {
		Power[1].Minute.MinByte     = ReadSingleReg(RTCC_HW_ADD, PWRDWMIN_ADD);
		Power[1].Hour12.Hour_12Byte = ReadSingleReg(RTCC_HW_ADD, PWRDWHOUR_ADD);
		Power[1].Hour24.Hour_24Byte = Power[1].Hour12.Hour_12Byte;
		Power[1].Date.DateByte      = ReadSingleReg(RTCC_HW_ADD, PWRDWDATE_ADD);
		Power[1].Month.MonthByte    = ReadSingleReg(RTCC_HW_ADD, PWRDWMTH_ADD);		
	} else {
		Power[0].Minute.MinByte     = ReadSingleReg(RTCC_HW_ADD, PWRUPMIN_ADD);
		Power[0].Hour12.Hour_12Byte = ReadSingleReg(RTCC_HW_ADD, PWRUPHOUR_ADD);
		Power[0].Hour24.Hour_24Byte = Power[0].Hour12.Hour_12Byte;
		Power[0].Date.DateByte      = ReadSingleReg(RTCC_HW_ADD, PWRUPDATE_ADD);
		Power[0].Month.MonthByte    = ReadSingleReg(RTCC_HW_ADD, PWRUPMTH_ADD);		
	}
}
//-----------------------------------------------------
//=========================================================================
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
//==============================
//	SRAM/EEPROM DEDICATED FUNCTIONS
//==============================

//-----------------------------------------------------
//	This function is useful to protect the 128 Byte EEPROM space.
//	To protect EEPROM spcae is necessary write a correct value into 
//	the "Eeprom Block Protection control register"
//==============================
//	128 Byte of EEPROM 
//	Start Address -> 0x00
//	End   Address -> 0x7F
//	Control Byte  -> 0xAE (Shifted value is 0x57)
//==============================
void MCP79410::Set_EEPROM_WriteProtection(uint8_t Section) {
	uint8_t TempReg;
	
	switch(Section)
	{
		case 0:		//	No Write Protected
			TempReg = 0x00;
			break;
		
		case 1:		//	Protect Upper 1/4 (0x60 to 0x7F)
			TempReg = 0x04;	
			break;

		case 2:		//	Protect Upper 1/2 (0x40 to 0x7F)
			TempReg = 0x08;	
			break;

		case 3:		//	Protect all
			TempReg = 0x0C;
			break;

		default:
			TempReg = 0x00;
			break;			
	}
	WriteSingleReg(EEPROM_HW_ADD, EEPROM_BLOCK_PROTECTION_ADD, TempReg);
}
//-----------------------------------------------------

//-----------------------------------------------------
//	This function is useful to write a byte into a protected EEPROM space.
//==============================
//	8 Byte of protected EEPROM 
//	Start Address -> 0xF0
//	End   Address -> 0xF7
//	Control Byte  -> 0xAE (Shifted value is 0x57)
//==============================
void MCP79410::WriteProtected_EEPROM(uint8_t RegAdd, uint8_t RegData) {
	WriteSingleReg(RTCC_HW_ADD, EEUNLOCK_ADD, 0x55);
	WriteSingleReg(RTCC_HW_ADD, EEUNLOCK_ADD, 0xAA);
	WriteSingleReg(EEPROM_HW_ADD, RegAdd, RegData);
	//WriteSingleReg(EEPROM_HW_ADD, ((RegAdd & 0x07) | 0xF0), RegData);
}
//-----------------------------------------------------

//==============================
//	64 Byte of SRAM 
//	Start Address -> 0x20
//	End   Address -> 0x5F
//	Control Byte  -> 0xDE (Shifted value is 0x6F)
//==============================

//----------------------------------------------------------------------------------------------------------