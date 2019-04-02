
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#include <ArduinoJson.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

#define DHTTYPE DHT11   // DHT 11
// Cập nhật thông tin
// Thông tin về wifi
// SDA D2
// SCL D1
#define ssid "lophocvui.com 2.4GHz"
#define password "88889999"
// Thông tin về MQTT Broker
#define mqtt_server "192.168.1.3" // Thay bằng địa chỉ IP 
#define mqtt_topic_pub "test"   //Giữ nguyên nếu bạn tạo topic tên là demo
#define mqtt_topic_sub "test"
#define mqtt_user "nhung"    //Giữ nguyên nếu bạn tạo user là lophocvui và pass là 123456
#define mqtt_pwd "123456"
 
const uint16_t mqtt_port = 1883; //Port của MQTT
int cambien = D3;// khai báo chân digital 10 cho cảm biến
int Led = D8;//kháo báo chân digital 8 cho đèn LED
 
WiFiClient espClient;
PubSubClient client(espClient);
 
const int DHTPin = D5;       //Đọc dữ liệu từ DHT11 ở chân D5 trên mạch esp8266
 
long lastMsg = 0;
char msg[50];
int value = 0;
// Timers auxiliar variables
long now = millis();
long lastMeasure = 0;
 
// Thiet Lap cam bien DHT.
DHT dht(DHTPin, DHTTYPE);

const long utcOffsetInSeconds = 7*60*60;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", utcOffsetInSeconds);

LiquidCrystal_I2C lcd(0x3F,16,2);
void setup() {
  pinMode(Led,OUTPUT);//pinMode xuất tín hiệu đầu ra cho led
  pinMode(cambien,INPUT);//pinMode nhận tín hiệu đầu vào cho cảm biê
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port); 
  dht.begin();
  timeClient.begin();
  lcd.begin();
}
// Hàm kết nối wifi
void setup_wifi() {
  delay(10);
  
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
}
// Hàm reconnect thực hiện kết nối lại khi mất kết nối với MQTT Broker
void reconnect() {
  // Chờ tới khi kết nối
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    lcd.setCursor(0, 1);
    lcd.printf("Connecting.");

    // Thực hiện kết nối với mqtt user và pass
    if (client.connect("ESP8266Client",mqtt_user,mqtt_pwd)) {
      Serial.println("connected");
      // Khi kết nối sẽ publish thông báo
      client.publish(mqtt_topic_pub, "ESP_reconnected");
      // ... và nhận lại thông tin này
      client.subscribe(mqtt_topic_sub);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Đợi 5s
      delay(5000);
    }
  }
}
void loop() {
 
  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
    client.connect("ESP8266Client");

  now = millis();
  // Publishes new temperature and humidity every 30 seconds
  timeClient.update();
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();
  if (now - lastMeasure > 1000) {
    lastMeasure = now;
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    int c = digitalRead(cambien);//lưu giá trị cảm biến vào biến value
    digitalWrite(Led,c);//xuất giá trị ra đèn LED
    if (isnan(h) || isnan(t)) {
      Serial.println("\nFailed to read from DHT sensor!");
      return;
    }
    static char temperatureTemp[7];
    dtostrf(t, 6, 2, temperatureTemp);
    
    static char humidityTemp[7];
    dtostrf(h, 6, 2, humidityTemp);

    data["SensorID"] = "SN-001";
    data["Temperature"] = t ;
    data["Humidity"] = h;
    data["Light"] = c;
    data["Time"] = String(timeClient.getFormattedTime());
    char payload[256];
    data.printTo(payload, sizeof(payload));
    String strPayload = String(payload);
    Serial.print("Format data: ");
    Serial.println(strPayload);
    client.publish("home/sensors/room1", strPayload.c_str());
    
    lcd.setCursor(0, 0);
    lcd.printf("Temp:%s*C",temperatureTemp);
    lcd.setCursor(0, 1);
    lcd.printf("Hum:%s",humidityTemp);
    lcd.print("%");
    
    Serial.print("\nHumidity: ");
    Serial.print(h);
    Serial.print(" %\t Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.print("\n ");
    delay(3000);
  }
}
