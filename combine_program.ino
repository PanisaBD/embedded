#define ENABLE_SERVICE_AUTH
#define ENABLE_DATABASE
#define ENABLE_FIRESTORE
#define ENABLE_FIRESTORE_QUERY
#define ENABLE_MESSAGING
#define ENABLE_STORAGE
#define ENABLE_CLOUD_STORAGE
#define ENABLE_FUNCTIONS

// ---------------- WIFI -----------------
#include <WiFi.h>
#include <WiFiClientSecure.h>

// ---------------- FIREBASE (NEW CLIENT) -----------------
#include <FirebaseClient.h>  // New Firebase SDK

#define API_KEY "AIzaSyDjuZu6T2rRkuGBGkx37qRWb_Ifdb8RYBQ"
#define DATABASE_URL "https://embedded-system-project-e1291-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define PROJECT_ID "embedded-system-project-e1291"
#define FIREBASE_CLIENT_EMAIL "firebase-adminsdk-fbsvc@embedded-system-project-e1291.iam.gserviceaccount.com"
const char PRIVATE_KEY[] PROGMEM = "-----BEGIN PRIVATE KEY-----\nMIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQDEMUNbczQD7Twv\n2kXwC4MxorLyNP2tCPng/FuwpzdRbdsz4X62qF6FDefFnwOW4vsh17WPN3t65jjh\ncrmsr+vnv0EEQou0ufqMZz5D3iuP4/3yeUUFRgykFpgYpu08fc+/uM9c2un4q6+1\nJT1Ct6plhpBcHRc1TSZdf82T8Efiz4vgMAARS9N83hQEP6q1/EhdJQqEp6CVaFAn\nm/HI1toi1AwooRqgbPke7nsnTjd2H5IDA6ZYWKHeE+vdB9fsMxE97+MhRilOTQal\nuSNbstOpRkg3RamFORw/IjERcysIrjv4a08j+zJf9Du52hKz/Wr2vnUjLiwXk9+5\nZ4UPDA+hAgMBAAECggEAHeBSnY6BkUeDLtZSaCeYpupbzb58EumXA8fqPpB4QBI2\nm0Pvg+lH24MulE2Owl92VsT+hPyx9Rkx+EL8O2vexL+/KuLnW3Im0VK3jKY8hAD4\nHwCQnij4q9Q0/SwOXHPcOxntmfpMPu4GJ9Ov7IUDkcpYGW7kpd1Fxl878xdYGxWz\nORz5Vd96jx0CS1AQ14R7XzrqylMGAH30/QFD/dcRjPxiB499zsIH/fCfZwH6ptYi\nqogcMuZh1uRQK0bv2WffibVfgWGHwtyGmxCUt7fnmfx3Dm90+7ioKAWgOXzgAEtN\n7H85EMjUTENaJ9+gm7ZK23ZgVo/Hff6nqyrEbygy8QKBgQD8/0CZKXLkgdAI0slB\n8L16EoXlUPf2O+LnC+Ht3Y2C6wkUs3twrICOLapI+Ra5+14flQsOeAo1PdJQhONq\n2I1a3zl8gpLIWEEOncfCy08+cEsoMHpBo+ILtvpiKQGxXEPrUTnysXJxXmwp0GvA\nO662+OWyVXMocVhLvW67The4DwKBgQDGhWgAvY2cNuKAxUGQSMBSdFLBVc6k0knc\nkNgNlkA5dgZMNgGDAfwLub2/rr7i72XIML9E/x/SlTjo0l43GP1qZsuBoxYOQFNY\nLr+eh6bOh8exckKl3/bS84mVrVCANdJMwlqRfn2KzWrD86VOYfe7Br3iLUDCPovg\nEhJzGwaNTwKBgQDjZx+OS9ZMlG4EcEqcLaJyBhI6E7Q9rIh/+otB/eGDqnVIGSe8\nGCGZKmykGNiXpNxucM42XCl4v6+51ih/0lQEOR73znnXAJTBaYqziYEZ819qqWBK\nfd7hfr/B2W//Fpqp+iGnBl6CjkeUshUX/c1GA6SazfmrgW3G7meIjmH9ewKBgG4x\nuws+LI7bwFquI7iNSYP7YbxDfxLqT2mJVQQltLywkMpY5/nqbhesIHF39ClPrXWz\n6VfJLcbcYdyKeRWoX3JiYPAysJl0h07ZlC/RKHxXzjbnL6uXaBCscmCM9bRVK8jD\n0TYVumYDpBHk+otJwhMkqN4VNG8d/NKB9Y/DHp3tAoGAbNbKegHXOOOzaqkZv3mg\nvURUVJwX2cd7lEx8rkPCfbXUWC8bHzyzzy+cwcSdMzwPfqxHZk8mDQC61L00je1D\nEWChuJZyLI1CdD0tj73hMxr2fm1qk4y5+wqf9VPdfK6Dww2TYDqBgLsMO5+8TCWH\nS2fX9xbU5BK5RIROxrMNvsY=\n-----END PRIVATE KEY-----\n";

// Google sheet
#include <HTTPClient.h>
String GOOGLE_SHEET_URL = "https://script.google.com/macros/s/AKfycbweJH5zc8eZjz6cdOAUZo-ocuosxb51uP1JKVe-MUfhqvlwKmmNywA-Gz5R12293euV/exec";

// Firebase setup
void processData(AsyncResult &aResult);
void getMsg(Messages::Message &msg);
ServiceAuth sa_auth(FIREBASE_CLIENT_EMAIL, PROJECT_ID, PRIVATE_KEY, 3000);

FirebaseApp app;

WiFiClientSecure ssl_client1, ssl_client2;

using AsyncClient = AsyncClientClass;
AsyncClient async_client1(ssl_client1), async_client2(ssl_client2);

RealtimeDatabase Database;
Messaging messaging;
Firestore::Documents Docs;
Storage storage;
CloudStorage cstorage;
CloudFunctions cfunctions;

AsyncResult dbResult, fcmResult, firestoreResult, functionResult, storageResult, cloudStorageResult;


// ---------------- TIME -----------------
#include <time.h>

// ---------------- RC522 RFID -----------------
#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 5
#define RST_PIN 22
MFRC522 mfrc522(SS_PIN, RST_PIN);

// ---------------- GAS SENSOR -----------------
#define GAS_D0 13

// ---------------- DHT22 -----------------------
#include "DHT.h"
#define DHTPIN 14
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ---------------- FIRE SENSOR -----------------
#define FIRE_PIN 27

// ---------------- BUZZER ----------------------
#define BUZZER_PIN 26

// ---------------- WIFI LOGIN ------------------
#define WIFI_SSID "Redmi Note 14"
#define WIFI_PASSWORD "9yjmncrj9egi3yj"


// =====================================================
// ===============  TIME SYNC FUNCTION  ===============
// =====================================================
void setTimeFromNTP() {
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for time");
  time_t now = time(nullptr);
  while (now < 1000000000) {  // wait until time is valid
    Serial.print(".");
    delay(500);
    now = time(nullptr);
  }
  Serial.println("\nTime set!");
}


// =========================== FIREBASE CALLBACKS ===========================
void processData(AsyncResult &aResult) {
  // Exits when no result is available when calling from the loop.
  if (!aResult.isResult())
    return;

  if (aResult.isEvent())
    Firebase.printf("Event task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.eventLog().message().c_str(), aResult.eventLog().code());

  if (aResult.isDebug())
    Firebase.printf("Debug task: %s, msg: %s\n", aResult.uid().c_str(), aResult.debug().c_str());

  if (aResult.isError())
    Firebase.printf("Error task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.error().message().c_str(), aResult.error().code());

  if (aResult.available())
    Firebase.printf("task: %s, payload: %s\n", aResult.uid().c_str(), aResult.c_str());
}

void getMsg(Messages::Message &msg) {
  msg.topic("test");

  Messages::Notification notification;
  notification.body("Notification body").title("Notification title");

  msg.notification(notification);
}


// =====================================================
// ===========  FIREBASE PUSH FUNCTION  ===============
// =====================================================
void uploadToRealtimeDB(
  bool gasDanger,
  bool fireDanger,
  float temp,
  float hum) {
  Serial.println("⬆ Uploading to Realtime DB...");

  String jsonStr = "{";
  jsonStr += "\"gasDanger\":" + String(gasDanger ? "true" : "false") + ",";
  jsonStr += "\"fireDanger\":" + String(fireDanger ? "true" : "false") + ",";
  jsonStr += "\"temp\":" + String(temp) + ",";
  jsonStr += "\"hum\":" + String(hum) + ",";
  jsonStr += "\"timestamp\":" + String((uint64_t)time(nullptr));
  jsonStr += "}";

  Database.set<object_t>(async_client1, "/sensor", object_t(jsonStr), processData, "RealtimeDatabase_SetTask");
}

// =====================================================
// ===============   MILLIS TIMERS   ===================
// =====================================================
unsigned long lastSensorTime = 0;
const unsigned long SENSOR_INTERVAL = 10000;  // 10 seconds

// =====================================================
// =============== CHECK RFID CONTINUOUSLY =============
// =====================================================
String checkRFID() {
  String uid = "";

  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {

    for (byte i = 0; i < mfrc522.uid.size; i++) {
      uid += String(mfrc522.uid.uidByte[i], HEX);
    }

    Serial.println("RFID UID: " + uid);

    // Small beep (allowed)
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    delay(3000);
    uploadRFIDToGoogleSheet(uid);
  }

  return uid;
}

void uploadRFIDToGoogleSheet(String uid) {
  HTTPClient http;
  http.begin(GOOGLE_SHEET_URL);
  http.addHeader("Content-Type", "application/json");

  String json = "{\"uid\":\"" + uid + "\"}";

  int httpCode = http.POST(json);

  Serial.print("Google Sheets response: ");
  Serial.println(httpCode);

  http.end();
}

// =====================================================
// ========== READ SENSORS & UPLOAD EVERY 10 SEC =======
// =====================================================
void readSensorsAndUpload() {

  // GAS
  bool gasDanger = (digitalRead(GAS_D0) == LOW);

  // FIRE
  bool fireDanger = (digitalRead(FIRE_PIN) == LOW);

  // DHT
  float hum = dht.readHumidity();
  float temp = dht.readTemperature();

  Serial.println("---- SENSOR UPDATE ----");
  Serial.println(gasDanger ? "⚠ Gas detected!" : "Gas OK");
  Serial.println(fireDanger ? "🔥 Fire detected!" : "Fire OK");
  Serial.printf("Temp %.2f  Hum %.2f\n", temp, hum);
  Serial.println("------------------------");

  if (app.ready()) {
    uploadToRealtimeDB(gasDanger, fireDanger, temp, hum);
  }
}

// =====================================================
// ======================  SETUP  =======================
// =====================================================
void setup() {
  Serial.begin(115200);

  // ---------------- WIFI ------------------
  Serial.println("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nWiFi Connected!");

  setTimeFromNTP();

  // IMPORTANT: give Firebase the NTP time!
  time_t now = time(nullptr);
  app.setTime(now);
  Serial.printf("Firebase time set: %ld\n", now);

  ssl_client1.setInsecure();
  ssl_client2.setInsecure();

  // Firebase init
  initializeApp(async_client1, app, getAuth(sa_auth), processData, "🔐 authTask");

  app.getApp<RealtimeDatabase>(Database);
  app.getApp<Messaging>(messaging);
  app.getApp<Firestore::Documents>(Docs);
  app.getApp<Storage>(storage);
  app.getApp<CloudStorage>(cstorage);
  app.getApp<CloudFunctions>(cfunctions);

  Database.url(DATABASE_URL);

  // ---------------- RC522 ------------------
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("RFID Ready!");

  // ---------------- GAS --------------------
  pinMode(GAS_D0, INPUT);

  // ---------------- DHT22 ------------------
  dht.begin();

  // ---------------- FIRE SENSOR ------------
  pinMode(FIRE_PIN, INPUT);

  // ---------------- BUZZER -----------------
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.println("System Ready!");
}


// =====================================================
// ======================= LOOP ========================
// =====================================================
void loop() {

  app.loop();

  // ---------- RFID always active ----------
  String rfidUID = checkRFID();

  // BUZZER logic
  bool gasDanger = (digitalRead(GAS_D0) == LOW);
  bool fireDanger = (digitalRead(FIRE_PIN) == LOW);
  bool buzzerState = gasDanger || fireDanger;
  digitalWrite(BUZZER_PIN, buzzerState ? HIGH : LOW);

  // ---------- Sensors only every 10 sec ----------
  unsigned long now = millis();
  if (now - lastSensorTime >= SENSOR_INTERVAL) {
    lastSensorTime = now;

    readSensorsAndUpload();  // upload to Realtime DB
  }
}
