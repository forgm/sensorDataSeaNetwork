#include <DigitalIO.h>
#include <SPI.h>
#include <Ethernet.h>
#include <RF24Network.h>
#include <RF24.h>

byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};
byte server[] = {192, 168, 8, 100};

struct payload_t {                 // Structure of our payload
  float field;
};


RF24 radio(7, 8);
RF24Network network(radio);
EthernetClient client;
RF24NetworkHeader header;
payload_t payload;
String buff;
String data;

const uint16_t this_node = 00;



void setup() {
  Serial.begin(57600);
  while (!Ethernet.begin(mac)) {
    Serial.println("Ethernet connection failed!");
  }
  Serial.println("Ethernet connected!");
  radio.begin();
  network.begin(90, this_node);  
 
}

void loop() {
  network.update();
    while(network.available()) {
      network.read(header,&payload,sizeof(payload));
      if(client.connect("arrogant-beaver-42005.herokuapp.com",80)){
        data.concat("field");
        data.concat(header.from_node);
        data.concat("=");
        data.concat(payload.field);
        client.println("POST /channel/apicode HTTP/1.1");
        client.println("Host: host.com:port");  
        client.println("User-Agent: Arduino/1.0"); 
        client.println("Connection: close");
        client.println("Content-Type: application/x-www-form-urlencoded");  
        client.print("Content-Length: "); 
        client.println(data.length());
        client.println();
        client.println(data);
        Serial.println(data);
        data = "";
        client.stop();  
      }     
    }
}
