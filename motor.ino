//BTS7960 motor driver sketch 

int R_IS = 6;
int R_EN = 2;
int R_PWM = 12;
int L_IS = 7;
int L_EN = 4;
int L_PWM = 13;

void setup() {
  // put your setup code here, to run once:
 pinMode(R_IS, INPUT);
 pinMode(R_EN, OUTPUT);
 pinMode(R_PWM, OUTPUT);
 pinMode(L_IS, INPUT);
 pinMode(L_EN, OUTPUT);
 pinMode(L_PWM, OUTPUT);
 digitalWrite(R_IS, LOW);
 digitalWrite(L_IS, LOW);
 digitalWrite(R_EN, HIGH);
 digitalWrite(L_EN, HIGH);
}

void loop(){
    int x;
    while(true){
        if(x < 255){
            // saat yönünde
            clkAnalogWrite(x);
            x += 5;
            delay(250);
        }else{
            x -= 5;
            clkAnalogWrite(x);
            delay(250);
        }
    }
}

void clkAnalogWrite(int _x){
    analogWrite(R_PWM, _x);
    analogWrite(L_PWM, 0);
}