
#include <EEPROM.h>

//======================================================================
//  Sets digital inputs
void SetInputPin(void) {
  pinMode(P1_Button, INPUT);          // set pin to input
  pinMode(P2_Button, INPUT);          // set pin to input
  pinMode(P3_Button, INPUT);          // set pin to input
  digitalWrite(P1_Button, HIGH);      // turn on pullup resistors
  digitalWrite(P2_Button, HIGH);      // turn on pullup resistors
  digitalWrite(P3_Button, HIGH);      // turn on pullup resistors 
 
  DigInputStatus.Input = 0xFF;  
}
//======================================================================

//======================================================================
//  Debouncing digital inputs
void DebouncingInput(void) {
  DigInputReaded.In.P1_Button = digitalRead(P1_Button);
  DigInputReaded.In.P2_Button = digitalRead(P2_Button);
  DigInputReaded.In.P3_Button = digitalRead(P3_Button);

  if (DigInputReaded.Input != DigInputStatus.Input) {
    if (DigInputReaded.Input != DigInputVar) {
       DigInputVar = DigInputReaded.Input;
       DebouncingTimeOut = T_50MSEC;
    } else {
      if (DebouncingTimeOut == 0) {
         DigInputVar = DigInputReaded.Input;
         DigInputStatus.Input = DigInputReaded.Input;
      }
    }
  } else {
      DigInputVar = DigInputStatus.Input;
  }
}
//======================================================================

//======================================================================
void Input_Idle(void) {
  if (DigInputStatus.In.P1_Button != 1) {
    TimeOutWait = T_2SEC;
    Input_Management = Input_WaitP1;
    Led_Management = Led_Idle; 
    return;
  }
  
  if (DigInputStatus.In.P2_Button != 1) {
    TimeOutWait = T_2SEC;
    Input_Management = Input_WaitP2;	 
    Led_Management = Led_Idle;
    return;
  }
  
//  if (DigInputStatus.In.P3_Button != 1) {
//    TimeOutWait = T_2SEC;
//    Input_Management = Input_WaitP3;
//    Led_Management = Led_Idle;
//    return;
//  }
}
//======================================================================

//======================================================================
void Input_WaitP1(void) {
  if ((TimeOutWait == 0) && (DigInputStatus.In.P1_Button != 1)) {
    Input_Management = Input_ReleasedP1;
    Led_Management   = Led_PxPressed;
  } else if ((TimeOutWait > 0) && (DigInputStatus.In.P1_Button != 0)) {
    Input_Management = Input_Idle;
  }
}

void Input_ReleasedP1(void) {
  if (DigInputStatus.In.P1_Button != 0) {
    if (StartTimeKeeperSettings == false) {
      StartTimeKeeperSettings = true;
      Led_Management = LedTimeSet_stp1;
    } else {
      StartTimeKeeperSettings = false;
      Led_Management = Led_Idle;
    }
    Input_Management = Input_Idle;
  }
}
//======================================================================

//======================================================================
void Input_WaitP2(void) {
  if ((TimeOutWait == 0) && (DigInputStatus.In.P2_Button != 1)) {
    Input_Management = Input_ReleasedP2;
    Led_Management   = Led_PxPressed;
  } else if ((TimeOutWait > 0) && (DigInputStatus.In.P2_Button != 0)) {
    Input_Management = Input_Idle;
  }
}

void Input_ReleasedP2(void) {
  if (DigInputStatus.In.P2_Button != 0) {
    if (StartAlarmSettings == false) {
      StartAlarmSettings = true;
      Led_Management = LedTimeSet_stp1;
    } else {
      StartAlarmSettings = false;
      Led_Management = Led_Idle;
    }
    Input_Management = Input_Idle;
  }
}
//======================================================================

//======================================================================
//void Input_WaitP3(void) {
//  if ((TimeOutWait == 0) && (DigInputStatus.In.P3_Button != 1)) {
//    Input_Management = Input_ReleasedP3;
//    Led_Management   = Led_PxPressed;
//  } else if ((TimeOutWait > 0) && (DigInputStatus.In.P3_Button != 0)) {
//    Input_Management = Input_Idle;
//  }
//}
//
//void Input_ReleasedP3(void)
//{
//  if (DigInputStatus.In.P3_Button != 0) {
//    if (StartPowerTimeStampSettings == false) {
//      StartPowerTimeStampSettings = true;
//    } else {
//      StartPowerTimeStampSettings = false;
//    }
//    Input_Management = Input_Idle;
//  }
//}
//======================================================================

