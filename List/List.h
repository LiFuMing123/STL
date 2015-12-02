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
		typedef MyCppSTL::list_iterator<T, alloc>	iterator;
		typedef MyCppSTL::const_list_iterator<T, alloc> const_iterator;
		typedef MyCppSTL::allocator<__list_node<T>>	alloc_type;   //����ڵ���
		typedef list<T, alloc>					    mylist;
		typedef T*									pointer;
		typedef const T*							const_pointer;
		typedef __list_node<T>*						NodePtr;
		typedef __list_node<T>  					ListNode;
	private:
		NodePtr cur;	//�ڵ�ָ��
		NodePtr end;    //β��ָ��
	public:
		list() //Ĭ�Ϲ��캯��
		{
			cur = creat_node(T());
			cur->_pre = cur;
			cur->_next = cur;
		}   
		list(size_type count, const T& value) //����count���ڵ㣬ÿ���ڵ��ֵΪvalue
		{
			cur = creat_node(count, value);
			end = cur->_pre;
		}
		explicit list(size_type count) //����count���ڵ㣬ʹ��Ĭ��ֵ
		{
			cur= creat_node(count, T());
			end = cur->_pre;
		}
		template< class InputIt >
		list(InputIt first, InputIt last)   //����һ�Է�Χ��Ҫ�������������������
		{
			list_aux(first, last, std::is_integral<InputIt>::type());
		}
		//��ֵ���캯��,ʹ�õ����������п���
		list(const list& other)
		{

		}
		//�ƶ����캯��
		list(list&& other) :cur(other.cur), end(other.end)
		{
			other.cur = other.end = 0;
		}
		//�б��ʼ��
		list(std::initializer_list<T> init)
		{
			list_aux(std::begin(init), std::end(init), std::false_type());
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
			NodePtr head = cur_tmp;
			while (count--)     //��������
			{
				cur = creat_node(x);
				cur->_pre = cur_tmp;
				cur_tmp->_next = cur;
				cur_tmp = cur;
			}
			cur->_next = head;
			head->_pre = cur;
			return head->_next;
		}
	
		//����һ���ڵ�
		void destory_node(const NodePtr&p)
		{
			alloc_type::destroy(&(p->data));
			alloc_type::deallocate(p);
		}
		//list����һ�Ե�������Χ���캯��
		template< class InputIt >
		void list_aux(InputIt first, InputIt last,std::false_type)   
		{
			NodePtr cur_tmp = creat_node(T());  //β��ڵ�
			NodePtr head = cur_tmp;
			for (; first != last; ++first)
			{
				cur = creat_node(*first);
				cur->_pre = cur_tmp;
				cur_tmp->_next = cur;
				cur_tmp = cur;
			}
			cur->_next = head;
			head->_pre = cur;
			cur = head->_next;
			end = cur->_pre;
		}

		template< class InputIt >
		void list_aux(InputIt count, InputIt value, std::true_type)
		{
			cur=creat_node(count, value);
			end = cur->_pre;
		}




	};


}






#endif
