// Use only core 1 for demo
#if CONFIG_FREERTOS_UNICORE
static const BaseTupy_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// Pins
static const int led_pin = 2;

// Task is to blink an LED
void toggleLED(void *parameter) {
  while (1) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(led_pin, OUTPUT);

  // Set task to run forever
  xTaskCreatePinnedToCore(   // Would use xTaskCreate() in standard FreeRTOS
    toggleLED,    // Function we're calling
    "Toggle LED", // Name of tast
    1024,         // Stack size (bytes in ESP32, words in FreeRTOS)
    NULL,         // Parameter to pass to function
    1,            // Task priority (0 to configMAX_PRIORITIES - 1)
    NULL,         // Task handle (lets this task be managed by other tasks)
    app_cpu);     // Run on one core for this demo

  // If this was vanilla FreeTROS, we'd call vTaskStartScheduler() in main
  // after setting up the tasts to make the scheduler start running.
  // But for ESP32's version, this is called for us
}

void loop() {
  // put your main code here, to run repeatedly:

}
