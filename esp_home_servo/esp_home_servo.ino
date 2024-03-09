#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti WiFiMulti;
#include <Servo.h>

//importiert einfach nur alle Bibliotheken, die benötigt werden

Servo servo;
//initialisiert den Servo-motor

const char* ssid = "ESP8266-Access-Point";
const char* password = "123456789";
//Die Addresse und das passwort des WiFis

const char* serverInput = "http://192.168.4.1/input";
//Unter dieser Addresse findet sich der Inpu des anderen esp's

String input;
int counter = 0;
bool up = true;
//eine Reihe variabeln, die dafür zuständig sind, das nur in den richtigen fällen etwas passiert

unsigned long previousMillis = 0;
const long interval = 5000;

int degree = 90;
//die Variable für den Servo


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
//beginnt den Seriellen Monitor
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //verbindet sich zum WiFi
  Serial.println("");
  Serial.println("Connected to WiFi");

  servo.attach(14);
  //verbindet sich zum servo
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();
  if (currentMillis-previousMillis >= interval) {
    if ((WiFiMulti.run() == WL_CONNECTED)) {
      input = httpGETRequest(serverInput);
      Serial.println(input);
      previousMillis == currentMillis;
    }
    else {
      Serial.println("WiFi Disconnected");  
    }
  }
  //wenn eine bestimmte Zeit erreicht ist und mit WLAN verbunden ist, rufe die Daten vom Input ab.
  if (input == "A" && counter == 0) {
    if (up == true){
      degree = 90;
      up = false;
    } else {
      degree = 0;
      up = true;
    }//wenn der Schalter oben ist (up == true), drücke ihn runter, wenn nicht, ziehe ihn hoch
    //(das ziehen soll mit doppelseitigem Klebeband funktionieren, muss noch getestet werden 
    counter = counter + 1;
    // der counter dient dazu, die aktion nur auszuführen, wenn das erste mal 'A' empfangen wird, damit nicht der Knopf dauerhaft umgelegt wird. 
  } else if (input == "A" && counter >= 1) {
    counter = counter + 1;
  } else if (input != "A") {
    counter = 0;
  } // wenn etwas anderes als 'A' empfangen wird, wird der counter zurückgesetzt und das Programm wird das nächste mal, wenn 'A' empfangen wird, die Aktion wieder ausführen.
  Serial.print("the counter is ");
  Serial.println(counter);
  Serial.print("Servo is at ");
  Serial.println(degree);
  servo.write(degree);
 
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "--"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
    //wenn ein response code eingekommen ist, der aussagt, das ETWAS passiert ist, gib den code aus und lade den Text unter der Adresse
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
    //wenn nicht, zeige dies an
  }
  // Free resources
  http.end();

  return payload;
  //gib den Text, der abgerufen wurde, zurück
}
//Die funktion ist dafür zuständig, Daten von einer Website abzurufen.
