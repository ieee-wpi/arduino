// THIS IS ONLY TO TEST YOU'RE CONNECTTION TO YOU'RE ARDUINO NANO.
// AS YOU UPLOAD THE CODE AND IT SUCCESSFULLY COMPILES. YOU SHOULD SEE 
// A BLINKING LED ON YOUR ARDUINO NANO
// IEEE Arduino workshop
#define LED_PIN LED_BUILTIN

volatile bool ledState = false; 

void toggleLED() {
  ledState = !ledState; 
  digitalWrite(LED_PIN, ledState);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);


  noInterrupts(); 

  TCCR1A = 0;
  TCCR1B = 0;

  TCCR1B |= (1 << WGM12);

  TCCR1B |= (1 << CS12) | (1 << CS10);

  OCR1A = 15624; 

  TIMSK1 |= (1 << OCIE1A);

  interrupts();
}

ISR(TIMER1_COMPA_vect) {
  toggleLED();
}

void loop() {

}
