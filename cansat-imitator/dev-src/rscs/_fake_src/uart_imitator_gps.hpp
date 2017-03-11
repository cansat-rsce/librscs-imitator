#ifndef GPS_IMITATOR_H_
#define GPS_IMITATOR_H_

#include <string>
#include <thread>

#include "uart_imitatator.hpp"


namespace rscs
{
	class uart_imitator_gps: public uart_imitator
	{
	public:
		uart_imitator_gps(ssize_t rx_buffer_size, ssize_t tx_buffer_size)
			: uart_imitator(rx_buffer_size, tx_buffer_size)
		{}

	protected:
		void main()
		{
			size_t current_line = 0;

			while(uart_imitator::should_i_stop())
			{
				const std::string & line = _fake_gps_lines[current_line];

				uart_imitator::imitator_write(line.begin(), line.end());

				current_line++;
				if (current_line >= _total_lines_count())
					current_line = 0;

				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			}
		}

	private:
		static const std::string _fake_gps_lines[];
		size_t _total_lines_count() const;

	};
}

#endif /* GPS_IMITATOR_H_ */
