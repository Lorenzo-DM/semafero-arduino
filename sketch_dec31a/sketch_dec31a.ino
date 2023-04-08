#define RED_LED 3
#define YELLOW_LED 4
#define GREEN_LED 5

void setup() {
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
}

void loop() {
  turnOn(RED_LED, 730000);      // 12 minutes and 10seconds
  turnOn(GREEN_LED, 50000);     // 50 seconds
  turnOn(YELLOW_LED, 10000);    // 10 seconds
}

void turnOn(int color, long seconds) {
  digitalWrite(color, HIGH);
  delay(seconds);
  digitalWrite(color, LOW);
}
