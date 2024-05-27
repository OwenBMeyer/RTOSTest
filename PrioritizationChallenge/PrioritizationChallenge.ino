/**
* Prioritization Challenge
*
* Create a process that takes input from the serial monitor, and another
* process that blinks an LED based on the input given to the first process
*/

// Only use 1 core for this project
#if CONFIG_FREERTOS_UNICORE
static const BaseTupy_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// String to print
//const char msg[] = "Yadda yadda talking big words.";

// Pins
static const int led_pin = 2;

// Integer time between LED blinks in ms
static int blink_rate = 500;

// Task handles
static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

/*--Tasks--*/

// Task: blink the LED
void toggleLED(void *parameter) {
  while(1) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(blink_rate / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(blink_rate / portTICK_PERIOD_MS);
  }
}

// Task: take input from serial monitor
void getDelay(void *parameter) {
  while (1) {
    if (Serial.available() > 1) {
      int temp = Serial.parseInt();
      if (temp > 0) {
        blink_rate = temp;
      }
      Serial.print("Updated delay to ");
      Serial.println(blink_rate);
    }
  }
}

/*--MAIN--*/

void setup() {
  
  pinMode(led_pin, OUTPUT);

  // Configure Serial to go slower
  Serial.begin(9600);

  // Wait to start (so we don't miss Serial output)
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---Begin Task---");

  // Print prompt for program
  Serial.println("Enter blink rate for LED in ms");

  // Task to run forever
  xTaskCreatePinnedToCore(
    toggleLED, 
    "Take input",
    1024,
    NULL,
    1,
    &task_1,  // Handle so we can reference from other tasks
    app_cpu);

  xTaskCreatePinnedToCore(
    getDelay, 
    "Blink LED",
    1024,
    NULL,
    1,
    &task_2,  // Handle so we can reference from other tasks
    app_cpu);

  vTaskDelete(NULL);
}

void loop() {
}