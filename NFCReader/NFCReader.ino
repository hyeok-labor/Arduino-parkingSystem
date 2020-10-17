// UNO board 
/******************************/
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#ifndef STASSID
#define STASSID "koreait202"
#define STAPSK  "koreait202"
#endif

// If using the breakout with SPI, define the pins for SPI communication.
#define PN532_SCK  (14)//(13)
#define PN532_MOSI (13)//(11)
#define PN532_SS   (5)//(9)
#define PN532_MISO (12)//(12)

const char* host = "192.168.22.79";
const uint16_t port  = 55120;

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1]; //buffer to hold incoming packet,
char  sendBuffer[] = "hello jaeho";       // a string to send back
char  local[] = "A1"; 
char  nothing[] = "n"; 

WiFiUDP Udp;

// If using the breakout or shield with I2C, define just the pins connected
// to the IRQ and reset lines.  Use the values below (2, 3) for the shield!
//#define PN532_IRQ   (2)
//#define PN532_RESET (3)  // Not connected by default on the NFC Shield

// Uncomment just _one_ line below depending on how your breakout or shield
// is connected to the Arduino:

// Use this line for a breakout with a software SPI connection (recommended):
Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

// Use this line for a breakout with a hardware SPI connection.  Note that
// the PN532 SCK, MOSI, and MISO pins need to be connected to the Arduino's
// hardware SPI SCK, MOSI, and MISO pins.  On an Arduino Uno these are
// SCK = 13, MOSI = 11, MISO = 12.  The SS line can be any digital IO pin.
//Adafruit_PN532 nfc(PN532_SS);

// Or use this line for a breakout or shield with an I2C connection:
//Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
// also change #define in Adafruit_PN532.cpp library file
   #define Serial SerialUSB
#endif

void setup(void) {
  #ifndef ESP8266
    while (!Serial); // for Leonardo/Micro/Zero
  #endif
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  IPAddress ip(192, 168, 22, 248);            // IP address of the server
  IPAddress gateway(192,168,22,1);           // gateway of your network
  IPAddress subnet(255,255,255,0);          // subnet mask of your network
  WiFi.config(ip, gateway, subnet);       // forces to use the fix IP+++
  
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  //Serial.println("Hello!");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    //Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  // Got ok data, print it out!
  //Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  //Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  //Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  // configure board to read RFID tags
  nfc.SAMConfig();
  
  //Serial.println("Waiting for an ISO14443A Card ...");
}


void loop(void) {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
    
  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  //String custID[4] = {};
  //byte leng=0;
  if (success) {
    // Display some basic information about the card
    //Serial.println("Found an ISO14443A card");
    //Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    //Serial.print("  UID Value: ");
    //nfc.PrintHex(uid, uidLength);
    Udp.beginPacket(host, port);
    Serial.print(uid[1]);
    Serial.print(":");
    if(uid[1] == 2){
      Serial.println(local);
      Udp.write(local);
    }
    else {
    Udp.write(nothing);
    }
    Udp.endPacket();
    delay(1000);
    
    /*for(int i=0; i<4; i++){
      if(uid[i]<10){
        s.print('0');
      }
      //custID[i] = String(uid[i],HEX);
      s.print(String(uid[i],HEX)); */
    }
    else {
      Udp.write(nothing);
      Udp.endPacket();
    }
    delay(1000);
  }
