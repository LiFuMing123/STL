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
		node* _cur;
		_hash_table* _htp;
	public:
		//���캯��
		hash_table_const_iterator(const node*n, const _hash_table* h) :_cur(n), _htp(h) {}
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
			return _cur->v;
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
		hash_table(const hash_table& rhs) //copy
		{
			
		}

		hash_table& operator=(const hash_table& rhs) //operator=
		{
			
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
			return bkt_num_key(get_key(obj), buckets.size());
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

} //end of namespace

#endif