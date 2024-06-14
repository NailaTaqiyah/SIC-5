#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>

const char *ssid = "scawol";
const char *password = "12345678";
const char *serverName = "http://192.168.83.171:5000/submit";

#define SOUND_SENSOR_PIN 34 // Pin untuk sensor suara

// NTP Server details
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 3600;

void setup() {
  Serial.begin(115200);
  
  // Setup WiFi connection
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize NTP
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // Setup pin sensor suara
  pinMode(SOUND_SENSOR_PIN, INPUT);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Membaca nilai dari sensor suara
    int soundValue = analogRead(SOUND_SENSOR_PIN);

    // Get current time
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
      return;
    }
    char timeStringBuff[50];
    strftime(timeStringBuff, sizeof(timeStringBuff), "%Y-%m-%dT%H:%M:%S%z", &timeinfo);
    String timestamp = String(timeStringBuff);

    // Construct JSON payload
    String jsonPayload = "{\"sound\":";
    jsonPayload += soundValue;
    jsonPayload += ", \"timestamp\":\"";
    jsonPayload += timestamp;
    jsonPayload += "\"}";

    // Send POST request
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(jsonPayload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end();

    delay(1000); // Delay sebelum pengukuran berikutnya
  } else {
    Serial.println("WiFi not connected");
    delay(1000);
  }
}
