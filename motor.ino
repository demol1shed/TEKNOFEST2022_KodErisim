//BTS7960 motor driver sketch 
#pragma region define bloğu
#define R_IS 6
#define R_EN 2
#define R_PWM 12
#define L_IS 7
#define L_EN 4
#define L_PWM 13
#pragma endregion

void setup() {
    #pragma region pin ayarlari
    pinMode(R_IS, INPUT);
    pinMode(L_IS, INPUT);
    pinMode(R_PWM, OUTPUT);
    pinMode(L_PWM, OUTPUT);
    pinMode(R_EN, OUTPUT);
    pinMode(L_EN, OUTPUT);
    #pragma endregion
    //~~~~~~~~~~~~~~~~~~~~~~~~
    #pragma region voltaj ayarlari
    digitalWrite(R_IS, LOW);
    digitalWrite(L_IS, LOW);
    digitalWrite(R_EN, HIGH);
    digitalWrite(L_EN, HIGH);
    #pragma endregion
}

void loop(){
    Main();
}

void ClkAnalogWrite(int _x){
    analogWrite(R_PWM, _x);
    analogWrite(L_PWM, 0);
}

void CtrClkAnalogWrite(int _x){
    analogWrite(R_PWM, 0);
    analogWrite(L_PWM, _x);
}

void Main(){
    int x;
    while(true){
        if(x < 255){
            // saat yönünde
            ClkAnalogWrite(x);
            x += 5;
            delay(250);
        }else{
            x -= 5;
            ClkAnalogWrite(x);
            delay(250);
        }
    }
}