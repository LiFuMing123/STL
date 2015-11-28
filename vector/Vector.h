#ifndef VECTOR_H
#define VECTOR_H


#include<initializer_list>
#include<numeric>
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
		typedef MyCppSTL::vector_const_iterator<T, alloc> const_iterator;
		typedef alloc								alloc_type;
		typedef vector<T, alloc>					myVector;
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
		vector(const vector<T>&rhs)							   //��ֵ���캯��
		{
			allocate_and_copy(rhs._first,rhs._last);
		}
		vector(vector<T>&&rhs):_first(rhs._first),_last(rhs._last),_end_of_storage(rhs._end_of_storage)//�ƶ����캯��
		{
			rhs._first = rhs._last = rhs._end_of_storage = 0;   //���ƺ�Դ���ڿ�����״̬
		}
		vector(std::initializer_list<T>il)							//�б��ʼ��
		{
			allocate_and_copy(il.begin(), il.end());
		}
		//��������
		~vector()
		{
			free(_first, _last);
		}
		/*******************************************************************/

		//��Ա����
		//��ֵ����
		myVector& operator=(const myVector &rhs)
		{
			if (this != &rhs) //���߲���ͬһ������
			{
				free(_first, _last);
				allocate_and_copy(rhs._first, rhs._last);
			}
			return *this;
		}
		//�ƶ���ֵ����
		myVector operator=(const myVector&&rhs)
		{
			if (*this != &rhs)
			{
				free(_first, _last);
				_first = rhs.begin();
				_last = rhs.end();
				_end_of_storage = rhs._end_of_storage;

				rhs.begin = rhs.end() = rhs._end_of_storage = 0;   //
			}
		}

		//Ԫ�ػ�ȡ
		reference at(size_type pos) 
		{
			check(pos);
			return *(_first + pos); 
		}
		const_reference at(size_type pos) const  //������
		{
			check(pos);
			return *(_first + pos);
		}
		reference operator[](size_type pos) //�±����
		{
			return _first[pos];
		}
		const_reference operator[](size_type pos) const //�����汾
		{
			return _first[pos];
		}
		reference front()
		{
			return *begin();
		}
		const_reference front()const   //�����汾
		{
			return *cbegin();     
		}

		reference back()
		{
			return *(end() - 1);
		}
		const_reference back() const
		{
			return *(cend() - 1);
		}

		//������
		const_iterator begin() const{ return const_iterator(_first); }   //�����汾
		const_iterator end() const { return const_iterator(_last); }     //
		const_iterator cbegin() const { return const_iterator(_first); }
		const_iterator cend() const { return const_iterator(_last); }
		iterator begin() { return iterator(_first); }   //�ǳ����汾
		iterator end() { return iterator(_last); }
		//



		//����(capacity)
		size_type capacity() const   //����
		{
			return (_end_of_storage - _first);
		}
		size_type size()const //������Ԫ�ش�С
		{
			return (_last - _first);
		}
		size_type max_size() const
		{
			return (std::numeric_limits<value_type>::max());
		}
		bool empty() const
		{
			return (_first == _last);
		}
		void reserve(size_type new_cap);
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

	   //��������,���������������ͺ͵���������
	   template<class InputIt>
	   void vector_aux(InputIt count, InputIt val, std::true_type)//������std::true_type����Ϊ�õ��ǿ⺯����trait
	   {
		   allocate_and_fill(count, val);
	   }
	   //��������,���ǵ�����
	   template<class InputIt>
	   void vector_aux(InputIt first, InputIt last, std::false_type)
	   {
		   allocate_and_copy(first, last);
	   }

	   //�������������ٷ���Ŀռ�
	   template<class InputIter>
	   void free(InputIter first,InputIter last)
	   {
		   if (first)
		   {
			   destroy(first, last);
			   alloc_type::deallocate(_first, _end_of_storage - _first);
		   }
	   }
	   //�����������߽���
	   void check(const size_type &pos)
	   {
		   if (pos >= size())
		   {
			   throw std::out_of_range("out of range");
		   }
	   }
	   
	   
	
	};





}


#endif 
