#include "FirebaseESP8266.h" 
#include <ESP8266WiFi.h>
#include <Servo.h>

#define FIREBASE_HOST "실시간 데이터베이스 주소 (http:// 빼고, 마지막 / 빼고)" 
#define FIREBASE_AUTH "실시간 데이터베이스 비밀번호"
#define WIFI_SSID "와이파이 SSID" // 연결 가능한 wifi의 ssid
#define WIFI_PASSWORD "와이파이비밀번호" // wifi 비밀번호

FirebaseData firebaseData;
FirebaseJson json;

Servo servo;

void setup()
{
  Serial.begin(9600);
 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println();
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  servo.attach(D4,600,2400);
  servo.write(90); // move center

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
 
  firebaseData.setBSSLBufferSize(1024, 1024);
  firebaseData.setResponseSize(1024);
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
}

void loop()
{
  String strAngle = "";

  if(Firebase.getString(firebaseData, "angle")) // download angle data(string) from firebase
  {
    strAngle = firebaseData.stringData();
  } 

  Serial.print("angle : ");  //Serial.println(strAngle);
  int intAngle = strAngle.toInt(); // transfer string data to integer data
  Serial.println(intAngle);
  
  servo.write(intAngle); // move servo
  
  Firebase.setInt(firebaseData, "CurrentAngle", intAngle); // upload angle data(intger) to firebase
  
/* // data Download
  if(Firebase.getBool(firebaseData, "Bool Data Tag")){
  bool valBool = firebaseData.intData();
  // write Code...
 }
  
  if(Firebase.getInt(firebaseData, "Int Data Tag")){
  int valInt = firebaseData.intData();
  // write Code...
 }
 if(Firebase.getFloat(firebaseData, "Float Data Tag")){
  float valFloat = firebaseData.floatData();
  // write Code...
 }
 if(Firebase.getString(firebaseData, "String Data Tag")){
  String valStr = firebaseData.stringData();
  // write Code...
 }
*/
 
/* // data upload
 Firebase.setBool(firebaseData, "Bool Tag", Bool data);
 Firebase.setInt(firebaseData, "Integer Tag", Integer data);
 Firebase.setFloat(firebaseData, "Float Tag", Float Data);
 Firebase.setString(firebaseData, "String Tag", String Data);
*/
 delay(1000); // 1초마다 반복
}
