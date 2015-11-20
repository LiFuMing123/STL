#ifndef CONSTRUCT_H
#define CONSTRUCT_H

//#include<traits_type>
/*
*	����alloc��Ҫʹ�õ�construct��destroy����
*   construct��������
*	destroy����������
*/

#include<new.h>

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




}
 


#endif
