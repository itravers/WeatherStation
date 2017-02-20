//Analog Meters
int lowMeter = 3;
int highMeter = 5;
int precipMeter = 6;

//zip code meters
int zipOne =   A0;
int zipTwo =   A1;
int zipThree = A2;
int zipFour =  A3;
int zipFive =  A4;

//Analog Meter Output Values
int lowMeterOutput;
int highMeterOutput;
int precipMeterOutput;

//zip code inputs
int zipOneInput;
int zipTwoInput;
int zipThreeInput;
int zipFourInput;
int zipFiveInput;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //Initialze Meters
  pinMode(lowMeter, OUTPUT);
  pinMode(highMeter, OUTPUT);
  pinMode(precipMeter, OUTPUT);


  //Initialize Zipcode Inputs
  pinMode(zipOne, INPUT);
  pinMode(zipTwo, INPUT);
  pinMode(zipThree, INPUT);
  pinMode(zipFour, INPUT);
  pinMode(zipFive, INPUT);

  zipOneInput = 0;
  zipTwoInput = 0;
  zipThreeInput = 0;
  zipFourInput = 0;
  zipFiveInput = 0;

  delay(1000);

  //test meters
  digitalWrite(lowMeter, HIGH);
  delay(1000);
  digitalWrite(highMeter, HIGH);
  delay(1000);
  digitalWrite(precipMeter, HIGH);
  delay(3000);

  digitalWrite(lowMeter, LOW);
  digitalWrite(highMeter, LOW);
  digitalWrite(precipMeter, LOW);

}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("sketch loaded");
  //delay(100);

  zipOneInput = analogRead(zipOne);
  zipTwoInput = analogRead(zipTwo);
  zipThreeInput = analogRead(zipThree);
  zipFourInput = analogRead(zipFour);
  zipFiveInput = analogRead(zipFive);
  
  analogWrite(lowMeter, map(zipOneInput, 0, 1023, 0, 255));
  analogWrite(highMeter, map(zipTwoInput, 0, 1023, 0, 255));
  analogWrite(precipMeter, map(zipThreeInput, 0, 1023, 0, 255));

  Serial.print("INPUTS: ");
  Serial.print(zipOneInput);
  Serial.print("  ");
  Serial.print(zipTwoInput);
  Serial.print("  ");
  Serial.print(zipThreeInput);
  Serial.print("  ");
  Serial.print(zipFourInput);
  Serial.print("  ");
  Serial.print(zipFiveInput);
  Serial.print("  ");

  Serial.print("ZIP: ");
  Serial.print(map(zipOneInput, 0, 1023, 0, 9));
  Serial.print(map(zipTwoInput, 0, 1023, 0, 9));
  Serial.print(map(zipThreeInput, 0, 1023, 0, 9));
  Serial.print(map(zipFourInput, 0, 1023, 0, 9));
  Serial.print(map(zipFiveInput, 0, 1023, 0, 9));
  Serial.println();
  delay(200);

}
