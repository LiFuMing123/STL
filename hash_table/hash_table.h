#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H


#include<utility>
#include"Allocator.h"
#include"hash_function.h"
#include"Vector.h"
#include"function.h"
#include"Iterator.h"


namespace MyCppSTL
{

	//hash table node
	template<typename value>
	struct hashtable_node
	{
		hashtable_node<value>*next;
		value v;
	};

	//ǰ������
	template<typename value, typename key, typename HashFun, typename ExtractKey, typename EqualKey, typename alloc>
	class hash_table;

	//const ������
	template<typename value,typename key,typename HashFun,typename ExtractKey,typename EqualKey,typename alloc=MyCppSTL::allocator<value>>
	class hash_table_const_iterator:public iterator<MyCppSTL::forward_iterator_tag,value>
	{
	public://��Ƕ�ͱ���
		using _hash_table = hash_table<value, key, HashFun, ExtractKey, EqualKey, alloc>;
		using _myIter = hash_table_const_iterator<value, key, HashFun, ExtractKey, EqualKey, alloc>;
		using node = hashtable_node<value>;  
		using value_type = value;
		using reference = value&;
		using const_reference = const value&;
		using pointer = value*;
		using const_pointer = const value*;
		using difference_type = std::ptrdiff_t;
		using size_type = std::size_t;
	public://��Ա����
		const node* _cur;
		const _hash_table* _htp;
	public:
		//���캯��
		hash_table_const_iterator(const node*n,const _hash_table* h) :_cur(n), _htp(h) {}
		hash_table_const_iterator(const hash_table_const_iterator&rhs) :_cur(rhs._cur), _htp(rhs._htp) {} //copy construct
		hash_table_const_iterator() {}  //default

		//��Ա����
		const_reference operator*() const { return _cur->v; }
		const_pointer operator->() const { return &operator*(); }

		_myIter&operator++();  //ǰ��++
		_myIter operator++(int); //����++

		bool operator==(const _myIter& rhs) const
		{
			return _cur == rhs._cur;
		}

		bool operator!=(const _myIter& rhs) const
		{
			return !(*this == rhs);
		}
	};
	//��const������
	template<typename value, typename key, typename HashFun, typename ExtractKey, typename EqualKey, typename alloc = MyCppSTL::allocator<value>>
	class hash_table_iterator :public hash_table_const_iterator<value, key, HashFun, ExtractKey, EqualKey>
	{
		//��Ƕ�ͱ���
	public:
		using _myIter = hash_table_iterator<value, key, HashFun, ExtractKey, EqualKey>;
		using _myBase = hash_table_const_iterator<value, key, HashFun, ExtractKey, EqualKey>;
		using value_type = value;
		using reference = value&;
		using const_reference = const value&;
		using pointer = value*;
		using const_pointer = const value*;
		using difference_type = std::ptrdiff_t;
		using size_type = std::size_t;
	public:
		//���캯��
		hash_table_iterator(const node*n, const _hash_table*htp) :hash_table_const_iterator(n, htp) {}
		hash_table_iterator() :hash_table_const_iterator() {}
		hash_table_iterator(const _myIter&rhs) :hash_table_const_iterator(rhs._cur, rhs._htp) {}
		//��Ա����
		reference operator*() const
		{
			return (reference)_cur->v;
		}
		pointer operator->() const
		{
			return &operator*();
		}

		_myIter& operator++()
		{
			++*(_myBase*)this;
			return *this;
		}

		_myBase operator++(int)
		{
			auto tmp = *this;
			++*this;
			return tmp;
		}

		//�߼�����
		//�����������

	};

	enum { num_primes = 28 };
	static const unsigned long primes_list[num_primes]=    //��ϣ���С��Ϊ������С
	{
		53ul,         97ul,         193ul,       389ul,       769ul,
		1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
		49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
		1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
		50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul,
		1610612741ul, 3221225473ul, 4294967291ul
	}; 


	inline unsigned long next_prime(unsigned long n)
	{
		const unsigned long*first = primes_list;
		const unsigned long*last = primes_list + num_primes;
		const unsigned long *pos = MyCppSTL::lower_bound(first, last, n);
		return pos == last ? *(last - 1) : *pos;
	}


	//hash table
	template<typename value,typename key,typename HashFun,typename ExtractKey,typename EqualKey,typename alloc=MyCppSTL::allocator<value>>
	class hash_table
	{
		//��Ԫ
	public:
		friend class hash_table_const_iterator<value,key,HashFun,ExtractKey,EqualKey,alloc>;
		friend class hash_table_iterator<value, key, HashFun, ExtractKey, EqualKey, alloc>;
		//��Ƕ�ͱ�
	public:
		using key_type = key;
		using value_type = value;
		using hasher = HashFun;
		using key_equal = EqualKey;

		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using pointer = value*;
		using const_pointer = const value*;
		using reference = value&;
		using const_reference = const value&;

		using iterator = hash_table_iterator<value, key, HashFun, ExtractKey, EqualKey>;
		using const_iterator = hash_table_const_iterator<value, key, HashFun, ExtractKey, EqualKey, alloc>;
	private:  //���ݳ�Ա
		using node = hashtable_node<value>;
		using hash_table_node_alloc = MyCppSTL::allocator<node>;  //�ڵ����
		hasher hash;    //��ϣ����
		key_equal equals; //�жϼ�ֵ���
		ExtractKey get_key;  //��ü�ֵ
		MyCppSTL::vector<node*> buckets;  //��ϣ��
		size_type element_num;  //Ԫ�ظ���

	public:
		//���캯��
		hash_table(size_type n, const HashFun&hf, const EqualKey&eql, const ExtractKey&ext) :
			hash(hf), equals(eql), get_key(ext), element_num(n)
		{
				//���г�ʼ��
			initialize_buckets(n);
		}
		hash_table(const hash_table& rhs):hash(rhs.hash),equals(rhs.equals),get_key(rhs.get_key),element_num(0) //copy
		{
			copy_from(rhs);
		}

		hash_table& operator=(const hash_table& rhs) //operator=
		{
			if (this != &rhs)
			{
				clear();
				hash = rhs.hash;
				equals = rhs.equals;
				get_key = rhs.get_key;
				element_num = rhs.element_num;
				copy_from(rhs);
				
			}
			return *this;
		}
		//��������
		~hash_table()
		{
			clear();  //��Ͱ���Ԫ�ض��ͷ��˺󣬻����vector�������������пռ��ͷ�
		}
		//��Ա����
		size_type size() const { return element_num; }
		size_type max_size() const { return size_type(-1); }
		bool empty() const{ return size() == 0; }
		void clear();

		//������
		iterator begin()
		{
			for (size_type i = 0; i < buckets.size(); ++i)
			{
				if (buckets[i])
				{
					return iterator(buckets[i], this);
				}
			}
			return end();
		}

		iterator end()
		{
			return iterator(0, this);
		}

		const_iterator begin() const
		{
			for (size_type i = 0; i < buckets.size(); ++i)
			{
				if (buckets[i])
				{
					return const_iterator(buckets[i], this);
				}
			}
			return end();
		}

		const_iterator end() const
		{
			return const_iterator(0, this);
		}

		size_type buckets_count() const { return buckets.size(); }    //Ͱ��Ŀ
		size_type max_buckets_count() const { return primes_list[num_primes - 1]; } //���Ͱ��Ŀ
		size_type elems_in_bucket(size_type bucket) const
		{
			size_type res = 0;
			for (node*cur = buckets[bucket]; cur; cur = cur->next)
			{
				++res;
			}
			return res;
		}

		//insert
		std::pair<iterator, bool> insert_unique(const value_type& value);
		iterator insert_equal(const value_type&value);
		std::pair<iterator, bool>insert_unique_noresize(const value_type&value);
		iterator insert_equal_noresize(const value_type&value);
		template<typename InputIt>
		void insert_unique(InputIt first, InputIt last);
		template<typename InputIt>
		void insert_equal(InputIt first, InputIt last);
		template<typename InputIt>
		void insert_unique(InputIt first, InputIt last, MyCppSTL::input_iterator_tag);
		template<typename InputIt>
		void insert_equal(InputIt first, InputIt last, MyCppSTL::input_iterator_tag);
		template<typename InputIt>
		void insert_unique(InputIt first, InputIt last, MyCppSTL::forward_iterator_tag);
		template<typename InputIt>
		void insert_equal(InputIt first, InputIt last, MyCppSTL::forward_iterator_tag);



		//find
		iterator find(const key_type&key);
		const_iterator find(const key_type&key) const;

		size_type count(const key_type&key) const;
		// equal_range(lower_bound~upper_bound)
		std::pair<iterator, iterator>equal_range(const key_type&key);
		std::pair<const_iterator, const_iterator>equal_range(const key_type&key) const;

		//erase
		size_type erase(const key_type &key);
		void erase(const iterator& it);
		void erase(iterator&first, iterator&second);
		void erase(const const_iterator &cit);
		void erase(const_iterator&cfirst, const_iterator&clast);

		void resize(size_type element_num);

	private://��������
		size_type next_size(size_type n) const
		{
			return next_prime(n);
		}
		void initialize_buckets(size_type n) //hash ��ĳ�ʼ��
		{
		    size_type n_buckets = next_size(n);
			buckets.reserve(n_buckets);
			buckets.insert(buckets.end(), n_buckets, (node*)0);
			element_num = 0;
		}
		//����ʵֵ��buckets�ĸ���
		size_type bkt_num(const value_type&obj, size_t n) const
		{
			return bkt_num_key(get_key(obj), n);
		}
		//ֻ����ʵֵ
		size_type bkt_num(const value_type&obj) const
		{
			return bkt_num_key(get_key(obj));   //get_key ͬ����һ���º���
		}
		//ֻ���ܼ�ֵ
		size_type bkt_num_key(const key_type&key) const
		{
			return bkt_num_key(get_key(key), buckets.size());
		}
		//���ܼ�ֵ��buckets����
		size_type bkt_num_key(const key_type&key, size_t n) const
		{
			return hash(key) % n;     //hash() ��һ���º���
		}

		//�����µĽڵ�
		node*new_node(const value_type&obj)
		{
			node* n = hash_table_node_alloc::allocate();
			n->next = NULL;
			MyCppSTL::construct(&n->v, obj);
			return n;
		}

		//ɾ���ڵ�
		void delete_node(node*n)
		{
			MyCppSTL::destroy(&n->v);  ////
			hash_table_node_alloc::deallocate(n);
		}

		void copy_from(const hash_table&rhs);
		
	};

	//�������ĳ�Ա��������
	template<typename value, typename key, typename HashFun, typename ExtractKey, typename EqualKey, typename alloc>
	hash_table_const_iterator<value,key,HashFun,ExtractKey,EqualKey,alloc>&
	hash_table_const_iterator<value, key, HashFun, ExtractKey, EqualKey, alloc>::operator++()  //ǰ��++
	{
		const node*old_node = _cur;
		_cur = _cur->next;
		if (!_cur)
		{
		    size_type bulk_num = _htp->bkt_num(old_node->v);
			while (!_cur && (++bulk_num < _htp->buckets.size()))
			{
				_cur = _htp->buckets[bulk_num];
			}
		}

		return *this;
	}

	template<typename value, typename key, typename HashFun, typename ExtractKey, typename EqualKey, typename alloc>
	hash_table_const_iterator<value, key, HashFun, ExtractKey, EqualKey, alloc>
	hash_table_const_iterator<value, key, HashFun, ExtractKey, EqualKey, alloc>::operator++(int)  //ǰ��++
	{ 
		auto temp = *this;
		++*this;
		return temp;
	}


	/***************************************************************/

	template<typename value, typename key, typename HashFun, typename ExtractKey, typename EqualKey, typename alloc>
	void hash_table<value, key, HashFun, ExtractKey, EqualKey, alloc>::clear()
	{
		for (size_t i = 0; i < buckets.size(); ++i)
		{
			node* cur = buckets[i];
			while (cur != 0)
			{
				node* next = cur->next;
				delete_node(cur);
				cur = next;
			}
			buckets[i] = NULL;
		}
		element_num = 0;
	}
	//insert_unique
	template<typename value, typename key, typename HashFun, typename ExtractKey, typename EqualKey, typename alloc>
	std::pair<typename hash_table<value,key,HashFun,ExtractKey,EqualKey,alloc>::iterator,bool>
		hash_table<value, key, HashFun, ExtractKey, EqualKey, alloc>::insert_unique_noresize(const value_type&value)
	{
		auto loc = bkt_num(value);
		for (node*cur = buckets[loc]; cur; cur = cur->next)
		{
			if (equals(get_key(value),get_key(cur->v)))return std::pair<iterator, bool>(iterator(cur,this), false);
		}
		//û����ͬ��Ԫ��
		node* n = new_node(value);
		n->next = buckets[loc];
		buckets[loc] = n;
		++element_num;
		return std::pair<iterator, bool>(iterator(n,this), true);
	}

	//insert_unique
	template<typename value, typename key, typename HashFun, typename ExtractKey, typename EqualKey, typename alloc>
	std::pair<typename hash_table<value, key, HashFun, ExtractKey, EqualKey, alloc>::iterator, bool>
		hash_table<value, key, HashFun, ExtractKey, EqualKey, alloc>::insert_unique(const value_type&value)
	{
		resize(element_num + 1);
		return insert_unique_noresize(value);
	}

	//resize
	template<typename value, typename key, typename HashFun, typename ExtractKey, typename EqualKey, typename alloc>
	void hash_table<value, key, HashFun, ExtractKey, EqualKey, alloc>::resize(size_type element_n)
	{
		const size_type old_size = buckets.size();
		const size_type new_size = next_prime(element_n);
		if (element_n > old_size)
		{
			vector<node*> tmp(new_size);  //�µ�buckets
			//���£�����Ͱ�ڵ�Ԫ�ؿ�������Ͱ��
			for (size_type i = 0; i < old_size; ++i)
			{
				node* first = buckets[i];
				while (first)
				{
					size_type new_pos = bkt_num(first->v, new_size);
					buckets[i] = first->next;  //������һ��
					first->next = tmp[new_pos];
					tmp[new_pos] = first;
					first = buckets[i];
				}
			}
			//copy ���
			buckets.swap(tmp);  
		}
	}

	//insert_equal
	template<typename value, typename key, typename HashFun, typename ExtractKey, typename EqualKey, typename alloc>
	typename hash_table<value, key, HashFun, ExtractKey, EqualKey, alloc>::iterator 
		hash_table<value, key, HashFun, ExtractKey, EqualKey, alloc>::insert_equal_noresize(const value_type&value)
	{
		const size_type pos = bkt_num(value);   //���㼸��Ͱ
		node* cur = buckets[pos];
		for (cur; cur; cur = cur->next) //�����Ƿ�����ͬ��Ԫ��
		{
			if (equals(get_key(value),get_key(cur->v)))//����ͬ��
			{
				node* tmp = new_node(value);
				tmp->next = cur->next;
				cur->next = tmp;
				++element_num;
				return iterator(cur, this);
			}
		}

		//û����ͬ��Ԫ��
		node* tmp = new_node(value);
		tmp->next = cur;
		buckets[pos] = tmp;
		++element_num;
		return iterator(buckets[pos], this);
	}

	template<typename value, typename key, typename HashFun, typename ExtractKey, typename EqualKey, typename alloc>
	typename hash_table<value, key, HashFun, ExtractKey, EqualKey, alloc>::iterator
		hash_table<value, key, HashFun, ExtractKey, EqualKey, alloc>::insert_equal(const value_type&value)
	{
		resize(element_num+1);
		return insert_equal_noresize(value);

	}

	//insert_unique
	template<typename value, typename key, typename HashFun, typename ExtractKey, typename EqualKey, typename alloc>
	template<typename InputIt>
	inline void hash_table<value, key, HashFun, ExtractKey, EqualKey, alloc>::insert_unique(InputIt first, InputIt last)
	{
		insert_unique(first, last, MyCppSTL::iterator_traits<InputIt>::iterator_category());
	}

	//insert equal
	template<typename value, typename key, typename HashFun, typename ExtractKey, typename EqualKey, typename alloc>
	template<typename InputIt>
	inline void hash_table<value, key, HashFun, ExtractKey, EqualKey, alloc>::insert_equal(InputIt first, InputIt last)
	{
		insert_equal(first, last, MyCppSTL::iterator_traits<InputIt>::iterator_category());
	}

	template<typename value, typename key, typename HashFun, typename ExtractKey, typename EqualKey, typename alloc>
	template<typename InputIt>
	inline void hash_table<value, key, HashFun, ExtractKey, EqualKey, alloc>::insert_unique(InputIt first, InputIt last, MyCppSTL::input_iterator_tag)
	{
		for (first; first != last; ++first)
			insert_unique(*first);
	}

	//equal
	template<typename value, typename key, typename HashFun, typename ExtractKey, typename EqualKey, typename alloc>
	template<typename InputIt>
	inline void hash_table<value, key, HashFun, ExtractKey, EqualKey, alloc>::insert_equal(InputIt first, InputIt last, MyCppSTL::input_iterator_tag)
	{
		for (first; first != last; ++first)
			insert_equal(*first);
	}

	template<typename value, typename key, typename HashFun, typename ExtractKey, typename EqualKey, typename alloc>
	template<typename InputIt>
	inline void hash_table<value, key, HashFun, ExtractKey, EqualKey, alloc>::insert_unique(InputIt first, InputIt last, MyCppSTL::forward_iterator_tag)
	{
		size_type n = 0;
		n = MyCppSTL::distance(first, last);
		resize(element_num + n);
		for (; n >0; --n,++first)
			insert_unique_noresize(*first);
	}

	//equal
	template<typename value, typename key, typename HashFun, typename ExtractKey, typename EqualKey, typename alloc>
	template<typename InputIt>
	inline void hash_table<value, key, HashFun, ExtractKey, EqualKey, alloc>::insert_equal(InputIt first, InputIt last, MyCppSTL::forward_iterator_tag)
	{
		size_type n = 0;
		n = MyCppSTL::distance(first, last);
		resize(element_num + n);
		for (; n >0; --n, ++first)
			insert_equal_noresize(*first);
	}

	//find
	template<typename value, typename key, typename HashFun, typename ExtractKey, typename EqualKey, typename alloc>
	typename hash_table<value, key, HashFun, ExtractKey, EqualKey, alloc>::iterator 
		hash_table<value, key, HashFun, ExtractKey, EqualKey, alloc>::find(const key_type&key)
	{
		const size_type pos = bkt_num(key);   //�����ֵ���ڵ�Ͱ
		node* cur = buckets[pos];
		for (; cur&&equals(get_ket(cur->v),key); cur = cur->next)
		{
		}
		return iterator(cur, this);
	}

	template<typename value, typename key, typename HashFun, typename ExtractKey, typename EqualKey, typename alloc>
	typename hash_table<value, key, HashFun, ExtractKey, EqualKey, alloc>::const_iterator
		hash_table<value, key, HashFun, ExtractKey, EqualKey, alloc>::find(const key_type&key) const
	{
		const size_type pos = bkt_num(key);   //�����ֵ���ڵ�Ͱ
		node* cur = buckets[pos];
		for (; cur&&equals(get_ket(cur->v), key); cur = cur->next)
		{
		}
		return const_iterator(cur, this);
	}

	template<typename value, typename key, typename HashFun, typename ExtractKey, typename EqualKey, typename alloc>
	typename hash_table<value, key, HashFun, ExtractKey, EqualKey, alloc>::size_type
		hash_table<value, key, HashFun, ExtractKey, EqualKey, alloc>::count(const key_type&key) const
	{
		size_type res = 0;
		const size_type pos = bkt_num(key);
		node*cur = buckets[pos];
		for (; cur; cur = cur->next)
		{
			if (equals(get_key(cur->v), key))++res;
		}
		return res;
	}

	template<typename value, typename key, typename HashFun, typename ExtractKey, typename EqualKey, typename alloc>
	std::pair<typename hash_table<value, key, HashFun, ExtractKey, EqualKey, alloc>::iterator, 
		      typename hash_table<value, key, HashFun, ExtractKey, EqualKey, alloc>::iterator>
		hash_table<value, key, HashFun, ExtractKey, EqualKey, alloc>::equal_range(const key_type&key)
	{
		const size_type pos = bkt_num(key);   //�����Ԫ�����ڵ�Ͱ
		node* cur = buckets[pos];  
		for (; cur; cur = cur->next)
		{
			if (equals(get_key(cur->v), key))
			{
				for (node*second = cur->next; second; second = second->next)
				{
					if (!equals(get_key(second->v), key))
					{
						return std::pair<iterator, iterator>(iterator(cur, this), iterator(second, this));
					}
				}
				for (auto i = pos + 1; i < buckets.size();++i)//ִ�е�����˵������һ��Ͱ��û�к�Ŀ�겻��ȵ�
				{
					if(buckets[i])return std::pair<iterator, iterator>(iterator(cur, this), iterator(buckets[i], this));
				}
			}
		}
		return std::pair<iterator, iterator>(end(),end());
	}

	template<typename value, typename key, typename HashFun, typename ExtractKey, typename EqualKey, typename alloc>
	std::pair<typename hash_table<value, key, HashFun, ExtractKey, EqualKey, alloc>::const_iterator,
		typename hash_table<value, key, HashFun, ExtractKey, EqualKey, alloc>::const_iterator>
		hash_table<value, key, HashFun, ExtractKey, EqualKey, alloc>::equal_range(const key_type&key) const
	{
		const size_type pos = bkt_num(key);   //�����Ԫ�����ڵ�Ͱ
		const node* cur = buckets[pos];
		for (; cur; cur = cur->next)
		{
			if (equals(get_key(cur->v), key))
			{
				for (const node*second = cur->next; second; second = second->next)
				{
					if (!equals(get_key(second->v), key))
					{
						return std::pair<const_iterator, const_iterator>(const_iterator(cur, this), const_iterator(second, this));
					}
				}
				for (auto i = pos + 1; i < buckets.size(); ++i)//ִ�е�����˵������һ��Ͱ��û�к�Ŀ�겻��ȵ�
				{
					if (buckets[i])return std::pair<const_iterator, const_iterator>(const_iterator(cur, this), const_iterator(buckets[i], this));
				}
			}
		}
		return std::pair<const_iterator, const_iterator>(end(), end());
	}
	
	//erase
	template<typename value, typename key, typename HashFun, typename ExtractKey, typename EqualKey, typename alloc>
	void hash_table<value, key, HashFun, ExtractKey, EqualKey, alloc>::erase(const iterator& it)
	{
		node*n = it._cur;  //ȡ���ڵ�
		const size_type pos = bkt_num(cur->v);  //�������ڵ�Ͱ
		node*first = buckets[pos];
		if (first == n)
		{
			buckets[pos] = n->next;
			delete_node(n);
			--element_num;
		}
		else
		{
			node*_next = first->next;
			while (_next)
			{
				if (_next == n)
				{
					first->next = _next->next;
					delete_node(_next);
					--element_num;
					break;
				}
				else
				{
					first = _next;
					_next = _next->next;
				}
			}
		}
	}

	template<typename value, typename key, typename HashFun, typename ExtractKey, typename EqualKey, typename alloc>
	typename hash_table<value, key, HashFun, ExtractKey, EqualKey, alloc>::size_type 
	hash_table<value, key, HashFun, ExtractKey, EqualKey, alloc>::erase(const key_type &key)
	{
		const size_type pos = bkt_num(key);
		size_type res = 0;
		node* first = buckets[pos];
		
		if (first)
		{
			node* cur = first;
			node* _next = cur->next;
			while (_next)
			{
				if (equals(get_key(_next->v), key))
				{
					cur->next = _next->next;
					delete_node(_next);
					_next = cur->next;
					++res;
				}
				else
				{
					cur = _next;
					_next = _next->next;
				}
			}
			if (equals(get_key(first->v), key))
			{
				buckets[pos] = first->next;
				delete_node(first);
				++res;
			}
			element_num -= res;
		}

		return res;
	}

	template<typename value, typename key, typename HashFun, typename ExtractKey, typename EqualKey, typename alloc>
	void hash_table<value, key, HashFun, ExtractKey, EqualKey, alloc>::erase(iterator&first, iterator&second)
	{
		for (; first != second; ++first)   //���Ч�ʻ�Ƚϵ�
			erase(first);
	}

	template<typename value, typename key, typename HashFun, typename ExtractKey, typename EqualKey, typename alloc>
	void hash_table<value, key, HashFun, ExtractKey, EqualKey, alloc>::erase(const const_iterator &cit)
	{
		erase(iterator(const_cast<node*>(cit._cur), const_cast<hash_table*>(cit._htp)));
	}

	template<typename value, typename key, typename HashFun, typename ExtractKey, typename EqualKey, typename alloc>
	void hash_table<value, key, HashFun, ExtractKey, EqualKey, alloc>::erase(const_iterator&cfirst, const_iterator&clast)
	{
		erase(iterator(const_cast<node*>(cfirst._cur), const_cast<hash_table*>(cfirst._htp)),
			iterator(const_cast<node*>(clast._cur), const_cast<hash_table*>(clast._htp)));
	}

	template<typename value, typename key, typename HashFun, typename ExtractKey, typename EqualKey, typename alloc>
	void hash_table<value, key, HashFun, ExtractKey, EqualKey, alloc>::copy_from(const hash_table&rhs)
	{
		buckets.clear();
		buckets.reserve(rhs.buckets.size());
		buckets.insert(buckets.end(), rhs.buckets.size(), (node*)0);
		for (size_type i = 0; i < rhs.buckets.size(); ++i)
		{
			node* cur = rhs.buckets[i];
			if (cur)
			{
				node*copy = new_node(cur->v);
				buckets[i] = copy;
				for (node*_next = cur->next; _next; cur = _next,_next = cur->next)
				{
					copy->next = new_node(_next->v);
					copy = copy->next;
				}
			}
		}
		element_num = rhs.element_num;

	}
} //end of namespace

#endif