#include <assert.h>

#include <memory>
#include <queue>

#include "uart_imitatator.hpp"
#include "uart_imitator_gps.hpp"

using namespace rscs;

extern "C"
{
	#include "../uart.h"
	#include "../librscs_config.h"

	struct rscs_uart_bus
	{
		std::unique_ptr<uart_imitator> impl;
	};
}


// Сами функции
// =============================


rscs_uart_bus_t * rscs_uart_init(rscs_uart_id_t id, int flags)
{
	std::unique_ptr<rscs_uart_bus_t> retval(new rscs_uart_bus_t());

	switch(id)
	{
	case RSCS_UART_ID_UART0:
		retval->impl.reset(new uart_imitator_gps(RSCS_UART_BUFSIZE_RX, RSCS_UART_BUFSIZE_TX));
		break;
	case RSCS_UART_ID_UART1:
	default:
		return NULL;
	}

	retval->impl->start();
	return retval.release();
}


void rscs_uart_deinit(rscs_uart_bus_t * bus)
{
	assert(bus != NULL);
	bus->impl->stop();
	delete bus;
}


void rscs_uart_set_character_size(rscs_uart_bus_t * bus, int character_size)
{

}


void rscs_uart_set_baudrate(rscs_uart_bus_t * bus, uint32_t baudrate)
{

}


void rscs_uart_set_parity(rscs_uart_bus_t * bus, rscs_uart_parity_t parity)
{

}


void rscs_uart_set_stop_bits(rscs_uart_bus_t * bus, rscs_uart_stopbits_t stopbits)
{

}


void rscs_uart_write(rscs_uart_bus_t * bus, const void * dataptr, size_t datasize)
{
	bus->impl->host_write(dataptr, datasize);
}


void rscs_uart_read(rscs_uart_bus_t * bus, void * dataptr, size_t datasize)
{
	bus->impl->host_read(dataptr, datasize);
}


size_t rscs_uart_write_some(rscs_uart_bus_t * bus, const void * dataptr, size_t datasize)
{
	return bus->impl->host_write_some(dataptr, datasize);
}


size_t rscs_uart_read_some(rscs_uart_bus_t * bus, void * dataptr, size_t datasize)
{
	return bus->impl->host_read_some(dataptr, datasize);
}
