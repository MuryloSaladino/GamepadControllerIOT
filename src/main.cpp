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

const uint8_t UP = 1;
const uint8_t RIGHT = 2;
const uint8_t DOWN = 4;
const uint8_t LEFT = 8;

const uint8_t BUTTONS[] = {A,B,X,Y,LB,RB,BACK,START,GUIDE,LS_B,RS_B};
const uint8_t BUTTONS_PINS[] = {23,22,21,19,18,5,4,2,15,13,12};

const uint8_t HATS[] = {UP,RIGHT,DOWN,LEFT};
const uint8_t HATS_PINS[] = {14,27,26,25};

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
        /*DIGITAL BUTTONS*/
        for(uint8_t i = 0; i < 9; i++)
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

        // inverted check for joystick button
        digitalRead(BUTTONS_PINS[10]) ? controller.release(BUTTONS[10]) : controller.press(BUTTONS[10]);
        digitalRead(BUTTONS_PINS[11]) ? controller.release(BUTTONS[11]) : controller.press(BUTTONS[11]);

        /*HATS BUTTONS*/
        uint8_t total = 0;

        for(uint8_t i = 0; i < 4; i++)
        {
            if(digitalRead(HATS_PINS[i]))
                total += HATS[i];
        }

        switch (total)
        {
        case UP:
        case LEFT + UP + RIGHT:
            controller.setHat4(1);
            break;
        case UP + RIGHT:
            controller.setHat4(2);
            break;
        case RIGHT:
        case UP + RIGHT + DOWN:
            controller.setHat4(3);
            break;
        case RIGHT + DOWN:
            controller.setHat4(4);
            break;
        case DOWN:
        case LEFT + DOWN + RIGHT:
            controller.setHat4(5);
            break;
        case DOWN + LEFT:
            controller.setHat4(6);
            break;
        case LEFT:
        case UP + LEFT + DOWN:
            controller.setHat4(7);
            break;
        case LEFT + UP:
            controller.setHat4(8);
            break;
        default:
            controller.setHat4(0);
            break;
        }

        controller.sendReport();
    }
}