#pragma once
#include <base/task/CancellationToken.h>
#include <base/task/TaskCanceledException.h>
#include <thread/pipe/IPipeSource.h>

namespace thread
{
	/// @brief
	/// @tparam T 必须是具有无参构造函数的类，实在不行可以用智能指针包装起来，
	/// 让 T 是一个智能指针。
	template <typename T>
	class ISource
	{
	public:
		virtual ~ISource() = default;

		/// @brief 从源中读取数据
		/// @param data 这里传入的是引用。派生类可以修改 data。
		/// @return 成功返回 0，失败返回负数的错误代码。
		virtual int ReadData(T &data) = 0;
	};
}
