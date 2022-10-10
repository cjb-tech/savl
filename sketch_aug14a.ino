#include <TimedAction.h>

#define LOOP_DELAY 500

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

#define SIZE 9

char led_value = 0;

bool is_switcher_on() {
  return digitalRead(SWITCHER) == HIGH;
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
    delay(delay_before_on);
    digitalWrite(relay_pin, !relay_pin_inverted);
  }

  void disable() {
    if (!is_enabled()) {
      return;
    }
    delay(delay_before_off);
    digitalWrite(relay_pin, relay_pin_inverted);
  }

  static void disable_static(Pin pin) {
    pin.disable();
  }
  static void enable_static(Pin pin) {
    pin.enable();
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

void toggle();
void blink();

TimedAction toggleThread = TimedAction(1000,toggle);
TimedAction blinkThread = TimedAction(100,blink);

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
  pinMode(LED, OUTPUT);
  for (int i = 0; i < SIZE; i++) {
    Pin pin = pins[i];
    pin.setup();
  }

  Serial.begin(9600);
}

void loop() {
  bool is_on = is_switcher_on();
  Serial.println(is_on ? "HIGH" : "LOW");
  toggleThread.check();
  blinkThread.check();
}

bool check_all_same() {
  for(int i = 0;i<SIZE-1;i++){
    Pin first_pin = pins[i];
    Pin second_pin = pins[i+1];
    if(first_pin.is_enabled() != second_pin.is_enabled()){
      return false;
    }
  }
  return true;
}

void blink(){
  led_value = !led_value;
  bool all_same = check_all_same();
  if(all_same) {
    digitalWrite(LED, !pins[0].is_enabled());
  }else {
    digitalWrite(LED, led_value);
  }
}

void toggle(){
  if (is_switcher_on()) {
    enable();
  } else {
    disable();
  }
}
