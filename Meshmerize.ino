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
#define LEDPIN 10

//Variables
int ls1, ls2, ls3, ls4, ls5;
int ds;
String bin = "";
String path = "";
int run_ID;
int path_ID;

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

  run_ID = 0;

  Serial.println("Begin");
}

void loop()
{

  if (run_ID == 0)
  {
    //Dry run
    dryRun();
  }
  else if (run_ID == 1)
  {
    //Actual run
    digitalWrite(LEDPIN, LOW);
    actualRun();
  }

  delay(2000);
}

void actualRun()
{
  bin = "";

  bin = readSensors();

  checkForIntersectionAR();
}

void dryRun()
{
  bin = "";

  bin = readSensors();

  Serial.println(bin);

  checkForIntersectionDR(bin);

  //delay(500);
}

String readSensors()
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

void checkForIntersectionAR()
{
  if (bin.equals("01111") || bin.equals("00111") || bin.equals("11100") || bin.equals("11110") || bin.equals("11111"))
  {
    turn(bin.charAt(path_ID++));
  }
  else
  {
    turn('S');
  }
}

void checkForIntersectionDR(String s)
{
  //Check for Left
  if (s.equals("11110") || s.equals("11100"))
  {
    Serial.println("L");
    path += "L";
    turn('L');
  }
  //Check for Right
  else if (s.equals("01111") || s.equals("00111"))
  {

    String res = moveInch("R");
    //Case of a Straight Right
    if (res.equals("SR"))
    {

      Serial.println("S");
      path += "S";
      turn('S');
    }
    //Case of a Right - Only
    else if (res.equals("R"))
    {

      Serial.println("R");
      path += "R";
      turn('R');
    }
  }
  //Check for Cross, T and EOM
  else if (s.equals("11111"))
  {

    String res = moveInch("3W");

    //In case of End Of Maze(EOM)
    if (res.equals("EOM"))
    {
      endOfMaze();
    }

    //In case of T or Cross way.
    else if (res.equals("L"))
    {
      path += "L";
      Serial.println("L");
      turn('L');
    }
  }
  //Check for U turn
  else if (s.equals("00000"))
  {
    path += "U";
    Serial.println("U");
    turn('U');
  }
  else
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

String moveInch(String ch)
{

  String ret = "";

  turn('S');

  delay(300);

  //Stop the motors
  digitalWrite(RIN1, LOW);
  digitalWrite(RIN2, LOW);
  digitalWrite(LIN1, LOW);
  digitalWrite(LIN2, LOW);

  String check = readSensors();

  //Check for Right Only or Staright-Right
  if (ch.equals("R"))
  {
    //Check for Straight-Right
    if (check.equals("01110") || check.equals("00110") || check.equals("01100"))
    {
      ret = "SR";
    }
    //Check for Right Only
    else if (check.equals("00000"))
    {
      ret = "R";
    }
  }
  //Check for 3 way Intersection
  else if (ch.equals("3W"))
  {
    //Check for EOM(End of Maze)
    if (check.equals("11111"))
    {
      ret = "EOM";
    }
    else if (check.equals("01110") || check.equals("00110") || check.equals("01100") || check.equals("00000"))
    {
      ret = "L";
    }
  }
  return ret;
}

void endOfMaze()
{

  digitalWrite(RIN1, LOW);
  digitalWrite(RIN2, LOW);
  digitalWrite(LIN1, LOW);
  digitalWrite(LIN2, LOW);

  digitalWrite(LEDPIN, HIGH);

  run_ID = 1;

  path_ID = 0;

  path.trim();

  optimizePath(path);
  delay(10000);
}

void optimizePath(String p)
{
  String temp = "";
  while (isPathOpt(p))
  {
    for (int i = 0; i < p.length(); i++)
    {
      if (p.charAt(i) == 'U')
      {
        //Serial.println("Path : " + p);
        if (i > 0)
          temp += p.charAt(i - 1);
        temp += p.charAt(i);
        if (i < p.length() - 1)
          temp += p.charAt(i + 1);
        //Serial.println("Temp : " + temp);
        String replace = checkReplacement(temp);
        if (!replace.equals(""))
        {
          //Serial.println(replace + "|" + p);
          p = p.substring(0, i - 1) + replace + p.substring(i + 2);
        }

        temp = "";
      }
    }
  }
  Serial.println("Optimized Path: " + p);
}

boolean isPathOpt(String p)
{
  boolean ret = false;
  for (int i = 0; i < p.length(); i++)
  {
    if (p.charAt(i) == 'U')
    {
      // Serial.println("In isPathOpt: Path - " + p);
      // Serial.println("U found at- :" + (i+1));
      ret = true;
      break;
    }
  }

  return ret;
}

String checkReplacement(String temp)
{
  // Serial.println();
  // Serial.println("In checkReplacement");
  // Serial.println("Paramter String: " +  temp);
  // Serial.println();
  String ret = temp;
  if (temp.equalsIgnoreCase("LUR"))
    ret = "U";
  if (temp.equalsIgnoreCase("LUS"))
    ret = "R";
  if (temp.equalsIgnoreCase("RUL"))
    ret = "U";
  if (temp.equalsIgnoreCase("SUL"))
    ret = "R";
  if (temp.equalsIgnoreCase("SUS"))
    ret = "U";
  if (temp.equalsIgnoreCase("LUL"))
    ret = "S";

  return ret;
}