  
// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

static const int queueLength = 10;

static QueueHandle_t msgQueue;




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  Serial.println("ECHO DEMO");

  msgQueue = xQueueCreate(queueLength, sizeof(String));
  
  //task
  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
              readSer,  // Function to be called
              "readSer",   // Name of task
              3000,         // Stack size (bytes in ESP32, words in FreeRTOS)
              NULL,         // Parameter to pass to function
              1,            // Task priority (0 to configMAX_PRIORITIES - 1)
              NULL,         // Task handle
              app_cpu);     // Run on one core for demo purposes (ESP32 only)
              
  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
              echo,  // Function to be called
              "echo",   // Name of task
              3000,         // Stack size (bytes in ESP32, words in FreeRTOS)
              NULL,         // Parameter to pass to function
              3,            // Task priority (0 to configMAX_PRIORITIES - 1)
              NULL,         // Task handle
              app_cpu);     // Run on one core for demo purposes (ESP32 only)

}

void loop() {}

void readSer(void *parameter) {
  String str;

  
  while (1) {
    if (Serial.available() > 0) {
      str = Serial.readString();
      if (xQueueSend(msgQueue, (void *)&str, 10) != pdTRUE) {
        Serial.println("Queue full");
        
      }
     }
     vTaskDelay(100 / portTICK_PERIOD_MS);
  }  
}

void echo (void *parameter) {
  String item;

  while (1) {
    if (xQueueReceive(msgQueue, (void *)&item, 0) == pdTRUE) {
      Serial.print(item);   
    }
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
  
}
