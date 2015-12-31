#ifndef STRING_H
#define STRING_H

#include"Allocator.h"
#include"Iterator.h"


namespace MyCppSTL
{
	template<class CharT=char>
	class string_const_iterator:public iterator<MyCppSTL::random_access_iterator_tag,CharT>
	{
	public: //��Ƕ�ͱ���
		typedef const CharT value_type;
		typedef const CharT& reference;
		typedef const CharT* pointer;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;
		typedef string_const_iterator<CharT> _MyIter;
	private: //����
		pointer _p; //ָ��ǰλ��
	public:
		//���캯��
		string_const_iterator() :_p(0) {}   
		string_const_iterator(pointer p) :_p(p) {}
		string_const_iterator(const _MyIter&other)  //copy
		{
			if (this != &other)
			{
				_p = other._p;
			}
		}
		//����
		reference operator*() const
		{
			return *_p;
		}
		reference operator[](size_type n) const
		{
			return *(_p + n);
		}
		pointer operator->() const
		{
			return &*_p;
		}

		_MyIter&operator++() //ǰ�õ���
		{
			++_p;
			return *this;
		}

	};


	template<class CharT=char,class Alloc=MyCppSTL::allocator<CharT>>
	class string
	{
		
	};  // end-string



}  //end-namespace


#endif  //end-string.h  
