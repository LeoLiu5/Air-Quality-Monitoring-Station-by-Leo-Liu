#include <LiquidCrystal_I2C.h>
#include <Wire.h> 
int totalColumns = 16;
int totalRows = 2;

LiquidCrystal_I2C lcd(0x27, totalColumns, totalRows);

void setup(){
lcd.init(); 
lcd.backlight(); // use to turn on and turn off LCD back light
}

void loop()
{
lcd.setCursor(0, 0);
lcd.print("Microcontrollers");
lcd.setCursor(0,1);
lcd.print("I2C LCD tutorial");
delay(1000);
lcd.clear(); 
lcd.setCursor(0, 0);
lcd.print("Static text");
delay(1000);
lcd.setCursor(0,1);
lcd.print("I2C LCD tutorial");
delay(1000);
lcd.clear(); 
}

// //sketch created by Akshay Joseph

// #include <LiquidCrystal_I2C.h>

// LiquidCrystal_I2C lcd(0x27, 16, 2);

// void setup()
// {

// 	lcd.begin(16,2);


// 	lcd.backlight();
// 	lcd.clear();
// 	lcd.setCursor(4,0);
// 	lcd.print("Hackster");
// }

// void loop()
// {
// }

// #include <Wire.h>
// #include <hd44780.h>                       // main hd44780 header
// #include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header

// hd44780_I2Cexp lcd;


// const int LCD_COLS = 16;
// const int LCD_ROWS = 2;

// void setup()
// {
// int status;


// 	status = lcd.begin(LCD_COLS, LCD_ROWS);
// 	if(status) // non zero status means it was unsuccesful
// 	{
// 		hd44780::fatalError(status); // does not return
// 	}
 
// 	lcd.print("Hello, World!");
// }

// void loop() {}