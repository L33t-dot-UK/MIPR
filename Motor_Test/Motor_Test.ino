int enRight = 9;
int enLeft = 5;
int i1A = 4; 
int i2A = 3;
int i3A = 8;
int i4A = 7;

void setup() 
{
}

void loop()  //This will move the robot forwards, backwards, left and then right.
{
    Forwards(254,254);
    delay(2000);
    Backwards(254,254);
    delay(2000);
    Left(100,100);
    delay(2000);
    Right(100,100);
    delay(2000);
}

/*
 * MOTOR FUNCTIONS
 */

 void Forwards(int leftSpeed, int rightSpeed)
 {
    digitalWrite(i1A,LOW);
    digitalWrite(i2A,HIGH);
    digitalWrite(i3A,HIGH);
    digitalWrite(i4A,LOW);
    analogWrite(enLeft, leftSpeed);
    analogWrite(enRight, rightSpeed);
 }
  void Backwards(int leftSpeed, int rightSpeed)
 {
    digitalWrite(i1A,HIGH);
    digitalWrite(i2A,LOW);
    digitalWrite(i3A,LOW);
    digitalWrite(i4A,HIGH);
    analogWrite(enLeft, leftSpeed);
    analogWrite(enRight, rightSpeed);
 }
 void Right(int leftRotationSpeed, int rightRotationSpeed)
 {
    digitalWrite(i1A,LOW);
    digitalWrite(i2A,HIGH);
    digitalWrite(i3A,LOW);
    digitalWrite(i4A,HIGH);
    analogWrite(enLeft, leftRotationSpeed);
    analogWrite(enRight, rightRotationSpeed);
 } 
 void Left(int leftRotationSpeed, int rightRotationSpeed)
 {
    digitalWrite(i1A,HIGH);
    digitalWrite(i2A,LOW);
    digitalWrite(i3A,HIGH);
    digitalWrite(i4A,LOW);
    analogWrite(enLeft, leftRotationSpeed);
    analogWrite(enRight, rightRotationSpeed);
 }
 void Halt() //Stops the robot by stopping the motors
 {
    digitalWrite(i1A,LOW);
    digitalWrite(i2A,LOW);
    digitalWrite(i3A,LOW);
    digitalWrite(i4A,LOW);
    analogWrite(enLeft, 254);
    analogWrite(enRight, 254);
 }
  void softStop() //stops the robot by letting the motors stop on their own
 {					//rolling stop
    analogWrite(enLeft, 0);
    analogWrite(enRight, 0);
 }
