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
		//typedef MyCppSTL::vector_iterator<T, alloc>	iterator;
		typedef MyCppSTL::vector_const_iterator<T, alloc> const_iterator;
		typedef alloc								alloc_type;
	private:   //��Ա
		pointer _first;    //����ͷ
		pointer _last;	   //����Ԫ��β
		pointer _end_of_storage;  //��������β
	public:	   
		//���캯��
		vector():_first(0), _last(0), _end_of_storage(0) {};	//Ĭ�Ϲ��캯��
	    explicit vector(size_type count)//����һ������������Ԫ��Ĭ�ϵ�ֵ,��������ʽ����ת��
		{ 
			allocate_and_fill(count, T());
		}	

	  //����һ�Ե�����ָ��ķ�Χ,��������������
		template<class InputIt>
		vector(InputIt first, InputIt last)
		{
			vector_aux(first, last, std::is_integral<InputIt>::type());
		}
		//��������
		template<class InputIt>
		void vector_aux(InputIt count, InputIt val, std::true_type)//������std::true_type����Ϊ�õ��ǿ⺯����trait
		{
			allocate_and_fill(count, val);
		}
		//��������,���ǵ�����
		template<class InputIt>
		void vector_aux(InputIt first, InputIt last, std::false_type)
		{
			/*
			_first = alloc::allocate(last-first);    //����ռ�
			pointer cur = _first;
			uninitialized_copy(first, last, cur);
			_last = _end_of_storage = _first + (last - first);  //����Ҫ�����ţ���Ϊ���Ͳ�ͬ
			*/
			allocate_and_copy(first, last);
		}
		vector(const vector<T>&rhs)							   //��ֵ���캯��
		{
			/*
			_first = alloc::allocate(rhs._last - rhs._first);	//����ռ�
			pointer cur = _first;
			uninitialized_copy(rhs._first, rhs._last, cur);
			_last = _end_of_storage = _first + (rhs._last - rhs._first);
			*/
			allocate_and_copy(rhs.begin(),rhs.end());
		}
		vector(vector<T>&&rhs):_first(rhs._first),_last(rhs._last),_end_of_storage(rhs._end_of_storage)//�ƶ����캯��
		{
			rhs._first = rhs._last = rhs._end_of_storage = 0;   //���ƺ�Դ���ڿ�����״̬
		}
		vector(std::initializer_list<T>il)							//�б��ʼ��
		{
			/*
			_first= alloc::allocate(il.end()-il.begin());
			pointer cur = _first;
			uninitialized_copy(il.begin(), il.end(), cur);
			_last = _end_of_storage = _first+(il.size());
			*/
			allocate_and_copy(il.begin(), il.end());
		}

		

		//��Ա����
		const_iterator begin() const{ return const_iterator(_first); }   //�����汾
		const_iterator end() const { return const_iterator(_last); }     //


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
		
	   template<class InputIterator>
	   void allocate_and_copy(InputIterator first, InputIterator last)
	   {
		   _first = alloc::allocate(last-first);
		   pointer cur = _first;
		   uninitialized_copy(first, last, cur);
		   _last = _end_of_storage = _first + (last-first);
	   }
	   
	
	};





}


#endif 
