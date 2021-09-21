#define START_SLEEP 5000
#define SWITCH_INTERVAL 5000
#define LOOP_DELAY 5000

#define ZERO 2
#define X32 3
#define DI 4
#define TOP_L 5
#define TOP_R 6
#define SUB_AND_MON_L 7
#define SUB_AND_MON_R 8
#define USELESS 9

#define SWITCHER 13

#define SIZE 8


bool is_switcher_on(){
  return digitalRead(SWITCHER) == HIGH;
}

struct Pin{
  
  int num;
  int power_on_delay;
  int power_off_delay;

  void setup(){
    pinMode(num, OUTPUT);
    digitalWrite(num, HIGH);  
  }
  
  bool enabled(){
    return digitalRead(num) == LOW;
  }

  void disable(){
    if(!enabled()){
      return;
    }

    delay(power_off_delay);
    digitalWrite(num, HIGH);
   }

  void enable(){
    if(enabled()){
      return;
    }

    delay(power_on_delay);
    digitalWrite(num, LOW);
   }

};

/*
 * pin, - number of pin on arduino
 * power on delay - delay after pin is enabled
 * power off delay - delay after pin is disabled
 */

Pin pins[SIZE] = {
  Pin{ZERO, 2000, 2000},
  Pin{X32, 2000, 2000},
  Pin{DI, 2000, 2000},
  Pin{TOP_L, 2000, 2000},
  Pin{TOP_R, 2000, 2000},
  Pin{SUB_AND_MON_L, 2000, 2000},
  Pin{SUB_AND_MON_R, 2000, 2000},
  Pin{USELESS, 2000, 2000}
};

void enable(){
  for(int i=0; i<SIZE; i++){
    if(!is_switcher_on()){
      return;
    }
    Pin pin = pins[i];
    pin.enable();
  }
}

void disable(){
  for(int i=SIZE-1; i>=0; i--){
    if(is_switcher_on()){
      return;
    }
    Pin pin = pins[i];
    pin.disable();
  }
}

void setup() {

  pinMode(SWITCHER, INPUT);
  
  for(int i=0; i<SIZE; i++){
    Pin pin = pins[i];
    pin.setup();
  }
  
  Serial.begin(9600);

  delay(START_SLEEP);
}

void loop() {
  if(is_switcher_on()){
    enable();
  }else{
    disable();
  }
  delay(LOOP_DELAY);
}
