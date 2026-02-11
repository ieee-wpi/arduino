/*
 * ESP8266 Radio Client - STARTER CODE
 * 
 * Connects to WiFi, reads potentiometer to select station (0-9),
 * and plays songs from server when play button is pressed.
 * 
 * Hardware:
 * - Buzzer on GPIO 14 (D5)
 * - Potentiometer on A0 (only analog input on ESP8266)
 * - Play button on GPIO 5 (D1) - press to play selected station
 * - Loop button on GPIO 4 (D2) - toggle loop on/off
 * 
 * Configuration:
 * - Set WIFI_SSID and WIFI_PASSWORD
 * - Set SERVER_IP to your server's IP address
 */

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

// WiFi credentials - UPDATE THESE
const char* WIFI_SSID = "WPI-IEEE-ARDUINO-WS";
const char* WIFI_PASSWORD = "wpiieee26";

// Server configuration - UPDATE THIS
const char* SERVER_IP = "192.168.0.100";  // Change to your server's IP
const int SERVER_PORT = 5000;
const char* SERVER_URL = "/song";

// Hardware pins
const int BUZZER_PIN = 14;          // GPIO 14 (D5)
const int POTENTIOMETER_PIN = A0;   // A0 (only ADC pin on ESP8266)
const int PLAY_BUTTON_PIN = 5;      // GPIO 5 (D1) - press to play song
const int LOOP_BUTTON_PIN = 4;      // GPIO 4 (D2) - toggle loop on/off

// Station configuration
const int NUM_STATIONS = 10;
const int POTENTIOMETER_MAX = 1023;  // ESP8266 ADC is 10-bit (0-1023)

// Song data storage
const int MAX_SONG_LENGTH = 100;
int songFrequencies[MAX_SONG_LENGTH];
int songDurations[MAX_SONG_LENGTH];
int songLength = 0;

// Station and playback state
int selectedStation = -1;  // Station selected by potentiometer
int currentNoteIndex = 0;
bool isPlaying = false;
bool loopEnabled = false;  // Loop toggle state
unsigned long lastNoteTime = 0;  // Timing for note playback

// Button state (for debouncing)
bool lastPlayButtonState = HIGH;
bool lastLoopButtonState = HIGH;

// WiFiClient instance for HTTP requests
WiFiClient wifiClient;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Configure pins
  pinMode(BUZZER_PIN, OUTPUT);
  // A0 does not need pinMode on ESP8266
  pinMode(PLAY_BUTTON_PIN, INPUT_PULLUP);  // Button with internal pull-up
  pinMode(LOOP_BUTTON_PIN, INPUT_PULLUP);  // Button with internal pull-up
  
  Serial.println("\nESP8266 Radio Client Starting...");
  
  // Connect to WiFi
  connectToWiFi();
  
  Serial.println("Setup complete!");
  Serial.println("Turn potentiometer to select station (0-9)");
  Serial.println("Press play button to play the selected station");
  Serial.println("Press loop button to toggle looping");
}

void loop() {
  // TODO 1: Check WiFi connection
  //   - If WiFi.status() is not WL_CONNECTED, print a message and call connectToWiFi(), then return


  // TODO 2: Read potentiometer and determine selected station
  //   - Use analogRead() to read POTENTIOMETER_PIN
  //   - Use map() to convert the value (0 to POTENTIOMETER_MAX) to a station number (0 to NUM_STATIONS - 1)
  //   - Use constrain() to keep it in bounds


  // TODO 3: Update selectedStation if it changed
  //   - If the new station differs from selectedStation, update it and print the new station


  // TODO 4: Check play button (LOW when pressed due to pull-up)
  //   - Read PLAY_BUTTON_PIN with digitalRead()
  //   - Detect a press: button is LOW now but was HIGH last time (lastPlayButtonState)
  //   - Add a small delay(5) for debounce, then confirm button is still LOW
  //   - If confirmed, call playSelectedStation()
  //   - Update lastPlayButtonState


  // TODO 5: Check loop button (LOW when pressed due to pull-up)
  //   - Same debounce pattern as the play button
  //   - If confirmed, call toggleLoop()
  //   - Update lastLoopButtonState


  // TODO 6: Play current song if we have one
  //   - If isPlaying is true and songLength > 0, call playSong()


  delay(10);  // Small delay to prevent overwhelming the system
}

void connectToWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(WIFI_SSID);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nWiFi connection failed!");
  }
}

bool requestSong(int station) {
  HTTPClient http;
  String url = "http://";
  url += SERVER_IP;
  url += ":";
  url += String(SERVER_PORT);
  url += SERVER_URL;
  url += "?station=";
  url += String(station);
  
  Serial.print("Requesting song from: ");
  Serial.println(url);
  
  http.begin(wifiClient, url);  // ESP8266 requires WiFiClient as first arg
  http.setTimeout(5000);  // 5 second timeout
  
  int httpCode = http.GET();
  
  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    Serial.print("Response received: ");
    Serial.println(payload);
    
    // Parse JSON response
    DynamicJsonDocument doc(4096);
    DeserializationError error = deserializeJson(doc, payload);
    
    if (error) {
      Serial.print("JSON parsing failed: ");
      Serial.println(error.c_str());
      http.end();
      return false;
    }
    
    // Extract frequencies and durations
    JsonArray freqArray = doc["frequencies"];
    JsonArray durArray = doc["durations"];
    
    if (freqArray.size() != durArray.size()) {
      Serial.println("Error: frequencies and durations arrays have different lengths");
      http.end();
      return false;
    }
    
    songLength = min(freqArray.size(), (size_t)MAX_SONG_LENGTH);
    
    for (int i = 0; i < songLength; i++) {
      songFrequencies[i] = freqArray[i].as<int>();
      songDurations[i] = durArray[i].as<int>();
    }
    
    Serial.print("Song loaded: ");
    Serial.print(songLength);
    Serial.println(" notes");
    
    http.end();
    return true;
    
  } else {
    Serial.print("HTTP request failed with code: ");
    Serial.println(httpCode);
    http.end();
    return false;
  }
}

void playSelectedStation() {
  if (selectedStation < 0 || selectedStation >= NUM_STATIONS) {
    Serial.println("Invalid station selected");
    return;
  }
  
  Serial.print("Playing station: ");
  Serial.println(selectedStation);
  
  // Stop current playback
  isPlaying = false;
  noTone(BUZZER_PIN);
  
  // Request and load song
  if (requestSong(selectedStation)) {
    isPlaying = true;
    currentNoteIndex = 0;
    lastNoteTime = millis();
    Serial.println("Song started");
  } else {
    Serial.println("Failed to load song");
  }
}

void toggleLoop() {
  loopEnabled = !loopEnabled;
  if (loopEnabled) {
    Serial.println("Loop: ON");
  } else {
    Serial.println("Loop: OFF");
  }
}

void playSong() {
  if (songLength == 0) {
    return;
  }
  
  unsigned long currentTime = millis();
  
  // Check if it's time to play the next note
  if (currentTime - lastNoteTime >= (unsigned long)songDurations[currentNoteIndex]) {
    // Stop previous note
    noTone(BUZZER_PIN);
    
    // Move to next note
    currentNoteIndex++;
    
    // Check if we've reached the end of the song
    if (currentNoteIndex >= songLength) {
      if (loopEnabled) {
        // Loop back to beginning
        currentNoteIndex = 0;
        Serial.println("Song looped");
      } else {
        // Stop playing
        isPlaying = false;
        currentNoteIndex = 0;
        Serial.println("Song finished");
        return;
      }
    }
    
    // Play current note (skip if frequency is 0, which represents a rest)
    int freq = songFrequencies[currentNoteIndex];
    if (freq > 0) {
      tone(BUZZER_PIN, freq);
    } else {
      noTone(BUZZER_PIN);
    }
    
    lastNoteTime = currentTime;
  }
}
