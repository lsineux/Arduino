

/*
  Repeating Web client

 This sketch connects to a a web server and makes a request
 using a Wiznet Ethernet shield. You can use the Arduino Ethernet shield, or
 the Adafruit Ethernet shield, either one will work, as long as it's got
 a Wiznet Ethernet module on board.

 This example uses DNS, by assigning the Ethernet client with a MAC address,
 IP address, and DNS address.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13

 created 19 Apr 2012
 by Tom Igoe
 modified 21 Jan 2014
 by Federico Vanzati

 http://www.arduino.cc/en/Tutorial/WebClientRepeating
 This code is in the public domain.

 */

#include <SPI.h>
#include <Ethernet.h>
#include <TextFinder.h>


// assign a MAC address for the ethernet controller.
// fill in your address here:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
// fill in an available IP address on your network here,
// for manual configuration:
//IPAddress ip(192, 168, 1, 177);

// fill in your Domain Name Server address here:
//IPAddress myDns(1, 1, 1, 1);

// initialize the library instance:
EthernetClient client;
TextFinder finder(client);

boolean firstConnection = 1;

boolean x=0;

char server[] = "infoclimat.fr";
//IPAddress server(64,131,82,241);

unsigned long lastConnectionTime = 0;             // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 20L*1000L; // delay between updates, in milliseconds
// the "L" is needed to use long type numbers

void setup() {

  Serial.begin(9600);
  
  Ethernet.begin(mac);
  delay(1000);
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
if (!firstConnection) {
    if (millis() - lastConnectionTime > postingInterval) {
      Serial.println("autre connexion");
      httpRequest();
    }
    
  } else {
    Serial.println("first connexion");
    httpRequest();
    firstConnection=0;
  }


}
void httpRequest() {

  client.stop();
  float TK;
  float TC;
  float MM;
if (client.connect(server, 80)) {
    Serial.println("connecting...");
    client.println("GET http://www.infoclimat.fr/public-api/gfs/xml?_ll=48.3467,2.45922&_auth=U0lWQQF%2FASMDLgA3VCIBKFU9DzpcKgUiVysEZwxpUi8CYlMzVDFSNwBwUC1VZwErAjUPal1rCDIEZVIqDH4FZFM5VjUBYAFiA2gAZlR7ASpVew9uXHwFIlc9BGMMf1IyAn9TMlQzUi4AbVAzVWEBKgIuD25dZgg1BG5SMAxlBWBTMFY3AWIBfANzAGdUYgFjVTMPOlxkBW5XNgQ3DDRSMQI0Uz9UNFIuAGpQO1VtATACMg9uXWUINgR4UioMGAUVUy1WcgEgATYDKgB%2FVDEBa1Uy&_c=da2ae6268777e3297b7a227782b0e270");
    client.println("Host: infoclimat.fr");
    client.println("Connection: close");
    client.println();
    lastConnectionTime = millis();

   
    
    if(finder.find("<echeance hour=\"24\" ")){
      if(finder.find("<temperature>")){
        if(finder.find("<level val=\"2m\">")){
          TK=finder.getFloat();
          Serial.print("TK : ");Serial.println(TK);
          TC=TK-273,15;
        }
      }

    }
  
    if(finder.find("<pluie interval=\"3h\">")){
      MM=finder.getFloat();     
      }

  client.stop();
  

} else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
   }
}
