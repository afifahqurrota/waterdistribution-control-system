void output1() {
      if((debit1<2.00) && (debit1>=0.00)) {
        stepperOutB.moveTo(-55);
        stepperOutB.runToPosition();
        Serial.print(",");
        Serial.print("b50");}
      else if((debit1>2.00) && (debit1<2.50)) {
        stepperOutB.moveTo(-50);
        stepperOutB.runToPosition();
        Serial.print(",");
        Serial.print("b45");}
      else if(debit1>3.00) {
        stepperOutB.moveTo(-45);
        stepperOutB.runToPosition();
        Serial.print(",");
        Serial.print("b40");}

      if((debit2<2.00) && (debit2>=0.00)){
        stepperOutA.moveTo(-50); 
        stepperOutA.runToPosition();
        Serial.print(",");
        Serial.print("a50");}
      else if((debit2>2.00) && (debit2<2.50)) {
        stepperOutA.moveTo(-45);
        stepperOutA.runToPosition();
        Serial.print(",");
        Serial.print("a45");}
      else if(debit2>3.00) {
        stepperOutA.moveTo(-40);
        stepperOutA.runToPosition();
        Serial.print(",");
        Serial.print("a40");}
 }
    
 void output2() {
    if((debit1<1.00) && (debit1>=0.00)) {
      stepperOutB.moveTo(-40);
      stepperOutB.runToPosition();
      Serial.print(",");
      Serial.print("b30");}
    else if((debit1>1.50) && (debit1<2.00)) {
      stepperOutB.moveTo(-35);
      stepperOutB.runToPosition();
      Serial.print(",");
      Serial.print("b25");}
    else if(debit1>2.00) {
      stepperOutB.moveTo(-30);
      stepperOutB.runToPosition();
      Serial.print(",");
      Serial.print("b20");}

    if((debit2<1) && (debit2>=0)){
      stepperOutA.moveTo(-30); 
      stepperOutA.runToPosition();
      Serial.print(",");
      Serial.print("a30");}
    else if((debit2>1.5) && (debit2<2.00)) {
      stepperOutA.moveTo(-25);
      stepperOutA.runToPosition();
      Serial.print(",");
      Serial.print("a25");}
    else if(debit2>2) {
      stepperOutA.moveTo(-20);
      stepperOutA.runToPosition();
      Serial.print(",");
      Serial.print("a20");}
  }
  
