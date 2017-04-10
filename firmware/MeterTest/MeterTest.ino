int lowMeter = 3;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Starting Serial");

  //Initialze Meters
  pinMode(lowMeter, OUTPUT);
   //test meters
  digitalWrite(lowMeter, HIGH);
  delay(500);

   //test meters
  digitalWrite(lowMeter, LOW);
  delay(500);
}

void loop() {
  for(int i = 0; i < 255; i++){
    analogWrite(lowMeter, i);
    Serial.println(i);
    delay(20);
  }

  for(int i = 255; i > 0; i--){
    analogWrite(lowMeter, i);
    Serial.println(i);
    delay(20);
  }

}
