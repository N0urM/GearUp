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
Ultrasonic FrontUltrasonic(9, 8);    // (Trigger , Echo)
Ultrasonic BackUltrasonic(A2, A1);    // (Trigger , Echo)

int distance=30, backDis, frontDis;
int rightspeed = 240 ,leftspeed=210 , mxspeed = 255 ,curveRightSpeed = 130 , curveLeftSpeed = 190;

const byte rIN1 = 13 , rIN2 = 12 , rEN = 5;   // Right Motor
const byte lIN1 = 10, lIN2 = 11 , lEN = 6 ;    // Left Motor
const byte FrontRightIR = A3 , FrontLeftIR = A5  ;     // Front IR sensors 
const byte BackLeftIR = 2 , BackRightIR = 4 ;         // Back IR sensors
byte Frontright, Frontleft, Backleft, Backright;

void setup() {
  
  pinMode(rIN1 , OUTPUT);
  pinMode(rIN2 , OUTPUT);
  pinMode(rEN , OUTPUT);
  pinMode(lIN1 , OUTPUT);
  pinMode(lIN2 , OUTPUT);
  pinMode(lEN , OUTPUT);
  pinMode(FrontRightIR  , INPUT);
  pinMode(FrontLeftIR   , INPUT);
  pinMode(BackLeftIR    , INPUT);
  pinMode(BackRightIR   , INPUT);
  Serial.begin(9600);
}

void ScanArea();
void AttackTarget(char c);
void Avoid();
void MoveForward(int sr , int sl);
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
  
    Backleft=digitalRead(BackLeftIR);
    Backright=digitalRead(BackRightIR);

    // no IR is active & we're away from the borders
    if (Frontright==LOW && Frontleft==LOW  && Backleft==LOW && Backright == LOW ){
      Serial.println("Scanning");
      ScanArea();
    }
    else { // maybe close to an edge.
      Serial.println("Avoiding");
      Avoid();
   
    }
/*
MoveForward(255 , 255);
delay(2000);
MoveBackward(255 , 255);
delay(2000);
TurnRight();
delay(1000);
TurnLeft();
delay(1000);

*/
}

void MoveForward (int rs , int ls){
  
  digitalWrite(rIN1  , 0);
  digitalWrite(rIN2  , HIGH);
  analogWrite (rEN , rs);
  digitalWrite(lIN1  , 0);
  digitalWrite(lIN2  , HIGH);
  analogWrite(lEN , ls);
  Serial.println("Moving Forward");
  
  }

  void MoveBackward(int rs , int ls){
    
  digitalWrite(rIN1  , HIGH);
  digitalWrite(rIN2  , 0);
  analogWrite (rEN , rs);
  digitalWrite(lIN1  , HIGH);
  digitalWrite(lIN2  , 0);
  analogWrite(lEN , ls);
  Serial.println("Moving backward");
  
  }
 void Stop () {
  digitalWrite(rIN1  , 0);
  digitalWrite(rIN2  , 0);
  analogWrite (rEN , 0);
  digitalWrite(lIN1  , 0);
  digitalWrite(lIN2  , 0);
  analogWrite(lEN , 0);
  Serial.println("Stopped");
  }
void  TurnRight () {
  digitalWrite(rIN1  , HIGH);
  digitalWrite(rIN2  , 0);
  analogWrite (rEN , curveLeftSpeed);
  digitalWrite(lIN1  , 0);
  digitalWrite(lIN2  , HIGH);
  analogWrite(lEN , curveRightSpeed);
  Serial.println("Right Turn");
    }
void  TurnLeft () {
  digitalWrite(rIN1  , 0);
  digitalWrite(rIN2  , HIGH);
  analogWrite (rEN , curveLeftSpeed);
  digitalWrite(lIN1  , HIGH);
  digitalWrite(lIN2  , 0);
  analogWrite(lEN , curveRightSpeed);
  Serial.println("Left Turn");
  }
  
void ScanArea() {
  
  int ttl = 1; // take values from 1 - 18 in order to make 180 degres and avoid moving in circles 
  
  frontDis = FrontUltrasonic.read();
  backDis = BackUltrasonic.read();
 
 
 if ( frontDis <= distance) // Enemy ahead
  {
   
    AttackTarget('f');
      Serial.println("Attacking Enemy in front");
  }
  else if ( backDis <= distance  ){  //Enemy is behind.
    AttackTarget('b');
      Serial.println("Attacking enemy behind");
  }
  else  // Nothing found so keep searching
  {
      Serial.println("Nothing found will keep searching");
    MoveForward(rightspeed , leftspeed); // moveForward to avoid remaining in the same place.
    delay (1000); // keep moving for 1 s
    while (ttl <= 20 && FrontUltrasonic.read() > distance && BackUltrasonic.read() > distance ) { // make turns while searching
      ttl ++;
      makeAngle ();
     
    }
    
  }
}
void makeAngle () {
 Serial.println("Making angle NOW");
 TurnRight (); // right or left is the same
 delay (500); // to be changed by try and error to make an angle approx = 30 degrees
 Stop(); // to give the sonics some space to capture readings.
 
  }

void AttackTarget(char c) {
  Serial.println("Attacking");
  if (c=='f'){ // argument passed by the search function
    if (FrontUltrasonic.read() <= distance / 2){ // IF the enemy is close we need to move fast to gain some tork.
        MoveForward (rightspeed+30 , leftspeed+30);
      }
      else { // other wise we need to move on avg speed to make sure we don't run out of the borders.
        MoveForward(rightspeed , leftspeed);
      }
  }else {
    if (BackUltrasonic.read() <= distance / 2){
      MoveBackward (rightspeed+30 , leftspeed+30);
    }
    else 
    {
      MoveBackward(rightspeed , leftspeed);
    }
  }
  
} 
void Avoid(){
  // Get reading from all IRS.
    Frontright=digitalRead(FrontRightIR);
    Frontleft=digitalRead(FrontLeftIR);
  
    Backleft=digitalRead(BackLeftIR);
    Backright=digitalRead(BackRightIR);

    if ((Frontright==HIGH && Frontleft==LOW ) || (Backleft==LOW && Backright==HIGH ) ){
        TurnLeft();
    }
    else if( (Frontright==LOW && Frontleft==HIGH ) || (Backleft==HIGH && Backright==LOW ) ){
        TurnRight();
    }  
    else if(Frontright==HIGH && Frontleft==HIGH ){
        MoveBackward(rightspeed , leftspeed);
    }
    else if(Backleft==HIGH && Backright==HIGH ) {
        MoveForward(rightspeed , leftspeed);
    }
   
} 
