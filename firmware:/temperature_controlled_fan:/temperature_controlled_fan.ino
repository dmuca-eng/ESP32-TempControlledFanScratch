#define DHTPIN 4

#define PWMPIN 14
#define FANCNTRL1 26
#define FANCNTRL2 27

#define LEDPIN 25


void setup() {
  Serial.begin(115200);

  pinMode(LEDPIN, OUTPUT);

  //Dictates fan pin direction
  pinMode(FANCNTRL1, OUTPUT);
  pinMode(FANCNTRL2, OUTPUT);
  digitalWrite(FANCNTRL1 , HIGH);
  digitalWrite(FANCNTRL2 , LOW);

  //Prepares PWMPIN to control the fan
  ledcAttach(PWMPIN, 5000, 8);
  ledcWrite(PWMPIN, 0);

  delay(1000);
}

bool readDHT( uint8_t data[5]) {
  pinMode(DHTPIN, OUTPUT);
  digitalWrite(DHTPIN, LOW);
  delay(20);
  pinMode(DHTPIN, INPUT);

  unsigned long start = micros();
  while ( digitalRead(DHTPIN) == HIGH ) {
    if ( micros() - start >= 1000 ) {
      return false;
    }
  }

  start = micros();
  while ( digitalRead(DHTPIN) == LOW ) {
    if ( micros() - start >= 1000 ) {
      return false;
    }
  }

  start = micros();
  while ( digitalRead(DHTPIN) == HIGH ) {
    if ( micros() - start >= 1000 ) {
      return false;
    }
  }

  //TIGHT LOOP!!!
  for ( int i = 0; i < 5; i++ ){
    uint8_t byte = 0;

    for ( int j = 0; j < 8; j++ ){
      bool bit = 0;

      unsigned long start = micros();
      while( digitalRead(DHTPIN) == LOW){
        if ( micros() - start >= 1000 ) {
          return false;
        }
      }

      start = micros();
      while( digitalRead(DHTPIN) == HIGH ) {
        if ( micros() - start >= 1000 ) {
          return false;
        }
      }

      unsigned long end = micros();
      unsigned long duration = end - start;

        if ( duration > 60 ){
          bit = 1;
        } else {
          bit = 0;
        }

        byte = (byte << 1) | bit;
    }

    data[i] = byte;
  }


  return true;
}

void loop() {
  uint8_t data[5];
  bool wirecheck = readDHT(data);

  if ( wirecheck ) {
    Serial.println("PACKET RECEIVED"); 
  } else {
    Serial.println("DHT11 TIMEOUT \nRETRYING IN 10 SECONDS");
  }

  if ( wirecheck && data[4] == static_cast<uint8_t>(data[0] + data[1] + data[2] + data[3]) ){

    Serial.println("Temperature: " + String(data[2]) + "." + String(data[3]) + " C");
    Serial.println("Humidity: " + String(data[0]) + "." + String(data[1]) + "%");
    
    if ( data[2] >= 26 ) {
      ledcWrite(PWMPIN, 255);
      digitalWrite(LEDPIN, HIGH);
      Serial.println("FAN ON \n");
    } else { 
      ledcWrite(PWMPIN, 0);
      digitalWrite(LEDPIN, LOW);
      Serial.println("FAN OFF \n");
      
    }

  } else if ( wirecheck ){
    Serial.println("PACKET CORRUPT \nRETRYING IN 10 SECONDS");
    digitalWrite(LEDPIN, LOW);
    ledcWrite(PWMPIN, 0);
  } else {
    ledcWrite(PWMPIN, 0);
    digitalWrite(LEDPIN, LOW);
  }


  delay(10000);
}