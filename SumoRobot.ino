/* 
0  Author   : Nourhan Mansour
  Date     : 11/3/2018
  Function : Arduino C code implemened to make a robot with 2 ultasonic & 6 IRs 
             Kicks other robots out of the ring.
             The algorith used is divided into simple steps
                1- scan the area for other opponents 
                  • if found any --> attack with avg speed
                  • otherwise move forward and scan again
                2- If you get anywhere near the edges of the ring --> turn with a small angle 
*/

#include <Ultrasonic.h>
Ultrasonic FrontUltrasonic(A0, A1);    // (Trigger , Echo)
Ultrasonic BackUltrasonic(A2, A3);    // (Trigger , Echo)
int distance;
int fspeed = 120;
int curveSpeed = 90;

const byte rIN1 = 13 , rIN2 = 12 , rEN = 11;   // Right Motor
const byte lIN1 = 9 , lIN2 = 8 , lEN = 10 ;    // Left Motor
const byte frontRightIR = 2 , FrontLeftIR = 4 , FrontMiddleIR = 3 ;     // Front IR sensors 
const byte backLeftIR = 5 , backRightIR = 7 , backMiddleIR = 6;         // Back IR sensors


void setup() {
  
  pinMode(rIN1 , OUTPUT);
  pinMode(rIN2 , OUTPUT);
  pinMode(rEN , OUTPUT);
  pinMode(lIN1 , OUTPUT);
  pinMode(lIN2 , OUTPUT);
  pinMode(lEN , OUTPUT);
  pinMode(frontRightIR  , INPUT);
  pinMode(FrontLeftIR   , INPUT);
  pinMode(FrontMiddleIR , INPUT);
  pinMode(backLeftIR    , INPUT);
  pinMode(backRightIR   , INPUT);
  pinMode(backMiddleIR  , INPUT);

}

void ScanArea();
void AttackTarget();
void Avoid();
void MoveForward();
void TurnRight();
void TurnLeft();
void MoveBackward();
void Stop();
void makeAngle();

void loop() {



}

void MoveForward (){
  
  digitalWrite(rIN1  , 0);
  digitalWrite(rIN2  , HIGH);
  analogWrite (rEN , fspeed);
  digitalWrite(lIN1  , 0);
  digitalWrite(lIN2  , HIGH);
  analogWrite(lEN , fspeed);
  
  }

  void MoveBackward(){
    
  digitalWrite(rIN1  , HIGH);
  digitalWrite(rIN2  , 0);
  analogWrite (rEN , fspeed);
  digitalWrite(lIN1  , HIGH);
  digitalWrite(lIN2  , 0);
  analogWrite(lEN , fspeed);
  
  }
 void Stop () {
  digitalWrite(rIN1  , 0);
  digitalWrite(rIN2  , 0);
  analogWrite (rEN , 0);
  digitalWrite(lIN1  , 0);
  digitalWrite(lIN2  , 0);
  analogWrite(lEN , 0);
  }
void  TurnRight () {
  digitalWrite(rIN1  , 0);
  digitalWrite(rIN2  , 0);
  analogWrite (rEN , 0);
  digitalWrite(lIN1  , 0);
  digitalWrite(lIN2  , HIGH);
  analogWrite(lEN , curveSpeed);
    }
void  TurnLeft () {
  digitalWrite(rIN1  , 0);
  digitalWrite(rIN2  , HIGH);
  analogWrite (rEN , curveSpeed);
  digitalWrite(lIN1  , 0);
  digitalWrite(lIN2  , 0);
  analogWrite(lEN , 0);
  }
  

