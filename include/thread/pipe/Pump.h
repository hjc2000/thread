#pragma once
#include <base/container/List.h>
#include <base/task/CancellationToken.h>
#include <stdexcept>
#include <thread/pipe/IPipeSource.h>
#include <thread/pipe/ISource.h>

namespace base
{
	template <typename T>
	class Pump
		: base::IPipeSource<T>
	{
	private:
		std::shared_ptr<base::ISource<T>> _source;
		base::List<std::shared_ptr<base::IConsumer<T>>> _consumer_list;
		bool _pump_started = false;
		std::function<void(T &data)> _on_before_sending_data_to_consumer;

	public:
		Pump(std::shared_ptr<base::ISource<T>> source)
		{
			if (source == nullptr)
			{
				throw std::invalid_argument{"source 不能为空指针"};
			}

			_source = source;
		}

		base::IList<std::shared_ptr<base::IConsumer<T>>> &ConsumerList() override
		{
			return _consumer_list;
		}

		/// @brief 订阅事件 BeforeSendingDataToConsumerEvent
		/// @note 此回调在每次从源中读出一个数据，在送给管道下级的消费者之前会被触发。
		/// @param func
		void SubscribeToBeforeSendingDataToConsumerEvent(std::function<void(T &data)> func)
		{
			if (_pump_started)
			{
				throw std::runtime_error{"泵启动后不允许订阅事件"};
			}

			_on_before_sending_data_to_consumer = func;
		}

		virtual void PumpDataToConsumers(std::shared_ptr<base::CancellationToken> cancellation_token)
		{
			_pump_started = true;
			T data{};
			while (true)
			{
				if (cancellation_token->IsCancellationRequested())
				{
					return;
				}

				int ret = _source->ReadData(data);
				if (ret < 0)
				{
					base::IPipeSource<T>::FlushEachConsumer();
					return;
				}

				// 触发回调。允许在每次将数据送给消费者之前通过事件回调修改数据
				if (_on_before_sending_data_to_consumer)
				{
					_on_before_sending_data_to_consumer(data);
				}

				base::IPipeSource<T>::SendDataToEachConsumer(data);
			}
		}
	};
} // namespace base
