#ifndef VECTOR_H
#define VECTOR_H


#include<initializer_list>
#include<numeric>
#include<type_traits>
#include"Allocator.h"
#include"Algorithm.h"
#include"__Type_traits.h"
#include"Iterator.h"
#include<utility>
#include<new>


namespace MyCppSTL{
	
	template<class T,class alloc=MyCppSTL::allocator<T>>
	class vector
	{
	public:    //��Ƕ�ͱ���
		typedef T									value_type;

		typedef T&									reference;
		typedef const T&							const_reference;
		typedef std::size_t							size_type;
		typedef std::ptrdiff_t						difference_type;
		typedef MyCppSTL::vector_iterator<T, alloc>	iterator;
		typedef MyCppSTL::vector_const_iterator<T, alloc> const_iterator;
		typedef alloc								alloc_type;
		typedef vector<T, alloc>					myVector;
		typedef T*									pointer;
		typedef const T*							const_pointer;
	private:   //��Ա
		pointer _first;    //����ͷ
		pointer _last;	   //����Ԫ��β
		pointer _end_of_storage;  //��������β
	public:
		//���캯��
		vector() :_first(0), _last(0), _end_of_storage(0) {};	//Ĭ�Ϲ��캯��
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
			allocate_and_copy(rhs._first, rhs._last);
		}
		vector(vector<T>&&rhs) :_first(rhs._first), _last(rhs._last), _end_of_storage(rhs._end_of_storage)//�ƶ����캯��
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
		const_iterator begin() const { return const_iterator(_first); }   //�����汾
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
		void reserve(size_type new_cap)//�ı�capacity,��new_capС�ڵ�ǰcap,�򲻱�
		{
			if (new_cap > capacity()) //���·���ռ䣬���Ѿ����ݿ������µĵ�ַ
			{
				pointer new_begin= alloc::allocate(new_cap);
				auto oldsize = size();
				pointer cur = new_begin;
				uninitialized_move(_first, _last, cur);
				free(_first, _last);
				_first = new_begin;
				_last = _first + oldsize;
				_end_of_storage = _first + new_cap;
				
			}
		}

		//������Ԫ�ص���ӣ��޸�
		void push_back(const T&value)
		{
			if (_last != _end_of_storage)//����Ҫ���·���ռ䣬ֱ����β�����Ԫ��
			{
				construct(_last, value);
				++_last;    //����
			}
			else   //�������·���ռ䣬���Ѿɵ�Ԫ�ؿ������µĿռ���,�����پɿռ�
			{
				const size_type old_size = size();
				const size_type new_size = ((size() == 0) ? 1 : 2 * size());  //���������Ĺ�ϵ����
				pointer new_begin = alloc::allocate(new_size);      //���·���ռ�
				pointer cur = new_begin;
				MyCppSTL::uninitialized_copy(_first, _last, cur);   //�������µĿռ���
				free(_first, _last);              //���پɿռ�
				_first = new_begin;				  //�����¿ռ��׵�ַ
				_last = _first + old_size + 1;    //�½�����ַ
				construct(_last - 1, value);        //���Ԫ��
				_end_of_storage = _first + new_size;
			}
		}


		void push_back(T&&value)
		{
			if (_last != _end_of_storage)
			{
				construct(_last, std::move(value));//�ƶ�����
				++_last;
			}
			else   //���·���ռ�
			{
				//const size_type old_size = size();
				const size_type new_size = ((size() == 0) ? 1 : 2 * size());  //���������Ĺ�ϵ����
				reserve(new_size);
				construct(_last, value);        //���Ԫ��
				++_last;
				//_end_of_storage = _first + new_size;
			}
		}

		void pop_back()
		{
			if (empty())
			{
				throw std::runtime_error("empty vector");
			}
			destroy(_last - 1);
			_last -= 1;
		}

		iterator erase(iterator pos)   //�Ƴ�ָ��λ�õ�Ԫ��
		{
			if (pos > end() || pos < begin())
			{
				throw std::out_of_range("out of range");
			}
			for (auto it = pos; it < end()-1; ++it)
			{
				*it = *(it + 1);
			}
			destroy(_last - 1);
			_last -= 1;
			return pos;
		}

		iterator erase(iterator first, iterator last) //�Ƴ�ָ������
		{
			difference_type n = last - first;
			auto pos = first;
			for (; last < end(); )
			{
				*first++ = *last++;
			}
			destroy(first, end());
			_last -= (size_type)n;
			return pos;
		}

		void clear()
		{
			erase(begin(), end());
		}

		void resize(size_type count, const T &value)
		{
			if (count < size())
			{
				erase(begin() + count, end());
			}
			else
			{
				insert(end(), count-size(), value);
			}
		}

		void resize(size_type count)
		{
			resize(count, T());
		}

		template< class... Args >
		iterator emplace(const_iterator pos, Args&&... args) //pos֮ǰֱ�ӹ���Ԫ��
		{
			auto pos_ptr = _first + (pos - begin());
			if ((size() + 1) <= capacity())//�ռ��㹻
			{
				
				for (auto it =_last - 1; it >= pos_ptr; --it)
				{
					construct(it + 1, *it);
				}
				construct(pos_ptr, std::forward<Args>(args)...);
				++_last;
				return iterator(pos_ptr);
			}
			else
			{
				const size_type new_size = ((size() == 0) ? 1 : 2 * size());  //���������Ĺ�ϵ����
				auto count = pos_ptr - _first;
				reserve(new_size);
				pos_ptr = _first + count;
				for (auto it = _last - 1; it >= pos_ptr; --it)
				{
					construct(it + 1, *it);
				}
				construct(pos_ptr, std::forward<Args>(args)...);
				++_last;
				return iterator(pos_ptr);
			}



		}

		template< class... Args >
		void emplace_back(Args&&... args)  //��ĩβ����Ԫ��
		{
			emplace(end(), args...);
		}


		iterator insert(iterator pos, const T&val)
		{
			/*�����ж����������Ԫ�غ�᲻�ᳬ��capacity*/
			if (pos > end())
			{
				throw std::out_of_range("out of range");
			}
			if ((size() + 1) <= capacity())
			{
				//����������һ��Ԫ��������һ���µ�Ԫ�أ�Ȼ������ľ͵������ƶ�
				construct(_last, *(_last - 1));
				++_last;
				for (auto it = pos+1; it < _last - 1; ++it)
				{
					*it = *(it - 1);
				}
				*pos = val;
				return pos;
			}
			else  //�ռ䲻������Ҫ���·���ռ�
			{
				difference_type distance = pos - begin();
				const size_type new_size = ((size() == 0) ? 1 : 2 * size());  //���������Ĺ�ϵ����
				reserve(new_size);
				auto new_pos = _first + distance;//�ָ�λ��
				insert(new_pos, val);  //�ݹ����
				return new_pos;
			}
		}

		template< class InputIt >
		void insert(iterator pos, InputIt first, InputIt last)//���ܵ�����ָ���ķ�Χ,Ҫ�������������������
		{
			insert_aux(pos, first, last, std::is_integral<InputIt>::type());
		}

		
		template<class T>
		void insert(iterator pos, size_type count, const T& value)//���ܵ�����ָ���ķ�Χ,Ҫ�������������������
		{
			insert_aux(pos, count, value, std::is_integral<T>::type());
		}
		

		//


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

	   //������������������
	   template<class InputIt>
	   void insert_aux(iterator pos, InputIt count, const value_type&value,std::true_type)//����count��value
	   {
		   if (pos > end())
		   {
			   throw std::out_of_range("out of range");
		   }
		   if ((size() + count) <= capacity())
		   {
			   difference_type distance = end() - pos;
			   uninitialized_fill_n(_last, count, *(_last - 1));//�ȹ����count���ռ�
			   if (((size_type)distance)<=((size_type)count))  //����Ԫ�ص�����count����pos��_lastԪ�ص�����
			   {
				   for (auto it = pos; it < _last; ++it) //�ƶ�Ԫ��
				   {
					   *(it + count) = *it;
				   }
			   }
			   else //����С��distance����ô���ƶ�β����Ԫ�أ����ƶ�pos֮���Ԫ��
			   {
				   for (auto it2 = end()-1; it2>pos-1; --it2)
				   {
					   *(it2 + count) = *it2;
				   }
			       for (auto it = pos; it < pos+count; ++it) //�ƶ�Ԫ��
				   {
						   *(it + count) = *it;
				    }
				
			   }
			   //  //����������ǲ��Ǻܲ��ã�
			   for (size_type n = 0; n<((size_type)count); ++n)//����Ԫ��
			   {
				   *pos++ = value;
			   }
			   _last += count;

		   }
		   else  //�ռ䲻��
		   {
			   difference_type distance = pos - begin();
			   const size_type new_size = ((size() == 0) ? 1 : (2 * (size() + count)));  //���������Ĺ�ϵ����
			   reserve(new_size);
			   auto new_pos = _first + distance;//�ָ�λ��
			   insert(new_pos, count, value);  //�ݹ����

		   }
	   }
	   //insert�ĸ����������ǵ�����
	   template< class InputIt >
	   void insert_aux(iterator pos, InputIt first, InputIt last,std::false_type)//���ܵ�����ָ���ķ�Χ,Ҫ�������������������
	   {
		   if ((pos > end()) || (last<first))
		   {
			   throw std::out_of_range("out of range");
		   }
		   difference_type distance = end() - pos;
		   difference_type count = last - first;//Ҫ����Ԫ�ص�����
		   if (((size_type)count + size()) <= capacity())//�пռ䣬����
		   {
			   uninitialized_fill_n(_last, count, *(_last - 1));//�ȹ����count���ռ�
			   if (((size_type)distance) <= ((size_type)count))  //����Ԫ�ص�����count����pos��_lastԪ�ص�����
			   {
				   for (auto it = pos; it < _last; ++it) //�ƶ�Ԫ��
				   {
					   *(it + count) = *it;
				   }
			   }
			   else //����С��distance����ô���ƶ�β����Ԫ�أ����ƶ�pos֮���Ԫ��
			   {
				   for (auto it2 = end() - 1; it2>pos - 1; --it2)
				   {
					   *(it2 + count) = *it2;
				   }
				   for (auto it = pos; it < pos + count; ++it) //�ƶ�Ԫ��
				   {
					   *(it + count) = *it;
				   }

			   }
			   for (size_type n = 0; n<((size_type)count); ++n)//����Ԫ��
			   {
				   *pos++ = *first++;
			   }
			   _last += count;
		   }
		   else//�ռ䲻��
		   {
			   difference_type distance = pos - begin();
			   const size_type new_size = ((size() == 0) ? 1 : (2 * (size() + count)));  //���������Ĺ�ϵ����
			   reserve(new_size);
			   auto new_pos = _first + distance;//�ָ�λ��
			   insert(new_pos, first, last);  //�ݹ����
		   }
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

	//�ǳ�Ա�������Ƚϲ���
	template<class T, class alloc = MyCppSTL::allocator<T>>
	bool operator==(const vector<T>&lhs, const vector<T>&rhs)
	{
		if (lhs.size() == rhs.size())
		{
			return (equal(lhs.begin(), lhs.end(), rhs.begin()));
		}

		return false;
	}

	template<class T, class alloc = MyCppSTL::allocator<T>>
	bool operator!=(const vector<T>&lhs, const vector<T>&rhs)
	{
		return !(lhs == rhs);
	}

	template<class T, class alloc = MyCppSTL::allocator<T>>
	bool operator<(const vector<T>&lhs, const vector<T>&rhs)
	{
		return(MyCppSTL::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}

	template<class T, class alloc = MyCppSTL::allocator<T>>
	bool operator>(const vector<T>&lhs, const vector<T>&rhs)
	{
		return (rhs < lhs);
	}

	template<class T, class alloc = MyCppSTL::allocator<T>>
	bool operator<=(const vector<T>&lhs, const vector<T>&rhs)
	{
		return !(lhs>rhs);
	}

	template<class T, class alloc = MyCppSTL::allocator<T>>
	bool operator>=(const vector<T>&lhs, const vector<T>&rhs)
	{
		return !(lhs < rhs);
	}

}


#endif 
