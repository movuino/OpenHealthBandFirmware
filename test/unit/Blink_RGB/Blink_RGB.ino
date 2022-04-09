
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
 pinMode(PIN_LED1, OUTPUT);
 pinMode(PIN_LED2, OUTPUT);
// pinMode(PIN_LED3, OUTPUT);
 Serial.begin(115200);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_RED, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_RED, HIGH);    // turn the LED off by making the voltage LOW
  delay(1000);  
  digitalWrite(LED_BLUE, LOW); 
  delay(1000);  
  digitalWrite(LED_BLUE, HIGH); 
  delay(1000); 
//  digitalWrite(LED_GREEN, LOW); 
 // delay(1000);  
 // digitalWrite(LED_GREEN, HIGH); 
 // delay(1000);
 
}
