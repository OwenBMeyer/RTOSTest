/*
* FreeRTOS Queue Challenge
* Blink a light with a delay that can be changed with serial input
*/

// Use only 1 core
#if CONFIG_FREERTOS_UNICORE
  static const BaseTupy_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

// Settings
static const uint8_t msg_queue_len = 5;

// Globals
static int delay = 1000;
static QueueHandle_t delay_queue;
static QueueHandle_t msg_queue;

// Pins
static const int led_pin = 2;

/*----------Tasks---------*/

// Task that blinks the LED and prints a message after 100 blinks
void blinkLED(void *parameter) {
  int blinkCount = 0;
  char* msg = "Blinked";
  while (1) {
    // See if there's a new delay time
    if (xQueueReceive(delay_queue, (void *)&delay, 0) == pdTRUE) {
      // This format for testing purposes
    }

    // Blink the LED
    digitalWrite(led_pin, HIGH);
    vTaskDelay(delay / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(delay / portTICK_PERIOD_MS);
    
    // Iterate blink count and check if we've blinked 100 times
    blinkCount++;
    if (blinkCount == 100) {
      // Add message to the msg queue
      if (xQueueSend(msg_queue, (void *)&msg, 10) != pdTRUE) {
        Serial.println("Message queue full");
      }

      // Reset blink count
      blinkCount = 0;
    }
  }
}

// Task that reads serial input and updates the delay and echos input
void updateDelay(void *parameter) {
  char *msg;
  while (1) {
    // Print every message in the message queue
    while (xQueueRecieve(msg_queue, (void *)&msg, 0) != pdFALSE) {
      Serial.println(msg);
    }

    // Read serial input from user

    // Echo input

    // Parse input to check if there is delay command

    // Push new delay to the delay queue
  }
}

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
