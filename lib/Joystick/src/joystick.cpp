#include <Arduino.h>
#include "joystick.h"

Joystick::Joystick(byte joy_x, byte joy_y){
    JOY_X = joy_x;
    JOY_Y = joy_y;
}
Joystick::Joystick(byte joy_x, byte joy_y, byte joy_but){
    JOY_X = joy_x;
    JOY_Y = joy_y;
    JOY_BUT = joy_but; 
}
Joystick::~Joystick(){}

void Joystick::joystickRead(int _delay){
    inputVectors[0] = analogRead(JOY_X);
    inputVectors[1] = analogRead(JOY_Y);
    delay(_delay);
}

bool Joystick::joystickButtonRead(){
    buttonState = digitalRead(JOY_BUT);
    return buttonState;
}