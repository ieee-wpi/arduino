/***************************************************
 * Arduino Music Recorder (STARTER CODE)
 *
 * Goal:
 *  - Use an analog input to select a musical note
 *  - Play the note on a speaker
 *  - Press a button to record notes
 *  - Press another button to play them back
 *
 * TODOs are marked clearly below.
 ***************************************************/

/* ================= NOTE TABLES (PROVIDED) ================= */

const char* NOTE_NAMES[] = {
    "NOTE_B0", "NOTE_C1", "NOTE_CS1", "NOTE_D1", "NOTE_DS1",
    "NOTE_E1", "NOTE_F1", "NOTE_FS1", "NOTE_G1", "NOTE_GS1",
    "NOTE_A1", "NOTE_AS1", "NOTE_B1",
    "NOTE_C2", "NOTE_CS2", "NOTE_D2", "NOTE_DS2",
    "NOTE_E2", "NOTE_F2", "NOTE_FS2", "NOTE_G2", "NOTE_GS2",
    "NOTE_A2", "NOTE_AS2", "NOTE_B2",
    "NOTE_C3", "NOTE_CS3", "NOTE_D3", "NOTE_DS3",
    "NOTE_E3", "NOTE_F3", "NOTE_FS3", "NOTE_G3", "NOTE_GS3",
    "NOTE_A3", "NOTE_AS3", "NOTE_B3",
    "NOTE_C4", "NOTE_CS4", "NOTE_D4", "NOTE_DS4",
    "NOTE_E4", "NOTE_F4", "NOTE_FS4", "NOTE_G4", "NOTE_GS4",
    "NOTE_A4", "NOTE_AS4", "NOTE_B4",
    "NOTE_C5", "NOTE_CS5", "NOTE_D5", "NOTE_DS5",
    "NOTE_E5", "NOTE_F5", "NOTE_FS5", "NOTE_G5", "NOTE_GS5",
    "NOTE_A5", "NOTE_AS5", "NOTE_B5",
    "NOTE_C6", "NOTE_CS6", "NOTE_D6", "NOTE_DS6",
    "NOTE_E6", "NOTE_F6", "NOTE_FS6", "NOTE_G6", "NOTE_GS6",
    "NOTE_A6", "NOTE_AS6", "NOTE_B6",
    "NOTE_C7", "NOTE_CS7", "NOTE_D7", "NOTE_DS7",
    "NOTE_E7", "NOTE_F7", "NOTE_FS7", "NOTE_G7", "NOTE_GS7",
    "NOTE_A7", "NOTE_AS7", "NOTE_B7",
    "NOTE_C8", "NOTE_CS8", "NOTE_D8", "NOTE_DS8"
};

const int NOTE_FREQS[] = {
  31, 33, 35, 37, 39, 41, 44, 46, 49, 52, 55, 58, 62,
  65, 69, 73, 78, 82, 87, 93, 98, 104, 110, 117, 123,
  131, 139, 147, 156, 165, 175, 185, 196, 208, 220, 233, 247,
  262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494,
  523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988,
  1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976,
  2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951,
  4186, 4435, 4699, 4978
};

/* ================= GLOBAL STATE ================= */

int prev_note = 0;

const int MAX_NUM_NOTES = 20;
int notes_for_song[MAX_NUM_NOTES] = {};
int song_index = 0;

/* ================= PIN DEFINITIONS ================= */

const int BUZZER_PIN = 2; //D2, buzzer output pin
const int NOTE_PIN  = A0; //Potentiometer input
const int RECORD_BTN  = 3; //D3, record button
const int PLAY_BTN    = 4; //D4, play button

/* ================= SETUP ================= */

void setup() {
  Serial.begin(9600);

  pinMode(BUZZER_PIN, OUTPUT);

  // Buttons use internal pull-ups
  pinMode(RECORD_BTN, INPUT_PULLUP);
  pinMode(PLAY_BTN, INPUT_PULLUP);

  Serial.println("=== Arduino Music Recorder ===");
  Serial.println("Turn the knob and press buttons to test wiring.");
}

/* ================= MAIN LOOP ================= */

void loop() {

  // TODO: Read analog input (0–1023)
  int analog_value = analogRead(NOTE_PIN);

  /* ================= START DEBUG ================= */
  // FOR DEBUGGING ONLY, COMMENT OUT WHEN DONE VERIFYING CIRCUIT IS CORRECT

  //Check potentiometer input
  Serial.print("Potentiometer Value: ");
  Serial.print(analog_value);

  Serial.print("\t\tRecord Button: ");
  Serial.print(digitalRead(RECORD_BTN)); //0 when pressed
  
  Serial.print("\tPlay Button: ");
  Serial.println(digitalRead(PLAY_BTN)); //0 when pressed
  
  if (digitalRead(PLAY_BTN) == LOW){
    tone(BUZZER_PIN,440,100);
  };

  /* ================= END DEBUG ================= */

  // TODO: Map analog value to a note index (0–87)
  int note_index = 0;

  // TODO: If the note changed:
  //  - Print note name + frequency
  //  - Play the tone
  //  - Update prev_note


  // TODO: If record button is pressed:
  //  - Debounce the button
  //  - Store the current note in notes_for_song[]
  //  - Increment song_index
  //  - Print confirmation to Serial


  // TODO: If play button is pressed:
  //  - Debounce the button
  //  - Loop through stored notes
  //  - Play each note with a delay
  //  - Stop when a 0 is encountered


  delay(100);
}