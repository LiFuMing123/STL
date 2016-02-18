#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H


#include<utility>
#include"Allocator.h"
#include"hash_function.h"
#include"Vector.h"
#include"function.h"


namespace MyCppSTL
{

	//hash table node
	template<typename value>
	struct hashtable_node
	{
		hashtable_node<value>*next;
		value v;
	};

	//������
	//������

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
		//��Ƕ�ͱ�
	public:
		using key_type = key;
		using value_type = value;
		using hasher = HashFun;
		using key_equal = EequalKey;

		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using pointer = value*;
		using const_pointer = const value*;
		using reference = value&;
		using const_reference = const value&;
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
	private://��������
		size_type next_size(size_type n) const
		{
			return next_prime(n);
		}
		void initialize_buckets(size_type n) //hash ��ĳ�ʼ��
		{
			const size_type n_buckets = next_size(n);
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

} //end of namespace

#endif