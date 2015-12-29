#ifndef FORWARD_LIST_H
#define FORWARD_LIST_H

#include"Allocator.h"
#include"Iterator.h"
#include<initializer_list>
#include<type_traits>

namespace MyCppSTL
{

	template<class T, class alloc>class forward_list; //ǰ������

	//����ڵ�
	template<class T>
	struct _node
	{
		typedef T value_type;
		_node<value_type>*_next;
		value_type data;
	};

	//����ĵ�����,�����汾
	template<class T>
	class forward_list_const_iterator:public iterator<MyCppSTL::forward_iterator_tag,T>
	{
	public:
		friend forward_list<T, MyCppSTL::allocator<T>>;
	public: //��Ƕ�ͱ�
		typedef const T value_type;
		typedef const T& reference;
		typedef const T* pointer;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;

		typedef MyCppSTL::forward_iterator_tag iterator_category;
		typedef forward_list_const_iterator<T> _MyIter;
		typedef _node<T>*  _nodePtr;
	protected: //���ݳ�Ա
		_node<T>* node;
	public:
		//���캯��
		forward_list_const_iterator() :node() {}
		explicit forward_list_const_iterator(const _nodePtr&n) :node(n) {}
		forward_list_const_iterator(const _MyIter&other) :node(other.node) {}
		//
		_MyIter&operator=(_MyIter&other)
		{
			node = other.node;
			return *this;
		}
		//
		//����������
		reference operator*() const
		{
			return node->data;
		}
		pointer operator->()const
		{
			return &(operator*())
		}
		_MyIter&operator++() //ǰ�õ���
		{
			node = node->_next;
			return *this;
		}
		_MyIter operator++(int)
		{
			auto tmp =*this;
			++*this;
			return tmp;
		}
		//�߼�����
		bool operator==(const _MyIter&rhs)
		{
			return (node == rhs.node);
		}
		bool operator!=(const _MyIter&rhs)
		{
			return !(*this == rhs);
		}
	};

	//���������ǳ����汾
	template<class T>
	class forward_list_iterator :public forward_list_const_iterator<T>
	{
	public://��Ƕ�ͱ�
		typedef T value_type;
		typedef T& reference;
		typedef const T& const_reference;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;

		typedef forward_list_iterator<T> _MyIter;
		typedef forward_list_const_iterator<T> _MyBase;
		typedef _node<T>*  nodePtr;
	public: //���캯��
		forward_list_iterator() :forward_list_const_iterator() {}
		explicit forward_list_iterator(const nodePtr&n) :forward_list_const_iterator(n) {}
		forward_list_iterator(const _MyIter&other) :forward_list_const_iterator(other.node) {}
		//
		_MyIter&operator=(const _MyIter&rhs)
		{
			node = rhs.node;
			return *this;
		}
		reference operator*()
		{
			return (reference)**(_MyBase*)this;
		}
		//����������
		_MyIter&operator++()
		{
			++*((_MyBase*)this);
			return *this;
		}
		_MyIter operator++(int)
		{
			auto tmp = *this;
			++*this;
			return tmp;
		}

	};


	//forward_list ģ�嶨��
	template<class T,class alloc=MyCppSTL::allocator<T>>
	class forward_list
	{
		//��Ƕ�ͱ���
	public:
		typedef T          value_type;
		typedef T&         reference;
		typedef const T&   const_reference;
		typedef T*         pointer;
		typedef const T*   const_pointer;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t diference_type;
		
		typedef forward_list_const_iterator<T> const_iterator;
		typedef forward_list_iterator<T>  iterator;
		typedef forward_list<T, alloc> _MyList;
		typedef MyCppSTL::allocator<_node<T>> _node_alloc;      //�ڵ������


		typedef _node<T>* _nodePtr;
	public:
		explicit forward_list()
		{
			cur = creat_node();
			head = cur;
			head->_next = NULL;
		}
		forward_list(size_type count, const T&value)
		{
			creat_node(count, value, head);
		}
		explicit forward_list(size_type count)
		{
			creat_node(count, T(), head);
		}
		template<class InputIt>
		forward_list(InputIt first, InputIt last)
		{
			_forward_list_dispatch(first, last, head,std::is_integral<InputIt>::type());
		}
		//copy construct
		forward_list(const forward_list<T>&other)
		{
			_forward_list_dispatch(other.begin(), other.end(), head,std::false_type());
		}
		forward_list(const std::initializer_list<int>&other)
		{
			_forward_list_dispatch(other.begin(), other.end(), head,std::false_type());
		}
		
		_MyList operator=(const _MyList&other)
		{
			if (this != &other)
			{
				destroy_all(head); //
				_forward_list_dispatch(other.begin(), other.end(), head, std::false_type());
			}
			return *this;
		}
		_MyList operator=(_MyList&&other)
		{
			head = other.head;
			cur = other.cur;
			other.head = NULL;
			other.cur = NULL;
		}
		_MyList &operator=(std::initializer_list<T> ilist)
		{
			_forward_list_dispatch(ilist.begin(), ilist.end(), std::false_type());
			return *this;
		}
		//����
		~forward_list()
		{
			destroy_all(head);
		}
		//������
		iterator before_begin() { return iterator(head); }
		iterator begin(){return iterator(head->_next);}
		const_iterator begin() const{return const_iterator(head->_next);}
		const_iterator cbegin() const{return const_iterator(head->_next);}
		iterator end(){return iterator();}
		const_iterator end() const { return const_iterator(); }
		const_iterator cend() const { return const_iterator(); }
		//access
		reference front()
		{
			return *begin();
		}
		const_reference front() const
		{
			return *begin();
		}
		bool empty() const
		{
			return (head->_next == NULL);
		}
		void clear()
		{
			destroy_all(head);
		}

		
		iterator insert_after(const_iterator pos, const T&value)
		{
			return insert_after_aux(pos, value);
		}

		/* ����move���塣����
		iterator insert_after(const_iterator pos, T&&value)
		{
			return insert_after_aux(pos, value);
		}
		*/
		iterator insert_after(const_iterator pos, size_type count, const T&value)
		{
			auto tmp = pos;
			while (count--)
			{
				insert_after(pos, value);
				pos++;
			}
			return iterator(tmp.node->_next);
		}
		template<class InputIt>
		iterator insert_after(const_iterator pos, InputIt first, InputIt last)
		{
			return (insert_after_dispatch(pos, first, last, std::is_integral<InputIt>::type()));
		}

		iterator insert_after(const_iterator pos, std::initializer_list<T>ilist)
		{
			return (insert_after(pos, ilist.begin(), ilist.end()));
		}

		void push_front(const T&value)
		{
			insert_after(before_begin(), value);
		}

		//move�汾
		/**/
		iterator erase_after(const_iterator&pos)
		{
			auto tmp = pos.node->_next;
			pos.node->_next = pos.node->_next->_next;
			tmp->_next = NULL;
			destroy_node(tmp);
			return iterator(pos.node->_next);
		}

		iterator erase_after(const_iterator&first, const_iterator&last)
		{
			
			for (;  first.node->_next!= last.node;)
			{
				erase_after(first);
			}
			return iterator(first.node);
		}

		void pop_front()
		{
			erase_after(before_begin());
		}

		size_type size()
		{
			size_type count = 0;
			auto cur = head;
			while (cur->_next != NULL)
			{
				++count;
				cur = cur->_next;

			}
			return(count);
		}

		void resize(size_t count,const value_type&value)
		{
			size_t len = 0;
			auto it = before_begin();
			for (; next(it)!=end() && len < count; ++it, ++len) 
			{

			}
			if (len == count) //��ǰ���ȳ���count
			{
				erase_after(it,end());
			
			}
			else //���ȴ���count
			{
				while (len != count)
				{
					insert_after(it,value);
					++it;
					++len;
				}
			}
		}

		void resize(size_t count)
		{
			resize(count, T());
		}

		void swap(_MyList&other)
		{
			auto tmp = head;
			auto tmp_cur = cur;
			head = other.head;
			cur = other.cur;
			other.head = tmp;
			other.cur = tmp_cur;
		}
		
		template<class...Args>
		iterator emplace_after(iterator pos, Args&& ...args)
		{
			return insert_after_aux(pos, std::forward<Args>(args)...);
		}

		template<class...Args>
		iterator emplace_front(Args&&...args)
		{
			return insert_after_aux(before_begin(), std::forward<Args>(args)...);
		}
		
		
		void merge(forward_list<T>&other)
		{
			if (this != &other)
			{
				iterator cur1 = before_begin();
				iterator cur2 = other.before_begin();
				iterator first1 = begin();
				iterator first2 = other.begin();
				for (; (first1 != end() && first2 != other.end()); )
				{
					if (*first1 > *first2)
					{
						insert_after(cur1, *first2);
						cur2 = first2;
						++first2;
					}
					else
					{
						cur1 = first1;
						++first1;
					}

				}
				if (first2 != other.end())insert_after(cur1, first2, other.end());
			}
		}

		template<class Comp>
		void merge(forward_list<T>&other, Comp comp)
		{
			if (this != &other)
			{
				iterator cur1 = before_begin();
				iterator cur2 = other.before_begin();
				iterator first1 = begin();
				iterator first2 = other.begin();
				for (; (first1 != end() && first2 != other.end()); )
				{
					if (!comp(*first1,*first2))
					{
						insert_after(cur1, *first2);
						cur2 = first2;
						++first2;
					}
					else
					{
						cur1 = first1;
						++first1;
					}

				}
				if (first2 != other.end())insert_after(cur1, first2, other.end());
			}
		}
		
		void splice_after(const_iterator pos, forward_list<T>&other, const_iterator it)
		{
			if (pos != it)
			{
				insert_after(pos, *it);
				auto first = other.before_begin().node;
				while (first->_next != it.node)
				{
					first = first->_next;
				}
				erase_after(const_iterator(first));

			}

		}

		void splice_after(const_iterator pos, forward_list<T>&other)
		{
			if (this != &other)
			{
				insert_after(pos, other.begin(), other.end());
				auto n = other.before_begin().node;
				destroy_all(n);
			}
		}

		void splice_after(const_iterator pos, forward_list& other, const_iterator first, const_iterator last)
		{
			if ((this != &other) && (!other.empty()))
			{
				insert_after(pos, first, last);
				auto it = other.before_begin();
				while (it.node->_next != first.node)
				{
					++it;
				}
				erase_after(it, last);
			}
		}

		void remove(const T& value)
		{
			auto first = begin();
			for (; first != end(); ++first)
			{
				if (first.node->_next->data == value)
				{
					erase_after(first);
				}
			}
		}

		void reverse()
		{
			if (size() <= 1)return;
			_nodePtr pre = begin().node;
			_nodePtr cur = pre->_next;
			_nodePtr next =cur;
			pre->_next = NULL;
			while (cur != NULL)
			{
				next = cur->_next;
				cur->_next = pre;
				pre = cur;
				cur = next;
			}

			head->_next = pre;
		}

		void unique()
		{
			_nodePtr cur = head->_next;
			_nodePtr next = cur->_next;
			while (next != NULL)
			{
				if (cur->data == next->data)
				{
					next = next->_next;
					erase_after(iterator(cur));
				}
				else
				{
					cur = next;
					next = next->_next;
				}
			}
		}

		//��������
	private: 

		iterator next(iterator it)
		{
			return ++it;
		}
	
		_nodePtr creat_node()//��������ڵ�
		{
			_nodePtr node_tmp = _node_alloc::allocate();
			return node_tmp;
		}
		_nodePtr creat_node(const T&value)
		{
			_nodePtr tmp = creat_node();
			MyCppSTL::construct(&(tmp->data), value);
			return tmp;
		}
		void destroy_node(_nodePtr &node)
		{
			_node_alloc::destroy(node);
			_node_alloc::deallocate(node);
		}
		void destroy_all(_nodePtr&head)
		{
			auto before_head = head;
			head = head->_next;
			auto tmp = head;
			while (tmp != NULL)
			{
				tmp = tmp->_next;
				destroy_node(head);
				
				head = tmp;
			}
			_node_alloc::deallocate(before_head);
			before_head = NULL;
		}
		_nodePtr creat_node(size_type count, const T&value,_nodePtr& head)
		{
			_nodePtr head_tmp = creat_node();
			_nodePtr cur_tmp = head_tmp;
			_nodePtr tmp = cur_tmp;
			while (count--)
			{
				tmp = creat_node(value);
				cur_tmp->_next = tmp;
				cur_tmp = tmp;
			}
			tmp->_next = NULL; 
			head = head_tmp;
			return head_tmp;
		}
		template<class Integer>
		void _forward_list_dispatch(Integer count, Integer value, _nodePtr& head,std::true_type)
		{
			creat_node(count, value, head, tail);
		}
		template<class InputIter>
		void _forward_list_dispatch(InputIter first, InputIter last, _nodePtr& head,std::false_type)
		{
			_nodePtr head_tmp = creat_node();
			_nodePtr cur_tmp = head_tmp;
			_nodePtr tmp = cur_tmp;
			for(; first != last; ++first)
			{
				tmp=creat_node(*first);
				cur_tmp->_next = tmp;
				cur_tmp = tmp;
			}
			tmp->_next = NULL; 
			head = head_tmp;
		}

		iterator insert_after_aux(const_iterator pos, const T&value)
		{
			auto insert_node = creat_node(value);
			insert_node->_next = pos.node->_next;
			pos.node->_next = insert_node;
			return iterator(pos.node->_next);
		}
		template<class InputIt>
		iterator insert_after_dispatch(const_iterator pos, InputIt first, InputIt last,std::false_type)
		{
			auto tmp = pos;
			for(; first != last; ++first)
			{
				insert_after_aux(pos, *first);
				++pos;
			}
			return iterator(tmp.node->_next);
		}

		template<class Integral>
		iterator insert_after_dispatch(const_iterator pos, Integral count, Integral value, std::true_type)
		{
			auto tmp = pos;
			while (count--)
			{
				insert_after_aux(pos, value);
				++pos;
			}
			return iterator(tmp.node->_next);
		}

	private:
		_nodePtr head;     
		_nodePtr cur; //ָ������ڵ��ָ��

	};



}// end of namespace

#endif   //end of forward_list.h file