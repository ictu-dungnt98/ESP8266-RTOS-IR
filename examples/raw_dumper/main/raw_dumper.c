#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <ir/rx.h>
#include <ir/raw.h>


#define IR_RX_GPIO 5

void ir_dump_task(void *arg) {

	printf("dungnt98 ir raw dumper\n");

    ir_rx_init(IR_RX_GPIO, 1024);
    ir_decoder_t *raw_decoder = ir_raw_make_decoder();

    uint16_t buffer_size = sizeof(int16_t) * 1024;
    int16_t *buffer = malloc(buffer_size);
    while (1) {
        int size = ir_recv(raw_decoder, 0, buffer, buffer_size);
        if (size <= 0) {
            continue;
		}

        printf("Decoded packet (size = %d):\n", size);
        for (int i=0; i < size; i++) {
            printf("%5d ", buffer[i]);
            if (i % 16 == 15)
                printf("\n");
        }

        if (size % 16)
            printf("\n");
    }
}

void ir_receiver_task(void *arg) {
    while (1) {
		ir_rx_loop();
		vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
void app_main()
{
    xTaskCreate(ir_dump_task, "IR dump", 2048, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(ir_receiver_task, "IR receiver", 2048, NULL, tskIDLE_PRIORITY, NULL);
}