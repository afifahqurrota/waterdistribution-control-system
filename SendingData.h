void readDataFromThingspeak () {
  statusCode = 0;
  
  last_relay_state = ThingSpeak.readFloatField(channelID, 4);
  statusCode = ThingSpeak.getLastReadStatus();
  
  if(statusCode >= 200) {
    if((last_relay_state == 1) && (pump == 0)) {
      digitalWrite(relay_pump, LOW);
      lcd.setCursor(0,3);
      lcd.print("P-ON |");
    }
    else if((last_relay_state == 0) && (pump == 1)){
      digitalWrite(relay_pump, HIGH);
      lcd.setCursor(0,3);
      lcd.print("P-OFF|");
    }
    
    lcd.setCursor(14,1);
    lcd.print(last_relay_state);
    lcd.setCursor(15,1);
    lcd.print("|");
    
    if(statusCode >= 200) {
      lcd.setCursor(0,1);
      lcd.print(statusCode);
      lcd.setCursor(3,1);
      lcd.print("|StatData| ");
    }  
    
    //Serial.print("The latest data from Field1 on ThingSpeak is : ");
    //Serial.println(last_relay_state);
  }
  else {
    //Serial.print("Problem reading channel. HTTP error code" + String(statusCode));
    //Serial.println(last_relay_state);
    
    if(statusCode < 200) {
      lcd.setCursor(0,1);
      lcd.print(statusCode);
      lcd.setCursor(4,1);
      lcd.print("|StatData|");
    }
  }
  delay(500);
}
