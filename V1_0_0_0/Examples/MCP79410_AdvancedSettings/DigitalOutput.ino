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
void Led_Idle(void) {
  if (TimeOutLed == 0) {
    digitalWrite(Led, HIGH);  // Led OFF
  }
}

void Led_PxPressed(void) {
  digitalWrite(Led, LOW);    // Led ON
  TimeOutLed = T_250MSEC;
  Led_Management = Led_Idle;
}

void LedTimeSet_stp1(void) {
  if (TimeOutLed > 0) {
    return; 
  }
  digitalWrite(Led, LOW);    // Led ON
  TimeOutLed = T_250MSEC;
  Led_Management = LedTimeSet_stp2; 
}

void LedTimeSet_stp2(void) {
  if (TimeOutLed > 0) {
    return; 
  }
  digitalWrite(Led, HIGH);  // Led OFF
  TimeOutLed = T_750MSEC;
  Led_Management = LedTimeSet_stp1;
}
//======================================================================


