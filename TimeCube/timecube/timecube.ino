// GENUINO 101


/ Подключаем стандартную библиотеку LiquidCrystal 
#include <LiquidCrystal.h> 
#include <CurieBLE.h> 
#include "CurieIMU.h" 

// Инициализируем объект-экран, передаём использованные 
// для подключения контакты на Arduino в порядке: 
// RS, E, DB4, DB5, DB6, DB7 
LiquidCrystal lcd(4, 5, 10, 11, 12, 13); 


BLEService cubeService("19B10010-E8F2-537E-4F6C-D104768A1214"); // create service 
BLEIntCharacteristic downCharacteristic("19B10012-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify); 
BLEIntCharacteristic upCharacteristic("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify); 





int time_up = 0; 
int time_down = 0; 
int time_prev = 0; 
int time_minute = 0; 
int time_second = 0; 


void setup() { 
// put your setup code here, to run once: 
lcd.begin(16, 2); 
lcd.setCursor(0, 0); 
Serial.begin(9600); // initialize Serial communication 


// initialize device 
lcd.print("Initializing..."); 
CurieIMU.begin(); 

// Set the accelerometer range to 2G 
CurieIMU.setAccelerometerRange(2); 
delay(1000); 

BLE.begin(); 

// set the local name peripheral advertises 
BLE.setLocalName("Cube"); 
// set the UUID for the service this peripheral advertises: 
BLE.setAdvertisedService(cubeService); 

// add the characteristics to the service 
cubeService.addCharacteristic(upCharacteristic); 
cubeService.addCharacteristic(downCharacteristic); 

// add the service 
BLE.addService(cubeService); 

upCharacteristic.setValue(0); 
downCharacteristic.setValue(0); 

// start advertising 
BLE.advertise(); 


time_prev = millis(); 
} 



void loop() { 


int orientation = - 1; // the board's orientation 
String orientationString; // string for printing description of orientation 
int x = CurieIMU.readAccelerometer(X_AXIS); 
int y = CurieIMU.readAccelerometer(Y_AXIS); 
int z = CurieIMU.readAccelerometer(Z_AXIS); 

// put your main code here, to run repeatedly: 
int absX = abs(x); 
int absY = abs(y); 
int absZ = abs(z); 
if ( (absZ > absX) && (absZ > absY)) { 
// base orientation on Z 
if (z > 0) { 
orientationString = "up"; 
orientation = 0; 
} else { 
orientationString = "down"; 
orientation = 1; 
} 
} else if ( (absY > absX) && (absY > absZ)) { 
// base orientation on Y 
if (y > 0) { 
orientationString = "digital pins up"; 
orientation = 2; 
} else { 
orientationString = "analog pins up"; 
orientation = 3; 
} 
} else { 
// base orientation on X 
if (x < 0) { 
orientationString = "connector up"; 
orientation = 4; 
} else { 
orientationString = "connector down"; 
orientation = 5; 
} 
} 




if (orientation == 0 ){ 
time_up = time_up +(millis()-time_prev) ; 
lcd.setCursor(0, 0); 
lcd.print("Work: "); 
time_minute = ((time_up/1000)/60); 
time_second =((time_up/1000)%60); 
lcd.print(time_minute); 
lcd.print(":"); 
lcd.print(time_second); 


//Serial.println(time_up/1000)); 

} else if (orientation == 1){ 
time_down = time_down +(millis()-time_prev) ; 
lcd.setCursor(0, 0); 
lcd.print("Rest: "); 
time_minute = ((time_down/1000)/60); 
time_second =((time_down/1000)%60); 
lcd.print(time_minute); 
lcd.print(":"); 
lcd.print(time_second); 

} 




// listen for BLE peripherals to connect: 
BLEDevice central = BLE.central(); 
if (central && central.connected()) { 
upCharacteristic.setValue(time_up/1000); 
downCharacteristic.setValue(time_down/1000); 


} else { 

} 


time_prev = millis(); 
delay(500); 
lcd.clear(); 


}
