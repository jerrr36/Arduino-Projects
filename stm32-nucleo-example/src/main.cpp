#include <mbed.h>

int main() {

  // put your setup code here, to run once:
  DigitalOut myLed(LED1);

  while(1) {
    // put your main code here, to run repeatedly:
    myLed = 1;
    wait_us(500000);

    myLed = 0;
    wait_us(500000);
    printf("Hello world\n");
  }
}