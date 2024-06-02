#include <Arduino.h>
#define SOUND_SPEED 0.034

// ultra sound
const int trig1 = 7;     //below 30
const int echo1 = 5;     
const int trig2 = 8;     //top 80
const int echo2 = 6;     

// light
const int ldr_pin = 2;
const int led_pin12 = 12;
const int led_pin13 = 13;

// water
const int water_pin = A0;

// buzzer
const int buzzer_pin = 4;

// vibration 
const int vibe_pin = 3; // micro vibration motor is connected with pin number 3 which is the pwm pin. 
 
void setup()
{
  Serial.begin(9600);     // giao tiếp Serial với baudrate 9600
  pinMode(trig1,OUTPUT);   // chân trig1 sẽ phát tín hiệu
  pinMode(echo1,INPUT);    // chân echo1 sẽ nhận tín hiệu
  pinMode(trig2,OUTPUT);   // chân trig2 sẽ phát tín hiệu
  pinMode(echo2,INPUT);    // chân echo2 sẽ nhận tín hiệu
  pinMode(ldr_pin,INPUT);
  pinMode(led_pin12,OUTPUT); 
  pinMode(led_pin13,OUTPUT);    
  pinMode(buzzer_pin,OUTPUT);

  digitalWrite(led_pin12, LOW);
  digitalWrite(led_pin13, LOW);
}

char Incoming_value = 0;
void isFinding(){
  if(Incoming_value == '1'){
    tone(buzzer_pin, 500);
    Serial.println("FINDING STICK");
  }            
  else if(Incoming_value == '0'){
    noTone(buzzer_pin);
  }
}

void detect_obstacle12(){
  unsigned long duration1, duration2; // biến đo thời gian
  int distance1, distance2;           // biến lưu khoảng cách
  
  digitalWrite(trig1,0);   // tắt chân trig1
  delayMicroseconds(2);
  digitalWrite(trig1,1);   // phát xung từ chân trig1
  delayMicroseconds(10);   // xung có độ dài 10 microSeconds
  digitalWrite(trig1,0);   // tắt chân trig1
  duration1 = pulseIn(echo1,HIGH);  // Đo độ rộng xung HIGH ở chân echo1.  
  delay(50);

  digitalWrite(trig2,0);   // tắt chân trig1
  delayMicroseconds(2);
  digitalWrite(trig2,1);   // phát xung từ chân trig1
  delayMicroseconds(10);   // xung có độ dài 10 microSeconds
  digitalWrite(trig2,0);   // tắt chân trig1
  duration2 = pulseIn(echo2,HIGH);  // Đo độ rộng xung HIGH ở chân echo1. 
  delay(50);

  distance1 = duration1 * SOUND_SPEED / 2;
  distance2 = duration2 * SOUND_SPEED / 2;
  
  /* In kết quả ra Serial Monitor */
  Serial.print("detect_obstacle 1: ");  Serial.print(distance1);   Serial.print(" ----------- ");
  Serial.print("detect_obstacle 2: ");  Serial.println(distance2);
  if((distance1 <= 30) || (distance2 <= 80)){
    analogWrite(vibe_pin, 200);
  }else{
    analogWrite(vibe_pin, 0); 
  }
  isFinding();
}

void detect_dark(){
  if(digitalRead(ldr_pin) == 1){
    digitalWrite(led_pin12, HIGH);
    digitalWrite(led_pin13, HIGH);
    Serial.println("Dark");
  }
  else{
    digitalWrite(led_pin12, LOW);
    digitalWrite(led_pin13, LOW);
    Serial.println("Bright");
  }
  isFinding();
}

void detect_water(){
  int water_value = analogRead(water_pin);
  if (water_value >= 600){
    tone(buzzer_pin, 500);
    Serial.print("water_value: ");
    Serial.println(water_value);
  }else{
    noTone(buzzer_pin);
  }
  isFinding();
}

// esp8266
void detect_drop(){

}

// HC-05
void find_stick(){
  if(Serial.available() > 0)  {
    Incoming_value = Serial.read();      
    Serial.print(Incoming_value);        
    Serial.print("\n");        
  }    
  isFinding();
}
 
void loop()
{
  detect_all();
}

void detect_all(){
  isFinding();
  detect_obstacle12(); //ob1
  find_stick();
  detect_dark();
  detect_water();
}