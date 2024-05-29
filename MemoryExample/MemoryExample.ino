/*
* FreeRTOS Stack Overflow Demo
* (Code provided by DigiKey)
*/

// Use only 1 core
#if CONFIG_FREERTOS_UNICORE
  static const BaseTupy_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

/*--Tasks--*/

void testTask(void *parameter) {
  while (1) {
    int a = 1;
    int b[100];

    // Do something with array so it's not optimized out by compiler
    for (int i=0; i < 100; i++) {
      b[i] = a + 1;
    }
    Serial.println(b[0]);

    // Print remaining stack memory
    Serial.print("High water mark (words): ");
    Serial.println(uxTaskGetStackHighWaterMark(NULL));

    // Print out number of free heap memory bytes before malloc
    Serial.print("Heap beore malloc (bytes): ");
    Serial.println(xPortGetFreeHeapSize());

    int *ptr = (int*) pvPortMalloc(1024 * sizeof(int));
    if (ptr == NULL) {
      Serial.println("Not enough heap.");
    } else {
      // Do something with memory so it isn't optimized out by compiler
      for (int i=0; i < 1024; i++) {
        ptr[i] = 3;
      }
    }

    // Print out number of free heap memory bytes after malloc
    Serial.print("Heep after malloc (bytes): ");
    Serial.println(xPortGetFreeHeapSize());

    // Free allocated memory
    vPortFree(ptr);

    // Wait a while
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

/*--Main--*/

void setup() {
  
  Serial.begin(115200);

  // Wait so we don't miss Serial output
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("--FreeRTOS Memory Demo--");

  // Start the only other task
  xTaskCreatePinnedToCore(
    testTask,
    "Test Task", 
    1500, 
    NULL, 
    1, 
    NULL, 
    app_cpu);

  // Delete setup and loop task
  vTaskDelete(NULL);
}

void loop() {
  // put your main code here, to run repeatedly:

}
