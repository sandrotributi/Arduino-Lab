/*
 * Tratto dal libro 'Progettare con Arduino' 
 * di Paolo Aliverti
 */

#define LED 9

unsigned long t1, dt;
int dly = 10;
int brightness;

void setup() {
  pinMode(LED, OUTPUT);
  randomSeed(analogRead(A0));
  t1 = millis();
}

void loop() {
  dt = millis() - t1;
  if (dt > dly) {
    dly = random(1, 30);
    t1 = millis();
    brightness = random(50,255);
    analogWrite(LED, brightness);
  } 
}
