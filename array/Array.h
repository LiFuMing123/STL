#ifndef ARRAY_H
#define ARRAY_H

#include "Iterator.h"
#include "Algorithm.h"

namespace MyCppSTL
{
	
	/*array��*/
	template<class T,std::size_t N>
	struct Array    
	{
		//��Ƕ����
		typedef T										value_type;
		typedef T* 										pointer;
		typedef const T*								const_pointer;
		typedef T&										reference;
		typedef const T&								const_reference;
		typedef std::ptrdiff_t							difference_type;
		typedef std::size_t								size_type;
		typedef MyCppSTL::array_iterator<T,N>			iterator;		//����������
		typedef MyCppSTL::array_const_iterator<T, N>	const_iterator; //�����汾
		typedef MyCppSTL::reserve_const_iterator<T, N>	reserve_const_iterator;
		//const_iterator
		// 
		//���캯��
		/*
		*	����array�Ǿۺ��࣬�������Ĺ��캯��������ʽ�Ĺ��� ����
		*/
		//Ԫ�ػ�ȡ
		T &at(std::size_t pos) 
		{
			check_pos(pos);
			return _array[pos];
		}
		const T&at(std::size_t pos)const
		{
			check_pos(pos);
			return _array[pos];
		}
		T&operator[](std::size_t pos) { return _array[pos]; }
		const T&operator[](std::size_t pos) const { return _array[pos]; }
		T&front() { return _array[0]; }
		const T&front()const { return _array[0]; }
		T& back() { return _array[N-1]; }
		const T&back() const{ return _array[N - 1]; }
		//array������

		//����
		const std::size_t size() { return N; }
		const std::size_t size() const { return N; }
		const bool empty() { return N == 0 ? true : false; }
		const bool empty() const { return N == 0 ? true : false; }
		const std::size_t max_size() { return N; }
		const std::size_t max_size() const { return N; }

		//����
		void fill(const T&val)
		{
			MyCppSTL::fill((*this).begin(), (*this).end(), val);//���ڷ��ַ��͵����ͣ�ʹ��forѭ������ֵ��
		}
		void swap(Array<T, N>&rhs)//���������������ݣ�������ָ�������
		{
			//using std::swap;  
			MyCppSTL::swap(*this, rhs);

		}
		//����������
		iterator begin() { return iterator(_array); }
		const_iterator  begin() const { return const_iterator(_array);  }
		const_iterator cbegin() const { return const_iterator(_array); }
		iterator end() { return iterator(_array + N); }
		const_iterator end()const { return const_iterator(_array + N); }
		const_iterator cend()const { return const_iterator(_array + N); }
		reserve_const_iterator rbegin() { return reserve_const_iterator(_array + N - 1); } //reserve�汾

		T _array[N];
	private:
		void check_pos(std::size_t pos) 
		{
			if (pos > N)throw std::out_of_range("out of range");
		}
	};
	//�ǳ�Ա����
	//�Ƚϲ�����
	template<class T,std::size_t N>
	bool operator==(const Array<T, N>&lhs, const Array<T, N>&rhs)
	{
		return (MyCppSTL::equal(lhs.begin(), lhs.end(), rhs.begin()));
	}
	template<class T,std::size_t N>
	bool operator!=(const Array<T, N>&lhs, const Array<T, N>&rhs)
	{
		return (!(lhs == rhs));
	}
	template<class T,std::size_t N>
	bool operator<(const Array<T, N>&lhs, const Array<T, N>&rhs)
	{
		return MyCppSTL::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}
	template<class T, std::size_t N>
	bool operator>(const Array<T, N>&lhs, const Array<T, N>&rhs)
	{
		return (rhs < lhs);
	}

	template<class T, std::size_t N>
	bool operator<=(const Array<T, N>&lhs, const Array<T, N>&rhs)
	{				//lhs<=rhs
		return !(lhs>rhs);
	}
	template<class T, std::size_t N>
	bool operator>=(const Array<T, N>&lhs, const Array<T, N>&rhs)
	{				//lhs<=rhs
		return !(lhs<rhs);
	}
	template<std::size_t I,class T,std::size_t N>
	T&get(const Array<T, N>&rhs)
	{
		return const_cast<T&>(rhs[I]);
	}



	
}


#endif