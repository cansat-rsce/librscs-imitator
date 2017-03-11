#ifndef UART_DEVICE_IMITATATOR_H_
#define UART_DEVICE_IMITATATOR_H_

#include <stdint.h>
#include <thread>
#include <atomic>

#include "uart_async_qeue.hpp"

namespace rscs
{

	//! Базовый класс - имитаторов устойств, подключенных к UART
	/*! Сделан без заморочек - хранит в себе два буфера rx и tx.
		в один буфер пишет хост, во второй буфер пишет наш имитатор, Чтение крест-накрест

		Запускает отдельный поток в котором работает виртуальный метод `main`
		для которого представлен классический интерфейс для чтения/записи */
	class uart_imitator
	{
	public:
		uart_imitator(ssize_t rx_buffer_size, ssize_t tx_buffer_size)
			: _host_rx_buffer(rx_buffer_size), _host_tx_buffer(tx_buffer_size)
		{}

		virtual ~uart_imitator()
		{
			stop();
		}

		// ИНТЕРФЕЙС ДЛЯ ХОСТА
		// ==================================================
	public:
		//! аналог rscs_uart_write_some
		size_t host_write_some(const void * data, size_t datasize)
		{
			const uint8_t * begin = static_cast<const uint8_t *>(data);
			return _host_tx_buffer.write_some(begin, begin + datasize) - begin;
		}

		//! аналог rscs_uart_write
		void host_write(const void * data, size_t datasize)
		{
			const uint8_t * begin = static_cast<const uint8_t *>(data);
			_host_tx_buffer.write(begin, begin + datasize);
		}

		//! аналог rscs_uart_read_some
		size_t host_read_some(void * data, size_t datasize)
		{
			uint8_t * begin = static_cast<uint8_t *>(data);
			return _host_rx_buffer.read_some(begin, datasize);
		}

		//! аналог rscs_uart_read
		void host_read(void * data, size_t datasize)
		{
			uint8_t * begin = static_cast<uint8_t *>(data);
			_host_rx_buffer.read(begin, datasize);
		}

		//! Запуск потока имитатора
		void start()
		{
			if (_imitator_thread.joinable())
			{
				return;
			}

			_stopflag.store(true);
			_imitator_thread = std::thread(
					[this](){ main(); }
			);
		}

		//! остановка потока имитатора
		void stop()
		{
			if (_imitator_thread.joinable())
			{
				_stopflag.store(true);
				_imitator_thread.join();
			}
		}

		// ИНТЕРФЕЙС ДЛЯ ИМИТАТОРА
		// ==================================================
	protected:
		template <typename input_iterator>
		//! неблокирующая передача данных хосту
		input_iterator imitator_write_some(input_iterator begin, input_iterator end)
		{
			return _host_rx_buffer.write_some(begin, end);
		}

		template <typename input_iterator>
		//! блокирующая передача данных хосту
		void imitator_write(input_iterator begin, input_iterator end)
		{
			_host_rx_buffer.write(begin, end);
		}

		template <typename output_iterator>
		//! неблокирующее получение данных от хоста
		size_t imitator_read_some(output_iterator & out, size_t count)
		{
			return _host_tx_buffer.read_some(out, count);
		}

		template <typename output_iterator>
		//! блокирующее получение данных от хоста
		void imitator_read(output_iterator & out, size_t count)
		{
			_host_tx_buffer.read(out, count);
		}

		//! Позволяет проверить имитатору, на то, что его просят завершиться
		bool should_i_stop()
		{
			return _stopflag.load();
		}

		//! аналог MAIN для имитатора
		virtual void main() = 0;



	private:
		//! буфер TX для хоста, RX для имитатора
		uart_async_queue _host_rx_buffer;

		//! буфер RX для хоста, TX для имитатора
		uart_async_queue _host_tx_buffer;

		//!
		std::thread _imitator_thread;
		std::atomic<bool> _stopflag;
	};
}

#endif /* UART_DEVICE_IMITATATOR_H_ */
