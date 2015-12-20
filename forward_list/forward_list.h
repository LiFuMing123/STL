#ifndef FORWARD_LIST_H
#define FORWARD_LIST_H

#include"Allocator.h"
#include"Iterator.h"
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
		typedef _node<T>* _nodePtr;
	protected: //���ݳ�Ա
		_node<T>* node;
	public:
		//���캯��
		forward_list_const_iterator() :node() {}
		explicit forward_list_const_iterator(const struct _node&n) :node(n) {}
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
	class forward_list_iterator :public forward_list_const_iterator
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
		typedef _node<T>  nodePtr;
	public: //���캯��
		forward_list_list() :forward_list_const_iterator() {}
		explicit forward_list_iterator(const nodePtr&n) :forward_list_const_iterator(n) {}
		forward_list_list(const _MyIter&other) :forward_list_const_iterator(other.node) {}
		//
		_MyIter&operator=(const _MyIter&rhs)
		{
			node = rhs.node;
			return *this;
		}
		//����������
		_MyIter&operator++()
		{
			++*(_MyBase)this);
			return *this;
		}
		_MyIter operator(int)
		{
			auto tmp = *this;
			++*this;
			return tmp;
		}

	};





}// end of namespace

#endif   //end of forward_list.h file