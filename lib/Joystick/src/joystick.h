#ifndef JOYSTICK_H
#define JOYSTICK_H
#include <Arduino.h>

class Joystick{
private:
    byte JOY_X;
    byte JOY_Y;
    byte JOY_BUT;
    int inputVectors[2];
    bool buttonState;
public:
    Joystick(byte joy_x, byte joy_y);
    Joystick(byte joy_x, byte joy_y, byte joy_but);
    ~Joystick();

    void joystickRead(int _delay){}
    bool joystickButtonRead(){}
};


#endif