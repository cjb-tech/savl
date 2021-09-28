#define LOOP_DELAY 500

#define ZERO 2
#define X32 3
#define DI 4
#define TOP_L 5
#define TOP_R 6
#define SUB_AND_MON_L 7
#define SUB_AND_MON_R 8
#define USELESS_1 9
#define USELESS_2 10

#define ZERO_ANALOG 15
#define X32_ANALOG 16
#define DI_ANALOG 17
#define TOP_L_ANALOG 18
#define TOP_R_ANALOG 19
#define SUB_AND_MON_L_ANALOG 20
#define SUB_AND_MON_R_ANALOG 21
#define USELESS_1_ANALOG 0
#define USELESS_2_ANALOG -1

#define SWITCHER 13

#define SIZE 9


bool is_switcher_on() {
  return digitalRead(SWITCHER) == HIGH;
}

struct Pin {
  int relay_pin;
  bool repay_pin_inverted;
  int led_pin;

  int delay_on;
  int delay_off;

  void setup() {
    pinMode(relay_pin, OUTPUT);
    digitalWrite(relay_pin, repay_pin_inverted);

    pinMode(led_pin, OUTPUT);
    digitalWrite(led_pin, 0);
  }

  bool is_enabled() {
    return digitalRead(relay_pin) != repay_pin_inverted;
  }

  void enable() {
    if (is_enabled()) {
      return;
    }

    delay(delay_on / 5);
    digitalWrite(relay_pin, !repay_pin_inverted);
    digitalWrite(led_pin, 255);
  }

  void disable() {
    if (!is_enabled()) {
      return;
    }

    delay(delay_off / 5);
    digitalWrite(relay_pin, repay_pin_inverted);
    digitalWrite(led_pin, 0);
  }
};

Pin pins[SIZE] = {
  //{relay_pin_num, inverted, led_pin_num,      delay_on, delay_off }
  {ZERO,            false,    ZERO_ANALOG,          2000, 2000},
  {X32,             true,     X32_ANALOG,           2000, 2000},
  {DI,              true,     DI_ANALOG,            2000, 2000},
  {TOP_L,           true,     TOP_L_ANALOG,         2000, 2000},
  {TOP_R,           true,     TOP_R_ANALOG,         2000, 2000},
  {SUB_AND_MON_L,   true,     SUB_AND_MON_L_ANALOG, 2000, 2000},
  {SUB_AND_MON_R,   true,     SUB_AND_MON_R_ANALOG, 2000, 2000},
  {USELESS_1,       true,     USELESS_1_ANALOG,     2000, 2000},
  {USELESS_2,       true,     USELESS_2_ANALOG,     2000, 2000}
};

void enable() {
  for (int i = 0; i < SIZE; i++) {
    if (!is_switcher_on()) {
      return;
    }
    Pin pin = pins[i];
    pin.enable();
  }
}

void disable() {
  for (int i = SIZE - 1; i >= 0; i--) {
    if (is_switcher_on()) {
      return;
    }
    Pin pin = pins[i];
    pin.disable();
  }
}

void setup() {
  pinMode(SWITCHER, INPUT);

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
