#ifndef ALGORITHM_H
#define ALGORITHM_H


#include"__Type_traits.h"
#include"Iterator.h"
//#include"Allocator.h"
#include"Construct.h"
namespace MyCppSTL
{
	//uninitialized_   
	//uninitialized_copy()
	//Ҫ��Ŀ�ĵ�ַ�Ǵ���ʼ����һ���ڴ棬���������������뷶Χ��ô��
	//���ص��ǵ�����ĵ�����
	template<class InputIterator, class ForwardIterator>
	ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator dest)
	{
		return (__uninitialized_copy(first, last, dest));
	}

	//��������
	template<class InputIterator, class ForwardIterator>
	inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator dest)
	{
		typedef MyCppSTL::iterator_traits<InputIterator>::value_type value_type;
		return (__uninitialized_copy_aux(first, last, dest, MyCppSTL::__type_traits<value_type>::is_POD_type()));
	}

	//������������pod,ֱ�Ӹ���
	template<class InputIterator, class ForwardIterator>
	inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator dest, MyCppSTL::__true_type)
	{
		InputIterator i = first;
		for (; i < last; ++i)
		{
			*dest++ = *i;
		}
		return dest;
	}

	//��������������pod,����construct���copy
	template<class InputIterator, class ForwardIterator>
	inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator dest, MyCppSTL::__false_type)
	{
		InputIterator i = first;
		for (; i < last; ++i, ++dest)
		{
			construct(&*(dest), *i);
		}

		return dest;
	}

	//uninitialized_copy���������汾,��ԣ�char*
	inline char*uninitialized_copy(const char*first, const char*last, char*dest)
	{
		std::memmove(dest, first, last - first);
		return dest + (last - first);
	}
	//ʹ��move���
	template<class InputIterator, class ForwardIterator>
	inline ForwardIterator uninitialized_move(InputIterator first, InputIterator last, ForwardIterator dest)
	{
		InputIterator i = first;
		for (; i < last; ++i, ++dest)
		{
			construct(&*(dest), std::move(*i)); //ʹ���ƶ��ķ���
		}
		return dest;
	}


	//uninitialized_fill()
	//Ҫ��[first,last)Ҫ����δ��ʼ�����ڴ�
	template<class InputIterator, class T>
	void uninitialized_fill(InputIterator first, InputIterator last, const T&val)
	{
		__uninitialized_fill(first, last, val);
	}

	//��������������Ԫ������ѡ��ͬ�����غ���
	template<class InputIterator, class T>
	inline void __uninitialized_fill(InputIterator first, InputIterator last, const T&val)
	{
		__uninitialized_fill_aux(first, last, val, MyCppSTL::__type_traits<T>::is_POD_type());
	}

	template<class InputIterator, class T>
	inline void __uninitialized_fill_aux(InputIterator first, InputIterator last, const T&val, MyCppSTL::__true_type)
	{
		MyCppSTL::fill(first, last, val);
	}
	//uninitialized_fill��������
	//����pod����,����construct����
	template<class InputIterator, class T>
	inline void __uninitialized_fill_aux(InputIterator first, InputIterator last, const T&val, MyCppSTL::__false_type)
	{
		for (; first < last; ++first)
		{
			construct(&*first, val);
		}
	}

	//uninitialized_fill_n
	//���ص��ǵ������first
	template<class InputIterator, class size, class T>
	InputIterator uninitialized_fill_n(InputIterator first, size n, const T&val)
	{
		return (__uninitialized_fill_n(first, n, val));
	}
	//�������� is_POD_type����
	template<class InputIterator, class size, class T>
	inline InputIterator __uninitialized_fill_n(InputIterator first, size n, const T&val)
	{
		return (__uninitialized_fill_n_aux(first, n, val, MyCppSTL::__type_traits<T>::is_POD_type()));
	}

	//������������POD��ֱ�ӵ����ϲ㺯�����
	template<class InputIterator, class size, class T>
	inline InputIterator __uninitialized_fill_n_aux(InputIterator first, size n, const T&val, MyCppSTL::__true_type)
	{
		return (MyCppSTL::fill_n(first, n, val));
	}
	//��������������pod,����construct�����
	template<class InputIterator, class size, class T>
	inline InputIterator __uninitialized_fill_n_aux(InputIterator first, size n, const T&val, MyCppSTL::__false_type)
	{

		for (; n > 0; ++first, --n)
		{
			construct(&*first, val);
		}
		return first;
	}


	//fill
	template<class Iterator, class value_type>
	void fill(Iterator first, Iterator last, const value_type&val)
	{

		for (; first != last; ++first)
			*first = val;


	}
	//fill_n
	template<class OutputIterator, class Size, class T>
	OutputIterator fill_n(OutputIterator first, Size n, const T&val)
	{
		for (; n > 0; --n, ++first)
		{
			*first = val;
		}
		return first;
	}

	//iter_swap
	template<class ForwardIterator1, class ForwardIterator2>
	void iter_swap(ForwardIterator1 a, ForwardIterator2 b)
	{
		MyCppSTL::iterator_traits<ForwardIterator1>::value_type tmp = *a;
		*a = *b;
		*b = tmp;
	}
	//swap
	template<class T>
	inline void swap(T&a, T&b)
	{
		T tmp = a;
		a = b;
		b = tmp;
	}

	//equal
	template<class ForwardIterator1, class ForwardIterator2>
	inline bool equal(ForwardIterator1 first, ForwardIterator1 last, ForwardIterator2 first2)
	{
		for (; first != last; ++first, ++first2)
		{
			if (*first != *first2)return false;
		}
		return true;
	}
	//lexicographical_compare
	//true if the first range is lexicographically less than the second.
	template<class ForwardIterator1, class ForwardIterator2>
	bool lexicographical_compare(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2)
	{
		for (; (first1 != last1) && (first2 != last2); ++first1, ++first2)
		{
			if (*first1 < *first2)return true;
			else if (*first1 > *first2)return false;
		}
		//equal or one range is a prefix of another
		return((first1 == last1) && (first2 != last2));
	}


	//��������
	//��ȫ�����汾
	//���ص��ǿ�����dest�ĵ�����(ָ�����һ��Ԫ�ص���һ��λ��)
	template<class InputIter, class OutputIter>
	inline OutputIter copy(InputIter first, InputIter last, OutputIter dest)
	{
		return _copy_dispatch<InputIter,OutputIter>()(first, last, dest);
	}
	//copy�ػ��汾
	template<>
	inline char* copy(const char*first, const char*last, char*dest)
	{
		memmove(dest, first, last - first);
		return dest + (last - first);
	}

	//copy�����汾�ĸ�������
	template<class InputIter,class OutputIter>
	struct _copy_dispatch
	{
		OutputIter operator()(InputIter first, InputIter last, OutputIter dest)
		{
			return _copy(first, last, dest, MyCppSTL::iterator_traits<InputIter>::iterator_category());  //���������bug
		}

	};

	//_copy_dispatch���ػ��汾
	template<class T>
	struct _copy_dispatch<T*, T*>
	{
		T*operator()(T*first, T*last, T*dest)
		{
			typedef typename __type_traits<T>::has_trivial_assignment_constructor t;
			return _copy_t(first, last, dest, t());
		}
	};
	template<class T>
	struct _copy_dispatch<const T*, T*>
	{
		T*operator()(const T*first, const T*last, T*dest)
		{
			typedef typename __type_traits<T>::has_trivial_assignment_constructor t;
			return _copy_t(first, last, dest, t());
		}
	};

	//_copy_dispatch�����汾�ĸ�������
	//input_iterator_bag�汾
	template<class InputIter,class OutputIter>
	inline OutputIter _copy(InputIter first, InputIter last, OutputIter dest, input_iterator_tag)
	{
		for (; first != last; ++first, ++dest)
		{
			*dest = *first;
		}
		return dest;
	}

	//random_access_iterator_tag�汾
	template<class InputIter, class OutputIter>
	inline OutputIter _copy(InputIter first, InputIter last, OutputIter dest, random_access_iterator_tag)
	{
		std::ptrdiff_t distance = last - first;
		return _copy_d(first, last, dest, distance);
	}

	//random_access_iterator_tag�汾��������
	template<class InputIter, class OutputIter>
	inline OutputIter _copy_d(InputIter first, InputIter last, OutputIter dest, std::ptrdiff_t distance)
	{
		for (; distance > 0; --distance, ++first, ++dest)
		{
			*dest = *first;
		}
		return dest;
	}

	//_copy_dispatch�ػ��汾�ĸ�������
	template<class T>
	inline T* _copy_t(const T* first, const T* last, T* dest, __true_type)  //
	{
		memmove(dest, first, sizeof(T)*(last - first));
		return dest + (last - first);
	}

	//_copy_dispatch�ػ��汾�ĸ�������
	template<class T>
	inline T* _copy_t(const T* first, const T* last, T* dest, __false_type)  //
	{
		return _copy_d(first, last, dest, last - first);
	}


	//copy_backward������ʵ��
	//copy_backward�����汾
	template<class BidirectionalIterator1,class BidirectionalIterator2>
	inline BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 dest)
	{
		return _copy_backward_dispatch<BidirectionalIterator1, BidirectionalIterator2>()(first, last, dest);
	}

	//�����汾�������ǵ���random�滹�ǵ���bidrection�汾,Ŀ����Ϊ�����Ч��
	template<class InputIterator, class OutputIterator>
	struct _copy_backward_dispatch
	{
		typedef typename MyCppSTL::iterator_traits<InputIterator>::iterator_category Cat;
		OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator dest)
		{
			
			return _copy_backward_aux(first, last, dest, Cat());//����
		}
	};

	//�ػ��汾,���POD��������
	template<class T>
	struct _copy_backward_dispatch<T*, T*>
	{
		T* operator()(const T*first, const T*last, T*dest)
		{
			return _copy_backward_t(first, last, dest, __type_traits<T>::has_trivial_assignment_constructor());
		}
		
	};

	template<class T>
	struct _copy_backward_dispatch<const T*, T*>
	{
		T*operator()(const T*first, const T*last, T*dest)
		{
			return _copy_backward_dispatch<T*, T*>()(first, last, dest);
		}
	};






	//bidirectional_iterator �汾����Ϊ��ײ��ʵ��
	template<class BidirectionalIterator1, class BidirectionalIterator2>
	inline BidirectionalIterator2 _copy_backward_aux(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 dest,bidirectional_iterator_tag)
	{
		--last;
		--dest;
		for (; last!=first; --last, --dest)
		{
			*dest = *last;
		}
		return dest;
	}
	//random_access_iterator�汾����Ϊ��ײ��ʵ��
	template<class BidirectionalIterator1, class BidirectionalIterator2>
	inline BidirectionalIterator2 _copy_backward_aux(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 dest, random_access_iterator_tag)
	{
		std::ptrdiff_t distance = last - first;
		--last;
		--dest;
		for (; distance > 0; --distance, --last, --dest)
		{
			*dest = *last;
		}
		return dest;
	}


	template<class T>
	inline T*_copy_backward_t(const T*first, const T*last, T*dest, MyCppSTL::__true_type)
	{
		std::ptrdiff_t n = last - first;
		memmove(dest - n, first, sizeof(T)*n);
		return dest - n;
	}

	template<class T>
	inline T*_copy_backward_t(const T*first, const T*last, T*dest, MyCppSTL::__false_type)
	{
		return _copy_backward_aux(first, last, dest, random_access_iterator_tag);
	}

	//�����
	//make_heap��push_heap,pop_heap,sort_heap
	//

	//�����ʵ�ά��
	//��Ϊ����Ѻ��ҶѾ�������
	template<class RandomAccessIterator>
	void __build_heap_aux(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator root_node)
	{
		auto index = root_node - first;
		std::ptrdiff_t largest = index;
		auto left_index = index * 2 + 1;
		auto right_index = index * 2 + 2;
		auto size = last - first;
		if ((left_index < size) && (*root_node < *(first + left_index)))
		{
			largest = left_index;
		}
		if ((right_index < size) &&(*(first + largest) < *(first + right_index)))
		{
			largest = right_index;
		}
		if (largest!=index)
		{
			auto tmp = *root_node;
			*root_node = *(first + largest);
			*(first + largest) = tmp;
			__build_heap_aux(first, last, first + largest);  //�ݹ����,��Ϊ����ֵ֮����ƻ��˶ѵ�����
		}
	}


	//������
	template<class RandomAccessIterator>
	void make_heap(RandomAccessIterator first, RandomAccessIterator last)
	{
		auto size = last - first;
		if (size < 2)return;
		auto root_index = size / 2 - 1; //�õ���ʼ���ѵ���ʼ���ڵ�
		while (true)
		{
			__build_heap_aux(first, last, first + root_index);
			if (root_index == 0)break;
			--root_index;
		}
	}

	//push_heap,��Ԫ�ز������
	//ǰ�����Ѿ���Ԫ�ز��뵽�ײ�������β��
	//������Դ����
	template<class RandomAccessIterator>
	void push_heap(RandomAccessIterator first, RandomAccessIterator last)
	{
		__push_heap(first, last);
	}

	template<class RandomAccessIterator>
	void __push_heap(RandomAccessIterator first, RandomAccessIterator last)
	{
		//�ҵ����ڵ�
		auto size = last - first;
		auto parent_index = (size - 2) / 2;
		while (true)
		{
			__build_heap_aux(first, last, first + parent_index);
			if (parent_index == 0)break;
			parent_index = (parent_index - 1) / 2;
		}
		
	}

	//pop_heap
	//ȡ�����ڵ�ŵ�������ĩβ
	template< class RandomAccessIterator >
	void pop_heap(RandomAccessIterator first, RandomAccessIterator last)
	{
		__pop_heap_aux(first, last);
	}

	template< class RandomAccessIterator >
	void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last)
	{
		auto tmp = *first;
		*first = *(last - 1);
		__build_heap_aux(first, last, first);
		*(last - 1) = tmp;

	}

	//sort_heap��������
	template<class RandomAccessIterator>
	void sort_heap(RandomAccessIterator first, RandomAccessIterator last)
	{
		while (first != last)
		{
			MyCppSTL::pop_heap(first, last--);
		}
	}

	//search
	template< class ForwardIt1, class ForwardIt2 >
    ForwardIt1 search(ForwardIt1 first, ForwardIt1 last,ForwardIt2 s_first, ForwardIt2 s_last)
    {
    	for(;;++first)
    	{
    		ForwardIt1 it=first;
    		for(ForwardIt2 s_it=s_first;;++s_it,++it)
    		{
    			if(s_it==s_last)return first;   //If [s_first, s_last) is empty, first is returned.//or find
    			if(it==last)return last;   //If no such subsequence is found, last is returned.
    			if(!(*s_it==*it))break;
    		} 
    	}
    }

	/*
	template<typename InputIt>
	typename MyCppSTL::iterator_traits<InputIt>::difference_type distance()
	distacne ������iteratorͷ�ļ���
	*/

	//Ҫ������Ҫ�ź���
	template<typename ForwardIt,typename T>
	ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T&value)
	{
		typename MyCppSTL::iterator_traits<ForwardIt>::difference_type count, step;
		ForwardIt it;
		count = MyCppSTL::distance(first, last);
		while (count > 0)
		{
			it = first;
			step = count / 2;
			MyCppSTL::advance(it, step);
			if (*it < value)
			{
				first = ++it;
				count -= step + 1;
			}
			else
			{
				count = step;
			}
		}

		return first;
	}
}


#endif