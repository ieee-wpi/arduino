// Reciever

#include <ESP8266WiFi.h>
#include <espnow.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>


LiquidCrystal_I2C lcd(0x27,16,2);

// Message datatype
// Must match the sender structure
typedef struct struct_message {
  bool enabled;
  bool hasString;
  char topline[17];
  char bottomline[17];
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Enable value: ");
  Serial.println(myData.enabled);

  // TODO: turn the built in LED on if myData.enabled is true
  // digitalWrite(LED_BUILTIN, XXXXXXX);

  // TODO: Display the string in the message on the LCD
  // Some helpful methods are: lcd.clear(), lcd.setCursor(x, y), and lcd.print(str)
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
    
  // TODO: Determine what role the sender should be
  // esp_now_set_self_role(ESP_NOW_ROLE_XXXX);

  // register the callback
  esp_now_register_recv_cb(OnDataRecv);

  // Initiate the LCD
  lcd.init();

  // Turn on LCD backlight
  lcd.backlight();

  // Set cursor position (x [0-15], y [0-1])
  lcd.setCursor(0,0);

  // Print message
  lcd.print("Hello");

  // Configure the built in LED as an output pin
  pinMode(LED_BUILTIN, OUTPUT);

}

void loop() {
  // Think: Why does this program work even though the loop() function is empty
}
