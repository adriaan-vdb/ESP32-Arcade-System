/*

For now the code just outputs to the serial monitor the following values:
x: -1, 0, 1 (left, middle or right for joystick)
y: -1, 0, 1 (down, middle or up for joystick)
sw: 0, 1 (1 when joystick is pressed)
b1: 0, 1 (1 when button 1 is pressed)
b2: 0, 1 (1 when button 2 is pressed)

In future, will adjust code to be able to:
1. Send values to a receiver
2. Display stuff to the screen

*/

// Pin numbers for joystick x, y and switch; originally, 1 2 3
const int joystick_x = 1;
const int joystick_y = 2;
const int joystick_sw = 3;

// Pin numbers for buttons; originally 11 10
const int button_1 = 11;
const int button_2 = 10;

#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
    int joystick_x_value;
    int joystick_y_value;
    int joystick_sw_value;
    int button_1_value;
    int button_2_value;
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

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
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Transmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
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

  myData.joystick_x_value = joystick_x_value;
  myData.joystick_y_value = joystick_y_value;
  myData.joystick_sw_value = joystick_sw_value;
  myData.button_1_value = button_1_value;
  myData.button_2_value = button_2_value;
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(100);
}
