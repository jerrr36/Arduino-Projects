
// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

static const int rate = 250;

static const int led = LED_BUILTIN;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  Serial.println("Starting task...");
  //task
  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
              toggleLed,  // Function to be called
              "Toggle",   // Name of task
              1024,         // Stack size (bytes in ESP32, words in FreeRTOS)
              NULL,         // Parameter to pass to function
              2,            // Task priority (0 to configMAX_PRIORITIES - 1)
              NULL,         // Task handle
              app_cpu);     // Run on one core for demo purposes (ESP32 only)

  Serial.println("Task started...");
}

void loop() {}

void toggleLed(void *parameter) {
  while(1) {
    digitalWrite(led, HIGH);
    vTaskDelay(rate / portTICK_PERIOD_MS);
    digitalWrite(led, LOW);
    vTaskDelay(rate / portTICK_PERIOD_MS);
  }
}
