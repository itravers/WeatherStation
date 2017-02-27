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
 #include <Bridge.h>
 #include <HttpClient.h>

//boxVersion and SerialNum
String boxVersion = "oneDay";
String serialNum = "0000";

 //Assigne a MAC address to the board, this will need to be different for every board
 byte mac[] = { 0xDE, 0xAD, 0xBC, 0xEF, 0xFE, 0xED };
 //the Server we make our api call to
 char server[] = "192.168.1.197";
 //Set the static ip address that will be used if DHCP fails
 IPAddress ip(192, 168, 1, 222);
 //Initialze the Ethernet client library
 EthernetClient client;
 HttpClient httpClient;

  String S_lowTemp;
    String S_highTemp;
    String S_precipChance;

//Analog Meters
int lowMeter = 3;
int highMeter = 5;
int precipMeter = 6;
int togglePin = 9;

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

//toggle inputs
boolean toggleInput;
boolean lastToggleInput; //if lastToggleInput doesn't equal toggle input, it means the switch has been toggled

//zip code inputs
int zipOneInput;
int zipTwoInput;
int zipThreeInput;
int zipFourInput;
int zipFiveInput;

//zip code inputs mapped from 0 - 9
int zipOneInputMapped;
int zipTwoInputMapped;
int zipThreeInputMapped;
int zipFourInputMapped;
int zipFiveInputMapped;


void setup() {
  // put your setup code here, to run once:

  //start bridge, wait for serial connection before going into loop
  ////Bridge.begin();
  Serial.begin(9600);

  //wait for serial to connect
  ////while(!Serial);

  //Start the Ethernet Connection
  if(Ethernet.begin(mac) == 0){
    Serial.println("Failed to configure Ethernet using DHCP");
    //try to configure ethernet using ip address
    Ethernet.begin(mac, ip);
  }

  //Give the ethernet shield time to initialize
  Serial.println("Connecting Ethernet...");
  delay(1000);

 /* //if we get a connection, print it to serial
  if(client.connect(server, 3001)){
    Serial.println("Ethernet Connected!");
    //make a demo http request
    client.println("GET /api/oneDay/96091 HTTP/1.1");
    client.println("HOST: 192.168.1.197");
    client.println("Connection: close");
    client.println();
  }else{
    Serial.println("Ethernet Connection FAILED!");
  }
  */

  //use httpclient to get connection from server
  ////httpClient.get("http://192.168.1.197:3001/api/");

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

  delay(500);

  //test meters
  digitalWrite(lowMeter, HIGH);
  delay(500);
  digitalWrite(highMeter, HIGH);
  delay(500);
  digitalWrite(precipMeter, HIGH);
  delay(1000);

  digitalWrite(lowMeter, LOW);
  digitalWrite(highMeter, LOW);
  digitalWrite(precipMeter, LOW);

  //toggleInput = digitalRead(togglePin);
  //lastToggleInput = toggleInput;

}

void loop() {
 
  String data;
  

   zipOneInput = analogRead(zipOne);
  zipTwoInput = analogRead(zipTwo);
  zipThreeInput = analogRead(zipThree);
  zipFourInput = analogRead(zipFour);
  zipFiveInput = analogRead(zipFive);

  while(client.available()){
    S_lowTemp = "";
    S_highTemp = "";
    S_precipChance = "";
    char c = client.read();
    if(c == '$') {//this is the start of our data
      while(client.available()){//put the data into the data string
        c = client.read();
        if(c == '$'){//we've completed s_lowTemp now go to next
          while(client.available()){
            c = client.read();
            if(c == '$'){//Weve completed s_highTemp, now do next
              while(client.available()){
                c = client.read();
                if(c == '$'){
                  //we've finished S_precipChange, we are done parsing our data.
                }else{
                  S_precipChance +=c;
                }
              }
            }else{
              S_highTemp += c;
            }
          }
        }else{
          S_lowTemp += c;
        }
      }
    }
  }

  if(S_lowTemp.length() > 0){
   // Serial.print("DATA: ");
   // Serial.println(data);

   

    Serial.print("Low : ");
    Serial.print(S_lowTemp);
    Serial.print("   High: ");
    Serial.print(S_highTemp);
    Serial.print("   Prec: ");
    Serial.println(S_precipChance);
  }

  //if the server is disconnected, stop the client
  if(!client.connected()){
   // Serial.println();
   // Serial.println("disconnecting from ethernet");
    client.stop();
  }

  toggleInput = digitalRead(togglePin);
  if(toggleInput != lastToggleInput){
    Serial.println("Switch has been toggled.");
    if(client.connect(server, 3001)){
      Serial.println("Ethernet Connected!");
      //make a demo http request
      String zipString1 = String(zipOneInputMapped);
      String zipString2 = String(zipTwoInputMapped);
      String zipString3 = String(zipThreeInputMapped);
      String zipString4 = String(zipFourInputMapped);
      String zipString5 = String(zipFiveInputMapped);
      String zipString = zipString1+zipString2+zipString3+zipString4+zipString5;
      if(zipString.equals("00000")){
        zipString = "96091";
      }
      Serial.print("ZIP STRING: ");
      Serial.println(zipString);
      client.println("GET /api/oneDay/"+zipString+"/"+boxVersion+"/"+serialNum+" HTTP/1.1");
      client.println("HOST: 192.168.1.197");
      client.println("Connection: close");
      client.println();
    }else{
      Serial.println("Ethernet Connection FAILED!");
   }
    
  }

 
  
  //analogWrite(lowMeter, map(zipOneInput, 0, 1023, 0, 255));
  //analogWrite(highMeter, map(zipTwoInput, 0, 1023, 0, 255));
  //analogWrite(precipMeter, map(zipThreeInput, 0, 1023, 0, 255));

  //get temps and write to meters
  analogWrite(lowMeter, map(S_lowTemp.toInt(), 0, 100, 0, 255));
  analogWrite(highMeter, map(S_highTemp.toInt(), 0, 100, 0, 255));
  analogWrite(precipMeter, map(S_precipChance.toInt(), 0, 100, 0, 255));

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
  zipOneInputMapped = map(zipOneInput, 0, 1021, 0, 9);
  zipTwoInputMapped = map(zipTwoInput, 0, 1021, 0, 9);
  zipThreeInputMapped = map(zipThreeInput, 0, 1021, 0, 9);
  zipFourInputMapped = map(zipFourInput, 0, 1021, 0, 9);
  zipFiveInputMapped = map(zipFiveInput, 0, 1021, 0, 9);
  
  Serial.print(zipOneInputMapped);
  Serial.print(zipTwoInputMapped);
  Serial.print(zipThreeInputMapped);
  Serial.print(zipFourInputMapped);
  Serial.print(zipFiveInputMapped);
  Serial.println();

  lastToggleInput = toggleInput;
  delay(1000);

}
