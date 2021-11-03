#pragma once
#include <thread>

// C++ doesn't allow to place definition
// of a template function in a separate .cpp file
template <class _Rep, class _Period>
bool thread_sleep(bool& stop, std::chrono::duration<_Rep, _Period> delay)
{
	using namespace std::literals::chrono_literals;
	std::chrono::time_point<std::chrono::system_clock> finish =
		std::chrono::system_clock::now() + delay;

	if (stop)
		return true;

	while (finish - std::chrono::system_clock::now() > 200ms)
		if (thread_sleep(stop, 200ms))
			return true;

	std::this_thread::sleep_until(finish);
	return false;
}
