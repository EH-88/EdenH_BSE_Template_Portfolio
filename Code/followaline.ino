/*This code allows the robot car to follow a line (it takes the output of the IR sensors, and uses the
information to either turn the robot car left, turn it right, or continue moving it straight) with
a button to turn the robot car on or off.*/

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

int buttonPin = 12;
int buttonRead;

bool button_hasBeenPressed = false;

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
}

void loop() {
  buttonRead = digitalRead(buttonPin);
  if(buttonRead == 1){
    button_hasBeenPressed = !button_hasBeenPressed;
    delay(100);
  }
  
  /*digitalWrite(input1M1, HIGH);
  digitalWrite(input2M1, LOW);
  digitalWrite(input1M2, HIGH);
  digitalWrite(input2M2, LOW);*/
  
  if(button_hasBeenPressed == false){
    analogWrite(enableM1, 0);
    analogWrite(enableM2, 0);
  } else if(button_hasBeenPressed == true){
    if(digitalRead(rightSensor) == white && digitalRead(leftSensor) == black){
      /*Turn Right (set motor 1 to high speed and motor 2 to low speed).*/
      digitalWrite(input1M2, LOW);
      digitalWrite(input2M2, HIGH);
      analogWrite(enableM1, 255);
      analogWrite(enableM2, 255);
    } else if(digitalRead(leftSensor) == white && digitalRead(rightSensor) == black){
      /*Turn Left (set motor 2 to high speed and motor 2 to low speed).*/
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
  }
  delay(150);
}
