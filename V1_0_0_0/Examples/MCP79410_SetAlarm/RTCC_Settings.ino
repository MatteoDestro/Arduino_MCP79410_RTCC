//======================================================================
//  State to set TimeKeeper registers
void RTCC_TimeKeeperSettings(void) {
  
  //  Set Date. 25/12/2015 - FRIDAY (Monday = 1; Tuesday = 2; Wednesday = 3; Thursday = 4; Friday = 5; Saturday = 6; Sunday = 7)
  mcp79410.TimeKeeper.Date.DateBit.DateOne    = 5;  // \
  mcp79410.TimeKeeper.Date.DateBit.DateTen    = 2;  //  | 
  mcp79410.TimeKeeper.Month.MonthBit.MonthOne = 2;  //  |
  mcp79410.TimeKeeper.Month.MonthBit.MonthTen = 1;  //   >--- Set 25/12/2015 - FRIDAY
  mcp79410.TimeKeeper.Year.YearBit.YearOne    = 5;  //  |
  mcp79410.TimeKeeper.Year.YearBit.YearTen    = 1;  //  |
  mcp79410.TimeKeeper.WeekDay.WkDayBit.WkDay  = 5;  // /
  
//  //  Set Time. 12H format. 02:25:30 - PM  
//  mcp79410.TimeKeeper.Hour24.Hour_12Byte       = 0;    // \
//  mcp79410.TimeKeeper.Hour12.Hour_12Bit._12_24 = 1;    //  |
//  mcp79410.TimeKeeper.Hour12.Hour_12Bit.AmPm   = 1;    //   >--- Set 02:25:30 - PM
//  mcp79410.TimeKeeper.Hour12.Hour_12Bit.HrOne  = 2;    //  |
//  mcp79410.TimeKeeper.Hour12.Hour_12Bit.HrTen  = 0;   // /
   
  //  Set Time. 24H format. 14:25:30
  mcp79410.TimeKeeper.Hour24.Hour_24Byte       = 0;    // \
  mcp79410.TimeKeeper.Hour24.Hour_24Bit._12_24 = 0;    //  |
  mcp79410.TimeKeeper.Hour24.Hour_24Bit.HrOne  = 4;    //  |
  mcp79410.TimeKeeper.Hour24.Hour_24Bit.HrTen  = 1;    //  |___ Set 14:25:30
  mcp79410.TimeKeeper.Minute.MinBit.MinOne     = 5;    //  | 
  mcp79410.TimeKeeper.Minute.MinBit.MinTen     = 2;    //  |
  mcp79410.TimeKeeper.Second.SecBit.SecOne     = 0;    //  |
  mcp79410.TimeKeeper.Second.SecBit.SecTen     = 3;    // /
  
  mcp79410.TimeKeeper.Second.SecBit.StartOsc   = 1;    // Start Oscillator
  mcp79410.TimeKeeper.WeekDay.WkDayBit.VbatEn  = 1;    // Enable battery supply
  
  mcp79410.WriteTimeKeeping(0);  //  Set Date and Time. Time in 24H format
//  mcp79410.WriteTimeKeeping(1);  //  Set Date and Time. Time in 12H format
}
//======================================================================

//======================================================================
//  State to set alarm registers or reset the alarm configuration
void RTCC_AlarmSettings(void) {
  mcp79410.Alarm0Bit(0);  //  Disable Alarm 0
  mcp79410.Alarm1Bit(0);  //  Disable Alarm 1
  
  //  Set Alarm Date. 25/12/---- - FRIDAY (Monday = 1; Tuesday = 2; Wednesday = 3; Thursday = 4; Friday = 5; Saturday = 6; Sunday = 7)
  mcp79410.Alarm[0].Date.DateBit.DateOne       = 5;
  mcp79410.Alarm[0].Date.DateBit.DateTen       = 2;
  mcp79410.Alarm[0].Month.MonthBit.MonthOne    = 2;
  mcp79410.Alarm[0].Month.MonthBit.MonthTen    = 1;
  mcp79410.Alarm[0].WeekDay.WkDayBit.WkDay     = 5;
  
  mcp79410.Alarm[0].WeekDay.WkDayBit.AlarmMask = 0;  //  Seconds Match
//  mcp79410.Alarm[0].WeekDay.WkDayBit.AlarmMask = 1;  //  Minutes Match
//  mcp79410.Alarm[0].WeekDay.WkDayBit.AlarmMask = 2;  //  Hours Match
//  mcp79410.Alarm[0].WeekDay.WkDayBit.AlarmMask = 3;  //  Day Of Week Match
//  mcp79410.Alarm[0].WeekDay.WkDayBit.AlarmMask = 4;  //  Date Match
//  mcp79410.Alarm[0].WeekDay.WkDayBit.AlarmMask = 7;  //  All Match

  mcp79410.Alarm[0].WeekDay.WkDayBit.AlarmPol = 1;  //  LHL - Logic High Level
//  mcp79410.Alarm[0].WeekDay.WkDayBit.AlarmPol = 0;  //  LLL - Logic Low Level

//  //  Set Alarm Time. 12H format. 02:25:30 - PM  
//  mcp79410.Alarm[0].Hour12.Hour_12Byte       = 0;
//  mcp79410.Alarm[0].Hour12.Hour_12Bit._12_24 = 1;
//  mcp79410.Alarm[0].Hour12.Hour_12Bit.AmPm   = 1;
//  mcp79410.Alarm[0].Hour12.Hour_12Bit.HrOne  = 2;
//  mcp79410.Alarm[0].Hour12.Hour_12Bit.HrTen  = 0;
 
  //  Set Alarm Time. 24H format. 14:25:30 
  mcp79410.Alarm[0].Hour24.Hour_24Byte       = 0;
  mcp79410.Alarm[0].Hour24.Hour_24Bit._12_24 = 0;
  mcp79410.Alarm[0].Hour24.Hour_24Bit.HrOne  = 4;
  mcp79410.Alarm[0].Hour24.Hour_24Bit.HrTen  = 1;
  mcp79410.Alarm[0].Minute.MinBit.MinOne     = 5;
  mcp79410.Alarm[0].Minute.MinBit.MinTen     = 2;         
  mcp79410.Alarm[0].Second.SecBit.SecOne     = 0;
  mcp79410.Alarm[0].Second.SecBit.SecTen     = 3;       
        
  mcp79410.WriteAlarmRegister(0, 0);
  mcp79410.Alarm0Bit(1);                //  Enable Alarm 0
}
//======================================================================

//======================================================================
//  This function, every 15 seconds, reads the configuration of TimeKeeper registers and
//  Alarm 0/1 configurations. Then prints on the screen (Using println function) the read data
void RTCC_Read_Registers(void) {
  if (TimeOutReadTimeKeeper == 0) {
    TimeOutReadTimeKeeper = T_10SEC;
    digitalWrite(Led, LOW);        // Led ON
    mcp79410.ReadTimeKeeping();     //  Reads TimeKeeping settings
    mcp79410.ControlReg.ControlByte = mcp79410.ReadSingleReg(RTCC_HW_ADD, CONTROL_ADD);
    mcp79410.ReadAlarmRegister(0);  //  Reads Alarm 0 settings
    if (mcp79410.TimeKeeper.WeekDay.WkDayBit.OSCrun != 0) {
      //  Oscillator is enabled and running
      ReadStringFLASH((uint8_t *)Sep1, strlen(Sep1), TRUE, FALSE);
      //---------------------------------------------
      //  Print TimeKeeper
      ReadStringFLASH((uint8_t *)OscillatorEnable, strlen(OscillatorEnable), TRUE, FALSE);
      ReadStringFLASH((uint8_t *)TimeReaded, strlen(TimeReaded), FALSE, FALSE);
      PrintTime(TIMEKEEPER_CASE);
      ReadStringFLASH((uint8_t *)DateReaded, strlen(DateReaded), FALSE, FALSE);
      PrintDate(TIMEKEEPER_CASE);        
      Serial.println("");
      //---------------------------------------------
      
      //---------------------------------------------
      //  Print Alarm 0 config
      ReadStringFLASH((uint8_t *)Alarm0_Conf, strlen(Alarm0_Conf), FALSE, FALSE);
      if (mcp79410.ControlReg.Bit.Alarm0_Enable == 1) {
        ReadStringFLASH((uint8_t *)Enable, strlen(Enable), TRUE, FALSE);  
      } else {
        ReadStringFLASH((uint8_t *)Disable, strlen(Disable), TRUE, FALSE); 
      }
      ReadStringFLASH((uint8_t *)AlarmOutputPol, strlen(AlarmOutputPol), FALSE, FALSE);
      if (mcp79410.Alarm[0].WeekDay.WkDayBit.AlarmPol == 1) {
        ReadStringFLASH((uint8_t *)MFP_LHL, strlen(MFP_LHL), TRUE, FALSE);   
      } else {
        ReadStringFLASH((uint8_t *)MFP_LLL, strlen(MFP_LLL), TRUE, FALSE);  
      }
      ReadStringFLASH((uint8_t *)AlarmIntFlag, strlen(AlarmIntFlag), FALSE, FALSE);
      if (mcp79410.Alarm[0].WeekDay.WkDayBit.AlarmIF == 1) {
        mcp79410.ResetAlarmIntFlagBit(0);
        Serial.println("1");
      } else {
        Serial.println("0");
      }        
      ReadStringFLASH((uint8_t *)AlarmTimeConf, strlen(AlarmTimeConf), FALSE, FALSE);
      PrintTime(ALARM_CASE);
      ReadStringFLASH((uint8_t *)AlarmDateConf, strlen(AlarmDateConf), FALSE, FALSE);
      PrintDate(ALARM_CASE);
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
void PrintTime(uint8_t SwitchCase) {
  switch (SwitchCase) {
    case TIMEKEEPER_CASE:
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
      break;
    case ALARM_CASE:
      Serial.print(mcp79410.Alarm[0].Hour24.Hour_24Bit.HrTen, DEC);
      Serial.print(mcp79410.Alarm[0].Hour24.Hour_24Bit.HrOne, DEC);
      Serial.print(":");
      Serial.print(mcp79410.Alarm[0].Minute.MinBit.MinTen, DEC);
      Serial.print(mcp79410.Alarm[0].Minute.MinBit.MinOne, DEC);
      Serial.print(":");
      Serial.print(mcp79410.Alarm[0].Second.SecBit.SecTen, DEC);
      Serial.print(mcp79410.Alarm[0].Second.SecBit.SecOne, DEC);
      if (mcp79410.Alarm[0].Hour24.Hour_24Bit._12_24 == 0) {
        //  24H format 
        Serial.println("");
      } else {
        //  12H format
        if (mcp79410.Alarm[0].Hour12.Hour_12Bit.AmPm == 0) {
          //  AM
          Serial.println("AM");            
        } else {
          //  PM
          Serial.println("PM");
        }
      }
      break;
    default:
      break;
  }   
}
//======================================================================

//======================================================================
//  This function prints the informations read by RTCC device
void PrintDate(uint8_t SwitchCase) {
  uint8_t TempData;
  
  switch (SwitchCase) {
    case TIMEKEEPER_CASE:
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
      break;
    case ALARM_CASE:
      TempData = mcp79410.Alarm[0].WeekDay.WkDayBit.WkDay;
      Serial.print(mcp79410.Alarm[0].Date.DateBit.DateTen, DEC);
      Serial.print(mcp79410.Alarm[0].Date.DateBit.DateOne, DEC);
      Serial.print("/");
      Serial.print(mcp79410.Alarm[0].Month.MonthBit.MonthTen, DEC);
      Serial.print(mcp79410.Alarm[0].Month.MonthBit.MonthOne, DEC);
      Serial.print("/");
      Serial.print("YYYY");     
      Serial.print(" - ");
      break;
    default:
      break;
  }   
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
  
  if (SwitchCase == ALARM_CASE) {
    ReadStringFLASH((uint8_t *)AlarmxMask, strlen(AlarmxMask), FALSE, FALSE);
    switch(mcp79410.Alarm[0].WeekDay.WkDayBit.AlarmMask) {
      case 0:  //  Seconds Match
        ReadStringFLASH((uint8_t *)SecondsMatch, strlen(SecondsMatch), TRUE, FALSE);
        break; 
      case 1:  //  Minutes Match
        ReadStringFLASH((uint8_t *)MinutesMatch, strlen(MinutesMatch), TRUE, FALSE);
        break;   
      case 2:  //  Hours Match
        ReadStringFLASH((uint8_t *)HoursMatch, strlen(HoursMatch), TRUE, FALSE);
        break; 
      case 3:  //  Day Of The Week Match
        ReadStringFLASH((uint8_t *)DayOfWeekMatch, strlen(DayOfWeekMatch), TRUE, FALSE);
        break;   
      case 4:  //  Date Match
        ReadStringFLASH((uint8_t *)DateMatch, strlen(DateMatch), TRUE, FALSE);
        break;       
      case 7:  //  All Match
        ReadStringFLASH((uint8_t *)AllMatch, strlen(AllMatch), TRUE, FALSE);
        break;
      default:
        ReadStringFLASH((uint8_t *)AlarmxMaskError, strlen(AlarmxMaskError), TRUE, FALSE);
        break;
    }
  }
}
//======================================================================
