#include "ESPAsyncWebServer.h"
#include <ESP8266WiFi.h>
#include <Arduino.h>
#include <IRremote.hpp>
//Sämtliche Nötigen Bibliotheken. Die ersten beiden fürs WiFi, das letzte fürs Infrarot.
const char* ssid = "ESP8266-Access-Point";
const char* password = "123456789";
// Die Daten für das WiFi
const int IR_RECEIVE_PIN = 14;
const int Arrsize = 18;
int x = 0;
char characters[] = {'A', 'B', 'C', 'U', 'L', 'R', 'D', 'x', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
int commands[Arrsize] = {69, 71, 9, 70, 68, 67, 21, 64, 7, 22, 25, 13, 12, 24, 94, 8, 28, 90};
String convchar;
//Die Arrays sind geordnet die Möglichen IR-Signale und die Dazugehörige Entschlüsselung
//Geordnet bedeutet, an erster Stelle im einen Array ist A und im anderen Array auch and der ersten Stelle ist das dazugehörige Signal, an zweiter Stelle B etc.
String getInput() {
  return convchar;
}
// Die Funktion gibt einfach nur das Entschlüsselte IR-Signal zurück

int findIndex(int val, int search[]){
  x = 0;
  for (int i=0; i<Arrsize; i++){
    if (val == search[i]){
      x = i;
      break;
      }
    }
    return x;
}
//Die Funktion sucht nach dem "Index", der Stelle an der der etwas steht, eines Wertes in einem Array

AsyncWebServer server(80);
//Initialisiert einen WebServer auf Port 80 des ESP8266
void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial.println();

  Serial.print("Setting Access Point...");
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("IP Address: ");
  Serial.println(IP);
  //Startet das Wifi des ESP
  server.on("/input", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getInput().c_str());
  });
  // unter der Subdomäne /input wird das ergebniss von getInput(), alias das Entschlüsselte IR-Signal, ausgegeben
  server.begin();

  

  IrReceiver.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK);
    Serial.print(F("Ready to receive IR signals of protocols: "));
    printActiveIRProtocols(&Serial);
  //Initialisiert den Infrarotsensor
}

void loop() {
  // put your main code here, to run repeatedly:
    if (IrReceiver.decode()) {
      //Wenn ein Signal empfangen wurde
      convchar = String(characters[findIndex(IrReceiver.decodedIRData.command, commands)]);
      /*eine etwas komplexerere Zeile, brechen wir sie runter:
       * zunächst suchen wir den Index des erhaltenen Signals in dem commands-array, das mit den unentschlüsselten Signalen.
       * Dann nehmen wir den Index aus der vorherigen Zeile und geben das Eelement aus, dass im characters-array, dem mit den entschlüsselten Daten, an der Stelle ist
       * Zuletzt wandeln wir dieses Element in einen String um, da es im Array als "Char" gespeichert ist.
       */

       
       Serial.println(convchar);
       IrReceiver.resume();
    }
}
