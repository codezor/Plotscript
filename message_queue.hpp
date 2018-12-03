/* Adapted from
www.justsoftwaresolutions.co.uk/threading/implementing-a-thread-safe-queue-using-condition-variables.html
*/

#include <condition_variable>
#include <mutex>
#include <queue>

struct OutMessage_t
{
	std::string error;
	Expression exp;
	enum MessageType
	{
		Errorstring, noterr
	};
	MessageType type;
};
// two message queues will be needed one for input and one for output
template<typename MessageType>
class message_queue
{
public:
	
	message_queue(message_queue const&) = delete;  // singleton
	void operator=(message_queue const&) = delete; // singleton

	static message_queue& get_instance() // singleton
	{
		static message_queue instance;
		return instance;
	}

	void push(MessageType const& message)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_queue.push(message);
		lock.unlock();
		m_condition.notify_one();
	}

	bool empty() const
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_queue.empty();
	}

	bool try_pop(MessageType& popped_value)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		if(m_queue.empty())
		{
			return false;
		}

		popped_value = m_queue.front();
		m_queue.pop();
		return true;
	}

	void wait_and_pop(MessageType& popped_value)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		while(m_queue.empty())
		{
			m_condition.wait(lock);
		}

		popped_value = m_queue.front();
		m_queue.pop();
	}

	std::size_t size()
	{
		std::lock_guard<std::mutex> gaurd(m_mutex);

		return m_queue.size();
	}


private:
	message_queue()	{}; // singleton

	std::queue<MessageType> m_queue;
	mutable std::mutex m_mutex;
	std::condition_variable m_condition;
};