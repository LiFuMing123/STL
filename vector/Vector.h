#ifndef VECTOR_H
#define VECTOR_H


#include<initializer_list>
#include<type_traits>
#include"Allocator.h"
#include"Algorithm.h"
#include"__Type_traits.h"
#include"Iterator.h"


namespace MyCppSTL{
	
	template<class T,class alloc=MyCppSTL::allocator<T>>
	class vector 
	{
	public:    //��Ƕ�ͱ���
		typedef T									value_type;
		typedef T*									pointer;
		typedef const T*							const_pointer;
		typedef T&									reference;
		typedef const T&							const_reference;
		typedef std::size_t							size_type;
		typedef std::ptrdiff_t						difference_type;
		typedef MyCppSTL::vector_iterator<T, alloc>	iterator;
		typedef alloc								alloc_type;
	private:   //��Ա
		pointer _first;    //����ͷ
		pointer _last;	   //����Ԫ��β
		pointer _end_of_storage;  //��������β
	public:	   
		//���캯��
		vector():_first(0), _last(0), _end_of_storage(0) {};	//Ĭ�Ϲ��캯��
		vector(size_type count, const T&val) //����һ��������һ����ֵ
		{
			allocate_and_fill(count, val);
		}
	  explicit vector(size_type count)//����һ������������Ԫ��Ĭ�ϵ�ֵ,��������ʽ����ת��
		{ 
			allocate_and_fill(count, T());
		}			
	     
	    //����һ��������
		//template<class InputIt>   
		//vector(InputIt first, InputIt last);					//����һ�Ե�����ָ��ķ�Χ
		template<class InputIt>
		vector(InputIt first, InputIt last)
		{
			vector_aux(first, last, std::is_integral<InputIt>::type());
		}
		//��������
		template<class InputIt>
		void vector_aux(InputIt first, InputIt last, std::true_type)
		{
			allocate_and_fill(first, last);
		}
		//��������,���ǵ�����
		template<class InputIt>
		void vector_aux(InputIt first, InputIt last, std::false_type)
		{
			//allocate_and_fill(first, last);
		}
		vector(const vector<T>&rhs);							//��ֵ���캯��
		vector(vector<T>&&rhs);									//�ƶ����캯��
		vector(std::initializer_list<T>&il);							//�б��ʼ��
		

		//��Ա����

	private:
		//��������,����ռ䲢��ָ��ֵ����ʼ��
		void allocate_and_fill(size_type n, const T&x)
		{
			_first = alloc::allocate(n);    //����ռ�
			pointer cur = _first;
			MyCppSTL::uninitialized_fill_n(cur, n, x);
			_last = _first + n;
			_end_of_storage = _last;
		}
		//��������
	

	
	};





}


#endif 
