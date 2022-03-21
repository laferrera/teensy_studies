int CV_PIN = 14;
void setup() {
  // put your setup code here, to run once:
  //pinMode(CV_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
//  digitalWriteFast(CV_PIN, LOW);
//  delay(500);
//  digitalWriteFast(CV_PIN, HIGH);
  analogWrite(CV_PIN, 50);
  delay(500);
    analogWrite(CV_PIN, 250);
//  analogWrite(CV_PIN, 50);
}
