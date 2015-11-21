#ifndef CONSTRUCT_H
#define CONSTRUCT_H

//#include<traits_type>
/*
*	����alloc��Ҫʹ�õ�construct��destroy����
*   construct��������
*	destroy����������
*/

#include<new>
#include<type_traits>


namespace MyCppSTL
{
	/*construct ������*/
	/*Ҫ��T2��T1�������ǿ���ת����*/
	template<class T1,class T2>
	inline void construct(T1*p,const T2& value)
	{
		new(p)T1(value);      //����ռλnew,��λ��p���������value
	}
	/*destroy ��������*/
	template<class T>
	inline void destroy(T*p)
	{
		p->~T();		   //ֱ�ӵ�������T����������
	}

	
	template<class Forwarditerator>
	inline void destroy(Forwarditerator first, Forwarditerator last)
	{
		//__destroy(first,last,)
	}
	
	/*
	template<class ForwardIterator,class T>
	inline void __destroy(ForwardIterator first, ForwardIterator last, T*)
	{
		//typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;

	}
	*/

	//�������汾
	template<>
	inline void destroy(char*, char*) {}
	template<>
	inline void destroy(int*, int*) {}
	template<>
	inline void destroy(double*, double*) {}




}
 


#endif
