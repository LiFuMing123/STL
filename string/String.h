#ifndef STRING_H
#define STRING_H

#include"Allocator.h"
#include"Iterator.h"
#include"Algorithm.h"


namespace MyCppSTL
{


	class string_const_iterator:public iterator<MyCppSTL::random_access_iterator_tag,char>
	{
	public: //��Ƕ�ͱ���
		typedef const char value_type;
		typedef const char& reference;
		typedef const char* pointer;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;
		typedef string_const_iterator _MyIter;
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
		_MyIter operator++(int) //���õ���
		{
			auto tmp = *this;
			++*this;
			return tmp;
		}
		_MyIter&operator--() //ǰ�õݼ�
		{
			--_p;
			return *this;
		}
		_MyIter operator--(int)
		{
			auto tmp = *this;
			--*this;
			return tmp;
		}

		_MyIter&operator+=(size_type n)
		{
			_p += n;
			return *this;
		}
		_MyIter operator+(size_type n)
		{
			auto tmp = *this;
			tmp += n;
			return tmp;
		}
		_MyIter&operator-=(size_type n)
		{
			_p -= n;
			return *this;
		}
		_MyIter operator-(size_type n)
		{
			auto tmp = *this;
			tmp -= n;
			return tmp;
		}

		difference_type operator-(_MyIter&other)
		{
			return (_p - other._p);
		}

		//�߼�����
		bool operator==(_MyIter& other)
		{
			return (_p == other._p);
		}
		bool operator!=(_MyIter& other)
		{
			return !(*this == other);
		}
	};


	class string_iterator:public string_const_iterator
	{
	public:
		typedef char value_type;
		typedef char& reference;
		typedef char* pointer;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;

		typedef string_const_iterator _MyBase;
		typedef string_iterator _MyIter;

		//construct
		string_iterator() :string_const_iterator() {}
		string_iterator(pointer p) :string_const_iterator(p) {}
		string_iterator(string_const_iterator&it) :string_const_iterator(it) {}

		//
		reference operator*()
		{
			return (reference)**(_MyBase*)this;
         }
		reference operator[](size_type n)
		{
			return (reference)(*(_MyBase*)this).operator[](n);
			//reference (*(_MyBase*)this)[n];
		}
		
		//ǰ�õ���
		_MyIter&operator++()
		{
			++*(_MyBase*)this;
			return *this;
		}
		//���õ���
		_MyIter operator++(int)
		{
			auto tmp = *this;
			++*this;
			return tmp;
		}
		//ǰ�õݼ�
		_MyIter operator--()
		{
			--*(_MyBase*)this;
			return *this;
		}
		//���õݼ�
		_MyIter operator--(int)
		{
			auto tmp = *this;
			--*this;
			return tmp;
		}

		//���ϼ�
		_MyIter&operator+=(size_type n)
		{
			*(_MyBase*)this += n;
			return *this;
		}
		//��
		_MyIter operator+(size_type n)
		{
			auto tmp = *this;
			tmp += n;
			return tmp;
		}
		//���ϼ�
		_MyIter&operator-=(size_type n)
		{
			*(_MyBase*)this -= n;
			return *this;
		}
		//��
		
		_MyIter operator-(size_type n)
		{
			auto tmp = *this;
			tmp -= n;
			return tmp;
		}
		
		//�������ľ���
		difference_type operator-(_MyIter&other)
		{
			return (*(_MyBase*)this - (*(_MyBase*)&other));
		}
		//�߼�����

	};


	class string
	{
	public:
		typedef char         value_type;
		typedef char*        pointer;
		typedef const char*  const_pointer;
		typedef  char&       reference;
		typedef  const char& const_reference;
		typedef std::size_t   size_type;
		typedef std::ptrdiff_t difference_type;

		typedef MyCppSTL::allocator<char>  alloc;     //�ռ������
		typedef string_const_iterator    const_iterator;
		typedef string_iterator          iterator;

		
	private: //member
		pointer _start;     //ָ���ַ����Ŀ�ʼ
		pointer _finish;    //ָ���ַ���ĩβ
		pointer _end_of_storage; //ָ��洢�ռ�ĩβ.
		enum { _default_size = 8 };
	public:
		static const size_type npos;
		//construct
		explicit string():_start(0),_finish(0),_end_of_storage(0) //default construct,empty string
		{
			allocate_block(_default_size);
		}
		string(size_type count, char value) //count value
		{
			allocate_block(count+1);   //����ռ�
			_finish = _start + count;
			uninitialized_fill(_start, _finish, value);  //
			*_finish = char();
		}
		string(const string&other, size_type pos, size_type count = string::npos)
		{
			if (pos > size())
			{
				std::out_of_range("out of range");
			}
			range_initializer(other._start + pos, other._start + pos+min(count,size()-pos));
		}

		string(const char*s, size_type count)
		{
			range_initializer(s, s + count);
		}

		//������
		iterator begin() { return iterator(_start); }
		const_iterator begin()const { return const_iterator(_start); }
		const_iterator cbegin()const { return const_iterator(_start); }
		iterator end() { return iterator(_finish); }
		const_iterator end() const { return const_iterator(_finish); }
		const_iterator cend() const { return const_iterator(_finish); }

		size_type size() const { return _finish - _start; }
		//��������
	private:
		void allocate_block(size_type n) //����ռ�
		{
			_start = alloc::allocate(n);
			_finish = _start;
			_end_of_storage = _start + n;
		}
		void deallocate_block()        //�ͷſռ�
		{
			alloc::deallocate(_start, _end_of_storage - _start);
			
		}
		template<class Iterator>
		void range_initializer(Iterator first, Iterator last)
		{
			auto n = last - first;
			allocate_block(n+1); //����ռ�
			MyCppSTL::uninitialized_copy(first, last, _start);
			*_finish = char();   //��ӽ�����
		}
	};  // end-string

	const string::size_type string::npos =(string::size_type)-1;



}  //end-namespace


#endif  //end-string.h  
