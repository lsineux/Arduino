/******* WEBSERVER *******
*  Arduino Uno+ESP8266
**************************/


// define serial connections to ESP and terminal
#include <SoftwareSerial.h>
# define pinRX 2 // RX pin for sw serial, goes to ESP TX
# define pinTX 3 // TX pin for sw serial, goes to ESP RX
SoftwareSerial espSerial(pinRX, pinTX); // link to ESP8266
HardwareSerial & dbgSerial = Serial; // serial monitor

// Set WiFi parameters
# define WIFI_SSID "mySSID"
# define WIFI_PWD "myWpaPassword"

// set pin numbers:
const int ledPin =  13;      // the number of the LED pin

// Global variable for led status:
int ledState = HIGH;             // ledState used to set the LED

// Buffer for storing characters
#define BUFFER_SIZE 128
char buffer[BUFFER_SIZE];

/***** Setup function, run only once ******/
void setup() { 
  pinMode(ledPin, OUTPUT);  // sets the led pin as digital output
  //pinMode(ESP8266_CHPD, OUTPUT);
      
  dbgSerial.begin(9600); // Serial monitor
  espSerial.begin(9600); // ESP8266

  dbgSerial.println(F("Welcome to ESP8266"));
  // connect to router
  connectWiFi(WIFI_SSID, WIFI_PWD);
      
  //set time out
  //dbgTerminal.print("AT+CIPSTO=15 : ");
  //dbgTerminal.println( GetResponse("AT+CIPSTO=15",10) );
  
   //print the ip addr
  dbgTerminal.print(F("ip address : "));
  dbgTerminal.println( GetResponse("AT+CIFSR", 10) );
  delay(200);

   
  dbgTerminal.println();
  dbgTerminal.println(F("Start Webserver"));

  digitalWrite(ledPin,ledState);  
}

void loop() {
  int ch_id, packet_len;
  char *pb;  
  espSerial.readBytesUntil('\n', buffer, BUFFER_SIZE);
  
  if(strncmp(buffer, "+IPD,", 5)==0) {
    // request: +IPD,ch,len:data
    sscanf(buffer+5, "%d,%d", &ch_id, &packet_len);
    if (packet_len > 0) {
      // read serial until packet_len character received
      // start from :
      pb = buffer+5;
      while(*pb!=':') pb++;
      pb++;
      if (strncmp(pb, "GET /led", 8) == 0) {
        dbgTerminal.print(millis());
        dbgTerminal.print(" : ");
        dbgTerminal.println(buffer);
        dbgTerminal.print( "get led from ch :" );
        dbgTerminal.println(ch_id);
 
        delay(100);
        clearSerialBuffer();
        
       if (ledState == LOW)
          ledState = HIGH;
       else
          ledState = LOW;
        digitalWrite(ledPin, ledState);
        
        homepage(ch_id);

      } else if (strncmp(pb, "GET / ", 6) == 0) {
        dbgTerminal.print(millis());
        dbgTerminal.print(" : ");
        dbgTerminal.println(buffer);
        dbgTerminal.print( "get Status from ch:" );
        dbgTerminal.println(ch_id);
        
        delay(100);
        clearSerialBuffer();

        homepage(ch_id);
      }
    }
  }
  clearBuffer();
}

void homepage(int ch_id) {
  String Header;

  Header =  "HTTP/1.1 200 OK\r\n";
  Header += "Content-Type: text/html\r\n";
  Header += "Connection: close\r\n";  
  //Header += "Refresh: 5\r\n";
  
  String Content;
  Content = "D";
  Content += String(ledState);
  
  Header += "Content-Length: ";
  Header += (int)(Content.length());
  Header += "\r\n\r\n";
  
  
  espSerial.print("AT+CIPSEND=");
  espSerial.print(ch_id);
  espSerial.print(",");
  espSerial.println(Header.length()+Content.length());
  delay(10);
  
  // for debug buffer serial error
  //while (espSerial.available() >0 )  {
  //  char c = espSerial.read();
  //  dbgTerminal.write(c);
  //  if (c == '>') {
  //      espSerial.print(Header);
  //      espSerial.print(Content);
  //  }
  //}
  
  if (espSerial.find(">")) {
      espSerial.print(Header);
      espSerial.print(Content);
      delay(10);
   }
 
//  Serial1.print("AT+CIPCLOSE=");
//  Serial1.println(ch_id);


}



// Get the data from the WiFi module and send it to the debug serial port
String GetResponse(String AT_Command, int wait){
  String tmpData;
  
  espSerial.println(AT_Command);
  delay(10);
  while (espSerial.available() >0 )  {
    char c = espSerial.read();
    tmpData += c;
    
    if ( tmpData.indexOf(AT_Command) > -1 )         
      tmpData = "";
    else
      tmpData.trim();       
          
   }
   return tmpData;
}
/*
boolean hardReset() {
  String tmpData;
  
  digitalWrite(ESP8266_CHPD,LOW);
  delay(100);
  digitalWrite(ESP8266_CHPD,HIGH);
  delay(1000);
    
  while ( espSerial.available() > 0 ) {
    char c = espSerial.read();
    tmpData +=c;
    espSerial.write(c);
    if ( tmpData.indexOf("Ready") > -1 ) {
      //Serial.println("Ready");
        clearBuffer();
        return 1;
    } 
  }
}
*/
void clearSerialBuffer(void) {
       while ( espSerial.available() > 0 ) {
         espSerial.read();
       }
}

void clearBuffer(void) {
       for (int i =0;i<BUFFER_SIZE;i++ ) {
         buffer[i]=0;
       }
}
         
/**** connectWiFi **********
* connects to Wifi Router
****************************/

boolean connectWiFi(String routerSSID,String routerPWD) 
{
  String cmd;
 
  // Set ESP8266 as station
  cmd="AT+CWMODE=1";
  dbgSerial.println( GetResponse(cmd,10) );
 
  // connect to router using SSID and Password
  cmd = "AT+CWJAP=\"";
  cmd += NetworkSSID;
  cmd += "\",\"";
  cmd += NetworkPASS;
  cmd += "\"";
  dbgSerial.println(GetResponse(cmd,10));

  //allow multiple connections
  cmd="AT+CIPMUX=1";
  dbgSerial.println( GetResponse(cmd,10) );
 
  //Start server listening on port 80
  cmd="AT+CIPSERVER=1,80";
  dbgSerial.println( GetResponse(cmd,10) );



}
