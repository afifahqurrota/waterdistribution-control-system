void CadBranch() {
  if((debit1 >= 1.00) && (debit2 >= 1.00)){
    stepperBranch.moveTo(10);
    stepperBranch.runToPosition();
    
    if((banding > 0) && (banding < 0.5)) {
      stepperBranch.moveTo(25);
      stepperBranch.runToPosition();}
  }
}
