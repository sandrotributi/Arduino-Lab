 /**
 * Reference: Paolo Aliverti, Progettare con Arduino (Italian Edition)
 *
 * Ultrasonic Parking Sensor
 * 
 * We must constantly measure the distance from a possible obstacle and emit a sound that is all the 
 * more rapid and insistent the lower the measurement is. 
 * We will in fact consider three cases:
 *  1. obstacle out of range: we will not make any sound;
 *  2. obstacle too close: we will make a continuous sound;
 *  3. obstacle at intermediate distance: the sounds are emitted with a frequency inversely proportional
 *     to the distance (closer, faster and higher).
 * 
 * Distances are expressed in centimeters.
 *
 * This code is in the public domain. 
 * 
 * by Sandro Tributi <sandro.tributi@gmail.com>
 *
 * https://github.com/sandrotributi/Arduino-Lab/edit/main/HC-SR04_Parking_Sensor/HC-SR04_Parking_Sensor.ino
 */

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
