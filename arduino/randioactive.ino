int time[4];
int length1, length2;
int count = 0;
boolean lastRead = false;

int bit = 0;
int clicks = 0;

int lengthcount = 0;
int randbyte[8];


byte mac[] = {};
byte server[] = { 173,203,98,29 }; 

void setup() {
  // Initializing ethernet and serial connection
  // Ethernet.begin(mac);
  Serial.begin(9600);
  
  // Waiting for the ethernet shield
  Serial.println("Booting up...");
  delay(1000);
  Serial.println("Randioactive v0.0.1");
}

void post(char string) {
  
}

void loop() {
  // Checking the CPM, every minute
  if(millis() != 0 && (millis() % 60000) == 0){
    if(clicks != 0){
      char message = "{\"type\":\"cpm\",\"data\":{\"cpm\":";
      message.concat(clicks, ",\"timestamp\":", millis(), "}}");
      post(message);
      clicks = 0;
    }
  }
  
  // Detecting the rising edge of the digital signal
  if(digitalRead(8) == HIGH) {
    if(lastRead == false) {
      
      Serial.print("C");
      Serial.println(millis());
      clicks++;
      
      lastRead = true;
      time[count] = millis();
      count++;

      if(count > 3) {
        // Compare
        length1 = time[1] - time[0];
        length2 = time[3] - time[2];
         
        // Set current bit
        if(length2 > length1) {
          bit = 1;
          Serial.println("B1");
        } else if(length1 > length2) {
          bit = 0; 
          Serial.println("B0");
        }
        
        // Write bit to a "artificial" byte and print it
        if(lengthcount < 8) {
          randbyte[lengthcount] = bit;
          lengthcount++;
        } else if(lengthcount == 8) {
            Serial.print("R");
            for(int i=0; i < 8; i++) {
              Serial.print(randbyte[i]);
            }
            Serial.println();
            lengthcount = 0;
        } else {
          Serial.print("LOL.");
        }
        count = 0; 
      }
    }
  } else { lastRead = false; }
}

