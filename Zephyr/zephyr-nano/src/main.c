#include <zephyr.h>
#include <sys/printk.h>


void main(void)
{
    while (true) {
        printk("Hello world! %s\n", CONFIG_BOARD);
        k_msleep(1000);
    }
}



