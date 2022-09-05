#define trigPin 6
#define echoPin 7
#define ledPin 11
#define buzzerPin 12

#define MAXDIST 100
#define MINDIST 25

long cm;
unsigned long t1, dt;
int tblink = 100;
int ledStatus = LOW;
int freq = 220;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  t1 = millis();
  Serial.begin(9600);
}

void loop() {
  cm = getDistance(trigPin, echoPin);
  
  Serial.print("Distance = ");
  Serial.print(cm);
  Serial.println(" cm");
  
  if (cm < MINDIST) {
    // too close - led always on, continuous sound
    digitalWrite(ledPin, HIGH);
    tone(buzzerPin, 880);
  } else if (cm > MAXDIST) {
    // too far - led off, no sound
    digitalWrite(ledPin, LOW);
    noTone(buzzerPin);
  } else {
    // in intermediate cases we will make the
    // led flash and simultaneously emit a "beep"
    // in proportion to the distance detected
    tblink = map(cm, MINDIST, MAXDIST, 100, 500);
    freq = map(cm, MINDIST, MAXDIST, 880, 220);
    
    dt = millis() - t1;
    if (dt > tblink) {
      ledStatus = !ledStatus;
      digitalWrite(ledPin, ledStatus);
      t1 = millis();
      if (ledStatus) {
        tone(buzzerPin, freq, 100);
      } else {
        // noTone(buzzerPin);
      }
    }
  }
  
  delay(100);
}

long getDistance(byte trigger, byte echo) {
  long duration;
  long distance;
  
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  
  duration = pulseIn(echo, HIGH);
  distance = duration / 58.0f;
  return distance;
}
