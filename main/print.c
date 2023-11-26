#include "print.h"
#include <stdio.h>
#include "esp_log.h"
#include "esp_partition.h"

void print_buffer(const uint8_t* buffer, uint32_t len, const char* msg)
{
	#define line_width 16
	int lines = len / line_width;
	for (int line = 0; line < lines; line++)
	{
		int index = line * line_width;
		if (msg != NULL)
			printf("%s: %02x: ", msg, index);
		else
			printf("%02x: ", index);
		for (int i = 0; i < line_width; i++)
		{
			printf("%02x ", buffer[index + i]);
		}
		printf("\n");
	}
	int leftovers = len % line_width;
	if (leftovers > 0)
	{
		int line = lines;
		int index = line * line_width;
		if (msg != NULL)
			printf("%s: %02x: ", msg, index);
		else
			printf("%02x: ", index);
		for (int i = 0; i < leftovers; i++)
		{
			printf("%02x ", buffer[index + i]);
		}
		printf("\n");
	}
}

void print_partitions(void)
{
    ESP_LOGI("print_partitions", "Listing partitions:");
    esp_partition_iterator_t it = esp_partition_find(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);

    for (; it != NULL; it = esp_partition_next(it)) {
        const esp_partition_t *part = esp_partition_get(it);
        ESP_LOGI("print_partitions", "- partition '%s', subtype %d, offset 0x%lx, size %ld kB",
        part->label, part->subtype, part->address, part->size / 1024);
    }

    esp_partition_iterator_release(it);
}
