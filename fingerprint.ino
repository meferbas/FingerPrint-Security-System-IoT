#include <cvzone.h>
#include <ESP8266WiFi.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>


SoftwareSerial mySerial(D2, D1);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

const int redLedPin = D5;  // Kırmızı LED için pin tanımı
const int greenLedPin = D6; // Sarı LED için pin tanımı

const char* ssid = "Wi-Fi_NAME";      // Wi-Fi SSID
const char* password = "Wi-Fi_PASSWORD";  // Wi-Fi Şifresi

const char* server = "api.thingspeak.com";
const String apiKey = "API_KEY";  // ThingSpeak API Anahtarı

WiFiClient client;

void setup() {
  Serial.begin(115200);
  while (!Serial); 
  delay(100);
  Serial.println("Adafruit parmak izi testi başlatılıyor...");

  mySerial.begin(57600);
  finger.begin(57600);

  pinMode(redLedPin, OUTPUT);     
  pinMode(greenLedPin, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.println("WiFi bağlandı");

  if (finger.verifyPassword()) {
    Serial.println("Parmak izi sensörü bulundu!");
  } else {
    Serial.println("Parmak izi sensörü bulunamadı.");
    while (1) { delay(1); }
  }

  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  Serial.print(F("Security level: ")); Serial.println(finger.security_level);
  Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);

  finger.getTemplateCount();

  if (finger.templateCount == 0) {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  } else {
    Serial.println("Waiting for a valid finger...");
    Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  }
}

void loop() {
  static unsigned long lastPrintTime = 0;
  const unsigned long printInterval = 5000; 

  // Durum mesajını belirli aralıklarla yazdır
  if (millis() - lastPrintTime >= printInterval) {
    Serial.println("Parmak izi bekleniyor...");
    lastPrintTime = millis();
  }

  // Parmak izi okuma işlemi
  if (finger.getImage() == FINGERPRINT_OK) {
    Serial.println("Parmak izi algılandı.");
    int result = getFingerprintID();
    if (result == -1) {
      digitalWrite(redLedPin, HIGH);   // Yanlış parmak izi için kırmızı LED'i yak
      digitalWrite(greenLedPin, LOW); // Sarı LED'i söndür
      Serial.write("N");
      sendToThingSpeak(0); 
    } else {
      digitalWrite(redLedPin, LOW);    // Kırmızı LED'i söndür
      digitalWrite(greenLedPin, HIGH); // Doğru parmak izi için sarı LED'i yak
      Serial.write("Y");
      sendToThingSpeak(1);
    }
  } else {
    // Parmak izi okunamadığında her iki LED'i de söndür
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, LOW);
  }
  delay(100);

}
int getFingerprintID() {
  uint8_t p = finger.image2Tz();
  int fingerID = -1;

  if (p != FINGERPRINT_OK) {
    Serial.println("Görüntü dönüştürülemedi.");
    return fingerID;
  }

  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    fingerID = finger.fingerID;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Eşleşme bulunamadı.");
    fingerID = -1;
  } else {
    Serial.println("Bilinmeyen hata.");
  }

  return fingerID;
}

void sendToThingSpeak(int value) {
  if (client.connect(server, 80)) {
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(value);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);


    delay(1000); // İstek tamamlanana kadar bekleyin

    client.stop();
  } 
}