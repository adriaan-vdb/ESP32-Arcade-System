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

const int button_3 = 43;
const int button_4 = 44;

#include <esp_now.h>
#include <WiFi.h>
#include <TFT_eSPI.h> 

TFT_eSPI tft = TFT_eSPI(); 

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x30, 0x30, 0xF9, 0x59, 0x2D, 0x90};

int main_controller = 1;

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
    int joystick_x_value;
    int joystick_y_value;
    int joystick_sw_value;
    int button_1_value;
    int button_2_value;
    int main_controller; // 1 if main controller; 0 if not
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
  pinMode(button_3, INPUT_PULLUP);
  pinMode(button_4, INPUT_PULLUP);

  // Enable battery power
  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);

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

  tft.init();
  tft.setRotation(3);  // rotate so that display is horizontal
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(3);
}

void drawShapes(int x, int y, int sw, int b1, int b2) {
  int yy = 100;
  int jx = 270;
  int js = 20;
  int ss = 15;
  
  // Draw Outlines
  if (main_controller) {
    tft.drawString("MAIN CONTROLLER", 10, 10);

    tft.drawCircle(30, yy, 20, TFT_WHITE);
    tft.drawCircle(80, yy, 20, TFT_WHITE);

    tft.drawTriangle(jx+ss+js, yy, jx+ss, yy-js/2, jx+ss, yy+js/2, TFT_WHITE); // >
    tft.drawTriangle(jx-ss-js, yy, jx-ss, yy-js/2, jx-ss, yy+js/2, TFT_WHITE); // <
    tft.drawTriangle(jx-js/2, yy+ss, jx+js/2, yy+ss, jx, yy+ss+js, TFT_WHITE); // V
    tft.drawTriangle(jx-js/2, yy-ss, jx+js/2, yy-ss, jx, yy-ss-js, TFT_WHITE); // ^
    tft.drawCircle(jx, yy, 10, TFT_WHITE);
    

    // Fill conditionally
    tft.fillCircle(30, yy, 18, (b1 == 1) ? TFT_WHITE : TFT_BLACK);
    tft.fillCircle(80, yy, 18, (b2 == 1) ? TFT_WHITE : TFT_BLACK);

    tft.fillTriangle(jx+ss+js-2, yy, jx+ss+2, yy-js/2+2, jx+ss+2, yy+js/2-2, (x==1) ? TFT_WHITE : TFT_BLACK); // >
    tft.fillTriangle(jx-ss-js+2, yy, jx-ss-2, yy-js/2+2, jx-ss-2, yy+js/2-2, (x==-1) ? TFT_WHITE : TFT_BLACK); // <
    tft.fillTriangle(jx-js/2+2, yy+ss+2, jx+js/2-2, yy+ss+2, jx, yy+ss+js-2, (y==1) ? TFT_WHITE : TFT_BLACK); // V
    tft.fillTriangle(jx-js/2+2, yy-ss-2, jx+js/2-2, yy-ss-2, jx, yy-ss-js+2, (y==-1) ? TFT_WHITE : TFT_BLACK); // ^
    tft.fillCircle(jx, yy, 9, (sw == 1) ? TFT_WHITE : TFT_BLACK);
  }
  else {
    tft.drawString("ADD. CONTROLLER", 10, 10);
    tft.drawCircle(30, yy, 20, TFT_WHITE);

    tft.fillCircle(30, yy, 18, (b1 == 1) ? TFT_WHITE : TFT_BLACK);
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
  int button_3_value = digitalRead(button_3);
  int button_4_value = digitalRead(button_4);

  int x = 0;
  int y = 0;
  // Variables set to {-1, 0, 1} depending on position of joystick
  if (joystick_x_value < 1500 || joystick_x_value > 2500 || joystick_y_value < 1500 || joystick_y_value > 2500) {
    float angle = atan2(joystick_y_value-2000,joystick_x_value-2000);
    Serial.println(y-2000);
    Serial.println(x-2000);
    Serial.println(angle);
    float pi = 3.14159;
    float num = 2.6; // AV To improve up sensitivity
    if (angle > (num * -1) * pi/12 && angle <= num * pi/12) {
      x = 1;
    }
    else if (angle > num * pi/12 && angle <= (12-num) * pi/12) {
      y = 1;
    }
    else if (angle > (12 - num) * pi/12 || angle <= (num - 12) * pi/12) {
      x = -1;
    }
    else {
      y = -1;
    }
  }



  // int x = (joystick_x_value < 1500) ? -1 : (joystick_x_value > 2500) ? 1 : 0;
  // int y = (joystick_y_value < 1500) ? 1 : (joystick_y_value > 2500) ? -1 : 0;
  int sw = !joystick_sw_value;
  int b1 = !(button_1_value && button_3_value);
  int b2 = !(button_2_value && button_4_value);

  // Render variables
  debugText(x, y, sw, b1, b2);
  drawShapes(x, y, sw, b1, b2);

  myData.joystick_x_value = joystick_x_value;
  myData.joystick_y_value = joystick_y_value;
  myData.joystick_sw_value = joystick_sw_value;
  myData.button_1_value = button_1_value && button_3_value;
  myData.button_2_value = button_2_value && button_4_value;
  myData.main_controller = main_controller;

  // Switch from main controller to add. controller (additional)
  if (!myData.joystick_sw_value && !myData.button_1_value && !myData.button_2_value) {
    main_controller = !main_controller;
    delay(200);
    tft.fillScreen(TFT_BLACK);
  }
  else {
    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    
    if (result == ESP_OK) {
      Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }
  }
  delay(50);
}
