#ifndef ASYNC_QEUE_H_
#define ASYNC_QEUE_H_

#include <queue>
#include <mutex>
#include <condition_variable>
#include <algorithm>

namespace rscs
{

	//! Асинхронный буффер для обеспечения связи между двумя абонентами (имитатор устройства и хост)
	class uart_async_queue
	{
	public:
		uart_async_queue(ssize_t maxsize): _maxsize(maxsize) {}
		~uart_async_queue() = default;
		uart_async_queue(const uart_async_queue & other) = delete;
		uart_async_queue& operator=(const uart_async_queue & other) = delete;


		//! Параметр шаблона - итератор на uint8_t
		template <typename input_iterator>
		//! неблокирующая передача данных (хостом или имитатором)
		/*! возвращает итератор на первый элемент, который записать не удалось (буфер кончился) */
		input_iterator write_some(input_iterator begin, input_iterator end)
		{
			std::unique_lock<std::mutex> lock(_mtx);
			return _do_write_some(begin, end);
		}


		//! Параметр шаблона - итератор на uint8_t
		template <typename input_iterator>
		/*! Блокирующая передача данных (хостом или имитатором). Не вовзращает контроль пока все
			элементы не будут записаны в буфер*/
		void write(input_iterator begin, input_iterator end)
		{
			std::unique_lock<std::mutex> lock(_mtx);
			while (1)
			{
				begin = _do_write_some(begin, end);
				if (begin != end)
				{
					_data_readed_cv.wait(lock);
					continue;
				}
				else
				{
					break;
				}
			};
		}


		//! Параметр шаблона - итератор, в который можно записывать uint8_t
		template <typename output_iterator>
		//! Копирует данные из буфера как будто он приёмный (для хоста или для имитатора)
		/*! данные копируеются в output_iterator по аналогии с любым стандартным output_iterator-ом
			count указывает сколько элементов максимум может быть скопировано
			возвращает сколько элементов удалось прочесть*/
		size_t read_some(output_iterator & out, size_t count)
		{
			std::unique_lock<std::mutex> lock(_mtx);
			return _do_read_some(out, count);
		}


		//! Параметр шаблона - итератор, в который можно записывать uint8_t
		template <typename output_iterator>
		//! Копирует данные из буфера как будто он приёмный (для хоста или для имитатора)
		/*! данные копируеются в output_iterator по аналогии с любым стандартным output_iterator-ом
			count указывает сколько элементов должно быть скопировано.
			не возвращается пока не прочтет все */
		void read(output_iterator & out, size_t count)
		{
			std::unique_lock<std::mutex> lock(_mtx);
			size_t readed = 0;
			while (readed != count)
			{
				readed += _do_read_some(out, count);
				_data_writed_cv.wait(lock);
			}
		}


	private:
		template <typename input_iterator>
		//! функция, которая реализует операцию write_some. Вынесена отдельно, чтобы
		//! можно было вызывать в различных контекстах блокировки мьютекса
		input_iterator _do_write_some(input_iterator begin, input_iterator end)
		{
			input_iterator local_end;
			if (_maxsize >= 0)
			{
				// если буфер ограничен в размерах

				// сколько хочет записать тот, кто нас вызывает
				const size_t portion_size = std::distance(begin, end);
				// сколько мы можем записать
				const size_t allowed_size = _maxsize - _data.size();
				// сколько будем записывать по факту
				const size_t to_write_size = std::min(allowed_size, portion_size);

				// устанавливаем local_end соответсвенно
				local_end = std::next(begin, to_write_size);
			}
			else
			{
				// если буфер не ограничен в размерах
				local_end = end;
			}

			while (begin != local_end)
			{
				_data.push(*begin);
				begin++;
			}

			_data_writed_cv.notify_all();
			return local_end;
		}


		template <typename output_iterator>
		//! функция, которая реализует операцию read_some. Вынесена отдельно, чтобы
		//! можно было вызывать в различных контекстах блокировки мьютекса
		size_t _do_read_some(output_iterator & out, size_t count)
		{
			count = std::min(count, _data.size());
			size_t writed = 0;
			while(writed < count)
			{
				*out = _data.front();
				out++;
				_data.pop();
				writed++;
			}

			_data_readed_cv.notify_all();
			return writed;
		}

	private:
		ssize_t _maxsize;
		std::queue<uint8_t> _data;
		std::mutex _mtx;
		std::condition_variable _data_readed_cv;
		std::condition_variable _data_writed_cv;
	};

}

#endif /* ASYNC_QEUE_H_ */
