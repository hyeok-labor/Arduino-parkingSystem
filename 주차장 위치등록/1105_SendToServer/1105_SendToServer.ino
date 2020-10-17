/*
    This sketch establishes a TCP connection to a "quote of the day" service.
    It sends a "hello" message, and then prints received data.
*/

#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID "koreait202"
#define STAPSK  "koreait202"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = "192.168.22.79";
const uint16_t port = 55112;

int NFC=0;
char data[8]="";
char tmp[8]="";


void setup() {
  Serial.begin(115200);
  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  
  IPAddress ip(192, 168, 22, 245);            // IP address of the server
  IPAddress gateway(192,168,22,1);           // gateway of your network
  IPAddress subnet(255,255,255,0);          // subnet mask of your network
  WiFi.config(ip, gateway, subnet);       // forces to use the fix IP+++
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  Serial.print("connecting to ");
  Serial.print(host);
  Serial.print(':');
  Serial.println(port);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    delay(5000);
    return;
  }
  
  // This will send a string to the server
  //Serial.println("sending data to server");
  
  if (client.connected()) {
    //client.print(temp);
    if(Serial.available()) {
      NFC = Serial.read();
      sprintf(data, "%d", NFC);
    }
    if(tmp !=data)
    {
      strcpy(tmp,data);
      client.print(tmp)
    }                                    // Serial.print(temp) 와 동일하게 출력됨, ==> 데이터 전송 시 client.print(temp); 로 수정
  }

  // wait for data to be available
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 10000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      delay(60000);
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  Serial.println("receiving from remote server");
  // not testing 'client.connected()' since we do not need to send data here
  while (client.available()) {
    char ch = static_cast<char>(client.read());
    Serial.print(ch);
  }

  // Close the connection
  Serial.println();
  Serial.println("closing connection");
  client.stop();

  delay(7000); // execute once every 5 minutes, don't flood remote service
}
