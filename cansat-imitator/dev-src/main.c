
#include <string.h>
#include <stdio.h>

#include "rscs/uart.h"
#include "rscs/gps_nmea.h"


int main()
{
	//rscs_uart_bus_t * uart = rscs_uart_init(RSCS_UART_ID_UART0, RSCS_UART_FLAG_ENABLE_RX);
	rscs_gps_t * gps = rscs_gps_init(RSCS_UART_ID_UART0);

	while(1)
	{
		float lon, lat, height;
		rscs_e error = rscs_gps_read(gps, &lon, &lat, &height);

		if (error == RSCS_E_NONE)
			printf("error = %i\n", error);
	}

	return 0;
}
