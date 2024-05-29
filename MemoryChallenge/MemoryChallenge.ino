/*
* FreeRTOS/esp32 Memory Challenge
*
* Create two tasks, one that listens for serial input and pushes it to heap,
* and another that prints the input to the terminal and frees it
*/

// Use only 1 core
#if CONFIG_FREERTOS_UNICORE
  static const BaseTupy_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

#define READY_TO_PRINT 1
#define WAIT_TO_PRINT 0

static volatile int flag = WAIT_TO_PRINT;
static char* ptr = NULL;

/*---Tasks---*/
// Task: Get string from user and store in heap
void getInput(void *parameter) {
  int available;
  while (1) {

    // Get number of bytes inputed
    available = Serial.available();

    if (available > 1) {
      /*
      // Print out number of free heap memory bytes before malloc
      Serial.print("Heap before malloc (bytes): ");
      Serial.println(xPortGetFreeHeapSize());
      */

      // Malloc a string the size of the inputed bytes
      ptr = (char*) pvPortMalloc(available * sizeof(char));
      if (ptr == NULL) {
        Serial.println("Out of heap memory.");
        continue;
      }

      // Add each inputed character to saved string
      char c;
      for (int i=0; i < available; i++) {
        c = Serial.read();
        if (c == '\n') {
          ptr[i] = '\0';
        } else {
          ptr[i] = c;
        }
      }

      /*
      // Print out number of free heap memory bytes after malloc
      Serial.print("Heep after malloc (bytes): ");
      Serial.println(xPortGetFreeHeapSize());
      */
      
      // Set the ready flag
      flag = READY_TO_PRINT;
    }
  }
}

// Task: If there is a string in heap, print string to output
void printString(void *parameter) {
  while (1) {

    // If the flag has been set, print phrase and free
    if (flag == 1) {
      Serial.println(ptr);
      vPortFree(ptr);
      ptr = NULL;
      flag = WAIT_TO_PRINT;

      /*
      // Print out free heap size after freeing
      Serial.print("Heep after freeing (bytes): ");
      Serial.println(xPortGetFreeHeapSize());
      */
    }
  }
}

/*---Main---*/
void setup() {

  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("--FreeRTOS Memory Challenge--");

  // Start tasks
  xTaskCreatePinnedToCore(
    getInput,
    "Get Input", 
    1024, 
    NULL, 
    1, 
    NULL, 
    app_cpu);
  
  xTaskCreatePinnedToCore(
    printString,
    "Print String", 
    1024, 
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
