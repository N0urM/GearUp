/* 
  Author       : Nourhan Mansour
  contributors : Sherif El-Deeb & ElSaeed Ahmed
  Date     : 11/3/2018
  Function : Arduino C code implemened to make a robot with 2 ultasonic & 6 IRs 
             Kicks other robots out of the ring.
             The algorith used is divided into simple steps
                1- scan the area for other opponents 
                  • if found any --> attack with avg speed if far away & with Max speed if close enough
                  • otherwise move forward and scan again
                2- If you get anywhere near the edges of the ring --> turn with a small angle to avoid falling 
*/

#include <Ultrasonic.h>
Ultrasonic FrontUltrasonic(10, 11);    // (Trigger , Echo)
Ultrasonic BackUltrasonic(A2, A3);    // (Trigger , Echo)

int distance=100, backDis, frontDis;
int fspeed = 120 , mxspeed = 200 ,curveSpeed = 90;

const byte rIN1 = 13 , rIN2 = 12 , rEN = A1;   // Right Motor
const byte lIN1 = 9 , lIN2 = 8 , lEN = A0 ;    // Left Motor
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

// system starts here
void loop() {

  // each loop gets new IR readings to ensure avoiding obstacles 
    Frontright=digitalRead(FrontRightIR);
    Frontleft=digitalRead(FrontLeftIR);
    Frontmiddle=digitalRead(FrontMiddleIR);
  
    Backleft=digitalRead(BackLeftIR);
    Backright=digitalRead(BackRightIR);
    Backmiddle=digitalRead(BackMiddleIR);

    // no IR is active & we're away from the borders
    if (Frontright==LOW &&Frontleft==LOW && Frontmiddle==LOW &&Backleft==LOW && Backright == LOW && Backmiddle == LOW ){
      ScanArea();
    }
    else { // maybe close to an edge.
      Avoid();
    }


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
  
  int ttl = 1; // take values from 1 - 18 in order to make 180 degres and avoid moving in circles 
  
  frontDis = FrontUltrasonic.read();
  backDis = BackUltrasonic.read();
 
 
 if (backDis >= distance && frontDis < distance) // Enemy ahead
  {
   
    AttackTarget('f');
  }
  else if ( backDis < distance && frontDis >= distance ){  //Enemy is behind.
    AttackTarget('b');
  }
  else  // Nothing found so keep searching
  {
    MoveForward(fspeed); // moveForward to avoid remaining in the same place.
    delay (500); // keep moving for 0.5 s
    while (ttl < 18 && FrontUltrasonic.read() > distance && BackUltrasonic.read() > distance ) { // make turns while searching
      ttl ++;
      makeAngle ();
     
    }
    
  }
}
void makeAngle () {
      
 TurnRight (); // right or left is the same
 delay (50); // to be changed by try and error to make a small angle approx = 10 degrees
 Stop(); // to give the sonics some space to capture readings.
 
  }

void AttackTarget(char c) {
  if (c=='f'){ // argument passed by the search function
    if (FrontUltrasonic.read() <= distance / 2){ // IF the enemy is close we need to move fast to gain some tork.
        MoveForward (mxspeed);
      }
      else { // other wise we need to move on avg speed to make sure we don't run out of the borders.
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
  // Get reading from all IRS.
    Frontright=digitalRead(FrontRightIR);
    Frontleft=digitalRead(FrontLeftIR);
    Frontmiddle=digitalRead(FrontMiddleIR);
  
    Backleft=digitalRead(BackLeftIR);
    Backright=digitalRead(BackRightIR);
    Backmiddle=digitalRead(BackMiddleIR);

    if ((Frontright==HIGH && Frontleft==LOW && Frontmiddle==LOW) || (Backleft==LOW && Backright==HIGH && Backmiddle==LOW) ){
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


