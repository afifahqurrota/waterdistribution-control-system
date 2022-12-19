 void CadSole() {
  int sole = digitalRead(relay_sole);
  Serial.print(",");
  Serial.print(sole);
  
  if((debit1<2) && (debit2<2)) {
    digitalWrite(relay_sole, LOW);
    sole = 1;
    Serial.print(",");
    Serial.print(sole);}
    
  else if((debit1>3) && (debit2>3)) {
    digitalWrite(relay_sole, HIGH);
    sole = 0;
    Serial.print(",");
    Serial.print(sole);}
}
