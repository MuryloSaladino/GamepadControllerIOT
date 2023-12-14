#include <Arduino.h>
#include <BleGamepad.h>
#include <BleConnectionStatus.h>

#define numOfButtons 16
#define numOfHatSwitches 4

const uint8_t A = 1;
const uint8_t B = 2;
const uint8_t X = 4;
const uint8_t Y = 5;

const uint8_t LB = 7;
const uint8_t RB = 8;

const uint8_t BACK = 11;
const uint8_t START = 12;
const uint8_t GUIDE = 13;

const uint8_t LS_B = 14;
const uint8_t RS_B = 15;

const uint8_t BUTTONS[] = {A,B,X,Y,LB,RB,BACK,START,GUIDE,LS_B,RS_B};
const uint8_t BUTTONS_PINS[] = {23,22,21,19,18,5,4,2,15,13,12};


BleGamepad controller;
BleGamepadConfiguration BleGamepadConfig;


void setup()
{
    Serial.begin(115200);
    Serial.println("Starting BLE work!");
    BleGamepadConfig.setAutoReport(false);
    BleGamepadConfig.setControllerType(CONTROLLER_TYPE_GAMEPAD); 
    BleGamepadConfig.setButtonCount(numOfButtons);
    BleGamepadConfig.setHatSwitchCount(numOfHatSwitches);
    BleGamepadConfig.setVid(0xe502);
    BleGamepadConfig.setPid(0xabcd);
	BleGamepadConfig.setAxesMin(0x0000);
    BleGamepadConfig.setAxesMax(0x7FFF);
    controller.begin(&BleGamepadConfig);
    for (auto pin : BUTTONS_PINS)
    {
        pinMode(pin, INPUT);
    }
}

void loop()
{
    if (controller.isConnected())
    {
        for(uint8_t i = 0; i < 11; i++)
        {
            if(digitalRead(BUTTONS_PINS[i]))
            {
                controller.press(BUTTONS[i]);
            }
            else
            {
                controller.release(BUTTONS[i]);
            }
        }
        

        controller.sendReport();
    }
}