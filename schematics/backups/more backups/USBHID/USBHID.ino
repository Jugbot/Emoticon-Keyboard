
byte statLED = 11; 
byte shiftMod = 22;
byte ctrlMod = 23;

long readingTime; //Controls how often we send our trimpot value to the computer
unsigned int counter = 0; //Used to show packet count sent to computer
unsigned int waitTime = 50;

byte outgoingBuffer[64];
byte lastKey;
      
void setup() 
{
  //Blink to show we're alive
  pinMode(statLED, OUTPUT);
  pinMode(shiftMod, INPUT);
  pinMode(ctrlMod, INPUT);
  for (int x = 0; x <= 6; x++) 
    pinMode(x, OUTPUT);
  for (int y = 15; y <= 21; y++)
    pinMode(y, INPUT);
    
  for(int i = 0 ; i < 5 ; i++)
  {
    digitalWrite(statLED, HIGH);
    delay(25);
    digitalWrite(statLED, HIGH);
    delay(25);
  }
  digitalWrite(statLED, LOW); //Turn off LED

  //Fill the buffer with static numbers
  for(int x = 0 ; x < 64 ; x++)
    outgoingBuffer[x] = 0; 
  
  readingTime = millis();
}

void loop() 
{
  //about 33Hz
  if (millis() - readingTime > waitTime) {
    readingTime += waitTime;

    //Fill the tail end with a counter
    outgoingBuffer[62] = counter >> 8; //MSB
    outgoingBuffer[63] = counter & 0xFF; //LSB
    counter++;

    for (int x = 0; x <= 6; x++) {//pins 0-6
      digitalWrite(x, HIGH);
      delay(50);
      for (int y = 15; y <= 21; y++) {//pins 15-21
        if (digitalRead(y) == HIGH) { //|| (y==20 && x == 1)) {
          outgoingBuffer[0] = (x * 7) + (y-15)+1;
          if (digitalRead(shiftMod) == HIGH)
            outgoingBuffer[0] += 49;
          else if (digitalRead(ctrlMod) == HIGH)
            outgoingBuffer[0] += 98;
          if (lastKey != outgoingBuffer[0])
            RawHID.send(outgoingBuffer, 100);
          lastKey = outgoingBuffer[0];
          outgoingBuffer[0] = 0;
          digitalWrite(x, LOW);
          return;
        }
      }
      digitalWrite(x, LOW);
    }
    lastKey = 0;
    
  }
}

