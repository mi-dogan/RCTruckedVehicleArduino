
const int enA = 9;        //motor definitions
const int motorPin1 = 4;
const int motorPin2 = 5;
const int enB = 10;
const int motorPin3 = 6;
const int motorPin4 = 7;

const int lo = 1120;  //RC transmitter  definitions
const int hi = 1870;
const int deadlo = 1470;
const int deadhi = 1520;
const int center = 1500;

int ch[4];

int x1 = 0;
int y1 = 0;
int x2 = 0;
int y2 = 0;

int y1_pwm = 0;
int x2_pwm = 0;
int x2_reverse_pwm = 0;
int x1_pwm = 0;
int x1_reverse_pwm = 0;

int throttle = 0;

void setup()
{
  Serial.begin(9600);

  pinMode(A0, INPUT);   //rc receiver pins
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  
//  pinMode(A4, INPUT);
//  pinMode(A5, INPUT);

  pinMode(enA, OUTPUT);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
}

void loop()
{
  //reading transmitter data
  ch[0] = pulseIn (A0, HIGH); //x2
  ch[1] = pulseIn (A1, HIGH); //y2
  ch[2] = pulseIn (A2, HIGH); //y1
  ch[3] = pulseIn (A3, HIGH); //x1

  //  ch[4] = pulseIn (A4, HIGH);//right pot
  //  ch[5] = pulseIn (A5, HIGH);//left pot

//stabilizing transmitter data
  for (int i = 0; i < 4; i++)
  {
    ch[i] = constrain(ch[i], lo, hi);
    if (ch[i] <= deadhi && ch[i] >= deadlo) {
      ch[i] = center;
    }
  }
  //redefinitions transmitter joysticks
  x1 = ch[3];
  y1 = ch[2];
  x2 = ch[0];
  y2 = ch[1];

  //getting pwm for motor control
  x1_pwm = map(x1, 1500, 1870, 0, 255);
  x1_reverse_pwm = map(x1, 1500, 1120, 0, 255);
  
  x2_pwm = map(x2, 1500, 1870, 0, 255);
  x2_reverse_pwm = map(x2, 1500, 1120, 0, 255);

  y1_pwm = map(y1, 1120, 1870, 0, 255);
  
  if (y1 > 1120) throttle = 1; else throttle = 0;
  if (throttle && y2 == 1500 && x2 == 1500 && x1 == 1500)
  {
    stop_all();
//    Serial.println("stop");
  }
  if (throttle && y2 > 1500 && x2 == 1500)
  {
    forward();
//    Serial.println ("forward");
  }
  if (throttle && y2 < 1500 && x2 == 1500)
  {
    backward();
//    Serial.println ("backward");
  }
  if (throttle && x1 > 1500)
  {
    right_on_its_axis();
//    Serial.print ("right: ");
//    Serial.println (x2_pwm);
  }
  if (throttle && x1 < 1500)
  {
    left_on_its_axis();
//    Serial.print ("left: ");
//    Serial.println (x2_reverse_pwm);
  }
  if (throttle && x2 > 1500 && y2 == 1500)
  {
    right_spin();
//    Serial.print ("right: ");
//    Serial.println (x2_pwm);
  }
  if (throttle && x2 < 1500 && y2 == 1500)
  {
    left_spin();
//    Serial.print ("left: ");
//    Serial.println (x2_reverse_pwm);
  }
  if (throttle && y2 > 1500 && x2 > 1500)
  {
    right_forward();
//    Serial.println ("right_forward");
  }
  if (throttle && y2 > 1500 && x2 < 1500)
  {
    left_forward();
//    Serial.println ("left forward");
  }
  if (throttle && y2 < 1500 && x2 > 1500)
  {
    right_backward();
//    Serial.println ("right_backward");
  }
  if (throttle && y2 < 1500 && x2 < 1500)
  {
    left_backward();
//    Serial.println ("left backward");
  }
  delay(50);

  //  Serial.print ("x1: ");
  //  Serial.println (x1);
  //
  //  Serial.print ("y1: ");
  //  Serial.println (y1);
  //
  //  Serial.print ("x2: ");
  //  Serial.println (x2);
  //
  //  Serial.print ("y2: ");
  //  Serial.println (y2);

  //  Serial.print ("Ch5:");
  //  Serial.println (ch[4]);
  //
  //  Serial.print ("Ch6:");
  //  Serial.println (ch[5]);
}

void forward()
{
  analogWrite(enA, y1_pwm);
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  analogWrite(enB, y1_pwm);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
}

void backward()
{
  analogWrite(enA, y1_pwm);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  analogWrite(enB, y1_pwm);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
}

void right_on_its_axis()
{
  analogWrite(enA, x1_pwm);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  analogWrite(enB, x1_pwm);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
}

void left_on_its_axis()
{
  analogWrite(enA, x1_reverse_pwm);
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  analogWrite(enB, x1_reverse_pwm);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);

}

void right_spin()
{
  analogWrite(enA, x2_pwm);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  analogWrite(enB, x2_pwm);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
}

void left_spin()
{
  analogWrite(enA, x2_reverse_pwm);
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  analogWrite(enB, x2_reverse_pwm);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);

}

void stop_all()
{
  analogWrite(enA, 0);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  analogWrite(enB, 0);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}
void right_forward()
{
  analogWrite(enA, 30);
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  analogWrite(enB, y1_pwm);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);

}
void left_forward()
{
  analogWrite(enA, y1_pwm);
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  analogWrite(enB, 30);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);

}
void right_backward()
{
  analogWrite(enA, 30);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  analogWrite(enB, y1_pwm);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
}
void left_backward()
{
  analogWrite(enA, y1_pwm);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  analogWrite(enB, 30);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
}


