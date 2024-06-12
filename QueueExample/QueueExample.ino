/*
* FreeRTOS Queue Demo
* (Code provided by DigiKey)
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
static QueueHandle_t msg_queue;

/*-------Tasks----------*/
void printMessages(void *parameter) {
  int item;

  while (1) {
    // See if there's a message in the queue
    if (xQueueReceive(msg_queue, (void *)&item, 0) == pdTRUE) {
      //Serial.println(item);
    }
    Serial.println(item);

    // Wait before trying again
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Queue Demo---");

  // Create queue
  msg_queue = xQueueCreate(msg_queue_len, sizeof(int));

  // Start print task
  xTaskCreatePinnedToCore(
    printMessages,
    "Print Messages",
    1024,
    NULL,
    1,
    NULL,
    app_cpu);
  
}

void loop() {
  static int num = 0;

  // Try to add item to queue for 10 ticks, fail if queue is full
  if (xQueueSend(msg_queue, (void *)&num, 10) != pdTRUE) {
    Serial.println("Queue full");
  }
  num++;

  // Wait before trying again
  vTaskDelay(1000 / portTICK_RATE_MS);
}
