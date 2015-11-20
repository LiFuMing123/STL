#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include"Alloc.h"
#include"Construct.h"

/*
*  �������ӿڣ���װ��һ�������������ṩ��׼�ӿ�
*/

namespace MyCppSTL 
{

	template<class T,class Alloc=malloc_alloc>
	class allocator
	{
	public:
		/*�ռ�ķ������ͷ�*/
		static T*allocate(size_t n)
		{
			return 0>=n?0:static_cast<T*> (Alloc::allocate(n*sizeof(T)));//����n*sizeof(T)���ռ�
		}
		static T*allocate(void)
		{
			return static_cast<T*>(Alloc::allocate(sizeof(T)));
		}
		static void deallocate(T*p, size_t n)
		{
			if (n != 0) { Alloc::deallocate(p, n); }
		}
		static void deallocate(T*p)
		{
			 Alloc::deallocate(p,sizeof(T)); 
		}
		/*����Ĺ���������*/
		static void construct(T*p,const T&n)
		{
			MyCppSTL::construct(p, n);
		}
		static void destroy(T*p)
		{
			MyCppSTL::destroy(p);
		}

	};

	

}

#endif