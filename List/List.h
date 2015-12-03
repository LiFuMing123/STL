#ifndef LIST_H
#define LIST_H

#include<initializer_list>
#include"Allocator.h"
#include"Iterator.h"
#include"Construct.h"

namespace MyCppSTL
{



	template<class T,class alloc=MyCppSTL::allocator<__list_node<T>>>
	class list
	{
		//��Ƕ�ͱ�
	public:
		typedef T									value_type;
		typedef T&									reference;
		typedef const T&							const_reference;
		typedef std::size_t							size_type;
		typedef std::ptrdiff_t						difference_type;
		typedef MyCppSTL::list_iterator<T>			iterator;
		typedef MyCppSTL::const_list_iterator<T>	const_iterator;
		typedef MyCppSTL::allocator<__list_node<T>>	alloc_type;   //����ڵ���
		typedef list<T, alloc>					    mylist;
		typedef T*									pointer;
		typedef const T*							const_pointer;
		typedef __list_node<T>*						NodePtr;
		typedef __list_node<T>  					ListNode;
	private:
		NodePtr head;	//ͷ�ڵ�ָ��
		NodePtr tail;    //β��ָ��
	public:
		list() //Ĭ�Ϲ��캯��
		{
			head = creat_node(T());
			head->_pre = head;
			head->_next = head;
			tail = head;
		}   
		list(size_type count, const T& value) //����count���ڵ㣬ÿ���ڵ��ֵΪvalue
		{
			head = creat_node(count, value);
			tail = head->_pre;
		}
		explicit list(size_type count) //����count���ڵ㣬ʹ��Ĭ��ֵ
		{
			head = creat_node(count, T());
			tail = head->_pre;
		}
		template< class InputIt >
		list(InputIt first, InputIt last)   //����һ�Է�Χ��Ҫ�������������������
		{
			list_aux(first, last, std::is_integral<InputIt>::type());
		}
		//��ֵ���캯��,ʹ�õ����������п���
		list(const list& other)
		{
			list_aux(other.begin(), other.end(), std::false_type());
		}
		//�ƶ����캯��
		list(list&& other) :head(other.cur), tail(other.tail)
		{
			other.head = other.tail = 0;
		}
		//�б��ʼ��
		list(std::initializer_list<T> init)
		{
			list_aux(std::begin(init), std::end(init), std::false_type());
		}

		//��������,ʹ�õ�����������
		~list()
		{
			free();
		}

		//������ֵ
		mylist& operator=(const mylist&rhs)
		{
			if (this != &rhs)
			{
				auto tmp = rhs;  //ʹ�ø�ֵ���캯��
				free();
				list_aux(tmp.begin(), tmp.end(), std::false_type());
			}
			return *this;
		}

		//�ƶ���ֵ
		mylist& operator=(mylist&&rhs)
		{
			if (this != &rhs)
			{
				this->head = rhs.head;
				this->tail = rhs.tail;
				rhs.head = rhs.tail = 0;
			}
			return *this;
		}
		//�б�ֵ
		mylist& operator=(std::initializer_list<T> init)
		{
			list_aux(std::begin(init), std::end(init), std::false_type());
			return *this;
		}

		//������
		
		iterator begin(){return iterator(head);}
		const_iterator begin() const{return const_iterator(head);}
		const_iterator cbegin() const{return const_iterator(head);}

		iterator end() { return iterator(tail); }
		const_iterator end() const { return const_iterator(tail); }
		const_iterator cend() const { return const_iterator(tail); }

		//Ԫ�ػ�ȡ
		reference front()
		{
			return head->data;
		}

		const_reference front() const
		{
			return head->data;
		}

		reference back()
		{
			return tail->_pre->data;
		}

		const_reference back()const
		{
			return tail->_pre->data;
		}
		
		//����
		bool empty() const
		{
			return (head == tail);
		}

		size_type size() const
		{
			size_type count = 0;
			auto cur = head;
			while (head != tail)
			{
				++count;
				cur = cur->_next;
			}
			return count;
		}

		size_type max_size() const
		{
			return size();
		}
	private://��������
		//����һ���ڵ�
		NodePtr get_node()    
		{
			return alloc_type::allocate();  //����һ���ڵ�
		}
		//�����ڵ�
		NodePtr creat_node(const T&x) 
		{
			auto nPtr = get_node();
			construct(&(nPtr->data), x);  //ʹ��ȫ�ֺ���construct
			return nPtr;
		}
		NodePtr creat_node(size_type count, const T&x)  //����n���ڵ�
		{
			NodePtr cur_tmp= creat_node(T());  //�ڱ��ڵ�,Ҳ��β��ڵ�
			NodePtr cur = cur_tmp;
			while (count--)     //��������
			{
				head = creat_node(x);
				head->_pre = cur_tmp;
				cur_tmp->_next = head;
				cur_tmp = head;
			}
			head->_next = cur;
			cur->_pre = head;
			return cur->_next;
		}
	
		//����һ���ڵ�
		void destory_node(const NodePtr&p)
		{
			alloc_type::destroy(p);
			alloc_type::deallocate(p);
		}
		//�������нڵ�
		void free()
		{
			if (head)
			{
				auto cur = head;
				auto tmp = head->_next; //tmpָ����һ���ڵ�
				for (; tmp != tail; tmp = tmp->_next)
				{
					destory_node(cur);
					cur = tmp;
				}
				destory_node(cur);
			}
		}
		//
		//list����һ�Ե�������Χ���캯��
		template< class InputIt >
		void list_aux(InputIt first, InputIt last,std::false_type)   
		{
			NodePtr cur_tmp = creat_node(T());  //β��ڵ�
			NodePtr cur = cur_tmp;
			for (; first != last; ++first)
			{
				head = creat_node(*first);
				head->_pre = cur_tmp;
				cur_tmp->_next = head;
				cur_tmp = head;
			}
			head->_next = cur;
			cur->_pre = head;
			head = cur->_next;
			tail = head->_pre;
		}

		template< class InputIt >
		void list_aux(InputIt count, InputIt value, std::true_type)
		{
			head =creat_node(count, value);
			tail = head->_pre;
		}




	};


}






#endif
