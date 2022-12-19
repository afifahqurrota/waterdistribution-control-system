  void Main() {
   while(digitalRead(limitSwitchMain)) {
    digitalWrite(dirPinMain,LOW);
    
    digitalWrite(stepPinMain, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPinMain, LOW);
    delayMicroseconds(2000);}
    
   if(!digitalRead(limitSwitchMain)) {
    digitalWrite(dirPinMain, HIGH);
    lcd.setCursor(0,2);
    lcd.print("M-done");}
  }
  
  void Branch() {
   while(digitalRead(limitSwitchBranch)) {
    digitalWrite(dirPinBranch,LOW);
    
    digitalWrite(stepPinBranch, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPinBranch, LOW);
    delayMicroseconds(2000);}
    
   if(!digitalRead(limitSwitchBranch)) {
    digitalWrite(dirPinBranch, HIGH);    
    lcd.setCursor(8,2);
    lcd.print("B-done");
  }
 }

 void OutA() {
  while(digitalRead(limitSwitchOutA)) {
    digitalWrite(dirPinOutA,HIGH);
    
    digitalWrite(stepPinOutA, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPinOutA, LOW);
    delayMicroseconds(2000);}
    
  if(!digitalRead(limitSwitchOutA)) {
    digitalWrite(dirPinOutA, LOW);    
    lcd.setCursor(8,2);
    lcd.print("OutA-done");
  }
 }

 void OutB() {
  while(digitalRead(limitSwitchOutB)) {
    digitalWrite(dirPinOutB,HIGH);
    
    digitalWrite(stepPinOutB, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPinOutB, LOW);
    delayMicroseconds(2000);}
    
  if(!digitalRead(limitSwitchOutB)) {
    digitalWrite(dirPinOutB, LOW);    
    lcd.setCursor(10,2);
    lcd.print("OutB-done");
  }
 }
