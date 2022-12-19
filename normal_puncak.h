void floatSwitch(){
  if(flswitchMain == LOW) {
    if (flswitchBranch == LOW) {
      lcd.setCursor(11,3);
      lcd.print("R2-L|");}
      digitalWrite(relay_pump, LOW);
      
      lcd.setCursor(0,3);
      lcd.print("P-ON |");
      lcd.setCursor(6,3);
      lcd.print("R1-L|");}

   if(flswitchMain == HIGH) {
    if(flswitchBranch == LOW) {
      lcd.setCursor(11,3);
      lcd.print("R2-L|");}
      digitalWrite(relay_pump, HIGH);
    
      lcd.setCursor(0,3);
      lcd.print("P-OFF|");
      lcd.setCursor(6,3);
      lcd.print("R1-F|");}
}

void floatSwitch2() {
  if(flswitchBranch == LOW){      
    if(flswitchMain == LOW) {
      digitalWrite(relay_pump, LOW);
      lcd.setCursor(0,3);
      lcd.print("P-ON |");
      lcd.setCursor(6,3);
      lcd.print("R1-L|");}
      lcd.setCursor(11,3);
      lcd.print("R2-L|");}
    
    if(flswitchBranch == HIGH) {
      lcd.setCursor(11,3);
      lcd.print("R2-F|");
      
      while(digitalRead(limitSwitchBranch)){
        digitalWrite(dirPinBranch,LOW);
    
        digitalWrite(stepPinBranch, HIGH);
        delayMicroseconds(2000);
        digitalWrite(stepPinBranch, LOW);
        delayMicroseconds(2000);}
    
      while(!digitalRead(limitSwitchBranch)) {
        digitalWrite(dirPinBranch, HIGH);  ;}
    }
  }
  
void rtc_setup() {   
    if(! rtc.begin()) {
      Serial.println("Couldn't Find RTC");
      while(1);}
    if(rtc.lostPower()) {
      Serial.println("RTC Lost Power, lets set the time!");
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));}
  }
  
void np() {
}
