#pragma once

template<typename T>
class CSafeDeque
{
private:
	boost::mutex m_mutex;
	boost::container::deque <T> m_que;
	BOOL m_bEndDeqque;

public:

	CSafeDeque()
	{

	}
	
	~CSafeDeque()
	{
		int nQue = (int)m_que.size();
		boost::timer t;
		m_que.clear();
		LOGD("CSafeDeque ªË¡¶"<<"≈• : "<<nQue<<"Sec : "<<t.elapsed());
	}

public:
	void SetEndQue(bool status)
	{
		boost::lock_guard<boost::mutex> lock( m_mutex ); 
		m_bEndDeqque = status;
	}

	BOOL IsEndQue()
	{
		boost::lock_guard<boost::mutex> lock( m_mutex ); 
		return m_bEndDeqque;
	}

public:
	void push_back(T& _val)
	{
		boost::lock_guard<boost::mutex> lock( m_mutex ); 
		//std::deque<T>::push_back(_val);

		m_que.push_back(_val);
	}

	void vector_push_back( std::vector<T> vpVal )
	{
		boost::lock_guard<boost::mutex> lock( m_mutex );

		BOOST_FOREACH(T _val, vpVal)
		{
			//std::deque<T>::push_back(_val);
			m_que.push_back(_val);
		}
	
	}
	void pop_front()
	{
		boost::lock_guard<boost::mutex> lock( m_mutex );
		//if(!std::deque<T>::empty())
		//	std::deque<T>::pop_front();
		if(!m_que.empty())
			m_que.pop_front();
	}

	bool empty()
	{
		boost::lock_guard<boost::mutex> lock( m_mutex ); 
		//return std::deque<T>::empty();
		return m_que.empty();
	}

	size_t size()
	{
		boost::lock_guard<boost::mutex> lock( m_mutex ); 
		//return (size_t)std::deque<T>::size();
		return m_que.size();
	}

	T& front()
	{
		boost::lock_guard<boost::mutex> lock( m_mutex ); 
		//return std::deque<T>::front();
		return m_que.front();
	}
};
