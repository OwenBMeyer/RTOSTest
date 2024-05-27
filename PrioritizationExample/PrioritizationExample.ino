/**
* Prioritization example
* (Code provided by DigiKey)
*/

// Only use 1 core for this project
#if CONFIG_FREERTOS_UNICORE
static const BaseTupy_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// String to print
const char msg[] = "Yadda yadda talking big words.";

// Task handles
static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

/*--Tasks--*/

// Low priority task: print to serial terminal
void startTask1(void *parameter) {
  // Count the number of characters
  int msg_len = strlen(msg);

  // Print each char in msg
  while(1) {
    Serial.println();
    for (int i=0; i < msg_len; i++) {
      Serial.print(msg[i]);
    }
    // Put this task into the blocked state
    Serial.println();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// High priority task: print to serial terminal
void startTask2(void *parameter) {
  while(1) {
    Serial.print('*');
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

/*--MAIN--*/

void setup() {
  
  // Configure Serial to go slower
  Serial.begin(300);

  // Wait to start (so we don't miss Serial output)
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---Begin Task Demo---");

  // Print self priority
  Serial.print("Setup and loop task running on core ");
  Serial.print(xPortGetCoreID());
  Serial.print(" with priority ");
  Serial.print(uxTaskPriorityGet(NULL));

  // Task to run forever
  xTaskCreatePinnedToCore(
    startTask1, 
    "Read out message",
    1024,
    NULL,
    1,
    &task_1,  // Handle so we can reference from other tasks
    app_cpu);

  // Task to run forever (with higher priority)
  xTaskCreatePinnedToCore(
    startTask2, 
    "Read out *",
    1024,
    NULL,
    2,
    &task_2,  // Handle so we can reference from other tasks
    app_cpu);
}

void loop() {
  
  // Suspend high priority task for some intervals
  for (int i=0; i < 3; i++) {
    vTaskSuspend(task_2);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    vTaskResume(task_2);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }

  // Delete the lower priority task
  if (task_1 != NULL) {
    // IMPORTANT: If you call vTaskDelete on an empty task, may cause CPU to crash
    vTaskDelete(task_1);
    task_1 = NULL;
  }

}
