//https://docs.arduino.cc/tutorials/nano-33-iot/Bluetooth
//
// iphone use lightblue app to connected
// 1,2,3 for LED control test
// CNHsu 2022/2/9
// use windwo installed "arduino_create_agent" double click to web version
// arduino nano 33 BLE
// https://www.arduino.cc/en/Reference/ArduinoBLE
// use usb joystick app (need follow key define)
// 
#include <ArduinoBLE.h>

long previousMillis = 0;
int interval = 0;
int ledState = LOW;

BLEService ledService("180A"); // BLE LED Service

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEByteCharacteristic switchCharacteristic("2A57", BLERead | BLEWrite);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // set built in LED pin to output mode
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("Nano 33 IoT");
  BLE.setAdvertisedService(ledService);

  // add the characteristic to the service
  ledService.addCharacteristic(switchCharacteristic);

  // add service
  BLE.addService(ledService);

  // set the initial value for the characteristic:
  switchCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("BLE LED Peripheral");
}

void loop() {
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
      // if the remote device wrote to the characteristic,
      // use the value to control the LED:
      if (switchCharacteristic.written()) {
        Serial.println(switchCharacteristic.value());
        switch (switchCharacteristic.value()) {   // any value other than 0
           case 00:
            Serial.println("LED 00");

            break;
          case 'a':
            Serial.println("LED on");
            //digitalWrite(LED_BUILTIN, HIGH);            // will turn the LED on
            digitalWrite(LEDR, HIGH);
            delay(1000);
            break;
          case 'b':
              Serial.println("LED fast blink");
              digitalWrite(LED_BUILTIN, HIGH);         // will turn the LED on
              delay(500);
              digitalWrite(LED_BUILTIN, LOW);         // will turn the LED off
              delay(500);
              digitalWrite(LED_BUILTIN, HIGH);      // will turn the LED on
              delay(500);
              digitalWrite(LED_BUILTIN, LOW);       // will turn the LED off
            break;
          case 'c':
            Serial.println("LED slow blink");
            digitalWrite(LED_BUILTIN, HIGH);         // will turn the LED on
              delay(1000);
              digitalWrite(LED_BUILTIN, LOW);         // will turn the LED off
              delay(1000);
              digitalWrite(LED_BUILTIN, HIGH);      // will turn the LED on
              delay(1000);
              digitalWrite(LED_BUILTIN, LOW);       // will turn the LED off
            break;
          case 'd':
            Serial.println("LED RGB  ON");
            digitalWrite(LEDR, LOW);         // will turn the LED on
            delay(1000);
            digitalWrite(LEDR, HIGH);       // will turn the LED off
            delay(1000);
            digitalWrite(LEDG, LOW);         // will turn the LED on
            delay(1000);
            digitalWrite(LEDG, HIGH);       // will turn the LED off
            delay(1000);
            digitalWrite(LEDB, LOW);         // will turn the LED on
            delay(1000);
            digitalWrite(LEDB, HIGH);       // will turn the LED off
            delay(1000);
            digitalWrite(LEDR, LOW);         // will turn the LED on
            delay(500);
            digitalWrite(LEDG, LOW);         // will turn the LED on
            delay(500);
            digitalWrite(LEDB, LOW);         // will turn the LED on
            delay(3000);                        
            digitalWrite(LEDR, HIGH);       // will turn the LED off            
            delay(1000);
            digitalWrite(LEDG, HIGH);       // will turn the LED off            
            delay(1000);
            digitalWrite(LEDB, HIGH);       // will turn the LED off            
            delay(1000);            
            break; 
         case 'e':
            Serial.println("LEDR on");
            digitalWrite(LEDR, LOW);
            digitalWrite(LEDG, HIGH);
            digitalWrite(LEDB, HIGH);
            delay(1000);            
            Serial.println("LEDG on");
            digitalWrite(LEDR, HIGH);
            digitalWrite(LEDG, LOW);
            digitalWrite(LEDB, HIGH);            
            delay(1000);
            Serial.println("LEDB on");
            digitalWrite(LEDR, HIGH);
            digitalWrite(LEDG, HIGH);
            digitalWrite(LEDB, LOW);            
            delay(1000);
            Serial.println("LEDRB on");
            digitalWrite(LEDR, LOW);
            digitalWrite(LEDG, HIGH);
            digitalWrite(LEDB, LOW);            
            delay(1000);
            Serial.println("LEDRG on");
            digitalWrite(LEDR, LOW);
            digitalWrite(LEDG, LOW);
            digitalWrite(LEDB, HIGH);            
            delay(1000);               
            Serial.println("LEDRGB on");
            digitalWrite(LEDR, LOW);
            digitalWrite(LEDG, LOW);
            digitalWrite(LEDB, LOW);            
            delay(1000);          
            Serial.println("LEDGB on");
            digitalWrite(LEDR, HIGH);
            digitalWrite(LEDG, LOW);
            digitalWrite(LEDB, LOW);            
            delay(1000);                
            break;
          default:
            Serial.println(F("LED off"));
            digitalWrite(LED_BUILTIN, LOW);          // will turn the LED off
            digitalWrite(LEDR, HIGH);          // will turn the LED off
            digitalWrite(LEDG, HIGH);          // will turn the LED off
            digitalWrite(LEDB, HIGH);          // will turn the LED off
            break;
        }
      }
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
    digitalWrite(LED_BUILTIN, LOW);         // will turn the LED off
    digitalWrite(LEDR, HIGH);          // will turn the LED off
    digitalWrite(LEDG, HIGH);          // will turn the LED off
    digitalWrite(LEDB, HIGH);          // will turn the LED off    
  }
}
