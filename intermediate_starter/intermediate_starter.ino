// Libraries
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
                          "latitude=23" // TODO: REPLACE ? WITH LATITUDE
                          "&longitude=32" // TODO: REPLACE ? WITH LONGITUDE
                          "&current=temperature_2m,relative_humidity_2m,apparent_temperature,weather_code"
                          "&daily=temperature_2m_max,temperature_2m_min"
                          "&timezone=America%2FNew_York"
                          "&forecast_days=1"
                          "&temperature_unit=fahrenheit";

// TODO: Create LCD object here


void setup() {
  // Begin serial
  Serial.begin(115200);

  // Initialize the LCD
  lcd.init();

  // TODO: Turn on LCD backlight

  Serial.print("Connecting to WiFi...");

  // TODO: Print connecting to LCD

  // TODO: Connect to WiFi

  // TODO: Wait for WiFi to connect


  Serial.println("Connected!")
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    // TODO: Create WiFi and HTTP client objects

    // TODO: Begin HTTP request

    // TODO: Perform GET request
    
    // HTTP 200 = OK
    if (httpCode == 200) {
      // TODO: Get response as string

      // TODO: Create JSON object from string

      // TODO: Get temperature (int) and condition (const char*) from JSON

      // TODO: Write temperature and condition to LCD
    } else {
      // TODO: Print HTTP request failed

    }

    // Close connection, free up memory
    http.end();
  } else {
      Serial.println("Disconnected from WiFi");
      
      // TODO: Print disconnected to LCD

  }

  // Wait before next update
  // Ensure no more than 10k calls per day (OpenMeteo limit)
  delay(10000);
}

// Returns corresponding weather condition (const char*) from weather code (int)
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
