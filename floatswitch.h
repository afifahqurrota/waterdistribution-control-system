class floatswitch {
  public:
  void floatswitch_setup() 
  {
    pinMode(floatSwitch_main, INPUT_PULLUP);
    pinMode(floatSwitch_branch, INPUT_PULLUP);
    pinMode(pb_on, INPUT_PULLUP);
    pinMode(pb_off, INPUT_PULLUP);
    pinMode(relay_pump, OUTPUT);
  }

  void floatswitch_loop() 
  {         
    onValue = digitalRead(pb_on);
    offValue = digitalRead(pb_off);
    
    if((onValue == LOW) && (flswitchMain == LOW))  {
      digitalWrite(relay_pump, LOW);
      lcd.setCursor(0,3);
      lcd.print("P-ON |");
      lcd.setCursor(6,3);
      lcd.print("R1-L|");
      last_relay_state == 1;
      //flagState = false;
    }    
    if(offValue == LOW) {
      digitalWrite(relay_pump, HIGH);
      lcd.setCursor(0,3);
      lcd.print("P-OFF|");
      //flagState = true;
      last_relay_state == 0;
    }
    
    if(flswitchMain == HIGH) {
      digitalWrite(relay_pump, HIGH);
      lcd.setCursor(6,3);
      lcd.print("R1-F");
      lcd.setCursor(0,3);
      lcd.print("P-OFF|");
      last_relay_state == 0;
    }

    if((flswitchMain == LOW) && (flswitchBranch == HIGH)) {
      digitalWrite(relay_pump, LOW);
      lcd.setCursor(0,3);
      lcd.print("P-ON |");
      lcd.setCursor(6,3);
      lcd.print("R1-L|");
      last_relay_state == 1;
    }
    
    if((flswitchMain == HIGH) && (flswitchBranch == LOW)) {
      digitalWrite(relay_pump, HIGH);
      lcd.setCursor(0,3);
      lcd.print("P-OFF|");
      lcd.setCursor(6,3);
      lcd.print("R1-F|");
      last_relay_state == 0;
    }
   
   if((flswitchMain == HIGH) && (flswitchBranch ==  HIGH)) {
      digitalWrite(relay_pump, HIGH);
      lcd.setCursor(0,3);
      lcd.print("P-OFF|");
      lcd.setCursor(6,3);
      lcd.print("R1-F|");
      last_relay_state == 0;
    }
  }
};
floatswitch fs;
