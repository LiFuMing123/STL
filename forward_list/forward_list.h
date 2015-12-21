#ifndef FORWARD_LIST_H
#define FORWARD_LIST_H

#include"Allocator.h"
#include"Iterator.h"
#include<initializer_list>

namespace MyCppSTL
{
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
		typedef MyCppSTL::allocator<_node<T>> _node_alloc;      //�ڵ������

		typedef _node<T>* _nodePtr;
	public:
		explicit forward_list()
		{
			cur = creat_node();
			head = tail = cur;
		}
		forward_list(size_type count, const T&value)
		{
			creat_node(count, value, head, tail);
		}
		explicit forward_list(size_type count)
		{
			creat_node(count, T(), head, tail);
		}
		template<class InputIt>
		forward_list(InputIt first, InputIt last)
		{
			_forward_list_dispatch(first, last, head, tail, std::is_integral<InputIt>::type());
		}
		//copy construct
		forward_list(const forward_list<T>&other)
		{
			_forward_list_dispatch(other.begin(), other.end(), head, tail, std::false_type());
		}
		forward_list(const std::initializer_list<int>&other)
		{
			_forward_list_dispatch(other.begin(), other.end(), head, tail, std::false_type());
		}

		//����
		~forward_list()
		{
			
		}
		//������
		iterator before_begin() { return iterator(--head); }
		iterator begin(){return iterator(head);}
		const_iterator begin() const{return const_iterator(head);}
		const_iterator cbegin() const{return const_iterator(head);}
		iterator end(){return iterator(tail);}
		const_iterator end() const { return const_iterator(tail); }
		const_iterator cend() const { return const_iterator(tail); }
		//��������
	private: 
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
		void destroy_node(_nodePtr node)
		{
			_node_alloc::destroy(node);
			_node_alloc::deallocate(node);
		}
		_nodePtr creat_node(size_type count, const T&value,_nodePtr& head,_nodePtr& tail)
		{
			_nodePtr head_tmp = creat_node(value);
			_nodePtr cur_tmp = head_tmp;
			_nodePtr tmp = cur_tmp;
			while (--count)
			{
				tmp = creat_node(value);
				cur_tmp->_next = tmp;
				cur_tmp = tmp;
			}
			tmp = creat_node();   //
			cur_tmp->_next = tmp;
			tmp->_next = NULL;  //��������Ϊ�ڷ���ռ��ʱ�򣬷�����Ҳ��һ�������ڴ��
			head = head_tmp;
			tail = tmp;

			return head_tmp;
		}
		template<class Integer>
		void _forward_list_dispatch(Integer count, Integer value, _nodePtr& head,_nodePtr& tail,std::true_type)
		{
			creat_node(count, value, head, tail);
		}
		template<class InputIter>
		void _forward_list_dispatch(InputIter first, InputIter last, _nodePtr& head, _nodePtr& tail,std::false_type)
		{
			_nodePtr head_tmp = creat_node(*first++);
			_nodePtr cur_tmp = head_tmp;
			_nodePtr tmp = cur_tmp;
			for(; first != last; ++first)
			{
				tmp=creat_node(*first);
				cur_tmp->_next = tmp;
				cur_tmp = tmp;
			}
			tmp = creat_node();   //
			cur_tmp->_next = tmp;
			tmp->_next = NULL; 
			head = head_tmp;
			tail = tmp;
		}

	private:
		_nodePtr head;     
		_nodePtr tail;
		_nodePtr cur; //ָ������ڵ��ָ��

	};



}// end of namespace

#endif   //end of forward_list.h file