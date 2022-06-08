/*
 Example sketch for the PS4 Bluetooth library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <PS4BT.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside
BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so

/* You can create the instance of the PS4BT class in two ways */
// This will start an inquiry and then pair with the PS4 controller - you only have to do this once
// You will need to hold down the PS and Share button at the same time, the PS4 controller will then start to blink rapidly indicating that it is in pairing mode
PS4BT PS4(&Btd, PAIR);

// After that you can simply create the instance like so and then press the PS button on the device
//PS4BT PS4(&Btd);

bool printAngle, printTouch;
uint8_t oldL2Value, oldR2Value;

void setup() {

  pinMode(39, OUTPUT);
  pinMode(41, OUTPUT);
  
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
  Serial3.begin(115200);
  
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); // Halt
  }
  Serial.print(F("\r\nPS4 Bluetooth Library Started"));
}
void loop() {

  Usb.Task();

  if (PS4.connected()) {
    
    if (PS4.getAnalogButton(L2) || PS4.getAnalogButton(R2)) { // These are the only analog buttons on the PS4 controller
      Serial.print(F("\r\nL2: "));
      Serial.print(PS4.getAnalogButton(L2));
      Serial.print(F("\tR2: "));
      Serial.print(PS4.getAnalogButton(R2));
    }
    
    if (PS4.getAnalogButton(L2) != oldL2Value || PS4.getAnalogButton(R2) != oldR2Value) {// Only write value if it's different
      PS4.setRumbleOn(PS4.getAnalogButton(L2), PS4.getAnalogButton(R2));
    oldL2Value = PS4.getAnalogButton(L2);
    oldR2Value = PS4.getAnalogButton(R2);
    }
    
    if (PS4.getButtonClick(PS)) {
      Serial.print(F("\r\nPS"));
      PS4.disconnect();
    }
    else {
      if (PS4.getButtonPress(TRIANGLE)) {
        Serial.print(F("\r\nTriangle"));
      }
      if (PS4.getButtonClick(CIRCLE)) {
        Serial.print(F("\r\nCircle"));
        Serial.print(F("\r\nはっしゃ"));
        digitalWrite(39, LOW);
        digitalWrite(41, HIGH);
        delay(1000);
        digitalWrite(41, LOW);
        digitalWrite(39, HIGH);
        delay(1000);
        digitalWrite(39, LOW);
      }
      if (PS4.getButtonPress(CROSS)) {
        Serial.print(F("\r\nCross"));
        Serial1.write('s');
        Serial2.write('s');
        Serial3.write('s');
        Serial.print(F("\r\n展開スター�?"));
      }else if (PS4.getButtonPress(SQUARE)) {
        Serial.print(F("\r\nSquare"));
        Serial1.write('g');
        Serial2.write('g');
        Serial3.write('g');
        Serial.print(F("\r\n展開終わ�?"));
      }else if (PS4.getButtonPress(UP)) {
        Serial1.write('a');
        Serial2.write('a');
        Serial3.write('a');
        Serial.print(F("\r\nUp"));
        PS4.setLed(Red);  
      } else if (PS4.getButtonPress(RIGHT)) {
        Serial1.write('c');
        Serial2.write('c');
        Serial3.write('c');
        Serial.print(F("\r\nRight"));
        PS4.setLed(Blue);
      } else if (PS4.getButtonPress(DOWN)) {
        Serial1.write('b');
        Serial2.write('b');
        Serial3.write('b');
        Serial.print(F("\r\nDown"));
        PS4.setLed(Yellow);
      } else if (PS4.getButtonPress(LEFT)) {
        Serial1.write('d');
        Serial2.write('d');
        Serial3.write('d');
        Serial.print(F("\r\nLeft"));
        PS4.setLed(Green);
      }else if (PS4.getButtonPress(L1)){
        Serial.print(F("\r\nL1"));
        Serial1.write('l');
        Serial2.write('l');
        Serial3.write('l');
      }else if (PS4.getButtonPress(R1)){
        Serial.print(F("\r\nR1"));
        Serial1.write('r');
        Serial2.write('r');
        Serial3.write('r');
      }else {
        Serial1.write('z');
        Serial2.write('z');
        Serial3.write('z');
        Serial.print(F("\r\nStop"));
      }
      
      if (PS4.getButtonClick(SHARE)){
        Serial.print(F("\r\nShare"));
      }
      if (PS4.getButtonClick(OPTIONS)) {
        Serial.print(F("\r\nOptions"));
        printAngle = !printAngle;
      }
      if (PS4.getButtonClick(TOUCHPAD)) {
        Serial.print(F("\r\nTouchpad"));
        printTouch = !printTouch;
      }

      if (printAngle) { // Print angle calculated using the accelerometer only
        Serial.print(F("\r\nPitch: "));
        Serial.print(PS4.getAngle(Pitch));
        Serial.print(F("\tRoll: "));
        Serial.print(PS4.getAngle(Roll));
      }

      if (printTouch) { // Print the x, y coordinates of the touchpad
        if (PS4.isTouching(0) || PS4.isTouching(1)) // Print newline and carriage return if any of the fingers are touching the touchpad
          Serial.print(F("\r\n"));
        for (uint8_t i = 0; i < 2; i++) { // The touchpad track two fingers
          if (PS4.isTouching(i)) { // Print the position of the finger if it is touching the touchpad
            Serial.print(F("X")); Serial.print(i + 1); Serial.print(F(": "));
            Serial.print(PS4.getX(i));
            Serial.print(F("\tY")); Serial.print(i + 1); Serial.print(F(": "));
            Serial.print(PS4.getY(i));
            Serial.print(F("\t"));
          }
        }
      }
    }
  }
}
