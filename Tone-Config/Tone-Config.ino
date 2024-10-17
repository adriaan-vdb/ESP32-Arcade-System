// Use the tone() function on the ESP32-S3 to generate a tone on a specific GPIO pin

// Define the pin for tone output
const int tonePin = 18;  // Replace with the GPIO pin you're using

void setup() {
  // Initialize serial communication for debugging purposes
  Serial.begin(115200);


}

void loop() {
  // No additional logic required in loop for this example
  // Start playing a tone at 1000Hz (1kHz) on the defined pin
  tone(tonePin, 1047, 80);
  tone(tonePin, 1568, 120);

  delay(1000);

  tone(tonePin, 1175, 80);
  tone(tonePin, 1760, 120);

  delay(1000);
  
}