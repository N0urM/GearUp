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
int distance=100, backDis, frontDis;
int fspeed = 120 , mxspeed = 200 ,curveSpeed = 90;

const byte rIN1 = 13 , rIN2 = 12 , rEN = 11;   // Right Motor
const byte lIN1 = 9 , lIN2 = 8 , lEN = 10 ;    // Left Motor
const byte FrontRightIR = 2 , FrontLeftIR = 4 , FrontMiddleIR = 3 ;     // Front IR sensors 
const byte BackLeftIR = 5 , BackRightIR = 7 , BackMiddleIR = 6;         // Back IR sensors
byte Frontright, Frontleft, Frontmiddle, Backleft, Backright, Backmiddle;

void setup() {
  
  pinMode(rIN1 , OUTPUT);
  pinMode(rIN2 , OUTPUT);
  pinMode(rEN , OUTPUT);
  pinMode(lIN1 , OUTPUT);
  pinMode(lIN2 , OUTPUT);
  pinMode(lEN , OUTPUT);
  pinMode(FrontRightIR  , INPUT);
  pinMode(FrontLeftIR   , INPUT);
  pinMode(FrontMiddleIR , INPUT);
  pinMode(BackLeftIR    , INPUT);
  pinMode(BackRightIR   , INPUT);
  pinMode(BackMiddleIR  , INPUT);

}

void ScanArea();
void AttackTarget(char c);
void Avoid();
void MoveForward(int s);
void TurnRight();
void TurnLeft();
void MoveBackward(int s);
void Stop();
void makeAngle();
void loop() {



}

void MoveForward (int s){
  
  digitalWrite(rIN1  , 0);
  digitalWrite(rIN2  , HIGH);
  analogWrite (rEN , s);
  digitalWrite(lIN1  , 0);
  digitalWrite(lIN2  , HIGH);
  analogWrite(lEN , s);
  
  }

  void MoveBackward(int s){
    
  digitalWrite(rIN1  , HIGH);
  digitalWrite(rIN2  , 0);
  analogWrite (rEN , s);
  digitalWrite(lIN1  , HIGH);
  digitalWrite(lIN2  , 0);
  analogWrite(lEN , s);
  
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
  
void ScanArea() {
  
  int ttl = 1;
  frontDis = FrontUltrasonic.read();
  backDis = BackUltrasonic.read();
 
 
 if (backDis >= distance && frontDis < distance)
  {
    //Enemy is in front.
    AttackTarget('f');
  }
  else if ( backDis < distance && frontDis >= distance ){
    //Enemy is behind.
    AttackTarget('b');
  }
  else
  {
    MoveForward(fspeed);
    delay (500); 
    while (ttl < 18 && FrontUltrasonic.read() > distance && BackUltrasonic.read() > distance ) {
      ttl ++;
      makeAngle ();
     
    }
    
  }
}
void makeAngle () {
      
 TurnRight ();
 delay (50);
 Stop();
 
  }

void AttackTarget(char c) {
  if (c=='f'){
    if (FrontUltrasonic.read() <= distance / 2){
        MoveForward (mxspeed);
      }
      else {
        MoveForward(fspeed);
      }
  }else {
    if (BackUltrasonic.read() <= distance / 2){
      MoveBackward (mxspeed);
    }
    else 
    {
      MoveBackward(fspeed);
    }
  }
  

} 
void Avoid(){
    Frontright=digitalRead(FrontRightIR);
    Frontleft=digitalRead(FrontLeftIR);
    Frontmiddle=digitalRead(FrontMiddleIR);
  
    Backleft=digitalRead(BackLeftIR);
    Backright=digitalRead(BackRightIR);
    Backmiddle=digitalRead(BackMiddleIR);
  
    if ( (Frontright==LOW && Frontleft==LOW && Frontmiddle==LOW) || (Backleft==LOW && Backright==LOW && Backmiddle==LOW) ){
        TurnLeft(); // Or Right
    }
    else if ((Frontright==HIGH && Frontleft==LOW && Frontmiddle==LOW) || (Backleft==LOW && Backright==HIGH && Backmiddle==LOW) ){
        TurnLeft();
    }
    else if (( Frontright==LOW && Frontleft==LOW && Frontmiddle==HIGH) || (Backleft==LOW && Backright==LOW && Backmiddle==HIGH ) ){
        TurnRight(); // Or Left
    }
    else if( (Frontright==HIGH && Frontleft==LOW && Frontmiddle==HIGH) || (Backleft==LOW && Backright==HIGH && Backmiddle==HIGH) ){
        TurnLeft();
    }
    else if( (Frontright==LOW && Frontleft==HIGH && Frontmiddle==LOW) || (Backleft==HIGH && Backright==LOW && Backmiddle==LOW) ){
        TurnRight();
    }
    else if( (Frontright==LOW && Frontleft==HIGH && Frontmiddle==HIGH) || (Backleft==HIGH && Backright==LOW && Backmiddle==HIGH) ){
        TurnRight();
    }
    
    else if(Frontright==HIGH && Frontleft==HIGH && Frontmiddle==LOW){
        MoveBackward(fspeed);
    }
    else if(Backleft==HIGH && Backright==HIGH && Backmiddle==LOW) {
        MoveForward(fspeed);
    }
    else if(Frontright==HIGH && Frontleft==HIGH && Frontmiddle==HIGH){
        MoveBackward(fspeed);
    }
    else if(Backleft==HIGH && Backright==HIGH && Backmiddle==HIGH) {
        MoveForward(fspeed);
    }
} 


