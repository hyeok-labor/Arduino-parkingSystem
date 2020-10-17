// Arduino MegaADK

 
#include <Servo.h>


#define SS_PIN 53
#define RST_PIN 7
#define Close 0
#define Open 90

/*****************************************/

// 적외선 
int pin_F_vcc[2]={8,10};
int pin_F_out[2]={9,11};

// 서보모터
Servo servo1;
Servo servo2;

int F_state[2] = {1,1};


/**************************************************************/

void InfraredSet(){   // 초음파, 근접센서 설정 함수 
  for(int i = 0; i<2; i++){
    F_state[i] = digitalRead(pin_F_out[i]);
    //Serial.print("F_state"); Serial.print(i); Serial.print(": "); Serial.println(F_state[i]);
  }
}
/**************************************************************/

void setup() { 
  
  Serial.begin(115200);
  
  // Sensor & Servo setup
  servo1.attach(22);
  servo2.attach(24);
  
  for(int i = 0; i<2; i++){
    pinMode(pin_F_vcc[i],OUTPUT);
    digitalWrite(pin_F_vcc[i],HIGH);
    // 근접센서
    pinMode(pin_F_out[i],INPUT);
  }  
  // close set 
  servo1.write(Close); servo2.write(Close);
}
char data;
void loop() {
  // Continuously operative Sensors
  InfraredSet();
  Serial.println(data);
  if(Serial.available()){
    data = Serial.read();
  }
  
  if(data=='a'){
    servo1.write(Open);
    delay(4000);
    if(F_state[0]==0){
      servo1.write(Open); Serial.println("servo1");
      delay(2000);
    }
    else
      servo1.write(Close); 
  }
  
  if(data=='a'){
    servo2.write(Open); Serial.println("servo1");
    delay(4000);
    if(F_state[1]==0){
      servo2.write(Open); 
      delay(2000);
    }
    else
      servo2.write(Close); 
  }
}
// 1010 edited by Hyeok
