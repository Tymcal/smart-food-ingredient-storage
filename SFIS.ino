#include<ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#define LED_PIN 4
#define DHTPIN 5
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Thepinesvilla_2.4G"; //teerawut_2.4G
const char* password = "0944260205"; //Hme1164741
const char* mqtt_server = "broker.netpie.io"; 
const int mqtt_port = 1883;
const char* mqtt_Client = "e3b63545-69fc-4b4d-b8af-9c7e8c82f73d";  //ClientID
const char* mqtt_username = "HeCZ9xeiny65c5Nq37o25bXh7UzL6nUS";  //Token
const char* mqtt_password = "f-cSgoxIy25Sb_p-mrddoY-4mu1FoM07"; //Secret

WiFiClient espClient; 
PubSubClient client(espClient);

long lastMsg = 0;
int value = 0;
char msg[100];
String DataString;

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
  if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) { //เชื่อมต่อกับ MQTT BROKER
    Serial.println("connected");
    client.subscribe("@msg/operator");
  }
  else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println("try again in 5 seconds");
    delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String message;
  for (int i = 0; i < length; i++) {
    message = message + char(payload[i]);
    
  }
  Serial.println(message);
  if(String(topic) == "@msg/operator") {
    if (message == "ON"){
      digitalWrite(LED_PIN,HIGH);
      //client.publish("@shadow/data/update", "{\"data\" : {\"led\" : \"on\"}}");
      Serial.println("LED ON"); }
    else if (message == "OFF") {
      digitalWrite(LED_PIN,LOW);
      //client.publish("@shadow/data/update", "{\"data\" : {\"led\" : \"off\"}}");
      Serial.println("LED OFF"); }
    } 
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN,OUTPUT);
  dht.begin();
/*
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback); //ตั้งค่าฟังก์ชันที่จะทำงานเมื่อมีข้อมูลเข้ามาผ่านการ Subscribe
  client.subscribe("@msg/operator");
  */

}

void loop() {
/*
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  */
  long now = millis();
  if (now - lastMsg > 5000) { //จับเวลาส่งข้อมูลทุก ๆ 5 วินาที
    lastMsg = now;
    ++value;

    //DHT
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    Serial.printf("Humidity: %.2f, Temperature: %.2f \n", h, t);

    //IR Proximity Sensor
    int boxlen = 11;
    int IRRX = analogRead(A0);
    Serial.print(IRRX);

    //convert analogread to centimeter
    int currentlen = 0.009*IRRX + 0.8729;
    Serial.printf(" %d cm", currentlen);

    //convert centimeter to percentage
    int percent = ((100*currentlen)/boxlen);

    if (percent < 0 || percent > 100)
      Serial.printf("please close the box\n");
      else
      Serial.printf("%d%%\n", percent);
    /*
    DataString = "{\"data\":{\"temperature\":"+(String)t + ",\"humidity\":" + (String)h  + ",\"IR\":" + (String)IRRX + "}}"; 
    // Example of data : {"data":{"temperature":25 , "humidity": 60, "IR": XXXX}}
    DataString.toCharArray(msg, 100);
    Serial.println("Hello NETPIE2020");
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("@shadow/data/update", msg); //ส่งข้อมูลไปยัง Real Time Database (Shadow)
  */
  }
  delay(1);
}
