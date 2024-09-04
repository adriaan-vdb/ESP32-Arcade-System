// Pin numbers for joystick x, y and switch
const int joystick_x = 1;
const int joystick_y = 2;
const int joystick_sw = 3;

// Pin numbers for buttons
const int button_1 = 11;
const int button_2 = 10;

void setup() {
  // put your setup code here, to run once:
  // NOTE: analog inputs (i.e. joystick_x and joystick_y, don't need to be configured)
  pinMode(joystick_sw, INPUT_PULLUP);
  // digitalWrite(joystick_sw, HIGH); // Random line that must be included
  pinMode(button_1, INPUT_PULLUP);
  pinMode(button_2, INPUT_PULLUP);

  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);

  Serial.begin(115200);
}

void debugText(int x, int y, int sw, int b1, int b2) {
  Serial.print("X: ");
  Serial.println(x);
  Serial.print("Y: ");
  Serial.println(y);
  Serial.print("SW: ");
  Serial.println(sw);
  Serial.print("B1: ");
  Serial.println(b1);
  Serial.print("B2: ");
  Serial.println(b2);
}

void loop() {
  // put your main code here, to run repeatedly:
  int joystick_x_value = analogRead(joystick_x);
  int joystick_y_value = analogRead(joystick_y);
  int joystick_sw_value = digitalRead(joystick_sw);
  int button_1_value = digitalRead(button_1);
  int button_2_value = digitalRead(button_2);

  // Variables set to {-1, 0, 1} depending on position of joystick
  int x = (joystick_x_value < 1500) ? -1 : (joystick_x_value > 2500) ? 1 : 0;
  int y = (joystick_y_value < 1500) ? 1 : (joystick_y_value > 2500) ? -1 : 0;
  int sw = !joystick_sw_value;
  int b1 = !button_1_value;
  int b2 = !button_2_value;

  // Render variables
  debugText(x, y, sw, b1, b2);
  delay(100);
}
