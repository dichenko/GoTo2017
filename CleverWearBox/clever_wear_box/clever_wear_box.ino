#include "Timer.h"

Timer t;
int pin = 13;
int i1, i, full;

#define TINY_GSM_MODEM_SIM900
#include <TinyGsmClient.h>

const char apn[] = "internet.mts.ru";
const char user[] = "mts";
const char pass[] = "mts";

// or Software Serial on Uno, Nano
#include <SoftwareSerial.h>
SoftwareSerial SerialAT(10, 11); // RX, TX

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);

const char server[] = "77.220.184.42";
const char resource[] = "/api/updatebox/";

#include <SharpIR.h>

SharpIR sensor(GP2Y0A02YK0F, A0);

void setup()
{
Serial.begin(9600);
pinMode(pin, OUTPUT);
t.oscillate(pin, 100, LOW);
t.every(180000, takeReading);
SerialAT.begin(9600);
}

void loop()
{
t.update();
}

void takeReading()
{
while (i != 60) {
i1 += sensor.getDistance();
delay(1000);
i++;
}
i = 0;
Serial.println((i1) / 60);
if (i1 < MAXDISTANCE)
{
while (request("100"))
{
request("100");
}
full = 1;
}
else if (full)
{
full = 0;
while (request("0"))
{
request("0");
}
}
i1 = 0;
}

bool request(String data)
{


pinMode(2, OUTPUT);
digitalWrite(2, HIGH);

Serial.begin(9600);
delay(10);



Serial.println("Starting...");

SerialAT.begin(9600);
delay(3000);
Serial.println("Initializing modem...");
modem.restart();

Serial.print("Waiting for network...");
if (!modem.waitForNetwork()) {
Serial.println(" fail");
delay(10000);
return 1;
}
Serial.println(" OK");

Serial.print("Connecting to ");
Serial.print(apn);
if (!modem.gprsConnect(apn, user, pass)) {
Serial.println(" fail");
delay(10000);
return 1;
}
Serial.println(" OK");

Serial.print("Connecting to ");
Serial.print(server);
if (!client.connect(server, 8888)) {
Serial.println(" fail");
delay(10000);
return 1;
}
Serial.println(" OK");

String PostData = "imei=2323&fillamount=" + data;
client.print(String("POST ") + resource + " HTTP/1.0\r\n");
client.print(String("Host: ") + server + "\r\n");
client.println("Content-Type: application/x-www-form-urlencoded");
client.print("Connection: close\r\n");
client.print("Content-Length: ");
client.println(PostData.length());
client.println();
client.println(PostData);

client.println();

client.stop();
Serial.println("Server disconnected");

modem.gprsDisconnect();
Serial.println("GPRS disconnected");

digitalWrite(2, 0);
return 0;
}
Намрег была в сети 15 минут назад
Перейти в конец истории
Выбрать файлы

