//For Distance Sensors
#define DS A0

//For Line Sensors
#define LS1 A1
#define LS2 A2
#define LS3 A3
#define LS4 A4
#define LS5 A5

// Motor Driver Pins

// Motor Right
#define RIN1 8
#define RIN2 9

// Motor Left
#define LIN1 6
#define LIN2 7

//Other Pins
#define LEDPIN 8

//Variables
int ls1, ls2, ls3, ls4, ls5;
int ds;
String bin;
int x = 0;
String path = "";
int run_ID;

void setup()
{

  // Ir Array Pin modes
  pinMode(LS1, INPUT);
  pinMode(LS2, INPUT);
  pinMode(LS3, INPUT);
  pinMode(LS4, INPUT);
  pinMode(LS5, INPUT);

  //Motor Driver Pin modes
  pinMode(RIN1, OUTPUT);
  pinMode(RIN2, OUTPUT);
  pinMode(LIN1, OUTPUT);
  pinMode(LIN2, OUTPUT);

  pinMode(LEDPIN, OUTPUT);

  Serial.begin(9600);

  delay(3000);

  Serial.println("Begin");
}

void loop()
{

  if (run_ID == 0)
  {
    //Dry run
    dryRun();
  }
}

void dryRun()
{
  bin = "";

  bin = readSensorsDR();

  //Serial.println(bin);

  checkForIntersectionDR(bin);

  //delay(500);
}

String readSensorsDR()
{
  String ret = "";
  ls1 = analogRead(LS1);
  ls2 = analogRead(LS2);
  ls3 = analogRead(LS3);
  ls4 = analogRead(LS4);
  ls5 = analogRead(LS5);

  if (ls1 > 500)
    ret += "1";
  else
    ret += "0";

  if (ls2 > 500)
    ret += "1";
  else
    ret += "0";

  if (ls3 > 500)
    ret += "1";
  else
    ret += "0";

  if (ls4 > 500)
    ret += "1";
  else
    ret += "0";

  if (ls5 > 500)
    ret += "1";
  else
    ret += "0";

  return ret;
}

void checkForIntersectionDR(String s)
{

  // Check for Left
  if (s.equals("11100") || s.equals("11110") || s.equals("11111"))
  {
    path += 'L';
    turn('L');
  }

  //Check for Right
  else if (s.equals("01111") || s.equals("00111"))
  {
    String result = moveInch('R');
    //Check for Only Right
    if (result.equals("R"))
    {
      path += 'R';
      turn('R');
    }
    //Check for Stright and Right
    else if (result.equals("SR"))
    {
      path += 'S';
      turn('S');
    }
  }

  //Keep Going Staright
  else if (s.equals("01110") || s.equals("00110") || s.equals("01100"))
    turn('S');
}

void turn(char dir)
{
  switch (dir)
  {
  case 'R':
    digitalWrite(RIN1, LOW);
    digitalWrite(RIN2, LOW);
    digitalWrite(LIN1, HIGH);
    digitalWrite(LIN2, LOW);
    break;
  case 'L':
    digitalWrite(RIN1, HIGH);
    digitalWrite(RIN2, LOW);
    digitalWrite(LIN1, LOW);
    digitalWrite(LIN2, LOW);
    break;
  case 'S':
    digitalWrite(RIN1, LOW);
    digitalWrite(RIN2, HIGH);
    digitalWrite(LIN1, LOW);
    digitalWrite(LIN2, HIGH);
    break;
  case 'U':
    digitalWrite(RIN1, HIGH);
    digitalWrite(RIN2, LOW);
    digitalWrite(LIN1, LOW);
    digitalWrite(LIN2, HIGH);
    break;
  default:
    Serial.println("Oh Boy!");
  }
}

String moveInch(char ch)
{
  turn('S');

  delay(500);

  //Stop the motors
  digitalWrite(RIN1, LOW);
  digitalWrite(RIN2, LOW);
  digitalWrite(LIN1, LOW);
  digitalWrite(LIN2, LOW);
  
  String read = readSensorsDR(), ret;


if(ch == 'R') {
  
  if (read.equals("01111") || read.equals("00111")) {
    ret = "SR";
  }  
}


if (ch == 'L') {
  
  if (read.equals("11110") || read.equals("11100")) {
    ret = "L";
  }
  
}
return ret;
}