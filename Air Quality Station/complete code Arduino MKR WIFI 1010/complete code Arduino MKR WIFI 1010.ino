/* Test sketch for sensors */
//Libraries
#include "Adafruit_PM25AQI.h"
#include "Adafruit_SGP30.h"
#include <DHT.h>;
#include <LiquidCrystal_I2C.h>
// #include "PMS.h"              // https://github.com/fu-hsi/pms
// comment these two lines if using hardware serial
// #include <SoftwareSerial.h>
// SoftwareSerial pmSerial(2, 3);
// #include <Wire.h>
// PMS pms(Serial1);

Adafruit_PM25AQI aqi = Adafruit_PM25AQI(); //Setting up PMS5003 data
Adafruit_SGP30 sgp; //Setting up SGP30 data

#define DHTPIN 3     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
//Constants
float hum;  //Stores humidity value
float temp; //Stores temperature value

int totalColumns = 16;
int totalRows = 2;
//I2C pins declaration for 1602A with I2C module
LiquidCrystal_I2C lcd(0x27, totalColumns, totalRows); 

void setup() {
  // Wait for serial monitor to open
  Serial.begin(115200);
  while (!Serial) delay(10);
  Serial.println("SGP30 test");

  if (! sgp.begin()){
    Serial.println("Sensor not found :(");
    while (1);
  }
  Serial.print("Found SGP30 serial #");
  Serial.print(sgp.serialnumber[0], HEX);
  Serial.print(sgp.serialnumber[1], HEX);
  Serial.println(sgp.serialnumber[2], HEX);
  // If you have a baseline measurement from before you can assign it to start, to 'self-calibrate'
  sgp.setIAQBaseline(0x8E68, 0x8F41);  // Will vary for each sensor!
  Serial.println(" PMS5003 Air Quality Sensor");

  // Wait 20 seconds for sensor to boot up!
  delay(20000);

  // If using serial, initialize it and set baudrate before starting!
  // Uncomment one of the following
  Serial1.begin(9600);
  // pmSerial.begin(9600);
  // There are 3 options for connectivity!
  // if (! aqi.begin_I2C()) {      // connect to the sensor over I2C
  if (! aqi.begin_UART(&Serial1)) { // connect to the sensor over hardware serial
  // if (! aqi.begin_UART(&pmSerial)) { // connect to the sensor over software serial 
    Serial.println("Could not find PM 2.5 sensor!");
    while (1) delay(10);}

  Serial.println("PM25 found!");
  dht.begin();
  lcd.init(); 
  // lcd.begin(16,2);//Defining 16 columns and 2 rows of lcd display
  lcd.backlight(); // use to turn on and turn off LCD back light
}

int counter = 0;

void loop() {
  // PMS::DATA data;
  PM25_AQI_Data data;
  if (! aqi.read(&data)) {
    Serial.println("Could not read from AQI");
    delay(1000);  // try again in a bit!
    return;
  }
  
  Serial.println("AQI reading success");

  Serial.println(F("Concentration Units (standard)"));
  Serial.println(F("---------------------------------------"));
  Serial.print(F("PM 1.0: ")); Serial.print(data.pm10_standard);
  Serial.print(F("\t\tPM 2.5: ")); Serial.print(data.pm25_standard);
  Serial.print(F("\t\tPM 10: ")); Serial.println(data.pm100_standard);
  Serial.println(F("Concentration Units (environmental)"));
  Serial.println(F("---------------------------------------"));
  Serial.print(F("PM 1.0: ")); Serial.print(data.pm10_env);
  Serial.print(F("\t\tPM 2.5: ")); Serial.print(data.pm25_env);
  Serial.print(F("\t\tPM 10: ")); Serial.println(data.pm100_env);
  Serial.println(F("---------------------------------------"));
  Serial.print(F("Particles > 0.3um / 0.1L air:")); Serial.println(data.particles_03um);
  Serial.print(F("Particles > 0.5um / 0.1L air:")); Serial.println(data.particles_05um);
  Serial.print(F("Particles > 1.0um / 0.1L air:")); Serial.println(data.particles_10um);
  Serial.print(F("Particles > 2.5um / 0.1L air:")); Serial.println(data.particles_25um);
  Serial.print(F("Particles > 5.0um / 0.1L air:")); Serial.println(data.particles_50um);
  Serial.print(F("Particles > 10 um / 0.1L air:")); Serial.println(data.particles_100um);
  Serial.println(F("---------------------------------------"));
  Serial.println(F("---------------------------------------"));
      //Read data and store it to variables hum and temp
    hum = dht.readHumidity();
    temp= dht.readTemperature();
    //Print temp and humidity values to serial monitor
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" %, Temp: ");
    Serial.print(temp);
    Serial.println(" Celsius");
    if (! sgp.IAQmeasure()) {
    Serial.println("Measurement failed");
    return;
  }
  Serial.print("TVOC "); Serial.print(sgp.TVOC); Serial.print(" ppb\t");
  Serial.print("eCO2 "); Serial.print(sgp.eCO2); Serial.println(" ppm");

  if (! sgp.IAQmeasureRaw()) {
    Serial.println("Raw Measurement failed");
    return;
  }
  Serial.print("Raw H2 "); Serial.print(sgp.rawH2); Serial.print(" \t");
  Serial.print("Raw Ethanol "); Serial.print(sgp.rawEthanol); Serial.println("");
    delay(1000);

  counter++;
  if (counter == 30) {
    counter = 0;

    uint16_t TVOC_base, eCO2_base;
    if (! sgp.getIAQBaseline(&eCO2_base, &TVOC_base)) {
      Serial.println("Failed to get baseline readings");
      return;
    }
    Serial.print("****Baseline values: eCO2: 0x"); Serial.print(eCO2_base, HEX);
    Serial.print(" & TVOC: 0x"); Serial.println(TVOC_base, HEX);
  }

    // Converting the sensor readings to string values that the LCD can display
String pm1String = String(data.pm10_env);
String pm25String = String(data.pm25_env);
String pm100String = String(data.pm100_env);
String tempString = String(temp);
String humString = String(hum);
String TVOCString = String(sgp.TVOC);
String eCO2String = String(sgp.eCO2);

unsigned long previousMillis = 0;
unsigned long interval = 1000; //Refreshes the LCD once every 1000ms (1 seconds)

unsigned long currentMillis = millis();
if (currentMillis - previousMillis > interval) {
  previousMillis = currentMillis;

  lcd.clear();//Clear the screen
  lcd.setCursor(0,0);
  lcd.print("PM1  PM2.5  PM10");
  lcd.setCursor(0,1);
  lcd.print(pm1String);
  lcd.setCursor(5,1);
  lcd.print(pm25String);
  lcd.setCursor(12,1);
  lcd.print(pm100String); delay(10000);

  lcd.clear();//Clear the screen
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.setCursor(6, 0);
  lcd.print(tempString);lcd.setCursor(11, 0);lcd.print((char) 223);
  lcd.setCursor(12, 0);lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Humidity:");
  lcd.setCursor(10, 1);
  lcd.print(humString);lcd.setCursor(15, 1);lcd.print("%");delay(10000);

  lcd.clear();//Clear the screen
  lcd.setCursor(0, 0);
  lcd.print("TVOC:");
  lcd.setCursor(6, 0);
  lcd.print(TVOCString);lcd.setCursor(13, 0);lcd.print("ppb");
  lcd.setCursor(0, 1);
  lcd.print("eco2:");
  lcd.setCursor(6, 1);
  lcd.print(eCO2String);lcd.setCursor(13, 1);lcd.print("ppm");delay(10000);
  }
}
