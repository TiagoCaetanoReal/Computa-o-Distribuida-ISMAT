// https://www.diyengineers.com/2020/12/31/ds18b20-with-arduino/
//sensor temperatura


/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/


#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into digital pin 2 on the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);	

// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

int msensor = A3;
int light_sensor = A2; 
int msvalue = 0;

String sensorsResults;
float sensorsResultsB[3] ;


void setup(){
  Serial.begin(9600);
  pinMode(msensor, INPUT);  
  sensors.begin();	// Start up the library
}

void loop(){
  sensorsResults = ' ';
  msvalue = analogRead(msensor);
  // Serial.println("Nivel de humidade no solo: ");
  // Serial.println(toPercentage(0,1023,msvalue));

  sensorsResults = toPercentage(0,1023,msvalue);
  sensorsResults += ' ';
  sensorsResultsB[0] = toPercentage(0,1023,msvalue);

  int raw_light = analogRead(light_sensor); // read the raw value from light_sensor pin (A3)
  // Serial.println("Nivel de luz ambiente: ");
  // Serial.print(toPercentage(1023,0,raw_light));
	// Serial.println("%"); 

  sensorsResults += toPercentage(1023,0,raw_light);
  sensorsResults += ' ';
  sensorsResultsB[1] = toPercentage(1023,0,raw_light);

  sensors.requestTemperatures(); 

  //print the temperature in Celsius
  // Serial.print("Temperature: ");
  // Serial.print(sensors.getTempCByIndex(0));
  // Serial.println("C");
  
  sensorsResults += sensors.getTempCByIndex(0);
  sensorsResultsB[2] = sensors.getTempCByIndex(0);

  for(int i=0; i<= 2; i++){
    Serial.println(sensorsResults);
  }

  Serial.write(sensorsResultsB, 3);

// tentar mandar dados em string
  
}

int toPercentage(int max, int min, int value){
  return map(value, min, max, 0, 100);
}
 

