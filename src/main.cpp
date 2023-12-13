#include <Arduino.h>
#include <BleGamepad.h>
#include <BleConnectionStatus.h>

#define numOfButtons 16
#define numOfHatSwitches 4

const uint8_t BUTTONS[] = {1,2,4,5,7,8,11,12,13,14,15}; 

BleGamepad bleGamepad;
BleGamepadConfiguration bleGamepadConfig;

void setup()
{
    Serial.begin(115200);
    Serial.println("Starting BLE work!");
    bleGamepadConfig.setAutoReport(false);
    bleGamepadConfig.setControllerType(CONTROLLER_TYPE_GAMEPAD); // CONTROLLER_TYPE_JOYSTICK, CONTROLLER_TYPE_GAMEPAD (DEFAULT), CONTROLLER_TYPE_MULTI_AXIS
    bleGamepadConfig.setButtonCount(numOfButtons);
    bleGamepadConfig.setHatSwitchCount(numOfHatSwitches);
    bleGamepadConfig.setVid(0xe502);
    bleGamepadConfig.setPid(0xabcd);
    // Some non-Windows operating systems and web based gamepad testers don't like min axis set below 0, so 0 is set by default
    //bleGamepadConfig.setAxesMin(0x8001); // -32767 --> int16_t - 16 bit signed integer - Can be in decimal or hexadecimal
	bleGamepadConfig.setAxesMin(0x0000); // 0 --> int16_t - 16 bit signed integer - Can be in decimal or hexadecimal
    bleGamepadConfig.setAxesMax(0x7FFF); // 32767 --> int16_t - 16 bit signed integer - Can be in decimal or hexadecimal 
    bleGamepad.begin(&bleGamepadConfig); // Simulation controls, special buttons and hats 2/3/4 are disabled by default
}

void loop()
{
    
}