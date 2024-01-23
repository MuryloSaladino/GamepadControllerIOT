#include <Arduino.h>
#include <BleGamepad.h>
#include <BleConnectionStatus.h>

#define numOfButtons 16
#define numOfHatSwitches 4

const uint8_t UP = 1;
const uint8_t RIGHT = 2;
const uint8_t DOWN = 4;
const uint8_t LEFT = 8;

// A,B,X,Y,LB,RB,BACK,START,LS_B,RS_B
const uint8_t BUTTONS[] = {1,2,4,5,7,8,11,12,14,15};
const uint8_t BUTTONS_PINS[] = {23,22,12,18,17,16,19,21,2,5};

const uint8_t TRIGGERS[] = {25,26};

const uint8_t HATS[] = {UP,RIGHT,DOWN,LEFT};
const uint8_t HATS_PINS[] = {36,39,32,33};

const uint8_t JOYSTICKS_PINS[] = {27,14,4,15};

BleGamepad controller;
BleGamepadConfiguration BleGamepadConfig;

float getValue(float x)
{
    float a = 0.05239583, b = -0.1474024;
    return x - (a * x + b);
}

void setup()
{
    Serial.begin(115200);
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
        pinMode(pin, INPUT_PULLDOWN);
    }
    for (auto pin : HATS_PINS)
    {
        pinMode(pin, INPUT_PULLDOWN);
    }
    for (auto pin : TRIGGERS)
    {
        pinMode(pin, INPUT_PULLDOWN);
    }
    for (auto pin : JOYSTICKS_PINS)
    {
        pinMode(pin, INPUT);
    }
}

void loop()
{
    if (controller.isConnected())
    {
        // /*DIGITAL BUTTONS*/
        for(uint8_t i = 0; i < 8; i++)
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
        if(digitalRead(BUTTONS_PINS[8]))
        {
            controller.release(BUTTONS[8]);
        }
        else
        {
            controller.press(BUTTONS[8]);
        }

        if(digitalRead(BUTTONS_PINS[9]))
        {
            controller.release(BUTTONS[9]);
        }
        else
        {
            controller.press(BUTTONS[9]);
        }

        /*HATS BUTTONS*/
        uint8_t total = 0;

        for(uint8_t i = 0; i < 4; i++)
        {
            if(digitalRead(HATS_PINS[i]))
            {
                total += HATS[i];
            }
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


        /*JOYSTICKS AXIS*/
        int rx = 32767.0 * getValue(analogRead(JOYSTICKS_PINS[0])*3.3/4095.0) / 3.3;
        int ry = 32767.0 * getValue(analogRead(JOYSTICKS_PINS[1])*3.3/4095.0) / 3.3;
        int lx = 32767.0 * getValue(analogRead(JOYSTICKS_PINS[2])*3.3/4095.0) / 3.3;
        int ly = 32767.0 * getValue(analogRead(JOYSTICKS_PINS[3])*3.3/4095.0) / 3.3;
        int rt = digitalRead(TRIGGERS[0]) ? 32767 : 0;
        int lt = digitalRead(TRIGGERS[1]) ? 32767 : 0;

        rx = rx > 15300 && rx < 17300 ? 16383 : rx;
        ry = ry > 15300 && ry < 17300 ? 16383 : ry;
        lx = lx > 15300 && lx < 17300 ? 16383 : lx;
        ly = ly > 15300 && ly < 17300 ? 16383 : ly;

        controller.setAxes(lx, ly, rt, lt, rx, ry);


        /*UPDATE CONTROLLER*/
        controller.sendReport();
    }
}