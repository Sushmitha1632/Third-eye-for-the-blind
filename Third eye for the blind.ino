#define BLYNK_PRINT Serial
#define BLYNK_MAX_SENDBYTES 256
#include <SoftwareSerial.h>                            //to connect gsm 
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <TinyGPSPlus.h>                           //gps library neo 6m
#include <HCSR04.h>                               //ultrsonic sensor library
//#include<SimpleTimer.h>

SoftwareSerial Gsm(D2, D3); // RX, TX
HCSR04 hc(D6, D7);           //set d6 as trig d7 echo

TinyGPSPlus gps;
BlynkTimer timer;
WidgetMap myMap(V1);
char auth[] = "NVuWZgkRvsujIZMdDmPTO1kSPr9BYEy9";     //

char ssid[] = "redme ns";
char pass[] = "sushmacu14";
#define motor D5
#define pb D4
#define speaker D8
int distance = 0;
float latitude;
float longitude;
void myTimerEvent()
{
  while (Serial.available() > 0)          //gps is connected to inbuild serial port
    if (gps.encode(Serial.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while (true);
  }
  int index = 1;
  myMap.location(index, latitude, longitude, "value");
  /*if (digitalRead(pb) == LOW)
  {
    Serial.println("sending email");
    BLYNK_LOG("Button is pressed.");
    Blynk.email("sushmacu2001@gmail.com", "Subject:3rd Eye for blind", "Hey, iam in trouble. Check blynk for my current location");
    delay(1000);
    Serial.println("done");
  }
*/
}

void displayInfo()
{
  Serial.print(F("Location: "));
  delay(100);
  if (gps.location.isValid())
  {
    latitude = (gps.location.lat(), 6);
    Serial.print(gps.location.lat(), 6);
    Serial.print("    ");

    longitude = (gps.location.lng(), 6);
    Serial.println(gps.location.lng(), 6);

  }
  else
  {
    Serial.println(F("INVALID"));
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Gsm.begin(9600);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);
  pinMode(motor, OUTPUT);
  pinMode(pb, INPUT_PULLUP);
  pinMode(speaker, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  timer.run();
  obstacle();
  if (digitalRead(pb) == LOW)
  {
    SendMessage();
  }

}

void obstacle()
{
  distance = hc.dist();
  delay(60);
  int  mapped_distance = (distance , 255, 0);
  analogWrite(mapped_distance, D5);

  if (distance < 20)
  {
    int randomnumber;
    srand(time(NULL));
    randomnumber = rand() % 10;
    if (randomnumber <= 5)
    {
      for (int i = 0; i < 10; i++)
      {
        digitalWrite(speaker, HIGH);
        delay(100);
        digitalWrite(speaker, LOW);
        delay(50);
      }

    }
    else
    {
      for (int i = 0; i < 10; i++)
      {
        digitalWrite(speaker, HIGH);
        delay(50);
        digitalWrite(speaker, LOW);
        delay(50);
      }

    }
  }
  else
  {
    digitalWrite(motor, LOW);
    digitalWrite(speaker, LOW);
  }
}


void SendMessage()
{
  Serial.println("sending message");
  Blynk.email("sushmacu2001@gmail.com", "3rd Eye for blind", "Hey, iam in trouble.please check  my current location");
  delay(1000);
  Gsm.print("AT+CMGF=1\r");
  delay(400);
  Gsm.print("AT+CMGS=\"+917337822753\"\r"); // Replace x with mobile number

//Gsm.print("+919895351385");               //change phone number here
  //Gsm.println("\"");
  delay(300);
  Gsm.print("Hey, iam in trouble. find me in this location:  ");
  Gsm.print("http://maps.google.com/maps?q=");
  Gsm.print(gps.location.lat(), 6);
  Gsm.print(",");
  Gsm.println(gps.location.lng(), 6);
  delay(200);
  Gsm.println((char)26); // End AT command with a ^Z, ASCII code 26
  delay(1000);
  Gsm.println();
  delay(200);
  Serial.print("send");
  //Gsm.println("ATD+917337822753;");
}
