#include <Wire.h>
#include <MPU6050.h>
#include <math.h>
#include <EMailSender.h>
#include <ESP8266WiFi.h>
MPU6050 mpu;

//115200
// const int led_pin = 2;
int time_present;

// const char* ssid = "Ty King";
// const char* password = "07092003";

const char* ssid = "H-wifi";
const char* password = "F1234567";

uint8_t connection_state = 0;
uint16_t reconnect_interval = 10000;

EMailSender emailSend("lechisang2003@gmail.com", "fvvb wgmb znal dwud");

uint8_t WiFiConnect(const char* nSSID = nullptr, const char* nPassword = nullptr)
{
  static uint16_t attempt = 0;
  Serial.print("Connecting to ");
  if(nSSID) {
    WiFi.begin(nSSID, nPassword);
    Serial.println(nSSID);
  }

  uint8_t i = 0;
  while(WiFi.status()!= WL_CONNECTED && i++ < 50)
  {
    delay(200);
    Serial.print(".");
  }
  ++attempt;
  Serial.println("");
  if(i == 51) {
    Serial.print("Connection: TIMEOUT on attempt: ");
    Serial.println(attempt);
    if(attempt % 2 == 0)
        Serial.println("Check if access point available or SSID and Password\r\n");
    return false;
  }
  Serial.println("Connection: ESTABLISHED");
  Serial.print("Got IP address: ");
  Serial.println(WiFi.localIP());
  return true;
}

void Awaits()
{
  uint32_t ts = millis();
  while(!connection_state)
  {
    delay(50);
    if(millis() > (ts + reconnect_interval) && !connection_state){
        connection_state = WiFiConnect();
        ts = millis();
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  mpu.initialize();

  connection_state = WiFiConnect(ssid, password);
  if(!connection_state)  // if not connected to WIFI
      Awaits();          // constantly trying to connect

  // pinMode(led_pin, OUTPUT);
  // digitalWrite(led_pin, LOW);
}

unsigned long startTime;
unsigned long elapsedTime;
bool start_count = false;

void send_alert(){
  EMailSender::EMailMessage message;
  message.subject = "Thông báo khẩn cấp";
  message.message = "Tôi bị ngã, hãy tới giúp tôi!";

  // EMailSender::Response resp = emailSend.send("quangsang0709@gmail.com", message);
  EMailSender::Response resp = emailSend.send("quocanle999@gmail.com", message);
  // EMailSender::Response resp3 = emailSend.send("cuongphan0318@gmail.com", message);

  Serial.println("Sending status: ");
  Serial.println(resp.status);
  Serial.println(resp.code);
  Serial.println(resp.desc);

  // Serial.println("Sending status2: ");
  // Serial.println(resp2.status);
  // Serial.println(resp2.code);
  // Serial.println(resp2.desc);

  // Serial.println("Sending status3: ");
  // Serial.println(resp3.status);
  // Serial.println(resp3.code);
  // Serial.println(resp3.desc);
  // digitalWrite(led_pin, HIGH);
}

float threshDrop = 2;
float thresh_lieDown = 1.3;
int count_lift = 0;
bool doAlert = false;
int time_send = 5000;

void loop() {
  int16_t accX, accY, accZ;
  mpu.getAcceleration(&accX, &accY, &accZ);
  float accX1 = accX/16384.0;
  float accY1 = accY/16384.0;
  float accZ1 = accZ/16384.0;
  float threshHold = sqrt(accX1*accX1 + accY1*accY1 + accZ1*accZ1);
  Serial.print("threshHold: ");  Serial.println(threshHold);

  if(threshHold > threshDrop){
    Serial.print("threshHold: ");  Serial.println(threshHold);
    delay(3000);

    startTime = millis();
    start_count = true;
    
    // 2. Start detecting if lifting the stick (if lift stick then break the loop)
    while(true){
      mpu.getAcceleration(&accX, &accY, &accZ);
      float accX1 = accX/16384.0;
      float accY1 = accY/16384.0;
      float accZ1 = accZ/16384.0;
      threshHold = sqrt(accX1*accX1 + accY1*accY1 + accZ1*accZ1);

      if(threshHold > thresh_lieDown){
        count_lift += 1;
        Serial.print("lift: ");  Serial.println(count_lift);
      }

      //Stuation the stick not intended blow away
      if(count_lift >= 10){
        doAlert = false;
        Serial.println("BREAK count_lift >=10");
        count_lift = 0;
        break;
      }

      if((millis() - startTime) >= time_send){
        doAlert = true;
        Serial.print("doAlert: ");  Serial.println(doAlert);
        break;
      }
      Serial.println(millis() - startTime);
    }
  }

  if(doAlert == true){
    send_alert();
    doAlert = false;
  }

  delay(20);
}


