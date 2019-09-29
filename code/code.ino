// ------------ Temperature Sensor KY-028 ------------
// Leds that will indicate if it's too cold or hot
int blueLEDPin = 13;
int redLEDPin = 12;
// Temperature Sensor Input pins
int tempSensorPin = A0;
int tempDigitalPin = 11;
// Temperature default and constant values
int ThermistorPin = A2;
int Vo, T;
float R1 = 10000; // value of R1 on board
float logR2, R2;
float c1 = 0.001129148, c2 = 0.000234125, c3 = 0.0000000876741; //steinhart-hart coeficients for thermistor
bool tempBuzzed = false;
// Temperature function
int analogToDegree(int Vo){
  R2 = R1 * (1023.0 / (float)Vo - 1.0); //calculate resistance on thermistor
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2)); // temperature in Kelvin
  T = T - 230;
  return T;
}


// ------------ Photocell ------------
int lightAnalogPin = A1;
int lightValue = 0;
int buzzerPin = 10;
bool lightBuzzed = false;
float sinVal;
int toneVal;


// ------------ LCD Screen ------------
#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// ------------ Bluetooth Module HC-05  ------------
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(8, 7); 

void buzzer(){
  sinVal = (sin(0*(3.1412/180)));
  toneVal = 2000+(int(sinVal*1000));
  tone(buzzerPin, toneVal, 1000);
}

void setup() {
  // Configure LED pins as output
  pinMode(blueLEDPin, OUTPUT);
  pinMode(redLEDPin, OUTPUT);

  Serial.begin (9600); // Configure Serial Monitor

  pinMode(tempDigitalPin, INPUT); // Configure Temperature Digital pin as input

  pinMode(buzzerPin, OUTPUT);

  lcd.begin(16, 2);
  lcd.print("Weather Station");

  BTSerial.begin(9600);
  
  delay(2000);

  lcd.clear();
}

void loop() {
  // Disable both Leds at the beginning of the loop
  digitalWrite(blueLEDPin, LOW);
  digitalWrite(redLEDPin, LOW);
  
  lcd.setCursor(0, 0);
  /*if (digitalRead(tempDigitalPin) == LOW ||fixInput == 0){ // If the sensor does not feel anything
    Serial.print("Temperature unavaible, recorded : ");
    Serial.println(analogRead(tempSensorPin));
    lcd.print("Temp false : ");
    lcd.print(analogRead(tempSensorPin));
    delay(1000); // Wait 1 more second
  }else {*/
    T = (int) analogToDegree(analogRead(ThermistorPin)); // Get Analog value and convert it to Celcius degrees
    if (T <= 17){ // If too Cold
      if (tempBuzzed == false){
        buzzer();
        tempBuzzed = true;
      }
      digitalWrite(blueLEDPin, HIGH);
    }else if (T >= 25){ // If too Hot
      if (tempBuzzed == false){
        buzzer();
        tempBuzzed = true;
      }
      digitalWrite(redLEDPin, HIGH);
    }
    Serial.print("Temperature = ");
    Serial.println(T);
    lcd.print("Temp: ");
    lcd.print(T);
    lcd.print(" C");
  //}

  lightValue = analogRead(lightAnalogPin);
  Serial.print("Light = ");
  if (lightValue < 250 && lightBuzzed == false){ // If luminosity under 250 lux
    buzzer();
    lightBuzzed = true;
    //Serial.print("Has to Buzz : ");
  }else if (lightValue >= 250 && lightBuzzed == true){
    lightBuzzed = false;
    //Serial.print("Had Buzzed : ");
  }
  Serial.println(lightValue);
  lcd.setCursor(0, 1);
  lcd.print("Light : ");
  lcd.print(lightValue);
  lcd.print(" lux");

  if (BTSerial.available() > 0){
      String inputString = "";
      inputString = BTSerial.readString();
      Serial.print("New BT Received : ");
      Serial.println(inputString);
      char inputChar[inputString.length()+1] ;
      inputString.toCharArray(inputChar,inputString.length()+1);
 
      char* command = strtok(inputChar, "&");
      while (command != 0)
      {
          char* valueCommand = strchr(command, ':');
          if (valueCommand != 0)
          {
              *valueCommand = 0;
              ++valueCommand;         
                /*if(String(command) == "temp"){
                  currentTemp = String(valueCommand).toInt() ;
                  Serial.print("New current temp");
                  Serial.println(currentTemp);
                }else if(String(command) == "sensor"){
                  fixInput = String(valueCommand).toInt() ;
                  Serial.print("New fixed Input");
                  Serial.println(fixInput);
                }*/
          }
          command = strtok(0, "&");
      }
  }
  
  delay(1000); // Wait 1 second between each loop
}
