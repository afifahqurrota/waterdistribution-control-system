void WriteDataToThingSpeak() {
  banding; flswitchMain; pump;
  if(millis()-waktuTerakhir>interval) {
   banding; flswitchMain; pump;
   ThingSpeak.setField(1, banding); 
   ThingSpeak.setField(2, flswitchMain);
   ThingSpeak.setField(3, flswitchBranch);
   ThingSpeak.setField(4, pump);
   ThingSpeak.setField(5, sole);
   
   int stat = ThingSpeak.writeFields(channelID, writeAPI);
   //Serial.println("Uploading/Writing 4 data to thingspeak");
   //Serial.print("status wiriting/sending data to thingspeak (200:success): ");    
   //Serial.println(stat);
    
    waktuTerakhir = millis();
  }
}
