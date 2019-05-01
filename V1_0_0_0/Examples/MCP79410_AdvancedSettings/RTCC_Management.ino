//======================================================================
//  Alarm 0 Flag = 1; FIRST Set Output pin ForceOn = 1 and than reset Alarm 0 Flag
void ResetAlarmFlag_0(void) {
  digitalWrite(ForceOn, HIGH);     // ForceOn ON
  delay(1000);
  mcp79410.ResetAlarmIntFlagBit(0);
}
//======================================================================

//======================================================================
//  Alarm 1 Flag = 1; FIRST Reset Alarm 1 Flag and then Set Output pin ForceOn = 0 
void ResetAlarmFlag_1(void) {
  mcp79410.ResetAlarmIntFlagBit(1);
  delay(1000);
  digitalWrite(ForceOn, LOW);     // ForceOn OFF
}
//======================================================================

//======================================================================
//  This function, every 15 seconds, reads the configuration of TimeKeeper registers and
//  Alarm 0/1 configurations. Then prints on the screen (Using println function) the read data
void RTCC_Read_Registers(void) {
  if ((StartTimeKeeperSettings == false) && (StartAlarmSettings == false)) {
    if (TimeOutReadTimeKeeper == 0) {
      TimeOutReadTimeKeeper = T_15SEC;
      mcp79410.ReadTimeKeeping();     //  Reads TimeKeeping settings
      mcp79410.ControlReg.ControlByte = mcp79410.ReadSingleReg(RTCC_HW_ADD, CONTROL_ADD);
      mcp79410.ReadAlarmRegister(0);  //  Reads Alarm 0 settings
      mcp79410.ReadAlarmRegister(1);  //  Reads Alarm 1 settings
      if (mcp79410.TimeKeeper.WeekDay.WkDayBit.OSCrun != 0) {
        //  Oscillator is enabled and running
        ReadStringFLASH((uint8_t *)Sep1, strlen(Sep1), TRUE, FALSE);
        //---------------------------------------------
        //  Print TimeKeeper
        ReadStringFLASH((uint8_t *)OscillatorEnable, strlen(OscillatorEnable), TRUE, FALSE);
        ReadStringFLASH((uint8_t *)TimeReaded, strlen(TimeReaded), FALSE, FALSE);
        PrintTime(TIMEKEEPER_CASE, 0);
        ReadStringFLASH((uint8_t *)DateReaded, strlen(DateReaded), FALSE, FALSE);
        PrintDate(TIMEKEEPER_CASE, 0);        
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
          ResetAlarmFlag_0();
          Serial.println("1");
        } else {
          Serial.println("0");
        }        
        ReadStringFLASH((uint8_t *)AlarmTimeConf, strlen(AlarmTimeConf), FALSE, FALSE);
        PrintTime(ALARM_CASE, 0);
        ReadStringFLASH((uint8_t *)AlarmDateConf, strlen(AlarmDateConf), FALSE, FALSE);
        PrintDate(ALARM_CASE, 0);
        Serial.println("");
        //---------------------------------------------

        //---------------------------------------------        
        //  Print Alarm 1 config
        ReadStringFLASH((uint8_t *)Alarm1_Conf, strlen(Alarm1_Conf), FALSE, FALSE);
        if (mcp79410.ControlReg.Bit.Alarm1_Enable == 1) {
          ReadStringFLASH((uint8_t *)Enable, strlen(Enable), TRUE, FALSE);  
        } else {
          ReadStringFLASH((uint8_t *)Disable, strlen(Disable), TRUE, FALSE); 
        }
        ReadStringFLASH((uint8_t *)AlarmOutputPol, strlen(AlarmOutputPol), FALSE, FALSE);
        if (mcp79410.Alarm[1].WeekDay.WkDayBit.AlarmPol == 1) {
          ReadStringFLASH((uint8_t *)MFP_LHL, strlen(MFP_LHL), TRUE, FALSE);   
        } else {
          ReadStringFLASH((uint8_t *)MFP_LLL, strlen(MFP_LLL), TRUE, FALSE);  
        }        
        ReadStringFLASH((uint8_t *)AlarmIntFlag, strlen(AlarmIntFlag), FALSE, FALSE);
        if (mcp79410.Alarm[1].WeekDay.WkDayBit.AlarmIF == 1) {
          ResetAlarmFlag_1();
          Serial.println("1");
        } else {
          Serial.println("0");
        }   
        ReadStringFLASH((uint8_t *)AlarmTimeConf, strlen(AlarmTimeConf), FALSE, FALSE);
        PrintTime(ALARM_CASE, 1);
        ReadStringFLASH((uint8_t *)AlarmDateConf, strlen(AlarmDateConf), FALSE, FALSE);
        PrintDate(ALARM_CASE, 1); 
        //---------------------------------------------
        
        ReadStringFLASH((uint8_t *)Sep1, strlen(Sep1), TRUE, FALSE);
        Serial.println("");
      } else {
        //  Oscillator has stopped or has been disabled
        ReadStringFLASH((uint8_t *)Sep1, strlen(Sep1), TRUE, FALSE);
        ReadStringFLASH((uint8_t *)OscillatorDisable, strlen(OscillatorDisable), TRUE, FALSE);
        ReadStringFLASH((uint8_t *)Sep1, strlen(Sep1), TRUE, FALSE);
      }
    }
  }  
}
//======================================================================

//======================================================================
//  This function prints the informations read by RTCC device
void PrintTime(uint8_t SwitchCase, uint8_t AlarmIndex) {
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
      Serial.print(mcp79410.Alarm[AlarmIndex].Hour24.Hour_24Bit.HrTen, DEC);
      Serial.print(mcp79410.Alarm[AlarmIndex].Hour24.Hour_24Bit.HrOne, DEC);
      Serial.print(":");
      Serial.print(mcp79410.Alarm[AlarmIndex].Minute.MinBit.MinTen, DEC);
      Serial.print(mcp79410.Alarm[AlarmIndex].Minute.MinBit.MinOne, DEC);
      Serial.print(":");
      Serial.print(mcp79410.Alarm[AlarmIndex].Second.SecBit.SecTen, DEC);
      Serial.print(mcp79410.Alarm[AlarmIndex].Second.SecBit.SecOne, DEC);
      if (mcp79410.Alarm[AlarmIndex].Hour24.Hour_24Bit._12_24 == 0) {
        //  24H format 
        Serial.println("");
      } else {
        //  12H format
        if (mcp79410.Alarm[AlarmIndex].Hour12.Hour_12Bit.AmPm == 0) {
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
void PrintDate(uint8_t SwitchCase, uint8_t AlarmIndex) {
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
      TempData = mcp79410.Alarm[AlarmIndex].WeekDay.WkDayBit.WkDay;
      Serial.print(mcp79410.Alarm[AlarmIndex].Date.DateBit.DateTen, DEC);
      Serial.print(mcp79410.Alarm[AlarmIndex].Date.DateBit.DateOne, DEC);
      Serial.print("/");
      Serial.print(mcp79410.Alarm[AlarmIndex].Month.MonthBit.MonthTen, DEC);
      Serial.print(mcp79410.Alarm[AlarmIndex].Month.MonthBit.MonthOne, DEC);
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
    switch(mcp79410.Alarm[AlarmIndex].WeekDay.WkDayBit.AlarmMask) {
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
