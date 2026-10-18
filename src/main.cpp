#include <Arduino.h>

constexpr uint8_t LED_PIN  = 16;
constexpr uint16_t DELAY = 1000;
volatile uint64_t last_state_change = 0;

enum Led_state{
  ON,
  OFF
};

class Led{
  public:
    Led_state state = OFF;
    void init(){
      pinMode(LED_PIN, OUTPUT);
    }

    void set(Led_state state){
      this->state = state;
    }
};

Led led;

void blinking(Led& led){
  if (led.state == ON && millis() > last_state_change + DELAY)
  {
    led.set(OFF);
    last_state_change = millis();

  }else if(led.state == OFF && millis() > last_state_change + DELAY){
    led.set(ON);
    last_state_change = millis();
  }
  
}

void setup() { 
  led.init();
}

void loop() {
  blinking(led);

  if (led.state == ON)
  {
    digitalWrite(LED_PIN, HIGH);
  }else{
    digitalWrite(LED_PIN, LOW);
  }
  
}
