#ifndef ALLOC_H
#define ALLOC_H

/*
*	Alloc ���������ǰ���ڴ���䡢������������ڴ����
*	allocate:     �ڴ����
*	deallocate:   �ڴ��ͷ�
*/



#if 0
#	include<new>
#	define __THROW_BAD_ALLOC throw bad_alloc      //���׳�bad_alloc
#elif !defined(__THROW_BAD_ALLOC)
#	include<iostream>
#	define __THROW_BAD_ALLOC std::cerr<<"out of memory"<<std::endl;exit(1); //����������ܷ�����û��ָ��������
																	    // ���������ӡ��Ϣ���˳�
#endif

namespace MyCppSTL
{
	 //��һ��������
	template<int inst>
	class __malloc_alloc_template 
	{
	private:
		/* oom: out of memory */
		static void *oom_malloc(size_t);					//out of memory when malloc
		static void *oom_realloc(void*, size_t);				//out of memory when reallocate
		static void(*__malloc_alloc_oom_handler)();			//the handler of out of memory
	public:
		static inline void* allocate(size_t n);            //allocate memory
		static inline void* reallocate(void*p, size_t n);   //reallocate memory
		static inline void deallocate(void*p,size_t);				//free memory
		static void(*set_malloc_handler(void(*)()))();	    //out of memory function
	};

	/*����ռ��Ա*/
	template<int inst>
	void* __malloc_alloc_template<inst>::allocate(size_t n)
	{
		auto*result = malloc(n);
		if (result == 0)result = oom_malloc(n);
		return result;
	}
	
	/*�ٷ���*/
	template<int inst>
	void* __malloc_alloc_template<inst>::reallocate(void*p, size_t n)
	{
		auto *result = realloc(p, n);
		if (result == 0)result = oom_realloc(p, n);
		return result;
	}

	/*�ͷ��ڴ�*/
	template<int inst>
	void __malloc_alloc_template<inst>::deallocate(void*p, size_t n)
	{
		free(p);
	}

	/*ָ������ʧ��ʱ�Ĵ�����*/
	template<int inst>
	void(*__malloc_alloc_template<inst>::set_malloc_handler(void(*f)()))()
	{
		void(*old)() = __malloc_alloc_oom_handler;
		__malloc_alloc_oom_handler = f;
		return (old);			//��old����֮ǰ�Ĵ����������øú�����ʵ��ʹ�õ���__malloc_alloc_oom_handlerָ���ĺ���					
	}

	

	/*out of memory ������,����ָ����Ϊ0*/
	template<int inst>
	void(*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;



	/*out of memory */
	template<int inst>
	void *__malloc_alloc_template<inst>::oom_malloc(size_t n)
	{
		void(*my_malloc_handler)();				//����һ��������
		void*result;
		for (;;)								//��������
		{
			my_malloc_handler = __malloc_alloc_oom_handler;
			if (0 == my_malloc_handler) { __THROW_BAD_ALLOC; }
			(*my_malloc_handler)();             //���ô�����
			result = malloc(n);
			if (result)return(result);			//����ɹ�����
		}
	}

	/*out of memory realloc*/
	template<int inst>
	void *__malloc_alloc_template<inst>::oom_realloc(void*p, size_t n)
	{
		void(*my_malloc_handler)();				//����һ��������
		void*result;
		for (;;)								//��������
		{
			my_malloc_handler = __malloc_alloc_oom_handler;
			if (0 == my_malloc_handler) { __THROW_BAD_ALLOC; }
			(*my_malloc_handler)();             //���ô�����
			result = realloc(p, n);
			if (result)return(result);			//����ɹ�����
		}
	}

	typedef __malloc_alloc_template<0> malloc_alloc;
	
}


#endif