#include <Arduino.h>
#include <pms.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET     -1 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Pmsx003 pms(D7, D9);
Adafruit_BME280 bme;



float temp, press;
int PM1, PM2, PM10, humid;
unsigned long lastMilis = 0;
unsigned long lastPageChange = 0;
unsigned int pageinterval = 5000;
unsigned int readInterval = 4500;
unsigned int page = 0;
unsigned long milis;

size_t a = Pmsx003::PM1dot0;
size_t b = Pmsx003::PM2dot5;
size_t c = Pmsx003::PM10dot0;



void setup(){
Serial.begin(9600);


if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  delay(1000);
 


    bme.begin(0x76, &Wire);
    pms.begin();
  pms.waitForData(Pmsx003::wakeupTime);
  pms.write(Pmsx003::cmdModeActive);

  display.clearDisplay();
  display.setCursor(30,0);
  display.print("End of setup");
  display.display();
  delay(500);

}

void loop(){
    milis = millis();
    readData();
    changePage();

    switch (page)
    {
    case 1:
          display.clearDisplay();
          display.setCursor(30,0);
     display.print("BME280 Data");
        display.setCursor(1,30);
        display.print("Temperature: ");
        display.print(temp);
        display.println(" *C");
            display.setCursor(1, 40);
        display.print("Pressure: ");
        display.print(press);
        display.println(" hPa");
            display.setCursor(1, 50);
        display.print("Humidity: ");
        display.print(humid);
        display.println(" %");
         display.display();
        break;
    case 2:
     display.clearDisplay();
     display.setCursor(30,0);
     display.print("PMS5003 Data");
     display.setCursor(1, 30);
     display.print("PM1: ");
     display.print(PM1);
     display.println(" ug/m3");
     display.setCursor(1, 40);
     display.print("PM2,5: ");
     display.print(PM2);
     display.println(" ug/m3");
     display.setCursor(1, 50);
     display.print("PM10: ");
     display.print(PM10);
     display.println(" ug/m3");
     display.display();

    break;
        
    }

	const auto n = Pmsx003::Reserved;
	Pmsx003::pmsData data[n];

	Pmsx003::PmsStatus status = pms.read(data, n);
    
   	switch (status) {
		case Pmsx003::OK:
		{
		
            PM1 = data[a];
            PM2 = data[b];
            PM10 = data[c];
			
			}
			break;
		
		case Pmsx003::noData:
			break;
		default:
			Serial.println("_________________");
			Serial.println(Pmsx003::errorMsg[status]);
	};
    
}

void readData(){
if (milis - lastMilis >= readInterval)
{
    temp = bme.readTemperature();
    press = bme.readPressure() / 100;
    humid = bme.readHumidity();

    lastMilis = milis;
    
}
    
}

void changePage(){
if (milis - lastPageChange >= pageinterval)
{
    ++page;
    lastPageChange = milis;
}

if (page == 3)
{
    page = 1;
}

}
