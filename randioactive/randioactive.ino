#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

int time[4];
int length1, length2;
int count = 0;
boolean lastRead = false;

int bit = 0;
int clicks = 0;

int lengthcount = 0;
int randbyte[8];

byte mac[]    = { 0xDE, 0xED, 0xBA, 0xFE, 0xEF, 0xED };
byte server[] = { 10, 23, 42, 210 }; 
byte ip[]     = { 10, 23, 42, 222 };

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}

EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);

void setup() {
  // Initializing ethernet and serial connection
  Ethernet.begin(mac, ip);
  client.connect("geiger");
  Serial.begin(9600);
  
  // Waiting for the ethernet shield
  delay(1000);
}

void loop() {
  // Checking the CPM, every minute
  if(millis() != 0 && (millis() % 60000) == 0){
    if(clicks != 0){
      char* message = "000000";
      sprintf(message, "%d\0", clicks);
      client.publish("hasi/sensors/indoor/radiation/cpm", message);
      clicks = 0;
    }
  }
  
  // Detecting the rising edge of the digital signal
  if(digitalRead(8) == HIGH) {
    if(lastRead == false) {
      
      client.publish("hasi/events/random/click", "\0");
      clicks++;
      
      lastRead = true;
      time[count] = millis();
      count++;

      if(count > 3) {
        // Compare
        length1 = time[1] - time[0];
        length2 = time[3] - time[2];
         
        // Set current bit and publish it
        if(length2 > length1) {
          bit = 1;
          client.publish("hasi/events/random/bitstream", "1\0");
        } else if(length1 > length2) {
          bit = 0; 
          client.publish("hasi/events/random/bitstream", "0\0");
        }
        
        // Write bit to a "artificial" byte and print it
        if(lengthcount < 8) {
          randbyte[lengthcount] = bit;
          lengthcount++;
        } else if(lengthcount == 8) {
          char* randstring = "000000000";
          sprintf(randstring, "%d%d%d%d%d%d%d%d\0", randbyte[0], randbyte[1], randbyte[2], randbyte[3], randbyte[4], randbyte[5], randbyte[6], randbyte[7]); 
          client.publish("hasi/events/random/byte", randstring);
          lengthcount = 0;
        } else {
          client.publish("hasi/events/random/byte", "ERROR");
        }
        count = 0; 
      }
    }
  } else { lastRead = false; }
  
  client.loop();
}

