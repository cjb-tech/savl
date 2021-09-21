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


#define ZERO_ANALOG 3
#define X32_ANALOG 5
#define DI_ANALOG 6
#define TOP_L_ANALOG 9
#define TOP_R_ANALOG 11
#define SUB_AND_MON_L_ANALOG 7
#define SUB_AND_MON_R_ANALOG 8
#define USELESS_ANALOG 9

#define SWITCHER 13

#define SIZE 8


bool is_switcher_on(){
  return true;//digitalRead(SWITCHER) == HIGH;
}

struct Pin{
  
  int num;
  int analogNum;
  int power_on_delay;
  int power_off_delay;

  void setup(){
    pinMode(num, OUTPUT);
    digitalWrite(num, HIGH);  
    analogWrite(analogNum, 0);
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
    analogWrite(analogNum, 255);
   }

  void enable(){
    if(enabled()){
      return;
    }

    delay(power_on_delay);
    digitalWrite(num, LOW);
    analogWrite(analogNum, 0);
   }

};

/*
 * pin, - number of pin on arduino
 * power on delay - delay after pin is enabled
 * power off delay - delay after pin is disabled
 */

Pin pins[SIZE] = {
  Pin{ZERO, ZERO_ANALOG, 2000, 2000},
  Pin{X32, X32_ANALOG, 2000, 2000},
  Pin{DI, DI_ANALOG, 2000, 2000},
  Pin{TOP_L, TOP_L_ANALOG, 2000, 2000},
  Pin{TOP_R, TOP_R_ANALOG, 2000, 2000},
  Pin{SUB_AND_MON_L, SUB_AND_MON_L_ANALOG, 2000, 2000},
  Pin{SUB_AND_MON_R, SUB_AND_MON_R_ANALOG, 2000, 2000},
  Pin{USELESS, USELESS_ANALOG, 2000, 2000}
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
