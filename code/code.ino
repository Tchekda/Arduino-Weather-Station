// ------------ Temperature Sensor KY-028 ------------
// Leds that will indicate if it's too cold or hot
int blueLEDPin = 13;
int redLEDPin = 12;
int orangeLEDPin = 10; // Alert on temp sensor working
// Temperature Sensor Input pins
int tempSensorPin = A0;
int tempDigitalPin = 11;
// Temperature default and constant values
int sensorValue = 0;
float temp = 0;
float currentTemp = 20.; // Set the current temperature
float fixtemp = currentTemp; 
int fixInput = 0; // Set the current value returned
float fixedDegreeValue = 5.5;
bool tempBuzzed = false;
// Temperature function
int analogToDegree(int sensorValue){
  temp = sensorValue - fixInput;
  temp = temp /fixedDegreeValue; 
  if (sensorValue > fixInput ){ // Cooler than default value
    fixtemp = fixtemp - temp;
  }  
  else if(sensorValue < fixInput) // Hoter than default value
  {
    fixtemp = fixtemp + temp;
  }
  else if(sensorValue == fixInput ){ // Default value
    fixtemp = currentTemp;
  }
  return fixtemp;
}


// ------------ Photocell ------------
int lightAnalogPin = A1;
int lightValue = 0;
int buzzerPin = 9;
bool lightBuzzed = false;
float sinVal;
int toneVal;


// ------------ LCD Screen ------------
#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);


void buzzer(){
  sinVal = (sin(100*(3.1412/180)));
  toneVal = 2000+(int(sinVal*1000));
  tone(buzzerPin, toneVal);
}

void setup() {
  // Configure LED pins as output
  pinMode(blueLEDPin, OUTPUT);
  pinMode(redLEDPin, OUTPUT);
  pinMode(orangeLEDPin, OUTPUT);

  Serial.begin (9600); // Configure Serial Monitor

  pinMode(tempDigitalPin, 11); // Configure Temperature Digital pin as input

  pinMode(buzzerPin, OUTPUT);

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Temperature");
  lcd.setCursor(0, 1);
  lcd.print("Light");
}

void loop() {
  // Disable both Leds at the beginning of the loop
  digitalWrite(blueLEDPin, LOW);
  digitalWrite(redLEDPin, LOW);
  digitalWrite(orangeLEDPin, LOW);

  if (digitalRead(orangeLEDPin) == LOW){ // If the sensor does not feel anything
    digitalWrite(orangeLEDPin, HIGH); // Enable Orange LED
    lcd.setCursor(0, 0);
    lcd.print("Temp not working");
    delay(1000); // Wait 1 more second
  }else {
    sensorValue = analogToDegree(analogRead(tempSensorPin)); // Get Analog value and convert it to Celcius degrees
    fixtemp = currentTemp; // Reset fixed temperature to default temp
    if (sensorValue <= 17){ // If too Cold
      if (tempBuzzed == false){
        buzzer();
        tempBuzzed = true;
      }
      digitalWrite(blueLEDPin, HIGH);
    }else if (sensorValue >= 25){ // If too Hot
      if (tempBuzzed == false){
        buzzer();
        tempBuzzed = true;
      }
      digitalWrite(redLEDPin, HIGH);
    }
    Serial.print("Temperature = ");
    Serial.println(sensorValue);
    lcd.setCursor(0, 0);
    lcd.print("Temperature : ");
    lcd.print(sensorValue);
  }

  lightValue = analogRead(lightAnalogPin);
  if (lightValue < 250 && lightBuzzed == false){ // If luminosity under 250 lux
    buzzer();
    lightBuzzed = true;
  }
  Serial.print("Light = ");
  Serial.println(lightValue);
  lcd.setCursor(0, 1);
  lcd.print("Light : ");
  lcd.print(lightValue);
  

 delay(1000); // Wait 1 second between each loop
}
