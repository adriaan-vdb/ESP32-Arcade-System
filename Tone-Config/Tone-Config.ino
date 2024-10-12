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
  tone(tonePin, 1000);  // Pin, frequency (in Hz)

  // Wait for 5 seconds
  delay(100);

  // Stop playing the tone
  noTone(tonePin);

  delay(500);
  
}