#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// WiFi Credentials
#define SSID "ieee"
#define PASSWORD "iloveieee"

// Open-Meteo API
const char* WEATHER_URL = "http://api.open-meteo.com/v1/forecast?"
                          "latitude=42.2743"
                          "&longitude=-71.8084"
                          "&current=temperature_2m,relative_humidity_2m,apparent_temperature,weather_code"
                          "&daily=temperature_2m_max,temperature_2m_min"
                          "&timezone=America%2FNew_York"
                          "&forecast_days=1"
                          "&temperature_unit=fahrenheit";

// Initalize LCD object (I2C address is 0x27)
LiquidCrystal_I2C lcd(0x27,16,2);

byte upArrow[8] = { 0x04, 0x0E, 0x1F, 0x04, 0x04, 0x04, 0x04, 0x04 };
byte downArrow[8] = { 0x04, 0x04, 0x04, 0x04, 0x04, 0x1F, 0x0E, 0x04 };

void setup() {
  Serial.begin(115200);

  // Initialize the LCD
  lcd.init();

  // Load custom characters into positions 0 and 1
  lcd.createChar(0, downArrow);
  lcd.createChar(1, upArrow);

  // Turn on LCD backlight
  lcd.backlight();

  // Set cursor position (x [0-15], y [0-1])
  lcd.setCursor(0,0);

  // Connect to WiFi
  Serial.print("Connecting to WiFi...");
  lcd.print("Connecting...");

  WiFi.begin(SSID, PASSWORD);

  // Wait for WiFi to connect
  while (WiFi.status() != WL_CONNECTED) {
    delay(100); // Ensure delay or yield is here — WDT will reset
  }

  // Connected to WiFI
  Serial.println("Connected!");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    // Start HTTP request
    http.begin(client, WEATHER_URL);

    // Perform GET request
    int httpCode = http.GET();

    // HTTP 200 OK
    if (httpCode == HTTP_CODE_OK) {
      // Get response
      String payload = http.getString();

      // Create JSON buffer
      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc, payload);

      // Successful JSON parsing
      if (!error) {
        // Get weather data from JSON
        int temp = doc["current"]["temperature_2m"];
        int min_temp = doc["daily"]["temperature_2m_min"][0];
        int max_temp = doc["daily"]["temperature_2m_max"][0];
        const char* condition = getWeatherCondition(doc["current"]["weather_code"]);

        Serial.println("Fetched weather data!");

        // Write to LCD
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(temp);
        lcd.print(char(223));
        lcd.print("F");

        lcd.setCursor(8, 0);
        lcd.print((char)0);
        lcd.print(min_temp);

        lcd.setCursor(12, 0);
        lcd.print((char)1);
        lcd.print(max_temp);

        lcd.setCursor(0, 1);
        lcd.print(condition);
      } else {
        // JSON parsing failed
        Serial.println("JSON Parsing Failed!");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Failed to parse!");
      }
    } else {
      // HTTP request failed
      Serial.print("HTTP Request Failed, Error Code");
      Serial.println(httpCode);

      // Write error code to LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Failed: ");
      lcd.print(httpCode);
    }

    // Close connection, free up memory
    http.end();
  } else {
      Serial.print("Disconnected from WiFi");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Disconnected!");
  }

  // Wait before next update
  // Ensure no more than 10k calls per day (OpenMeteo limit)
  delay(10000);
}

// Returns weather condition from weather code
const char* getWeatherCondition(int code) {
  switch (code) {
    case 0: case 1: return "Clear";
    case 2: return "Partly Cloudy";
    case 3: return "Overcast";
    case 45: case 48: return "Fog";
    case 51: case 53: case 55: return "Drizzle";
    case 56: case 57: return "Freezing Drizzle";
    case 61: case 63: case 65: return "Rain";
    case 66: case 67: return "Freezing Rain";
    case 71: case 73: case 75: case 77: return "Snow";
    case 80: case 81: case 82: return "Rain";
    case 85: case 86: return "Snow";
    case 95: case 96: case 99: return "Thunderstorm";
    default: return "Unknown";
  }
}
