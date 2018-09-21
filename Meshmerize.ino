
//Array Pins
#define IR1 A1
#define IR2 A2
#define IR3 A3
#define IR4 A4
// #define IR5 A5

//Motor A (Left)
const int motorPin1  = 10;    // Pin 15 of L293D IC
const int motorPin2  = 11;    // Pin 10 of L293D IC

//Motor B (Right)
const int motorPin3  = 9;   // Pin  7 of L293D IC
const int motorPin4  = 8;   // Pin  2 of L293D IC

//Led Pin
const int led_Pin = 12;

//Variables
int ir1, ir2, ir3, ir4, ir5;
int ar[4];
String sensors = "";

void setup() {

  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(IR4, INPUT);
  // pinMode(IR5, INPUT);

  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);  

  pinMode(led_Pin, OUTPUT);


  Serial.begin(9600);
}

void loop() {

//  Serial.print("Test");

  ir1 = analogRead(A1);
  ir2 = analogRead(A2);
  ir3 = analogRead(A3);
  ir4 = analogRead(A4);

  // ir5 = analogRead(A5);
  
  arrayFill();

  Serial.print("\n");

  delay(1000);
}

void arrayFill(){

  Serial.println("arrayFill");

  if(ir1 > 500) {
    // Serial.print("1");
    ar[0] = 1;
    sensors += "1";
  }
  else {
    ar[0] = 0;
    sensors += "0";
    // Serial.print("0");
  }
  
  if(ir2 > 500) {
    ar[1] = 1;
    sensors += "1";
    // Serial.print("1");
  }
  else {
    ar[1] = 0;
    sensors += "0";
    // Serial.print("0");
  }

  if(ir3 > 500) {
    ar[2] = 1;
    sensors += "1";
    // Serial.print("1");
  }
  else {
    ar[2] = 0;
    sensors += "0";
    // Serial.print("0");
  }

  if(ir4 > 500) {
    ar[3] = 1;
    sensors += "1";
    // Serial.print("1");
  }
  else {
    ar[3] = 0;
    sensors += "0";
    // Serial.print("0");
  }

  
  for(int i = 0; i < 4; i++)
  {
    Serial.print(ar[i]);
  }

  Serial.println(sensors);
  

  // if(e > 500) {
  //   Serial.print("1");
  // }
  // else {
  //   Serial.print("0");
  // }
  
}

//  function to move the bot around
void move_bot(char m){
    switch(m)
    {
    case 'R':
      digitalWrite(motorPin1, LOW);
      digitalWrite(motorPin2, LOW);
      digitalWrite(motorPin3, HIGH);
      digitalWrite(motorPin4, LOW);
      break;

    case 'L':
      digitalWrite(motorPin1, HIGH);
      digitalWrite(motorPin2, LOW);
      digitalWrite(motorPin3, LOW);
      digitalWrite(motorPin4, LOW);
      break;

    case 'S':
      digitalWrite(motorPin1, HIGH);
      digitalWrite(motorPin2, LOW);
      digitalWrite(motorPin3, HIGH);
      digitalWrite(motorPin4, LOW);
      break;

    case 'U':
        //  motor driver: turn 180 degrees
        break;

    case 'E':
        //  motor driver: move a bit forward in the same direction
        break;

    case 'H':
      //  motor driver: halt
      digitalWrite(motorPin1, LOW);
      digitalWrite(motorPin2, LOW);
      digitalWrite(motorPin3, LOW);
      digitalWrite(motorPin4, LOW);

      // blink led_Pin

      ledBlink();

      break;
    }
}

char det_Turn(){

  char ret = '\0'; 

  //Check for left
  if( sensors == "1110" || sensors == "1100" ){
    //  Left turn
    ret = 'L';
  }

  //Check for right
  else if( sensors == "0011" || sensors == "0111" ){
    //  Right turn.
    // Move Inch Implementation.
    String s = moveInch('R');
    if(s.equals("SR")){
      ret = 'S';
    }
    else if(s.equals("R")){
      ret  = 'R';
    }
  }

  //Check for straight
  if( sensors == "0110" )
    ret = 'S';

  return ret;

}

String moveInch(char ch){

  //Motor Driver moves the bot move forward.
  
  String ret = "";

  arrayFill();

  if(ch == 'L'){

    if(sensors.equals("0110")){
      ret = "SL";
    }
    else if(sensors.equals("0000")){
      ret = "L";
    }

  }

  else if(ch == 'R'){

    if(sensors.equals("0110")){
      ret = "SR";
    }
    else if(sensors.equals("0000")){
      ret = "R";
    }

  }

  else if(ch == 'D') {
    ret = "DE";
  }

  else if (ch == 'T') {
    
    if (sensors.equals("0000")) {
      ret = "T";
    }
    else if (sensors.equals("1111")) {
      ret = "EOM";
    }
      
    else if (sensors.equals("0110")) {
      ret = "CR";
    }
  }

  return ret;

}

void ledBlink() {

  digitalWrite(led_Pin, HIGH);
  delay(1000);
  digitalWrite(led_Pin, LOW);
  delay(1000);
}
