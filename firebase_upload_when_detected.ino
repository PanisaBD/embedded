#define ENABLE_SERVICE_AUTH
#define ENABLE_DATABASE
#define ENABLE_FIRESTORE
#define ENABLE_FIRESTORE_QUERY
#define ENABLE_MESSAGING
#define ENABLE_STORAGE
#define ENABLE_CLOUD_STORAGE
#define ENABLE_FUNCTIONS

// For ESP32
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <FirebaseClient.h>
#include <FirebaseJson.h>
#include "esp_camera.h"
#include "base64.h"

// WIFI
#define WIFI_SSID "Redmi Note 14"
#define WIFI_PASSWORD "9yjmncrj9egi3yj"

// FIREBASE
#define API_KEY "AIzaSyDjuZu6T2rRkuGBGkx37qRWb_Ifdb8RYBQ"
#define DATABASE_URL "https://embedded-system-project-e1291-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define PROJECT_ID "embedded-system-project-e1291"
#define FIREBASE_CLIENT_EMAIL "firebase-adminsdk-fbsvc@embedded-system-project-e1291.iam.gserviceaccount.com"
const char PRIVATE_KEY[] PROGMEM = "-----BEGIN PRIVATE KEY-----\nMIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQDEMUNbczQD7Twv\n2kXwC4MxorLyNP2tCPng/FuwpzdRbdsz4X62qF6FDefFnwOW4vsh17WPN3t65jjh\ncrmsr+vnv0EEQou0ufqMZz5D3iuP4/3yeUUFRgykFpgYpu08fc+/uM9c2un4q6+1\nJT1Ct6plhpBcHRc1TSZdf82T8Efiz4vgMAARS9N83hQEP6q1/EhdJQqEp6CVaFAn\nm/HI1toi1AwooRqgbPke7nsnTjd2H5IDA6ZYWKHeE+vdB9fsMxE97+MhRilOTQal\nuSNbstOpRkg3RamFORw/IjERcysIrjv4a08j+zJf9Du52hKz/Wr2vnUjLiwXk9+5\nZ4UPDA+hAgMBAAECggEAHeBSnY6BkUeDLtZSaCeYpupbzb58EumXA8fqPpB4QBI2\nm0Pvg+lH24MulE2Owl92VsT+hPyx9Rkx+EL8O2vexL+/KuLnW3Im0VK3jKY8hAD4\nHwCQnij4q9Q0/SwOXHPcOxntmfpMPu4GJ9Ov7IUDkcpYGW7kpd1Fxl878xdYGxWz\nORz5Vd96jx0CS1AQ14R7XzrqylMGAH30/QFD/dcRjPxiB499zsIH/fCfZwH6ptYi\nqogcMuZh1uRQK0bv2WffibVfgWGHwtyGmxCUt7fnmfx3Dm90+7ioKAWgOXzgAEtN\n7H85EMjUTENaJ9+gm7ZK23ZgVo/Hff6nqyrEbygy8QKBgQD8/0CZKXLkgdAI0slB\n8L16EoXlUPf2O+LnC+Ht3Y2C6wkUs3twrICOLapI+Ra5+14flQsOeAo1PdJQhONq\n2I1a3zl8gpLIWEEOncfCy08+cEsoMHpBo+ILtvpiKQGxXEPrUTnysXJxXmwp0GvA\nO662+OWyVXMocVhLvW67The4DwKBgQDGhWgAvY2cNuKAxUGQSMBSdFLBVc6k0knc\nkNgNlkA5dgZMNgGDAfwLub2/rr7i72XIML9E/x/SlTjo0l43GP1qZsuBoxYOQFNY\nLr+eh6bOh8exckKl3/bS84mVrVCANdJMwlqRfn2KzWrD86VOYfe7Br3iLUDCPovg\nEhJzGwaNTwKBgQDjZx+OS9ZMlG4EcEqcLaJyBhI6E7Q9rIh/+otB/eGDqnVIGSe8\nGCGZKmykGNiXpNxucM42XCl4v6+51ih/0lQEOR73znnXAJTBaYqziYEZ819qqWBK\nfd7hfr/B2W//Fpqp+iGnBl6CjkeUshUX/c1GA6SazfmrgW3G7meIjmH9ewKBgG4x\nuws+LI7bwFquI7iNSYP7YbxDfxLqT2mJVQQltLywkMpY5/nqbhesIHF39ClPrXWz\n6VfJLcbcYdyKeRWoX3JiYPAysJl0h07ZlC/RKHxXzjbnL6uXaBCscmCM9bRVK8jD\n0TYVumYDpBHk+otJwhMkqN4VNG8d/NKB9Y/DHp3tAoGAbNbKegHXOOOzaqkZv3mg\nvURUVJwX2cd7lEx8rkPCfbXUWC8bHzyzzy+cwcSdMzwPfqxHZk8mDQC61L00je1D\nEWChuJZyLI1CdD0tj73hMxr2fm1qk4y5+wqf9VPdfK6Dww2TYDqBgLsMO5+8TCWH\nS2fX9xbU5BK5RIROxrMNvsY=\n-----END PRIVATE KEY-----\n";


// ESP32-CAM PINS (AI Thinker)
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

// Motion Detection Settings
#define MOTION_THRESHOLD 25
#define MOTION_PERCENT 0.15  // 15% pixel change

uint8_t *prev_buf = NULL;
size_t prev_len = 0;

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


// ======================== INIT GRAYSCALE CAMERA =========================
void initCameraGray() {
  camera_config_t config_cam;
  config_cam.ledc_channel = LEDC_CHANNEL_0;
  config_cam.ledc_timer = LEDC_TIMER_0;
  config_cam.pin_d0 = Y2_GPIO_NUM;
  config_cam.pin_d1 = Y3_GPIO_NUM;
  config_cam.pin_d2 = Y4_GPIO_NUM;
  config_cam.pin_d3 = Y5_GPIO_NUM;
  config_cam.pin_d4 = Y6_GPIO_NUM;
  config_cam.pin_d5 = Y7_GPIO_NUM;
  config_cam.pin_d6 = Y8_GPIO_NUM;
  config_cam.pin_d7 = Y9_GPIO_NUM;
  config_cam.pin_xclk = XCLK_GPIO_NUM;
  config_cam.pin_pclk = PCLK_GPIO_NUM;
  config_cam.pin_vsync = VSYNC_GPIO_NUM;
  config_cam.pin_href = HREF_GPIO_NUM;
  config_cam.pin_sscb_sda = SIOD_GPIO_NUM;
  config_cam.pin_sscb_scl = SIOC_GPIO_NUM;
  config_cam.pin_pwdn = PWDN_GPIO_NUM;
  config_cam.pin_reset = RESET_GPIO_NUM;
  config_cam.xclk_freq_hz = 20000000;

  config_cam.pixel_format = PIXFORMAT_GRAYSCALE;  // grayscale faster
  config_cam.frame_size = FRAMESIZE_QQVGA;        // 160x120 small frame
  config_cam.fb_count = 1;

  esp_err_t err = esp_camera_init(&config_cam);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed 0x%x\n", err);
    ESP.restart();
  }
}

// ======================== BASE64 UPLOAD =========================
void uploadBase64ToFirebase(camera_fb_t *fb) {
  Serial.println("Converting image to Base64...");
  String base64Image = base64::encode(fb->buf, fb->len);

  Serial.println("Uploading Base64 string to Firebase...");

  Database.set(async_client1, "/images", base64Image, processData, "RealtimeDatabase_SetTask");

  // bool ok = Firebase.RTDB.setString(
  //   &fbdo,
  //   "images/lastGrayImage",
  //   base64Image
  // );

  // if (ok) {
  //   Serial.println("Upload success!");
  // } else {
  //   Serial.println("Upload error: " + fbdo.errorReason());
  // }
}

void uploadBase64ToFirestore(camera_fb_t *fb) {
  Serial.println("Preparing Firestore document...");

  // Convert grayscale / jpeg buffer to Base64
  String base64Image = base64::encode(fb->buf, fb->len);

  // Create Firestore document content
  Document<Values::Value> doc;

  // Add fields
  doc.add("base64Image", Values::Value(Values::StringValue(base64Image)));
  doc.add("timestamp", Values::Value(Values::IntegerValue((uint64_t)time(nullptr))));
  doc.add("type", Values::Value(Values::StringValue("motion")));
  doc.add("format", Values::Value(Values::StringValue("grayscale")));
  doc.add("size", Values::Value(Values::IntegerValue(fb->len)));

  // Document path e.g. "motion_events/event_123"
  String documentPath = "motion_events/event_";
  documentPath += time(nullptr);

  Serial.println("Uploading Firestore document...");

  // Async create request (same as example)
  Docs.createDocument(
    async_client1,                  // your AsyncClient
    Firestore::Parent(PROJECT_ID),  // Firestore parent path
    documentPath,                   // Document path
    DocumentMask(),                 // No field mask
    doc,                            // The Firestore document
    processData,                    // Your callback
    "FirestoreUploadTask"           // Task ID
  );
}


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

// ======================== SETUP =========================
void setup() {
  Serial.begin(115200);
  delay(1000);

  // WiFi connect
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nWiFi connected!");

  // Camera (grayscale)
  initCameraGray();

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
}

// ======================== LOOP (MOTION DETECTION) =========================
void loop() {
  app.loop();
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) return;

  // First frame initialization
  if (!prev_buf) {
    prev_len = fb->len;
    prev_buf = (uint8_t *)malloc(prev_len);
    memcpy(prev_buf, fb->buf, prev_len);
    esp_camera_fb_return(fb);
    return;
  }

  long changed = 0;

  // SAMPLE PIXELS FOR SPEED (every 2px)
  for (int i = 0; i < fb->len; i += 2) {
    if (abs(fb->buf[i] - prev_buf[i]) > MOTION_THRESHOLD)
      changed++;
  }

  int totalChecked = fb->len / 2;
  float percent = (float)changed / totalChecked;

  if (percent > MOTION_PERCENT) {
    Serial.println("⚠️ MOTION DETECTED — Uploading GRAYSCALE Base64...");
    if (app.ready()) {
      // uploadBase64ToFirebase(fb);   // Upload to RTDB
      uploadBase64ToFirestore(fb);  // Upload to Firestore
    } else {
      Serial.println("Firebase not ready");
    }
    delay(5000);
  }

  // Update previous frame
  memcpy(prev_buf, fb->buf, fb->len);
  esp_camera_fb_return(fb);

  processData(dbResult);
  processData(fcmResult);
  processData(firestoreResult);
  processData(functionResult);
  processData(storageResult);
  processData(cloudStorageResult);

  delay(150);
}
