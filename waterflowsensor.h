void waterFlowSensor() {
    currentTime = millis();
    if(currentTime >= (cloopTime + 1000)) 
    {
      cloopTime = currentTime;
      float x1 = (flowcount1 / 7.5);
      float x2 = (flowcount2 /  7.5);
      flowcount1 = 0; flowcount2 = 0;

      //kalibrasi water flow sensor
      debit1 = ((0.3513*x1)+0.55);
      debit2 = ((0.3818*x2)+0.55);
    }
  
    banding = abs(debit1 - debit2);
    totalDebit = debit1 + debit2;
    lcd.setCursor(0,2);
    lcd.print("Debit= ");
    lcd.print(banding);
    lcd.print("L/min");
    
    /*Serial.print(banding);
    Serial.print(",");
    Serial.print(totalDebit);
    Serial.print(",");
    Serial.print(debit1);
    Serial.print(",");
    Serial.print(debit2);
    Serial.println();*/}
