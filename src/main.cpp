#include <Arduino.h>

constexpr uint8_t LED_PIN  = 16;
constexpr uint8_t BUTTON_PIN = 4;
constexpr uint16_t DELAY = 1000;
volatile uint64_t last_state_change = 0;


enum Led_state{
  ON,
  OFF
};
enum Blinking_mode {
  BLINKING,
  ALWAYS_ON,
  ALWAYS_OFF
};

class IterationsCounter{
  private:
    uint16_t loops = 0; 
    uint32_t iteration_start = 0;
    uint32_t iterations_sum = 0;
  public:
    uint16_t get_loops(){
      return loops;
    }
    uint32_t get_duration(){
      return micros() - iteration_start;
    }
    uint32_t get_avarage_time(){
      if(loops == 0){
        return 0;
      }
      return iterations_sum/loops;
    }
    void set_start(uint64_t iteration_start){
      this->iteration_start = iteration_start;
    }
    void add_iteration_time(uint16_t time){
      this->iterations_sum += time;
    }
    void add_loop(){
      loops++;
    }
    void reset_counter(){
      loops = 0;
      iteration_start = 0;
      iterations_sum = 0;
    }
    
};

class Led{
  private:
        Blinking_mode mode = BLINKING;
        Led_state state = ON;
  public:
    Blinking_mode get_mode(){
      return mode;
    }
    Led_state get_state(){
      return state;
    }

    void init(){
      pinMode(LED_PIN, OUTPUT);
    }

    void set_mode(Blinking_mode mode){
      this->mode = mode;
    }

    void set_state(Led_state state){
      this->state = state;
    }
};

Led led;
IterationsCounter iterations_counter;

void blinking(Led& led){
  if (led.get_mode() == BLINKING)
  {
      if (led.get_state() == ON)
      {
        led.set_state(OFF);
        last_state_change = millis();

      }else if(led.get_state() == OFF && millis() > last_state_change + DELAY){
        led.set_state(ON);
        last_state_change = millis();
      }
  }else if (led.get_mode() == ALWAYS_ON )
  {
    led.set_state(ON);
  }else if (led.get_mode() == ALWAYS_OFF)
  {
    led.set_state(OFF);
  }
  
  
  
  
}

void setup() { 
  Serial.begin(115200);
  led.init();
}

void loop() {
  iterations_counter.set_start(micros());

  blinking(led);

  if (led.get_state() == ON)
  {
    digitalWrite(LED_PIN, HIGH);
  }else{
    digitalWrite(LED_PIN, LOW);
  }
  
  iterations_counter.add_loop();
  iterations_counter.add_iteration_time(iterations_counter.get_duration());
  if (iterations_counter.get_loops() == 1000)
  {
    Serial.print("Average time for last 1000 loops is ");
    Serial.print(iterations_counter.get_avarage_time());
    Serial.println("us");
    iterations_counter.reset_counter();
  }
  


  
}
