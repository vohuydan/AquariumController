

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
pinMode(22,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(22,HIGH);
  Serial.println("LED ON");
  delay(1000);
  digitalWrite(22,LOW);
  Serial.println("LED Off");
  delay(1000);

}
