#include <SPI.h>
#include <SD.h>
const int chipSelect = 4;

// Curva frecuencia a SWC
const float SWC_a = 112;
const float SWC_b = -0.15;

#include <DHT.h>
#include <string.h>
#define DHTPIN 2     // Pin DTH11
#define DHTTYPE DHT11   // DHT 11 
      
DHT dht(DHTPIN, DHTTYPE);

    //#define IND_OP 13
    #define SEN_IN_1 8
    #define SEN_IN_2 9
    boolean sensor1_act;
    boolean sensor2_act;

    // soil temp

  float tempC;
int soiltemp;
int tempPin = 5;
    
void setup() {
    sensor1_act = false;
    sensor2_act = false;
   //pinMode(IND_OP, OUTPUT);
    pinMode(SEN_IN_1, INPUT);
    pinMode(SEN_IN_2, INPUT);



// initialize serial communication at 9600 bits per second:
    Serial.begin(9600);
      while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
      }
    dht.begin();
    delay(1000);
    
  Serial.println(String("########     Arduino Agrosensor        ##########"));
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

    File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(String("Temp, HR, SWC1, SWC2, frec1, frec2, soiltemp"));
    dataFile.close();
    Serial.println(String("Temp, HR, SWC1, SWC2, frec1, frec2, soiltemp"));
  }

}

    
void loop() {
  
  // make a string for assembling the data to log:
  String dataString = "";
  String dataString2 = "";
  
   if(digitalRead(SEN_IN_1) == LOW ){
   //digitalWrite(IND_OP, HIGH);
   // print out the value you read:
   //Serial.println("sensor 1 mojado");
   sensor1_act = true;
   delay(1);        // delay in between reads for stability
   } else {
   //digitalWrite(IND_OP, LOW);
   // print out the value you read:
   //Serial.println("sensor 1 seco");
   //delay(1);        // delay in between reads for stability
   sensor1_act = false;
   }

   if(digitalRead(SEN_IN_2) == LOW  ){
   //digitalWrite(IND_OP, HIGH);
   // print out the value you read:
   //Serial.println("sensor 2 mojado");
   sensor2_act = true;
   delay(1);        // delay in between reads for stability
   } else {
   //digitalWrite(IND_OP, LOW);
   // print out the value you read:
   //Serial.println("sensor 2 seco");
   sensor2_act = false;
   delay(1);        // delay in between reads for stability
   }

//// SEnsor de temp y hum ambiente
 // delay(2000);
   float h = dht.readHumidity();
   // Read temperature as Celsius
   float t = dht.readTemperature();
   
   // Check if any reads failed and exit early (to try again).
   if (isnan(h) || isnan(t) ) {
   Serial.println("Failed to read from DHT sensor!");
   return;
   }

   int sensor1Value = analogRead(A3); //Sensor de humedad 1
   int sensor2Value = analogRead(A4); //Sensor de humedad 2


   int value = analogRead(tempPin);
  float millivolts = (value / 1023.0) * 5000;
  float soiltemp = millivolts / 10; 

   dataString = String(t) + "," + String(h) + "," + String((sensor1Value*SWC_b) + SWC_a) + "," + String((sensor2Value*SWC_b) + SWC_a) + "," + String(sensor1Value)+ "," + String(sensor2Value) + "," + String(soiltemp);
   dataString2 = String(t) + "," + String(h) + "," + String((sensor1Value*SWC_b) + SWC_a) + "," + String((sensor2Value*SWC_b) + SWC_a)  + "," + String(soiltemp);

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString2);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }

  delay(2000);

}

