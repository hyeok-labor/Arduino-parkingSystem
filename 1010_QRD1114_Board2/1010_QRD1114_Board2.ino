/*
 * Board 2 ==> Board 1 
 * 
 */
#define ALL 6 

#include <SoftwareSerial.h>  
SoftwareSerial s(11,12);

const int signal[ALL] = {4,5,6,7,8,9}; 
const int Qrd_Gnd[8] = {40,41,42,43,44,45,46,47};
const int led_pin[ALL] = {22,23,24,25,26,27};
const int led_Gnd = 13;
static int state[ALL] = {};
static int count[ALL] = {};
static int sum = 0;
static int avail = 0;
int data = 0;
const int rtGnd = 2;

void fc_Counting(){
  for(int i=0; i<ALL; i++){
    if(state[i]==0){
      count[i] = 1;
      }
    else count[i] = 0;
  }
    sum = count[0]+count[1]+count[2]+count[3]+count[4]+count[5];
    avail = ALL-sum; 
}
void setup() {
  s.begin(9600);
  Serial.begin(9600);
  for(int i=0; i<ALL; i++){
    pinMode(led_pin[i],OUTPUT);
  }
  
  pinMode(led_Gnd,OUTPUT);            // GND for leds (13)
  digitalWrite(led_Gnd,LOW);
  
  pinMode(rtGnd,OUTPUT);
  digitalWrite(rtGnd,LOW);
  
  for(int i=0; i<8; i++){
    pinMode(Qrd_Gnd[i],OUTPUT);
    digitalWrite(Qrd_Gnd[i],LOW);
  }
}

void loop() {
  fc_Counting();    // counting function
  
  for(int i=0; i<ALL; i++){
      state[i] = digitalRead(signal[i]);
      if(state[i] == 0){
      Serial.print(i); Serial.println("detected"); 
      digitalWrite(led_pin[i],HIGH);
      
    }else{
      digitalWrite(led_pin[i],LOW);
    }
  }
  delay(500);
      Serial.print("sum:"); Serial.print(sum); Serial.println();
      Serial.print("available:"); Serial.println(avail);
      data = sum;
      s.write(data);
      
  delay(500);
}
