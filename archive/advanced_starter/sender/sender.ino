// Sender
#include <ESP8266WiFi.h>
#include <espnow.h>

#define BUTTON_PIN 2

// REPLACE WITH RECEIVER MAC Address
uint8_t broadcastAddress[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

// Message datatype
// Must match the receiver structure
typedef struct struct_message {
  bool enabled;
  bool hasString;
  char topline[17];
  char bottomline[17];
} struct_message;


// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}
 

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Configure the ESP to send data
  // TODO: Determine what role the sender should be
  // esp_now_set_self_role(ESP_NOW_ROLE_XXXX);

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  // Configure GPIO pins


  // TODO: Configure the button pin (GPIO 2) as an INPUT pin and enable 
  // the internal pullup
  // pinMode(BUTTON_PIN, XXXXXX);

}


// Create a struct_message called myData
struct_message myData;

void loop() {

  // TODO: Set myData.enabled depending on the reading on the button pin
  // myData.enabled = XXXXXX

  myData.hasString = false;
  myData.bottomline[0] = 0x00;

  int i = 0;

  if (Serial.available()) {
    delay(10);  // wait for all serial data to arrive
  }

  while (Serial.available() > 0) {
    myData.hasString = true;
    char inChar = Serial.read();

    if (i < 16) { // First line
      myData.topline[i] = inChar;
      myData.topline[i + 1] = 0x00;
    } else if (i < 32) { // The input is too long, its time to wrap to the next line
      myData.bottomline[i - 16] = inChar;
      myData.bottomline[i - 15] = 0x00;
    }
    i++;
  }

  // Send the data over esp now!!
  esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));

  delay(20);
}