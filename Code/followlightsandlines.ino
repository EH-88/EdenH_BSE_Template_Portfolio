/*This code allows the robot car to either follow a line (by taking the output of the IR sensors, and
using the information to either turn the robot car left, turn it right, or continue moving it straight)
or track the sun/light (by using four photoresistors) with a button to turn the robot car on or off and
switch between modes/functions.*/

int black = 1;
int white = 0;

int rightSensor = 8;
int leftSensor = 7;

int enableM1 = 11;
int input1M1 = 6;
int input2M1 = 5;
int input1M2 = 4;
int input2M2 = 3;
int enableM2 = 10;

int pR1 = A0;
int pR2 = A1;
int pR3 = A2;
int pR4 = A3;

int pR1Value;
int pR2Value;
int pR3Value;
int pR4Value;

float pR1Cal;
float pR2Cal;
float pR3Cal;
float pR4Cal;

float minCal;

int buffer = 5;

int highLimit = 100;

int buttonPin = 12;
int buttonRead;

int buttonMode = 0;

void setup() {
  Serial.begin(9600);
  
  pinMode(rightSensor, INPUT);
  pinMode(leftSensor, INPUT);
  
  pinMode(enableM1, OUTPUT);
  pinMode(input1M1, OUTPUT);
  pinMode(input2M1, OUTPUT);
  pinMode(input1M2, OUTPUT);
  pinMode(input2M2, OUTPUT);
  pinMode(enableM2, OUTPUT);
  
  pinMode(buttonPin, INPUT);
  
  pinMode(pR1, INPUT);
  pinMode(pR2, INPUT);
  pinMode(pR3, INPUT);
  pinMode(pR4, INPUT);
  
  /*pR1Cal = calibratePR(pR1, pR1Cal);
  pR2Cal = calibratePR(pR2, pR2Cal);
  pR3Cal = calibratePR(pR3, pR3Cal);
  pR4Cal = calibratePR(pR4, pR4Cal);*/
}

void loop() {
  buttonRead = digitalRead(buttonPin);
  if(buttonRead == 1){
    if(buttonMode == 2){
      buttonMode = 0;
    } else if(buttonMode == 1){
      pR1Cal = calibratePR(pR1, pR1Cal);
      pR2Cal = calibratePR(pR2, pR2Cal);
      pR3Cal = calibratePR(pR3, pR3Cal);
      pR4Cal = calibratePR(pR4, pR4Cal);
      buttonMode++;
    } else{
      buttonMode++;
    }
    delay(100);
  }
  
  if(buttonMode == 0){
    analogWrite(enableM1, 0);
    analogWrite(enableM2, 0);
  } else if(buttonMode == 1){
    if(digitalRead(rightSensor) == white && digitalRead(leftSensor) == black){
      /*Turn Right (set motor 1 to high speed and motor 2 to low speed).*/
      digitalWrite(input1M2, LOW);
      digitalWrite(input2M2, HIGH);
      analogWrite(enableM1, 255);
      analogWrite(enableM2, 255);
    } else if(digitalRead(leftSensor) == white && digitalRead(rightSensor) == black){
      /*Turn Left (set motor 2 to high speed and motor 1 to low speed).*/
      digitalWrite(input1M1, LOW);
      digitalWrite(input2M1, HIGH);
      analogWrite(enableM1, 255);
      analogWrite(enableM2, 255);
    } else if (digitalRead(leftSensor) == white && digitalRead(rightSensor) == white){
      /*Go Straight (set both motors to high speed).*/
      digitalWrite(input1M1, LOW);
      digitalWrite(input2M1, HIGH);
      digitalWrite(input1M2, LOW);
      digitalWrite(input2M2, HIGH);
      analogWrite(enableM1, 255);
      analogWrite(enableM2, 255);
    } else if(digitalRead(leftSensor) == black && digitalRead(rightSensor) == black){
      digitalWrite(input1M1, HIGH);
      digitalWrite(input2M1, LOW);
      digitalWrite(input1M2, HIGH);
      digitalWrite(input2M2, LOW);
      analogWrite(enableM1, 255);
      analogWrite(enableM2, 255);
    }
  } else if(buttonMode == 2){
    pR1Value = analogRead(pR1);
    pR2Value = analogRead(pR2);
    pR3Value = analogRead(pR3);
    pR4Value = analogRead(pR4);
    
    pR1Value = map(analogRead(pR1), pR1Cal, 1023, 0, highLimit);
    pR2Value = map(analogRead(pR2), pR2Cal, 1023, 0, highLimit);
    pR3Value = map(analogRead(pR3), pR3Cal, 1023, 0, highLimit);
    pR4Value = map(analogRead(pR4), pR4Cal, 1023, 0, highLimit);
    
    if(pR1Value <= 0){
      pR1Value = 0;
    } else if(pR2Value <= 0){
      pR2Value = 0;
    } else if(pR3Value <= 0){
      pR3Value = 0;
    } else if(pR4Value <= 0){
      pR4Value = 0;
    }
    
    if((pR1Value-buffer) > pR2Value && (pR1Value-buffer) > pR3Value && (pR1Value-buffer) > pR4Value){
      Serial.println("Right and Forwards");
      digitalWrite(input1M1, HIGH);
      digitalWrite(input2M1, LOW);
      digitalWrite(input1M2, HIGH);
      digitalWrite(input2M2, LOW);
      analogWrite(enableM1, 0);
      analogWrite(enableM2, 255);
    } else if((pR2Value-buffer) > pR1Value && (pR2Value-buffer) > pR3Value && (pR2Value-buffer) > pR4Value){
      Serial.println("Left and Forwards");
      digitalWrite(input1M1, HIGH);
      digitalWrite(input2M1, LOW);
      digitalWrite(input1M2, HIGH);
      digitalWrite(input2M2, LOW);
      analogWrite(enableM1, 255);
      analogWrite(enableM2, 0);
    } else if((pR3Value-buffer) > pR1Value && (pR3Value-buffer) > pR2Value && (pR3Value-buffer) > pR4Value){
      Serial.println("Left and Backwards");
      digitalWrite(input1M1, LOW);
      digitalWrite(input2M1, HIGH);
      digitalWrite(input1M2, LOW);
      digitalWrite(input2M2, HIGH);
      analogWrite(enableM1, 255);
      analogWrite(enableM2, 0);
    } else if((pR4Value-buffer) > pR1Value && (pR4Value-buffer) > pR2Value && (pR4Value-buffer) > pR3Value){
      Serial.println("Right and Backwards");
      digitalWrite(input1M1, LOW);
      digitalWrite(input2M1, HIGH);
      digitalWrite(input1M2, LOW);
      digitalWrite(input2M2, HIGH);
      analogWrite(enableM1, 0);
      analogWrite(enableM2, 255);
    } else if((pR1Value-buffer) > pR4Value && (pR2Value-buffer) > pR3Value && abs(pR1Value-pR2Value) <= buffer){
      Serial.println("Straight Forwards");
      digitalWrite(input1M1, HIGH);
      digitalWrite(input2M1, LOW);
      digitalWrite(input1M2, HIGH);
      digitalWrite(input2M2, LOW);
      analogWrite(enableM1, 255);
      analogWrite(enableM2, 255);
    } else if((pR4Value-buffer) > pR1Value && (pR3Value-buffer) > pR2Value && abs(pR4Value-pR3Value) <= buffer){
      Serial.println("Straight Backwards");
      digitalWrite(input1M1, LOW);
      digitalWrite(input2M1, HIGH);
      digitalWrite(input1M2, LOW);
      digitalWrite(input2M2, HIGH);
      analogWrite(enableM1, 255);
      analogWrite(enableM2, 255);
    } else{
      Serial.println("Stay");
      analogWrite(enableM1, 0);
      analogWrite(enableM2, 0);
    }
  }
  delay(150);
}

int calibratePR(int pin, int calValue){
  minCal = 0;
  calValue = 1023;
  for(int cal = 0; cal < 50; cal++){
    minCal = min(calValue, analogRead(pin));
    calValue = minCal;
    delay(2);
  }
  calValue = minCal;
  return calValue;
}
