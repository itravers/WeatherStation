/**
 * Firmware for the one day weather station.
 * This uses ethernet to make an api call to chicosystems
 * which returns the low temp, high temp and precipation chance
 * for a given zip code.
 * 
 * The zip code is chosen via 5 potentiometers
 * The result is displayed on 3 analog panel meters.
 * When the user flips the refresh switch an api call is made
 * with the current zip code settings, the result is then displayed
 */

 #include <SPI.h>
 #include <Ethernet.h>

 //Assigne a MAC address to the board, this will need to be different for every board
 byte mac[] = { 0xDE, 0xAD, 0xBC, 0xEF, 0xFE, 0xED };
 //the Server we make our api call to
 char server[] = "192.168.1.197";
 //Set the static ip address that will be used if DHCP fails
 IPAddress ip(192, 168, 1, 222);
 //Initialze the Ethernet client library
 EthernetClient client;

//Analog Meters
int lowMeter = 3;
int highMeter = 5;
int precipMeter = 6;
int togglePin = 12;

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

  //Start the Ethernet Connection
  if(Ethernet.begin(mac) == 0){
    Serial.println("Failed to configure Ethernet using DHCP");
    //try to configure ethernet using ip address
    Ethernet.begin(mac, ip);
  }

  //Give the ethernet shield time to initialize
  Serial.println("Connecting Ethernet...");
  delay(1000);

  //if we get a connection, print it to serial
  if(client.connect(server, 3001)){
    Serial.println("Ethernet Connected!");
    //make a demo http request
    client.println("GET /api/connectionTest HTTP/1.1");
    client.println("HOST: 192.168.1.197");
    client.println("Connection: close");
    client.println();
  }else{
    Serial.println("Ethernet Connection FAILED!");
  }

  //init toggle pin
  pinMode(togglePin, INPUT);
 // digitalWrite(togglePin, HIGH);

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

  boolean toggleInput = digitalRead(togglePin);

  zipOneInput = analogRead(zipOne);
  zipTwoInput = analogRead(zipTwo);
  zipThreeInput = analogRead(zipThree);
  zipFourInput = analogRead(zipFour);
  zipFiveInput = analogRead(zipFive);
  
  analogWrite(lowMeter, map(zipOneInput, 0, 1023, 0, 255));
  analogWrite(highMeter, map(zipTwoInput, 0, 1023, 0, 255));
  analogWrite(precipMeter, map(zipThreeInput, 0, 1023, 0, 255));

  Serial.print("Toggle: ");
  Serial.print(toggleInput);
  Serial.print(" ");

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
