/* 
  Author   : Nourhan Mansour
  Date     : 11/3/2018
  Function : Arduino C code implemened to make a robot with 2 ultasonic & 6 IRs 
             Kicks other robots out of the ring.
             The algorith used is divided into simple steps
                1- scan the area for other opponents 
                  • if found any --> attack with avg speed
                  • otherwise move forward and scan again
                2- If you get anywhere near the edges of the ring --> reverse direction
*/

#include <Ultrasonic.h>
Ultrasonic ultrasonic(12, 13);    // (Trigger , Echo)
int distance;

const byte rIN1 = 13 , rIN2 = 12 , rEN = 11;   // Right Motor
const byte lIN1 = 9 , lIN2 = 8 , lEN = 10 ;    // Left Motor
const byte frontRightIR = A0 , FrontLeftIR = A2 , FrontMiddleIR = A1 ;     // Front IR sensors 
const byte backLeftIR = A3 , backRightIR = A5 , backMiddleIR = A4;         // Back IR sensors


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

bool ScanArea();
void AttackTarget();
void ReverseDirection();
void MoveForward();
void Stop();

void loop() {



}
