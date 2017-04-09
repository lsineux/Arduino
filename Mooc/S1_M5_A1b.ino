#include <SPI.h>
#include <Ethernet.h>
#include <TextFinder.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char server[] = "infoclimat.fr";
IPAddress ip(192, 168, 1, 177);
EthernetClient client;
TextFinder finder(client);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("hello");

  // start the Ethernet connection:
if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
 
  delay(1000);
  Serial.println("connecting...");
  if (client.connect(server, 80)) {
    Serial.println("connected");
    client.println("GET http://www.infoclimat.fr/public-api/gfs/xml?_ll=48.85341,2.3488&_auth=U0kCFQJ8VnQCLwYxBXMAKQRsAzYPeVB3B3sKaQFkVSgAa1c2VjZSNAVrUy5VegM1VntTMAswBDRTOAtzXS9QMVM5Am4CaVYxAm0GYwUqACsEKgNiDy9QdwdsCmsBclU3AGpXO1YrUjcFbFMvVWcDNlZiUywLKwQ9UzQLZF02UDJTMwJkAmJWNQJsBnsFKgAxBD8Dag80UGEHYApoAWtVZwBjV2VWNVIwBWhTL1VgAz5WZ1M6CzEEPlMzC21dL1AsU0kCFQJ8VnQCLwYxBXMAKQRiAz0PZA%3D%3D&_c=d1bfb7a6d663a64de4e2a86e2357e929 HTTP/1.1");
    client.println("Host: infoclimat.fr");
    client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();

    float TC;
    

if(finder.find("<echeance hour=\"24\"")){
  if(finder.find("<temperature>")){
    if(finder.find("<level val=\"2m\">")){
      float TK=finder.getFloat(); /*TK=finder_temp.getFloat(' ');*/
      TC = TK - 273.15;
      Serial.print("TK : ");Serial.println(TK);
      Serial.print("TC : ");Serial.println(TC);
    }
}
}


    
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}

void loop() {
  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();

    // do nothing forevermore:
    while (true);
  }
}


