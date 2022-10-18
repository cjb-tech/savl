#define LOOP_DELAY 500
#define BLINK_INTERVAL 100
#define SIZE 9

#define ZERO 2
#define X32 3
#define DI 4
#define TOP_L 5
#define TOP_R 6
#define SUB_AND_MON_L 7
#define SUB_AND_MON_R 8
#define STUFF_1 9
#define STUFF_2 10

#define LED 12
#define SWITCHER 13


bool is_switcher_on() {
  return digitalRead(SWITCHER) == HIGH;
}

void set_led(bool enabled) {
  digitalWrite(LED, enabled ? HIGH : LOW);
}

void delay_with_blink(int duration) {
  for (int i = 0; i < duration / BLINK_INTERVAL; i++) {
    delay(BLINK_INTERVAL);
    digitalWrite(LED, digitalRead(LED) == HIGH ? LOW : HIGH);
  }
}

struct Pin {
  int relay_pin;
  bool relay_pin_inverted;

  int delay_before_on;
  int delay_before_off;

  void setup() {
    pinMode(relay_pin, OUTPUT);
    digitalWrite(relay_pin, relay_pin_inverted);
  }

  bool is_enabled() {
    return digitalRead(relay_pin) != relay_pin_inverted;
  }

  void enable() {
    if (is_enabled()) {
      return;
    }
    delay_with_blink(delay_before_on);
    digitalWrite(relay_pin, !relay_pin_inverted);
  }

  void disable() {
    if (!is_enabled()) {
      return;
    }
    delay_with_blink(delay_before_off);
    digitalWrite(relay_pin, relay_pin_inverted);
  }
};

Pin pins[SIZE] = {
  //{relay_pin_num, inverted, delay_before_on, delay_before_off }
  {ZERO,            true ,    0,               500},
  {X32,             true,     500,             500},
  {DI,              true,     5000,            2000}, // TODO: Засікти скільки врубаєтся пульт
  {TOP_L,           true,     2000,            500},
  {TOP_R,           true,     500,             2000},
  {SUB_AND_MON_L,   true,     5000,            500},
  {SUB_AND_MON_R,   true,     500,             2000},
  {STUFF_1,         true,     2000,            500},
  {STUFF_2,         true,     2000,            0},
};

void enable() {
  for (int i = 0; i < SIZE; i++) {
    if (!is_switcher_on()) {
      return;
    }
    Pin pin = pins[i];
    pin.enable();
  }
  set_led(true);
}

void disable() {
  for (int i = SIZE - 1; i >= 0; i--) {
    if (is_switcher_on()) {
      return;
    }
    Pin pin = pins[i];
    pin.disable();
  }
  set_led(false);
}

void setup() {
  pinMode(SWITCHER, INPUT);
  pinMode(LED, OUTPUT);
  for (int i = 0; i < SIZE; i++) {
    Pin pin = pins[i];
    pin.setup();
  }

  Serial.begin(9600);
}

void loop() {
  delay(LOOP_DELAY);
  if (is_switcher_on()) {
    enable();
  } else {
    disable();
  }
}
