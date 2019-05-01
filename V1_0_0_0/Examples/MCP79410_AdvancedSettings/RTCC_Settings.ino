//======================================================================
//  (TimeKeeper) Key words used to set Date and Time
//
//  TimeSet:                ->  Sets time. Example:
//                          ->  12:24:32       24 Hour Format
//                          ->  10:15:10 - AM  12 Hour Format
//                          ->  09:45:14 - PM  12 Hour Format
//  DateSet:                ->  Sets Date. Example:
//                          ->  01/01/2001 - MON
//                          ->  01/01/2001 - TUE
//                          ->  01/01/2001 - WED
//                          ->  01/01/2001 - THU
//                          ->  01/01/2001 - FRI
//                          ->  01/01/2001 - SAT
//                          ->  01/01/2001 - SUN
//
//  (Alarm 0/1) Key words used to set Date and Time Alarm
//
//  ResetAlarm(x):          This command resets alarm. (x=0 Alarm 0 - x=1 Alarm 1)
//
//  DisableAlarm(x):        This command disables alarm. (x=0 Alarm 0 - x=1 Alarm 1)
//
//  EnableAlarm(x):         This command enables alarm. (x=0 Alarm 0 - x=1 Alarm 1)
//
//  TimeSetAlarm(x):        ->  Sets time. Example (x=0 Alarm 0 - x=1 Alarm 1):
//                          ->  12:24:32       24 Hour Format
//                          ->  10:15:10 - AM  12 Hour Format
//                          ->  09:45:14 - PM  12 Hour Format
//
//  DateSetAlarm(x):        ->  Sets Date. Example (x=0 Alarm 0 - x=1 Alarm 1):
//                          ->  01/01/2001 - MON - ALMxMASK - INTPOL
//                          ->  01/01/2001 - TUE - ALMxMASK - INTPOL
//                          ->  01/01/2001 - WED - ALMxMASK - INTPOL
//                          ->  01/01/2001 - THU - ALMxMASK - INTPOL
//                          ->  01/01/2001 - FRI - ALMxMASK - INTPOL
//                          ->  01/01/2001 - SAT - ALMxMASK - INTPOL
//                          ->  01/01/2001 - SUN - ALMxMASK - INTPOL
//
// Where ALMxMASK is:       -> SecondsMatch
//                          -> MinutesMatch
//                          -> HoursMatch
//                          -> DayOfWeekMatch
//                          -> DateMatch
//                          -> AllMatch
//
// Where INTPOL is:         -> LHL (Logic High Level)
//                          -> LLL (Logic Low Level)
//
//----------------------------------------------------------------------
//  Examples:
//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
//  How to set Date and Time for TimeKeeper:
//
//  First  Step -> To set Date use this command: "DateSet: 11/12/2015 - FRI"  (With this command the user set the Date and the WeekDay)
//  Second Step -> To set Time use this command: "TimeSet: 15:30:22"          (With this command the user set the Time in 24H format)
//                                               "TimeSet: 03:30:22 - PM"     (With this command the user set the Time in 12H format)
//
//  The Second command freeze the data into timekeeper registers
//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
//  How to set Alarm 0 or 1
//
//  First  Step -> To set Date for Alarm 0 use this command: "DateSetAlarm(0): 11/12/2015 - FRI - SecondsMatch - LHL" (With this command the user set the Alarm Date, Alarm WeekDay, Alarm Mask bit and Alarm Level Logic)
//  Second Step -> To set Time for Alarm 0 use this command: "TimeSetAlarm(0): 15:30:22"                              (With this command the user set the Alarm Time in 24H format)
//                                                           "TimeSetAlarm(0): 03:30:22 - PM"                         (With this command the user set the Alarm Time in 12H format)
//
//  The Second command freeze the data into Alarm 0 or 1 registers
//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
//  How To Reset Alarm settings
//  
//  ResetAlarm(0):  (With this command the user reset the Alarm 0 configuration)
//  ResetAlarm(1):  (With this command the user reset the Alarm 1 configuration)
//
//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
//  How To Enable/Disable Alarm
//
//  EnableAlarm(0):  (With this command the user Enable Alarm 0)
//  EnableAlarm(1):  (With this command the user Enable Alarm 1)
//
//  DisableAlarm(0): (With this command the user Disable Alarm 0)
//  DisableAlarm(1): (With this command the user Disable Alarm 1)
//
//----------------------------------------------------------------------
//======================================================================

//======================================================================
void RTCC_SetupIdle(void) {
  if (StartTimeKeeperSettings == true) {
    RTCC_Config_Management = RTCC_TimeKeeperSettings;
    ReadStringFLASH((uint8_t *)Sep1, strlen(Sep1), TRUE, FALSE);
    ReadStringFLASH((uint8_t *)StartTimeKeeperSet, strlen(StartTimeKeeperSet), TRUE, FALSE);
    ReadStringFLASH((uint8_t *)Sep1, strlen(Sep1), TRUE, FALSE);
    Serial.println("");
  } else if (StartAlarmSettings == true) {
    RTCC_Config_Management = RTCC_Alarm_Settings; 
    ReadStringFLASH((uint8_t *)Sep1, strlen(Sep1), TRUE, FALSE);
    ReadStringFLASH((uint8_t *)StartAlarmSet, strlen(StartAlarmSet), TRUE, FALSE);
    ReadStringFLASH((uint8_t *)Sep1, strlen(Sep1), TRUE, FALSE);
    Serial.println("");
  }
}
//======================================================================

//======================================================================
//  State to set TimeKeeper registers
void RTCC_TimeKeeperSettings(void) {
  if (StartTimeKeeperSettings == false) {
    RTCC_Config_Management = RTCC_SetupIdle;
    ReadStringFLASH((uint8_t *)Sep1, strlen(Sep1), TRUE, FALSE);
    ReadStringFLASH((uint8_t *)StopTimeKeeperSet, strlen(StopTimeKeeperSet), TRUE, FALSE);
    ReadStringFLASH((uint8_t *)Sep1, strlen(Sep1), TRUE, FALSE);
    Serial.println("");
  }  
  if (Serial.available() > 0) {
    ReadSerialCmd();
    StringIndex = CheckChar(':');
    if (StringIndex > 0) {
      TempString = SerialInput.substring(0, StringIndex);
      if (TempString.compareTo(ReadStringFLASH((uint8_t *)TimeSet, strlen(TimeSet), TRUE, TRUE)) == 0) {
        //  Received command "TimeSet: hh:mm:ss or hh:mm - AM/PM"
        TimeKeeper_Alarm_TimeStamp = TIMEKEEPER_CASE;  //  TimeKeeper Setting
        RTCC_Config_Management = RTCC_TimeSet;
      } else if (TempString.compareTo(ReadStringFLASH((uint8_t *)DateSet, strlen(DateSet), TRUE, TRUE)) == 0) {
        //  Received command "DateSet: 01/01/2001 - MON"
        TimeKeeper_Alarm_TimeStamp = TIMEKEEPER_CASE;  //  DateKeeper Setting
        RTCC_Config_Management = RTCC_DateSet;        
      } else {
        ReadStringFLASH((uint8_t *)cmdError, strlen(cmdError), TRUE, FALSE); 
        return;        
      }
    } else {
      ReadStringFLASH((uint8_t *)cmdError, strlen(cmdError), TRUE, FALSE); 
      return;
    }
  }
}
//======================================================================

//======================================================================
//  State to set alarm registers or reset the alarm configuration
void RTCC_Alarm_Settings(void) {
  if (StartAlarmSettings == false) {
    RTCC_Config_Management = RTCC_SetupIdle;
    ReadStringFLASH((uint8_t *)Sep1, strlen(Sep1), TRUE, FALSE);
    ReadStringFLASH((uint8_t *)StopAlarmSet, strlen(StopAlarmSet), TRUE, FALSE);
    ReadStringFLASH((uint8_t *)Sep1, strlen(Sep1), TRUE, FALSE);
    Serial.println("");
  }
  if (Serial.available() > 0) {
    ReadSerialCmd();
    StringIndex = CheckChar(':');
    if (StringIndex > 0) {
      TempString = SerialInput.substring(0, StringIndex - 3);
      AlarmIndex = SerialInput.substring((CheckChar('(') + 1), CheckChar(')')).toInt();
      if (TempString.compareTo(ReadStringFLASH((uint8_t *)TimeAlarmSet, strlen(TimeAlarmSet), TRUE, TRUE)) == 0) {
        //  Received command "TimeSetAlarm(x): hh:mm:ss or hh:mm - AM/PM"
        TimeKeeper_Alarm_TimeStamp = ALARM_CASE;  //  Time Alarm Setting
        RTCC_Config_Management = RTCC_TimeSet;
      } else if (TempString.compareTo(ReadStringFLASH((uint8_t *)DateAlarmSet, strlen(DateAlarmSet), TRUE, TRUE)) == 0) {
        //  Received command "DateSetAlarm(x): 01/01/2001 - MON - ALMxMASK"
        TimeKeeper_Alarm_TimeStamp = ALARM_CASE;  //  Date Alarm Setting
        RTCC_Config_Management = RTCC_DateSet;
      } else if (TempString.compareTo(ReadStringFLASH((uint8_t *)ResetAlarm, strlen(ResetAlarm), TRUE, TRUE)) == 0) {
        //  Reset alarm x. (x=0 Alarm 0 - x=1 Alarm 1)
        ResetAlarm_x(AlarmIndex);
      } else if (TempString.compareTo(ReadStringFLASH((uint8_t *)DisableAlarm, strlen(DisableAlarm), TRUE, TRUE)) == 0) {
        //  Disable alarm x. (x=0 Alarm 0 - x=1 Alarm 1)
        DisableAlarm_x(AlarmIndex);
      } else if (TempString.compareTo(ReadStringFLASH((uint8_t *)EnableAlarm, strlen(EnableAlarm), TRUE, TRUE)) == 0) {
        //  Enable alarm x. (x=0 Alarm 0 - x=1 Alarm 1)
        EnableAlarm_x(AlarmIndex);
      } else {
        ReadStringFLASH((uint8_t *)cmdError, strlen(cmdError), TRUE, FALSE); 
        return;        
      }      
    } else {
      ReadStringFLASH((uint8_t *)cmdError, strlen(cmdError), TRUE, FALSE); 
      return;
    }   
  }  
}
//======================================================================

//======================================================================
//  This function resets the alarm settings
void ResetAlarm_x(uint8_t Index) {
  mcp79410.Alarm[Index].Second.SecByte     = 0;
  mcp79410.Alarm[Index].Minute.MinByte     = 0;
  mcp79410.Alarm[Index].Hour12.Hour_12Byte = 0;
  mcp79410.Alarm[Index].Hour24.Hour_24Byte = 0;
  mcp79410.Alarm[Index].WeekDay.WkDayByte  = 0;
  mcp79410.Alarm[Index].Date.DateByte      = 0;
  mcp79410.Alarm[Index].Month.MonthByte    = 0;
  mcp79410.WriteAlarmRegister(Index, 0);
  if (Index == 0) {
    mcp79410.Alarm0Bit(0);  //  Disable Alarm 0
  } else {
    mcp79410.Alarm1Bit(0);  //  Disable Alarm 1    
  }
}
//======================================================================

//======================================================================
//  This function disables alarm x
void DisableAlarm_x(uint8_t Index) {
  if (Index == 0) {
    mcp79410.Alarm0Bit(0);  //  Disable Alarm 0
  } else {
    mcp79410.Alarm1Bit(0);  //  Disable Alarm 1    
  }
}
//======================================================================

//======================================================================
//  This function disables alarm x
void EnableAlarm_x(uint8_t Index) {
  if (Index == 0) {
    mcp79410.Alarm0Bit(1);  //  Enable Alarm 0
  } else {
    mcp79410.Alarm1Bit(1);  //  Enable Alarm 1    
  }
}
//======================================================================

//======================================================================
//  Decode Command "DateSet: gg/mm/yyyy - MON"  
void RTCC_DateSet(void) {
  SerialInput.remove(0, StringIndex + 1);
  SerialInput.trim();
  ExtractDate(TimeKeeper_Alarm_TimeStamp, AlarmIndex);
  
  if (StartTimeKeeperSettings == true) {
    RTCC_Config_Management = RTCC_TimeKeeperSettings;
  } else if (StartAlarmSettings == true) {
    RTCC_Config_Management = RTCC_Alarm_Settings; 
  }
}
//======================================================================

//======================================================================
//  Decode Command "TimeSet: hh:mm:ss or hh:mm - AM/PM"
void RTCC_TimeSet(void) {
  uint8_t  TempValue;

  SerialInput.remove(0, StringIndex + 1);
  SerialInput.trim();
  
  //  Check if in the string is located the string AM or PM
  TempValue = Check_AMorPM();
  if (TempValue == 1) {
    //  AM string founded - 12H format
    if (ExtractTime(1, TimeKeeper_Alarm_TimeStamp, AlarmIndex) == 1) {
      //  TimeSet OK - Write RTCC Registers
      switch (TimeKeeper_Alarm_TimeStamp) {
        case TIMEKEEPER_CASE:
          mcp79410.WriteTimeKeeping(1);
          break;
        case ALARM_CASE:
          mcp79410.WriteAlarmRegister(AlarmIndex, 1);
          if (AlarmIndex == 0) {
            mcp79410.Alarm0Bit(1);  //  Enable Alarm 0
          } else {
            mcp79410.Alarm1Bit(1);  //  Enable Alarm 1    
          }
          break;
        default:
          break;
      }      
    }
  } else if (TempValue == 2) {
    //  PM string founded - 12H format
    if (ExtractTime(2, TimeKeeper_Alarm_TimeStamp, AlarmIndex) == 1) {
      //  TimeSet OK - Write RTCC Registers
      switch (TimeKeeper_Alarm_TimeStamp) {
        case TIMEKEEPER_CASE:
          mcp79410.WriteTimeKeeping(1);
          break;
        case ALARM_CASE:
          mcp79410.WriteAlarmRegister(AlarmIndex, 1);
          if (AlarmIndex == 0) {
            mcp79410.Alarm0Bit(1);  //  Enable Alarm 0
          } else {
            mcp79410.Alarm1Bit(1);  //  Enable Alarm 1    
          }
          break;
        default:
          break;
      }  
    }
  } else {
    //  No AM or PM string. 24H format
    if (ExtractTime(0, TimeKeeper_Alarm_TimeStamp, AlarmIndex) == 1) {
      //  TimeSet OK - Write RTCC Registers
      switch (TimeKeeper_Alarm_TimeStamp) {
        case TIMEKEEPER_CASE:
          mcp79410.WriteTimeKeeping(0);
          break;
        case ALARM_CASE:
          mcp79410.WriteAlarmRegister(AlarmIndex, 0);
          if (AlarmIndex == 0) {
            mcp79410.Alarm0Bit(1);  //  Enable Alarm 0
          } else {
            mcp79410.Alarm1Bit(1);  //  Enable Alarm 1    
          }
          break;
        default:
          break;
      }
    }
  }
  
  if (StartTimeKeeperSettings == true) {
    RTCC_Config_Management = RTCC_TimeKeeperSettings;
  } else if (StartAlarmSettings == true) {
    RTCC_Config_Management = RTCC_Alarm_Settings; 
  }
}
//======================================================================

//======================================================================
void ReadSerialCmd(void) {
  SerialInput = "";
  SerialInput = Serial.readString();
  SerialInput.trim();
  ReadStringFLASH((uint8_t *)cmdReceived, strlen(cmdReceived), FALSE, FALSE);
  Serial.println(SerialInput);
}
//======================================================================

//======================================================================
//  This function extract the Date to set TimeKeeper, Alarm or TimeStamp
//  The "TimeKeeprAlarm" identify:
//  - "0" -> TimeKeepr
//  - "1" -> Alarm
//  The "AlarmTimeIndex" identify:
//  - "0" -> Array index 0
//  - "1" -> Array index 1
uint8_t ExtractDate(uint8_t TimeKeeprAlarm, uint8_t AlarmIndex) {
  uint16_t  TempValue;
  
  //-----------------------------------------
  //  Extract Days  
  StringIndex = CheckChar('/');
  if (StringIndex > 0) {
    TempValue = CheckDaysMonthsAndYears(0);
    if (TempValue == 255) {
      //  Syntax Error
      ReadStringFLASH((uint8_t *)cmdError, strlen(cmdError), TRUE, FALSE);
      return(0);
    } else if (TempValue == 0) {
      TempValue = 1;
    } else if (TempValue > MAX_DAYS) {
      TempValue = MAX_DAYS;
    }
    switch (TimeKeeprAlarm) {
      case TIMEKEEPER_CASE:
        mcp79410.TimeKeeper.Date.DateBit.DateOne = (TempValue % 10);
        mcp79410.TimeKeeper.Date.DateBit.DateTen = (TempValue / 10);
        break; 
      case ALARM_CASE:
        mcp79410.Alarm[AlarmIndex].Date.DateBit.DateOne = (TempValue % 10);
        mcp79410.Alarm[AlarmIndex].Date.DateBit.DateTen = (TempValue / 10);
        break;
      default:
        break;
    }
  } else {
    ReadStringFLASH((uint8_t *)cmdError, strlen(cmdError), TRUE, FALSE);
    RTCC_Config_Management = RTCC_TimeKeeperSettings;
    return(0);
  }
  //-----------------------------------------
  
  //-----------------------------------------
  //  Extract Months
  SerialInput.remove(0, StringIndex + 1);
  StringIndex = CheckChar('/');
  if (StringIndex > 0) {
    TempValue = CheckDaysMonthsAndYears(1);
    if (TempValue == 255) {
      //  Syntax Error
      ReadStringFLASH((uint8_t *)cmdError, strlen(cmdError), TRUE, FALSE);
      return(0);
    } else if (TempValue == 0) {
      TempValue = 1;
    } else if (TempValue > MAX_MONTHS) {
      TempValue = MAX_MONTHS;
    }
    switch (TimeKeeprAlarm) {
      case TIMEKEEPER_CASE:
        mcp79410.TimeKeeper.Month.MonthBit.MonthOne = (TempValue % 10);
        mcp79410.TimeKeeper.Month.MonthBit.MonthTen = (TempValue / 10);
        break;   
      case ALARM_CASE:
        mcp79410.Alarm[AlarmIndex].Month.MonthBit.MonthOne = (TempValue % 10);
        mcp79410.Alarm[AlarmIndex].Month.MonthBit.MonthTen = (TempValue / 10);
        break;  
      default:
        break;
    }
  } else {
    ReadStringFLASH((uint8_t *)cmdError, strlen(cmdError), TRUE, FALSE);
    RTCC_Config_Management = RTCC_TimeKeeperSettings;
    return(0);
  }
  //-----------------------------------------  
  
  //-----------------------------------------
  //  Extract Years (Only for Timekeeping settings)
  SerialInput.remove(0, StringIndex + 1);
  StringIndex = CheckChar('-');
  if (StringIndex > 0) {
    TempValue = CheckDaysMonthsAndYears(2);
    if (TempValue == 255) {
      //  Syntax Error
      ReadStringFLASH((uint8_t *)cmdError, strlen(cmdError), TRUE, FALSE);
      return(0);
    }
    TempValue %= 100;
    switch (TimeKeeprAlarm) {
      case TIMEKEEPER_CASE:
        mcp79410.TimeKeeper.Year.YearBit.YearOne = (TempValue % 10);
        mcp79410.TimeKeeper.Year.YearBit.YearTen = (TempValue / 10);
      default:
      break;
    }
  } else {
    ReadStringFLASH((uint8_t *)cmdError, strlen(cmdError), TRUE, FALSE);
    RTCC_Config_Management = RTCC_TimeKeeperSettings;
    return(0);
  }
  //-----------------------------------------  
  
  //-----------------------------------------
  //  Extract Week days
  SerialInput.remove(0, StringIndex + 1);
  SerialInput.trim();
  switch (TimeKeeprAlarm) {
    case TIMEKEEPER_CASE:
      StringIndex = SerialInput.length();
      break;  
    case ALARM_CASE:
      StringIndex = CheckChar('-');
      break;    
    default:
      break;
  }
  if (StringIndex > 0) {
    TempString = SerialInput.substring(0, StringIndex);
    TempString.trim();
    if (TimeKeeprAlarm == TIMEKEEPER_CASE) {
      //  Only for TimeKeeping settings
      mcp79410.TimeKeeper.WeekDay.WkDayBit.VbatEn = 1;
    }
    if (TempString.compareTo(ReadStringFLASH((uint8_t *)Monday, strlen(Monday), TRUE, TRUE)) == 0) {
      //  Monday
      TempValue = 1;
    } else if (TempString.compareTo(ReadStringFLASH((uint8_t *)Tuesday, strlen(Tuesday), TRUE, TRUE)) == 0) {
      //  Tuesday
      TempValue = 2;
    } else if (TempString.compareTo(ReadStringFLASH((uint8_t *)Wednesday, strlen(Wednesday), TRUE, TRUE)) == 0) {
      //  Wednesday
      TempValue = 3;      
    } else if (TempString.compareTo(ReadStringFLASH((uint8_t *)Thursday, strlen(Thursday), TRUE, TRUE)) == 0) {
      //  Thursday
      TempValue = 4;      
    } else if (TempString.compareTo(ReadStringFLASH((uint8_t *)Friday, strlen(Friday), TRUE, TRUE)) == 0) {
      //  Friday
      TempValue = 5;      
    } else if (TempString.compareTo(ReadStringFLASH((uint8_t *)Saturday, strlen(Saturday), TRUE, TRUE)) == 0) {
      //  Saturday
      TempValue = 6;   
    } else if (TempString.compareTo(ReadStringFLASH((uint8_t *)Sunday, strlen(Sunday), TRUE, TRUE)) == 0) {
      //  Sunday
      TempValue = 7;
    } else {
      ReadStringFLASH((uint8_t *)WeekDayError, strlen(WeekDayError), TRUE, FALSE);
      RTCC_Config_Management = RTCC_TimeKeeperSettings;
      return(0);      
    }    
    switch (TimeKeeprAlarm) {
      case TIMEKEEPER_CASE:
        mcp79410.TimeKeeper.WeekDay.WkDayBit.WkDay = TempValue;
        break;  
      case ALARM_CASE:
        mcp79410.Alarm[AlarmIndex].WeekDay.WkDayBit.WkDay = TempValue;
        break;    
      default:
        break;
    }   
  } else {
    ReadStringFLASH((uint8_t *)cmdError, strlen(cmdError), TRUE, FALSE);
    RTCC_Config_Management = RTCC_TimeKeeperSettings;
    return(0);
  }      
  //-----------------------------------------

  //-----------------------------------------
  //  Extract Alarm Mask - Only for alarm setting procedure  
  if (TimeKeeprAlarm == ALARM_CASE) {
    SerialInput.remove(0, StringIndex + 1);
    SerialInput.trim();
    StringIndex = CheckChar('-');
    if (StringIndex > 0) {
      TempString = SerialInput.substring(0, StringIndex);
      TempString.trim();   
      if (TempString.compareTo(ReadStringFLASH((uint8_t *)SecondsMatch, strlen(SecondsMatch), TRUE, TRUE)) == 0) {
        //  Seconds Match
        mcp79410.Alarm[AlarmIndex].WeekDay.WkDayBit.AlarmMask = 0;
      } else if (TempString.compareTo(ReadStringFLASH((uint8_t *)MinutesMatch, strlen(MinutesMatch), TRUE, TRUE)) == 0) {
        //  Minutes Match
        mcp79410.Alarm[AlarmIndex].WeekDay.WkDayBit.AlarmMask = 1;
      } else if (TempString.compareTo(ReadStringFLASH((uint8_t *)HoursMatch, strlen(HoursMatch), TRUE, TRUE)) == 0) {
        //  Hours Match
        mcp79410.Alarm[AlarmIndex].WeekDay.WkDayBit.AlarmMask = 2;
      } else if (TempString.compareTo(ReadStringFLASH((uint8_t *)DayOfWeekMatch, strlen(DayOfWeekMatch), TRUE, TRUE)) == 0) {
        //  Day Of Week Match
        mcp79410.Alarm[AlarmIndex].WeekDay.WkDayBit.AlarmMask = 3;
      } else if (TempString.compareTo(ReadStringFLASH((uint8_t *)DateMatch, strlen(DateMatch), TRUE, TRUE)) == 0) {
        //  Date Match
        mcp79410.Alarm[AlarmIndex].WeekDay.WkDayBit.AlarmMask = 4;
      } else if (TempString.compareTo(ReadStringFLASH((uint8_t *)AllMatch, strlen(AllMatch), TRUE, TRUE)) == 0) {
        //  All Match
        mcp79410.Alarm[AlarmIndex].WeekDay.WkDayBit.AlarmMask = 7;
      } else {
        ReadStringFLASH((uint8_t *)AlarmMaskError, strlen(AlarmMaskError), TRUE, FALSE);
        RTCC_Config_Management = RTCC_TimeKeeperSettings;
        return(0);      
      }
    } else {
      ReadStringFLASH((uint8_t *)cmdError, strlen(cmdError), TRUE, FALSE);
      RTCC_Config_Management = RTCC_TimeKeeperSettings;
      return(0);
    }
  }
  //-----------------------------------------
  
  //-----------------------------------------
  //  Extract Output Polarity  
  if (TimeKeeprAlarm == ALARM_CASE) {
    SerialInput.remove(0, StringIndex + 1);
    SerialInput.trim();
    StringIndex = SerialInput.length();
    if (StringIndex > 0) {
      SerialInput = SerialInput.substring(0, StringIndex);
      SerialInput.trim();  
      if (SerialInput.compareTo(ReadStringFLASH((uint8_t *)LHL_Alarm, strlen(LHL_Alarm), TRUE, TRUE)) == 0) {
        //  LHL - Logic High Level
        mcp79410.Alarm[AlarmIndex].WeekDay.WkDayBit.AlarmPol = 1;
      } else if (SerialInput.compareTo(ReadStringFLASH((uint8_t *)LLL_Alarm, strlen(LLL_Alarm), TRUE, TRUE)) == 0) {
        //  LLL - Logic Low Level
        mcp79410.Alarm[AlarmIndex].WeekDay.WkDayBit.AlarmPol = 0;
      } else {
        ReadStringFLASH((uint8_t *)MfpPollError, strlen(MfpPollError), TRUE, FALSE);
        RTCC_Config_Management = RTCC_TimeKeeperSettings;
        return(0);      
      }            
    } else {
      ReadStringFLASH((uint8_t *)cmdError, strlen(cmdError), TRUE, FALSE);
      RTCC_Config_Management = RTCC_TimeKeeperSettings;
      return(0);
    }    
  }
  //-----------------------------------------
  return(1);
}
//======================================================================

//======================================================================
//  This function extract the Hours, minutes and seconds to set TimeKeeper or Alarm
//  The "HourType" variable identify 12H or 24H format
//  The "TimeKeeprAlarm" identify:
//  - "0" -> TimeKeepr
//  - "1" -> Alarm
//  The "AlarmIndex" identify:
//  - "0" -> Array index 0
//  - "1" -> Array index 1
uint8_t ExtractTime(uint8_t HourType, uint8_t TimeKeeprAlarm, uint8_t AlarmIndex) {
  uint8_t  TempValue;
    
  if ((HourType == 1) || (HourType == 2)) {
    //-----------------------------------------
    //  Extract Hours - 12H Format
    StringIndex = CheckChar(':');
    if (StringIndex > 0) {
      TempValue = CheckHoursMinutesAndSeconds(0);
      if (TempValue == 255) {
        //  Syntax Error
        ReadStringFLASH((uint8_t *)cmdError, strlen(cmdError), TRUE, FALSE);
        return(0);
      } else if (TempValue > MAX_HOURS_12H) {
        TempValue = MAX_HOURS_12H;
      }
      switch (TimeKeeprAlarm) {
        case TIMEKEEPER_CASE:
          mcp79410.TimeKeeper.Hour12.Hour_12Bit._12_24 = 1;
          mcp79410.TimeKeeper.Hour12.Hour_12Bit.AmPm = 1;
          if (HourType == 1) {
            mcp79410.TimeKeeper.Hour12.Hour_12Bit.AmPm = 0;
          }
          mcp79410.TimeKeeper.Hour12.Hour_12Bit.HrOne = (TempValue % 10);
          mcp79410.TimeKeeper.Hour12.Hour_12Bit.HrTen = (TempValue / 10);
          break;       
        case ALARM_CASE:
          mcp79410.Alarm[AlarmIndex].Hour12.Hour_12Bit._12_24 = 1;
          mcp79410.Alarm[AlarmIndex].Hour12.Hour_12Bit.AmPm = 1;
          if (HourType == 1) {
            mcp79410.Alarm[AlarmIndex].Hour12.Hour_12Bit.AmPm = 0;
          }
          mcp79410.Alarm[AlarmIndex].Hour12.Hour_12Bit.HrOne = (TempValue % 10);
          mcp79410.Alarm[AlarmIndex].Hour12.Hour_12Bit.HrTen = (TempValue / 10);
          break;        
        default:
          break;
      }
    } else {
      ReadStringFLASH((uint8_t *)cmdError, strlen(cmdError), TRUE, FALSE);
      return(0);
    }
    //-----------------------------------------
  } else {
    //-----------------------------------------
    //  Extract Hours
    StringIndex = CheckChar(':');
    if (StringIndex > 0) {
      TempValue = CheckHoursMinutesAndSeconds(0);
      if (TempValue == 255) {
        //  Syntax Error
        ReadStringFLASH((uint8_t *)cmdError, strlen(cmdError), TRUE, FALSE);
        return(0);
      } else if (TempValue > MAX_HOURS_24H) {
        TempValue = MAX_HOURS_24H;
      }
      switch (TimeKeeprAlarm) {
        case TIMEKEEPER_CASE:
          mcp79410.TimeKeeper.Hour24.Hour_24Byte = 0;
          mcp79410.TimeKeeper.Hour24.Hour_24Bit.HrOne = (TempValue % 10);
          mcp79410.TimeKeeper.Hour24.Hour_24Bit.HrTen = (TempValue / 10);
          break;
        case ALARM_CASE:
          mcp79410.Alarm[AlarmIndex].Hour24.Hour_24Byte = 0;
          mcp79410.Alarm[AlarmIndex].Hour24.Hour_24Bit.HrOne = (TempValue % 10);
          mcp79410.Alarm[AlarmIndex].Hour24.Hour_24Bit.HrTen = (TempValue / 10);
          break;         
        default:
          break;
      }
    } else {
      ReadStringFLASH((uint8_t *)cmdError, strlen(cmdError), TRUE, FALSE);
      return(0);
    }
    //-----------------------------------------
  }
  
  //-----------------------------------------
  //  Extract Minutes
  SerialInput.remove(0, StringIndex + 1);
  StringIndex = CheckChar(':');
  if (StringIndex > 0) {
    TempValue = CheckHoursMinutesAndSeconds(1);
    if (TempValue == 255) {
        //  Syntax Error
        ReadStringFLASH((uint8_t *)cmdError, strlen(cmdError), TRUE, FALSE);
        return(0);
    } else if (TempValue > MAX_MINUTES) {
      TempValue = MAX_MINUTES;
    }
    switch (TimeKeeprAlarm) {
      case TIMEKEEPER_CASE:
        mcp79410.TimeKeeper.Minute.MinBit.MinOne = (TempValue % 10);
        mcp79410.TimeKeeper.Minute.MinBit.MinTen = (TempValue / 10);
        break;       
      case ALARM_CASE:
        mcp79410.Alarm[AlarmIndex].Minute.MinBit.MinOne = (TempValue % 10);
        mcp79410.Alarm[AlarmIndex].Minute.MinBit.MinTen = (TempValue / 10);
        break;      
      default:
        break;      
    }
  } else {
    ReadStringFLASH((uint8_t *)cmdError, strlen(cmdError), TRUE, FALSE);
    return(0);
  }
  //-----------------------------------------

  //-----------------------------------------
  //  Extract Seconds
  SerialInput.remove(0, StringIndex + 1);
  if ((HourType == 1) || (HourType == 2)) {
    StringIndex = CheckChar('-');
    if (StringIndex == 0) {
      ReadStringFLASH((uint8_t *)cmdError, strlen(cmdError), TRUE, FALSE);
      return(0);      
    }
  } else {
    StringIndex = SerialInput.length();
  }
  TempValue = CheckHoursMinutesAndSeconds(2);
  if (TempValue == 255) {
    //  Syntax Error
    ReadStringFLASH((uint8_t *)cmdError, strlen(cmdError), TRUE, FALSE);
    return(0);
  } else if (TempValue > MAX_SECONDS) {
    TempValue = MAX_SECONDS;
  }
  switch (TimeKeeprAlarm) {
    case TIMEKEEPER_CASE:
      mcp79410.TimeKeeper.Second.SecBit.StartOsc = 1;
      mcp79410.TimeKeeper.Second.SecBit.SecOne = (TempValue % 10);
      mcp79410.TimeKeeper.Second.SecBit.SecTen = (TempValue / 10);
      break;   
    case ALARM_CASE:
      mcp79410.Alarm[AlarmIndex].Second.SecBit.SecOne = (TempValue % 10);
      mcp79410.Alarm[AlarmIndex].Second.SecBit.SecTen = (TempValue / 10);
      break;    
    default:
      break;      
  }
  //-----------------------------------------  
  return(1);
}
//======================================================================

//======================================================================
uint8_t CheckHoursMinutesAndSeconds(uint8_t HourMinSec) {
  uint8_t  Count;
  char     TempChar;
  
  TempString = SerialInput.substring(0, StringIndex);
  TempString.trim();
  Count = 0;
  do {
    TempChar = TempString.charAt((Count));      
    if (isDigit(TempChar) == false) {
      switch (HourMinSec) {
        case 0:
          ReadStringFLASH((uint8_t *)HoursError, strlen(HoursError), TRUE, FALSE);
          break;
        case 1:
          ReadStringFLASH((uint8_t *)MinutesError, strlen(MinutesError), TRUE, FALSE);
          break;
        case 2:
          ReadStringFLASH((uint8_t *)SecondsError, strlen(SecondsError), TRUE, FALSE);
          break;
        default:
          ReadStringFLASH((uint8_t *)cmdError, strlen(cmdError), TRUE, FALSE);
          break;      
      }
      return(255);
    }
  } while (++Count < TempString.length());
  return(TempString.toInt());
}
//======================================================================

//======================================================================
uint16_t CheckDaysMonthsAndYears(uint8_t DayMonthYear) {
  uint8_t  Count;
  char     TempChar;

  TempString = SerialInput.substring(0, StringIndex);
  TempString.trim();
  Count = 0;
  do {
    TempChar = TempString.charAt((Count));
    if (isDigit(TempChar) == false) {
      switch (DayMonthYear) {
        case 0:
          ReadStringFLASH((uint8_t *)DaysError, strlen(DaysError), TRUE, FALSE);
          break;
        case 1:
          ReadStringFLASH((uint8_t *)MonthsError, strlen(MonthsError), TRUE, FALSE);
          break;
        case 2:
          ReadStringFLASH((uint8_t *)YearsError, strlen(YearsError), TRUE, FALSE);
          break;
        default:
          ReadStringFLASH((uint8_t *)cmdError, strlen(cmdError), TRUE, FALSE);
          break;      
      }
      return(255);
    }
  } while (++Count < TempString.length());
  return(TempString.toInt());   
}
//======================================================================

//======================================================================
//  if char is located return value n
//  Otherwiese return "0"
uint8_t CheckChar(char CharToSearch) {
  uint8_t TempIndex = 0;
  TempIndex = SerialInput.indexOf(CharToSearch, 0);
  if (TempIndex == 255) {
    return(0);
  }
  return(TempIndex);
}
//======================================================================

//======================================================================
//  Search the string "AM" or "PM" in the received string command
//  If "AM" is located return value "1"
//  If "PM" is located return value "2"
//  Otherwiese return "0"
uint8_t Check_AMorPM(void) {
  uint8_t TempIndex = 0;
  //-----------------------------------------
  //  Check AM string
  TempIndex = SerialInput.indexOf("AM", 0);
  if (TempIndex == 255) {
    //-----------------------------------------
    //  Check PM string
    TempIndex = 0;
    TempIndex = SerialInput.indexOf("PM", 0);
    if (TempIndex == 255) {
      return(0);
    } else {
      return(2);
    }
    //-----------------------------------------
  } else {
    return(1);
  }
  //-----------------------------------------
}
//======================================================================
