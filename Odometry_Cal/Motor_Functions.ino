/*
//Author David Bradshaw
//Contains all motor funtions
//
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
 void Halt()
 {
    digitalWrite(i1A,LOW);
    digitalWrite(i2A,LOW);
    digitalWrite(i3A,LOW);
    digitalWrite(i4A,LOW);
    analogWrite(enLeft, 254);
    analogWrite(enRight, 254);
 }
  void softStop()
 {
    analogWrite(enLeft, 0);
    analogWrite(enRight, 0);
 }
