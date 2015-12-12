#ifndef DEQUE_H
#define DEQUE_H

#include"Allocator.h"
#include"Iterator.h"
#include"Algorithm.h"
#include"__Type_traits.h"
#include<algorithm>
#include<initializer_list>




namespace MyCppSTL
{

	
	#define	_NODE_SIZE 8
	#define max(a,b)  (a>b)?(a):(b)
	inline std::size_t __deque_buf_size(std::size_t n) { return n < 512 ? (512 / n) : std::size_t(1); }

	/*  deque��const������ */
	template<class T,class alloc=MyCppSTL::allocator<T>>
	class deque_const_iterator :public iterator<MyCppSTL::random_access_iterator_tag, T>
	{
	
	public:
		//��Ƕ�ͱ�
		typedef  T							value_type;
		typedef  T&							reference;
		typedef const T&					const_reference;
		typedef  T*							pointer;
		typedef const T*					const_pointer;
		typedef std::size_t					size_type;
		typedef std::ptrdiff_t				difference_type;
		typedef deque_const_iterator<T>    _MyIter;
		typedef pointer*				   map_pointer;
		//�洢����С
		static inline std::size_t deque_buf_size(std::size_t n){return __deque_buf_size(n);}
	public:
		//��Ա
		pointer _cur; //ָʾ��ǰλ��
		pointer _first;
		pointer _last;
		map_pointer _node;    //ָʾ��ǰ���ڵ�map�ڵ�
	public:
		//Ĭ�Ϲ��캯��
		deque_const_iterator():_cur(0), _first(0), _last(0),_node(0)
		{
		}
		//���캯��
		deque_const_iterator(pointer cur,map_pointer node):_cur(cur),_node(node)
		{
			set_node(node);
		}
	   
		//�������캯��
		deque_const_iterator(_MyIter&rhs) :_cur(rhs._cur), _node(rhs._node), _first(rhs._first), _last(rhs._last)
		{
		}
		
		//����������
		const_reference operator*() const //ȡֵ
		{
			return *_cur;
		}
	
		//���������� ++
		_MyIter&operator++() //ǰ�õ���
		{
			++_cur;
			if (_cur == _last)   //����û�������ĩβ
			{
				set_node(_node + 1);
				_cur = _first;
			}
			return *this;
		}

		_MyIter operator++(int) //���õ���
		{
			auto tmp = *this;
			++*this;
			return tmp;
		}

		_MyIter&operator+=(difference_type n) //���ϸ�ֵ
		{
			difference_type offset = n + (_cur - _first);
			if (offset >= 0 && offset < (difference_type)deque_buf_size(sizeof(size_type)))
			{
				_cur += n;
			}
			else
			{
				difference_type node_offset = offset > 0 ? (offset / (difference_type)deque_buf_size(sizeof(size_type))) :
					(offset / (difference_type)deque_buf_size(sizeof(size_type))) - 1;

				set_node(_node + node_offset);
				_cur = _first + (offset - node_offset*deque_buf_size(sizeof(size_type)));
			}
			return *this;
		}

		_MyIter operator+(difference_type n) const//
		{
			auto tmp = *this;
			tmp += n;
			return tmp;
		}

		_MyIter&operator--() //ǰ�õݼ�
		{
			//--_cur;
			if (_cur == _first)   //����û�������ĩβ
			{
				set_node(_node - 1);
			}
			_cur = --_last;
			return *this;
		}

		_MyIter operator--(int) //���õݼ�
		{
			auto tmp = *this;
			--*this;
			return tmp;
		}
		_MyIter&operator-=(difference_type n) //���ϸ�ֵ
		{
			*this += -n;
			return *this;
		}

		difference_type operator-(const _MyIter&rhs) //����������
		{
			auto n_node = _node-rhs._node;
			auto distance_tmp = n_node*__deque_buf_size(sizeof(T));
			auto distance = distance_tmp + (_cur - _first);
			return distance;
		}

		const_reference operator[](const size_type n)
		{
			return *(*this + n);
		}

		//�߼�����
		bool operator==(_MyIter&rhs) const
		{
			return _cur == rhs._cur;
		}
		bool operator!=(_MyIter&rhs) const
		{
			return (!(*this == rhs));
		}
		bool operator<(_MyIter&rhs) const
		{
			return (_node == rhs._node ? _cur < rhs._cur : _node < rhs._node);
		}
		bool operator>(_MyIter&rhs) const
		{
			return (rhs < *this);
		}
		bool operator<=(_MyIter&rhs) const
		{
			return (!(*this>rhs));
		}

		bool operator>=(_MyIter&rhs) const
		{
			return (!(*this<rhs));
		}


	public:
		void set_node(map_pointer node)
		{
			_node = node;
			_first = *node;
			_last = _first + difference_type(deque_buf_size(sizeof(value_type)));
		}
		
	
	}; //end-of-const-iterator



template<class T, class alloc = MyCppSTL::allocator<T>>
class deque_iterator :public deque_const_iterator<T>
{
	public:
		//��Ƕ�ͱ�
		typedef  T							value_type;
		typedef  T&							reference;
		typedef const T&					const_reference;
		typedef  T*							pointer;
		typedef const T*					const_pointer;
		typedef std::size_t					size_type;
		typedef std::ptrdiff_t				difference_type;
		typedef deque_const_iterator<T>    _MyIter;
		typedef pointer*				   map_pointer;
		typedef deque_iterator<T>          _Myiter;
		typedef deque_const_iterator<T>    _MyBase;
	public:
		//���캯��
		//Ĭ�Ϲ��캯��
		deque_iterator() :deque_const_iterator() {}
		//�������������Ĺ��캯��
		deque_iterator(pointer cur, map_pointer node) :deque_const_iterator(cur, node) {}
		//�������캯��
		deque_iterator(_Myiter&rhs) :deque_const_iterator(rhs) {}
		
		//�������Ĳ���
		reference operator*()   //������
		{
			return (reference)**((_MyBase*)this);
		}
		_Myiter&operator++()  //ǰ�õ���
		{
			++*(_MyBase*)this;
			return *this;
		}
		_Myiter operator++(int) //���õ���
		{
			auto tmp = *this;
			++*this;
			return tmp;
		}
		_Myiter&operator--()  //ǰ�õݼ�
		{
			--*(_MyBase*)this;
			return *this;
		}
		_Myiter operator--(int) //���õݼ�
		{
			auto tmp = *this;
			--*this;
			return tmp;
		}

		_Myiter &operator+=(difference_type n) //
		{
			*(_MyBase*)this += n;
				return *this;
		}

		_Myiter operator+(difference_type n)
		{
			auto tmp = *this;
			tmp += n;
			return tmp;
		}

		_Myiter operator-(difference_type n)
		{
			auto tmp = *this;
			tmp += -n;
			return tmp;
		}

		_Myiter &operator-=(difference_type n)
		{
			return *this += -n;
		}

		reference operator[](const size_type &n)
		{
			return *(*this + n);
		}


};  //end of iterator

/***************/
//dequeģ��
template<class T,class alloc=MyCppSTL::allocator<T>>
class deque {
	public:
		typedef  T							value_type;
		typedef  T&							reference;
		typedef const T&					const_reference;
		typedef  T*							pointer;
		typedef const T*					const_pointer;
		typedef std::size_t					size_type;
		typedef std::ptrdiff_t				difference_type;
		typedef deque<T>					my_deque;

		typedef deque_const_iterator<T>		const_iterator;
		typedef deque_iterator<T>			iterator;
		typedef pointer*					map_pointer;
		
		typedef MyCppSTL::allocator<pointer>  map_alloc;
		enum {NODESIZE= _NODE_SIZE};
	public:
		//���캯��
	explicit deque()  //Ĭ�Ϲ��캯�� 
			{
				create_map(0);   //����map
			}
	
	deque(size_type count, const T& value)  //����һ��������ֵ��ʼ��
	{
		create_map_and_node(count, value);
	}

	explicit deque(size_type count)
	{
		create_map_and_node(count, T());
	}

	template< class InputIt >
	deque(InputIt first, InputIt last)
	{
		initialize_dispatch(first, last, std::is_integral<InputIt>::type());//����Ŀ������������������������������������ò�ͬ�ĺ���
	}

	deque(const deque& other) //�������캯��
	{
		initialize_dispatch(other.begin(), other.end(),std::false_type());
	}

	deque(deque&& other):_begin(other._begin),_end(other._end),_map(other._map),_map_size(other._map_size)
	{
		other._begin._cur= other._begin._first=other._begin._last=0;
		other._begin._node =0;
		other._end._cur =other._end._first = other._end._last = 0;
		other._end._node = 0;
		other._map = 0;
		other._map_size = 0;
	}

	deque(std::initializer_list<T> init)
	{
		range_initialize(init.begin(), init.end(), forward_iterator_tag());
	}
	//����
	~deque()
	{
		if (_begin._cur)
		{
			auto it = begin()._node;
			for (; it < end()._node; ++it)
			{
				destroy(*it, *it + __deque_buf_size(sizeof(T)));
				alloc::deallocate(*it, __deque_buf_size(sizeof(T)));
			}
			destroy(*it, *it + (end()._cur - *it));
			alloc::deallocate(*it, end()._cur - *it);
		}
	}

	//������
	
	iterator begin() { return iterator(_begin); }
	const_iterator begin() const 
	{ 
		return const_iterator(_begin._cur, _begin._node);
		
	}
	const_iterator cbegin() const 
	{
		return const_iterator(_begin._cur,_begin._node);
	}
	
	iterator end() { return _end; }
	const_iterator end() const 
	{ 
		return const_iterator(_end._cur,_end._node);
	}
	const_iterator cend() const
	{ 
		return const_iterator(_end._cur, _end._node); 
	}

	//Ԫ�ز���
	void push_back(const value_type&value)
	{
		if (_end._cur != _end._last)
		{
			construct(_end._cur, value);
		}
		else
		{
		
			
		}
	}
	protected:
		//��������������map
		std::size_t deque_buf_size(std::size_t n) { return __deque_buf_size(n); }
		void create_map(size_type elem_num);
		void create_map_and_node(size_type elem_num,value_type value);
		void node_move(map_pointer node);

		
		template<class InputIter>
		inline void initialize_dispatch(InputIter first, InputIter last, std::false_type);
		template<class Integer>
		inline void initialize_dispatch(Integer count, Integer value, std::true_type);

		template<class InputIter>
		void range_initialize(InputIter first, InputIter last, MyCppSTL::forward_iterator_tag);
		template<class InputIter>
		void range_initialize(InputIter first, InputIter last, MyCppSTL::input_iterator_tag);
		
		void push_back_aux(const value_type&value); //��map�ռ䲻����ʱ����·���
		void push_front_aux(const value_type&value);

		
		void reserve_map_at_back(size_type nodes_to_add = 1);
		void reserve_map_at_front(size_type nodes_to_add = 1);
		void reallocate_map(size_type nodes_to_add, bool add_at_front);

	private:
		map_pointer	_map; //ָ��map��ָ��
		size_type _map_size; //map��С
		iterator _begin; 
		iterator _end;   
	
	};  //end of deque


	//deque implement
	/***��������****/

	//����map

    template<class T, class alloc = MyCppSTL::allocator<T>>
	void deque<T, alloc>::create_map(size_type elem_num)
	{
		//������Ҫ���map�ڵ�
		//deque_buf_size���ص��ǵ����ڵ�洢��С
		size_type num_nodes = elem_num / deque_buf_size(sizeof(value_type)) +1;  
		_map_size = max(NODESIZE, num_nodes+2);   
		_map =map_alloc::allocate(_map_size);    //����ڵ�
		map_pointer nstart = _map + (_map_size - num_nodes) / 2;//���м俪ʼ
		map_pointer nend = nstart + num_nodes-1;    
		map_pointer cur = nstart;
		for (; cur <= nend; ++cur)
		{
			*cur=alloc::allocate(deque_buf_size(sizeof(value_type))); //Ϊÿ���ڵ����洢��
		}
		_begin.set_node(nstart);
		_end.set_node(nend);
		_begin._cur = _begin._first;
		_end._cur = _end._first + elem_num%deque_buf_size(sizeof(value_type));
	}

	template<class T,class alloc=MyCppSTL::allocator<T>>
	void deque<T, alloc>::create_map_and_node(size_type count, value_type value)
	{
		create_map(count);    //����map�ڵ㣬����ʼ����Ա
		map_pointer cur = _begin._node;
		for (; cur < _end._node - 1; ++cur)
		{
			uninitialized_fill(*cur, *cur + deque_buf_size(sizeof(value_type)), value);
		}
		uninitialized_fill(_end._first, _end._cur, value);
	}
	

	template<class T,class alloc=MyCppSTL::allocator<T>>
	template<class Integer>
	inline void deque<T,alloc>::initialize_dispatch(Integer count, Integer value, std::true_type)
	{
		create_map_and_node(count, value);
	}

	template<class T,class alloc=MyCppSTL::allocator<T>>
	template<class InputIter>      //�ǵ�����
	inline void deque<T, alloc>::initialize_dispatch(InputIter first, InputIter last,std::false_type)
	{
		//�����input_iterator,����push_back��ʵ��
		//�����forward_iterator����õĵ���������������ǵ�copy ���캯����ʵ��
		typedef typename iterator_traits<InputIter>::iterator_category Cat;
		range_initialize(first, last, Cat());   //������Ӧ�ĵ�����
	}

	template<class T,class alloc=MyCppSTL::allocator<T>>
	template<class ForwardIter>
	void deque<T, alloc>::range_initialize(ForwardIter first, ForwardIter last, MyCppSTL::forward_iterator_tag)
	{
		difference_type n = last - first;
		create_map(n);   //�����map�ڵ�,�����úõ�����
		iterator cur_iter = _begin;
		for (; n> 0; --n,++first)
		{
			construct(cur_iter._cur, *first);  //����
			++(cur_iter._cur);
			if (cur_iter._cur == cur_iter._last)
			{
				++cur_iter;
			}

		}
	}

	template<class T,class alloc=MyCppSTL::allocator<T>>
	template<class InputIter>
	void deque<T, alloc>::range_initialize(InputIter first, InputIter last, MyCppSTL::input_iterator_tag)
	{
		//����push_back�����
	}




}  




#endif 
