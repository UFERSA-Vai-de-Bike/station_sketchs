/*
  DHCP-based IP printer

  This sketch uses the DHCP extensions to the Ethernet library
  to get an IP address via DHCP and print the address obtained.
  using an Arduino Wiznet Ethernet shield.

  Circuit:
   Ethernet shield attached to pins 10, 11, 12, 13

  created 12 April 2011
  modified 9 Apr 2012
  by Tom Igoe
  modified 02 Sept 2015
  by Arturo Guadalupi

*/
//#include <ArduinoJson.h>
#include <SPI.h>
#include <Ethernet.h>
#define SERVER_PORT 3000
#define SERVER_1 192 // PRIMEIRA PARTE DO IP
#define SERVER_2 168 // SEGUNDA PARTE
#define SERVER_3 0 // TERCEIRA PARTE
#define SERVER_4 2 // QUARTA PARTE
//StaticJsonBuffer<200> jsonBuffer;

boolean doRequest = true;
// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {
  0x00, 0xAB, 0xBC, 0xCD, 0xDF, 0x03
};

// Inicialize a biblioteca
// com o endereço ip e porta do servidor
// that you want to connect to (port 80 is default for HTTP):
IPAddress server(SERVER_1,SERVER_2,SERVER_3,SERVER_4);
EthernetClient client;
String serverIp = String(SERVER_1 + '.' + SERVER_2 + '.' + SERVER_3 + '.' + SERVER_4);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // this check is only needed on the Leonardo:
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // iniciando a conexão ethernet:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Falha na configuração usando DHCP");
    // no point in carrying on, so do nothing forevermore:
    for (;;)
      ;
  }
  // de um segundo para o shield ethernet inicializar
  delay(1000);
  // imprima seu endereço ip
  printIPAddress();
}

void loop() {
  if (doRequest) {
    Serial.println("Conectando...");
    // if you get a connection, report back via serial:
    if (client.connect(server, 3000)) {
      Serial.println("Conectado!");
      // Make a HTTP request:
      doGetAPI("/api");
      doRequest = false;
    } else {
      // if you didn't get a connection to the server:
      Serial.println("connection failed");
    }
  }
  if (!doRequest) {
    // if there are incoming bytes available
    // from the server, read them and print them:
    while(client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.print(c);
        //JsonObject& root = jsonBuffer.parse(client);
        //root.printTo(Serial);
      } 
    }

    // if the server's disconnected, stop the client:
    if (!client.connected()) {
      Serial.println();
      Serial.println("disconnecting.");
      client.stop();
  
      // do nothing forevermore:
      while (true);
    }
  }
}

bool openConnection(){
  Serial.println("Abrindo conexão...");
  lcd.print();
    // if you get a connection, report back via serial:
  if (client.connect(server, 30000) {
    return true;
  } else {
    return false;
  }
}

void printIPAddress()
{
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(".");
  }

  Serial.println();
}

void doGetAPI(String path) {
    client.println(String("GET " +path+ " HTTP/1.1"));
    client.println(String("Host: " + serverIp));
    client.println("Cache-Control: no-cache");
    client.println("Connection: close");
//    client.println("Connection: keep-alive");
    client.println();
}
void doPostAPI(String path) {
    client.println(String("POST "+path+" HTTP/1.1"));
    client.println(String("Host: " + serverIp));
    client.println("Cache-Control: no-cache");
    client.println("Connection: close");
//    client.println("Connection: keep-alive");
    client.println();
}

