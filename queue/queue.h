#ifndef QUEUE_H
#define QUEUE_H

#include"deque.h"

namespace MyCppSTL
{
	template<class T, class Sequence> class queue;
	template<class T, class Sequence>
	bool operator==(const queue<T, Sequence>&lhs,const queue<T, Sequence>&rhs);
	template<class T, class Sequence>
	bool operator<(const queue<T, Sequence>&lhs,const queue<T, Sequence>&rhs);

	template<class T,class Sequence=MyCppSTL::deque<T>>
	class queue
	{
	public:
		//��Ƕ�ͱ�
		typedef typename Sequence::value_type value_type;
		typedef typename Sequence::reference reference;
		typedef typename Sequence::const_reference const_reference;
		typedef typename Sequence::size_type size_type;
	public:
		friend bool operator==<>(const queue<T>&lhs,const queue<T>&rhs);
		friend bool operator< <>(const queue<T>&lhs,const queue<T>&rhs);
	public:
		explicit queue(const Sequence&cont) :container(cont)
		{
		}
		explicit queue(Sequence&&cont= Sequence()) :container(std::move(cont))
		{
		}
		queue(const queue<T>&rhs) :container(rhs.container)  //�����ǵ��õײ������Ŀ������캯��
		{
		
		}
		queue(queue<T>&&rhs) :container(std::move(rhs.container)) //���õײ��move���캯��
		{
		
		}
		~queue()
		{
		}

		//
		queue&operator=(const queue<T>&rhs)
		{
			if (this != &rhs)
			{
				container = rhs.container;
			}
			return *this;
		}

		reference front()
		{
			return container.front();
		}

		const_reference front() const
		{
			return container.front();
		}

		reference back()
		{
			return container.back();
		}
		const reference back() const
		{
			return container.back();
		}

		bool empty()
		{
			return container.empty();
		}

		size_type size()
		{
			return container.size();
		}

		void push(const value_type&value)
		{
			container.push_back(value);
		}
		void push(const value_type&&value)
		{
			container.push_back(std::move(value));
		}

		void pop()
		{
			container.pop_front();
		}

		void swap(queue<T>&rhs)
		{
			auto tmp = rhs.container;
			rhs.container = container;
			container = tmp;
		}



	private:
		Sequence container;   //�ײ�����ʵ��

	}; //end queue

	template<class T,class Sequence=MyCppSTL::queue<T>>
	bool operator==(const queue<T>&lhs,const queue<T>&rhs)
	{
		return (lhs.container == rhs.container);
	}
	
	template<class T, class Sequence>
	bool operator<(const queue<T>&lhs,const queue<T>&rhs)
	{
		return (lhs.container < rhs.container);
	}
}  //end namespce



#endif    //end queue.h
