int pot =   A0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Starting Serial");

   pinMode(pot, INPUT);
   delay(500);
}

void loop() {
  int potValue = analogRead(pot);
  Serial.print(potValue);
  Serial.print(" : ");
  int potValueMapped = map(potValue, 0, 1021, 0, 9);
  Serial.println(potValueMapped);
  delay(20);

}
