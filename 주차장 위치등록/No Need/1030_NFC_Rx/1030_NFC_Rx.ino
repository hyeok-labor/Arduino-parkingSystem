// Mega ADK ==> ought to be converted to D1 mini
// 
void setup() {
  Serial.begin(115200);
}

void loop() {
  char temp[8];
  if(Serial.available()){
    byte leng = Serial.readBytes(temp,8);
    Serial.println(leng);
    for(int i = 0; i<leng; i++){
      Serial.print(temp[i]);
    }
  
    else 
    Serial.println("");
    Serial.println(temp); // 송신할 데이터 
  }
}
