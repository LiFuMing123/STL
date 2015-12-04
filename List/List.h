#ifndef LIST_H
#define LIST_H

#include<initializer_list>
#include"Allocator.h"
#include"Iterator.h"
#include"Construct.h"
#include<cassert>

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
			head=list_aux(first, last, std::is_integral<InputIt>::type());
			tail = head->_pre;
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
			free(head,tail);
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
			while (cur != tail)
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


		//�޸�Ԫ��
		iterator insert(iterator pos, const value_type& value)//��pos֮ǰ����Ԫ��
		{
			auto tmp = creat_node(value);
			tmp->_next = pos.getMynode();
			tmp->_pre = pos.getMynode()->_pre;  
			pos.getMynode()->_pre->_next = tmp;
			(pos.getMynode())->_pre = tmp;
			if (head == pos.getMynode())head = head->_pre;  //����ͷ�����룬��ͷ���ƶ�
			return --pos;
		}

		void insert(iterator pos, size_type count, const T& value)  //����n��Ԫ��
		{
			NodePtr sub_list=creat_node(count, value);   //�ȹ�����count������
			NodePtr sub_tail = sub_list->_pre;
			link_list(pos, sub_list, sub_tail);
		}

		
		template< class InputIt >     //Ҫ����������������ʱ������
		void insert(iterator pos, InputIt first, InputIt last)
		{
			insert_aux(pos, first, last, std::is_integral<InputIt>::type());
		}
		
		void push_back(const value_type&data)
		{
			insert(end(), data);

		}
		void push_back(value_type&&data) //�ƶ��汾
		{
			insert(end(), data);
		}
		void pop_back()
		{
			auto tmp = tail->_pre;
			tail->_pre->_pre->_next = tail;
			tail->_pre = tail->_pre->_pre;
			destory_node(tmp);
		}
		void push_front(const value_type& data)
		{
			insert(begin(), data);

		}

		void pop_front()
		{
			auto tmp = head;
			tail->_next = head->_next;
			head->_next->_pre = tail;
			head = tail->_next;   //ͷ���ƶ�
			destory_node(tmp);
		}

		void clear()   //���
		{
			tail = head;
			head->_next = tail;
			tail->_pre = head;
		}

		iterator erase(iterator pos)
		{
			auto tmp = pos.getMynode();
			if (head == tmp)head = tmp->_next;
			assert(!(tmp == tail));
			++pos;
			tmp->_pre->_next = tmp->_next;
			tmp->_next->_pre = tmp->_pre;
			tmp->_pre = tmp->_next = 0;
			destory_node(tmp);
			return pos;
		}

		
		iterator erase(const_iterator pos)
		{
			auto tmp = pos.getMynode();
			if (head == tmp)head = tmp->_next;
			assert(!(tmp == tail));
			++pos;
			tmp->_pre->_next = tmp->_next;
			tmp->_next->_pre = tmp->_pre;
			tmp->_pre = tmp->_next = 0;
			destory_node(tmp);
			return iterator(pos.getMynode());
		}

		iterator erase(iterator first, iterator last)
		{
			auto first_node = first.getMynode();
			auto last_node = last.getMynode();
			if (head == first_node)head = last_node;
			first_node->_pre->_next = last_node;
			last_node->_pre = first_node->_pre;
			free(first_node, last_node->_pre);
			return last;
		}

		iterator erase(const_iterator first, const_iterator last)
		{
			auto first_node = first.getMynode();
			auto last_node = last.getMynode();
			if (head == first_node)head = last_node;
			first_node->_pre->_next = last_node;
			last_node->_pre = first_node->_pre;
			free(first_node, last_node->_pre);
			return iterator(last_node);          //ʹ��ģ�������⸴��ճ��������
		}


		void resize(size_type count)
		{
			if (count >= size())
			{
				insert(end(), count - size(), T());
			}
			else
			{
				if (count == 0) 
				{
					clear();
					return;
				}
				auto tmp = head;
				while (--count)
				{
					tmp = tmp->_pre;
				}
				tmp->_next = tail;
				tail->_pre = tmp;
			}
		}
		void resize(size_type count, const value_type& value)
		{
			
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
			NodePtr head_tmp = cur_tmp;
			while (count--)     //��������
			{
				head_tmp = creat_node(x);
				head_tmp->_pre = cur_tmp;
				cur_tmp->_next = head_tmp;
				cur_tmp = head_tmp;
			}
			head_tmp->_next = cur;
			cur->_pre = head_tmp;
			return cur->_next;
		}

		template<class InputIt>   //����ڵ㣬���뷶Χ�ǵ�����
		NodePtr creat_node(InputIt first, InputIt last,std::false_type)
		{
			NodePtr cur_tmp = creat_node(T());  //β��ڵ�
			NodePtr cur = cur_tmp;
			NodePtr head_tmp = cur_tmp;
			for (; first != last; ++first)
			{
				head_tmp = creat_node(*first);
				head_tmp->_pre = cur_tmp;
				cur_tmp->_next = head_tmp;
				cur_tmp = head_tmp;
			}
			head_tmp->_next = cur;
			cur->_pre = head_tmp;
			return cur->_next;

		}
	
		//����һ���ڵ�
		void destory_node(const NodePtr&p)
		{
			alloc_type::destroy(p);
			alloc_type::deallocate(p);
		}
		//�������нڵ�
		void free(NodePtr&begin,NodePtr&end)
		{
			if (begin)
			{
				auto cur = begin;
				auto tmp = begin->_next; //tmpָ����һ���ڵ�
				for (; tmp != end; tmp = tmp->_next)
				{
					destory_node(cur);
					cur = tmp;
				}
				destory_node(cur);
			}
		}


		//������������һ���������ӵ�������ָ��λ��
		void link_list(iterator pos, NodePtr&sub_list, NodePtr&sub_tail)
		{
			sub_tail->_pre->_next = pos.getMynode();
			sub_list->_pre = pos.getMynode()->_pre;
			pos.getMynode()->_pre->_next = sub_list;
			pos.getMynode()->_pre = sub_tail->_pre;
			if (head == pos.getMynode())head = sub_list;
			destory_node(sub_tail);
		}
		//
		//list����һ�Ե�������Χ���캯��
		template< class InputIt >
		NodePtr list_aux(InputIt first, InputIt last,std::false_type)
		{
			return (creat_node(first, last, std::false_type()));
		}

		template< class InputIt >
		NodePtr list_aux(InputIt count, InputIt value, std::true_type)
		{
			head =creat_node(count, value);
			return head;
		}

		
		template< class InputIt >    
		void insert_aux(iterator pos, InputIt first, InputIt last,std::false_type) //������������
		{
			NodePtr sub_list = creat_node(first, last, std::false_type());
			NodePtr sub_tail = sub_list->_pre;
			link_list(pos, sub_list, sub_tail);
		}

		template< class InputIt >
		void insert_aux(iterator pos, InputIt first, InputIt last, std::true_type) //���ǵ�����
		{
			NodePtr sub_list = creat_node(first, last);
			NodePtr sub_tail = sub_list->_pre;
			link_list(pos, sub_list, sub_tail);
		}
		

	};


}






#endif
