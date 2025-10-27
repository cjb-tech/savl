#define LOOP_DELAY 500
#define BLINK_INTERVAL 100
#define SIZE 9

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
#define UPS_STATUS A0
#define UPS_BUTTON A1


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

class Pin {
protected:
  int relay_pin;
  bool relay_pin_inverted;
  int delay_before_on;
  int delay_before_off;
  
public:
  Pin(
    int relay_pin = 0,
    bool relay_pin_inverted = false,
    int delay_before_on = 0,
    int delay_before_off = 0
  ) : relay_pin(relay_pin), 
      relay_pin_inverted(relay_pin_inverted), 
      delay_before_on(delay_before_on), 
      delay_before_off(delay_before_off) {
  }

  virtual void setup() {
    pinMode(relay_pin, OUTPUT);
    digitalWrite(relay_pin, relay_pin_inverted);
  }

  virtual bool is_enabled() {
    return digitalRead(relay_pin) != relay_pin_inverted;
  }

  virtual void enable() {
    if (is_enabled()) {
      return;
    }
    delay_with_blink(delay_before_on);
    digitalWrite(relay_pin, !relay_pin_inverted);
  }

  virtual void disable() {
    if (!is_enabled()) {
      return;
    }
    delay_with_blink(delay_before_off);
    digitalWrite(relay_pin, relay_pin_inverted);
  }
};

class UpsPin : public Pin {
public:
  UpsPin(int relay_pin = 0, bool inverted = false, int delay_on = 0, int delay_off = 0)
    : Pin(relay_pin, inverted, delay_on, delay_off) {}

  void setup() override {
    pinMode(UPS_STATUS, INPUT_PULLUP);
    pinMode(UPS_BUTTON, OUTPUT);
  }

  bool isUpsEnabled() {
    int isHigh = digitalRead(UPS_STATUS);
    Serial.print("Is ups enabled: ");
    Serial.print(isHigh ? "Disabled" : "Enabled");
    Serial.println();
    return isHigh != HIGH;
  }

  void press_button() {
    bool isEnabled = isUpsEnabled();
    Serial.println("Switching ups 1");
    digitalWrite(UPS_BUTTON, HIGH);
    delay_with_blink(4500);
    Serial.println("Switching ups 0");
    digitalWrite(UPS_BUTTON, LOW);
    while(isUpsEnabled() == isEnabled) {
      delay_with_blink(200);
    }
    Serial.println("Switching ups finished");
  }

  void enable() override {
    if(isUpsEnabled()) {
      return;
    }
    press_button();
  }

  void disable() override {
    if(!isUpsEnabled()) {
      return;
    }
    press_button();
  }

};

Pin* pins[SIZE] = {
  new Pin(X32,             true,     0,               1000),
  new UpsPin(),
  new Pin(DI,              true,     16000,           5000),
  new Pin(TOP_L,           true,     2000,            2000),
  new Pin(TOP_R,           true,     2000,            5000),
  new Pin(SUB_AND_MON_L,   true,     6000,            2000),
  new Pin(SUB_AND_MON_R,   true,     2000,            5000),
  new Pin(STUFF_1,         true,     2000,            2000),
  new Pin(STUFF_2,         true,     2000,            0)
};

void enable() {
  for (int i = 0; i < SIZE; i++) {
    if (!is_switcher_on()) {
      return;
    }
    Pin* pin = pins[i];
    pin->enable();
  }
  set_led(true);
}

void disable() {
  for (int i = SIZE - 1; i >= 0; i--) {
    if (is_switcher_on()) {
      return;
    }
    Pin* pin = pins[i];
    pin->disable();
  }
  set_led(false);
}

void setup() {
  pinMode(SWITCHER, INPUT);
  pinMode(LED, OUTPUT);
  for (int i = 0; i < SIZE; i++) {
    Pin* pin = pins[i];
    pin->setup();
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
