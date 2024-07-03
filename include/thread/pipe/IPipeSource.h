#pragma once
#include <base/container/IList.h>
#include <memory>
#include <thread/pipe/IConsumer.h>

namespace base
{
	/// @brief 管道式的数据源。
	/// @note 管道出口可以连接多个消费者，向管道入口送入数据时会向所有消费者都送入一次。
	/// @tparam T
	template <typename T>
	class IPipeSource
	{
	public:
		virtual ~IPipeSource() = default;

		/// @brief 消费者列表
		/// @return
		virtual base::IList<std::shared_ptr<base::IConsumer<T>>> &ConsumerList() = 0;

		/// @brief 将数据送给每个消费者
		/// @note 任何一个消费者在 SendFrame 方法中抛出异常都会导致本方法抛出异常，并且会
		/// 中断循环，导致从这里开始，后面的消费者不会被送入数据。如果要实现一个消费者发生错误，
		/// 其他消费者不受影响，那这些消费者就不应该在 SendFrame 方法中抛出异常。
		///
		/// @param data
		void SendDataToEachConsumer(T &data)
		{
			for (std::shared_ptr<base::IConsumer<T>> &consumer : ConsumerList())
			{
				if (consumer)
				{
					consumer->SendFrame(data);
				}
			}
		}

		/// @brief 冲洗每个消费者
		/// @note 任何一个消费者在 Flush 方法中抛出异常都会导致本方法抛出异常，并且会
		/// 中断循环，导致从这里开始，后面的消费者不会被冲洗。如果要实现一个消费者发生错误，
		/// 其他消费者不受影响，那这些消费者就不应该在 Flush 方法中抛出异常。
		void FlushEachConsumer()
		{
			for (std::shared_ptr<base::IConsumer<T>> &consumer : ConsumerList())
			{
				if (consumer)
				{
					consumer->Flush();
				}
			}
		}
	};
} // namespace base
