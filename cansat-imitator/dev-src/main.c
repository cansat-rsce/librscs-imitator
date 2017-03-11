
#include <string.h>
#include <stdio.h>

#include "rscs/uart.h"


int main()
{
	rscs_uart_bus_t * uart = rscs_uart_init(RSCS_UART_ID_UART0, RSCS_UART_FLAG_ENABLE_RX);

	char data[100];

	while(1)
	{
		memset(data, 0x00, sizeof(data));
		size_t readed = rscs_uart_read_some(uart, data, sizeof(data));

		if (readed)
		{
			printf("readed = %zd, data = %s\n", readed, data);
		}
	}

	return 0;
}
