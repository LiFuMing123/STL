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

	
	

/***************************************************************************************/
//�ڶ���������

	enum {__ALIGN=8};  //С�����鰴��8�ֽڶ���
	enum {__MAX_BYTES=128};//С�����������ֽ�
	enum {__NUMFREELIST=__MAX_BYTES/__ALIGN}; //free_lists�ĸ���
	enum { __DEFAULT_NOBJS = 20 };		//���·���ʱ��Ĭ�ϵ�����

template<int inst>
class __default_alloc_template
{
public:
	static size_t ROUND_UP(size_t bytes) { return (((bytes)+__ALIGN - 1)&~(__ALIGN - 1)); }  //��������ֽ���������Ϊ8�ı���
	union obj    //free_lists�ڵ�
	{
		union obj*free_list_link;    
		char client_data[1];
	};

	static obj* volatile free_lists[__NUMFREELIST]; //�洢free_lists�ڵ�ָ������飬ͨ��������Ѱ�Ҷ�ӦС�����������
	static size_t FREELIST_INDEX(size_t bytes)//ͨ����������ֽ���������Ҫʹ�õ�С�������������������
	{
		return (((bytes)+__ALIGN - 1) / __ALIGN - 1);
	}

	static void*refill(size_t n);
	/*�����СΪsize������Ϊnobjs�Ŀռ䣬ע��nobjs�������õķ�ʽ���ݣ�nobjs��ӳʵ�ʷֵ����ڴ��С*/
	static char*chunk_alloc(size_t size, int &nobjs);

	/*�ڴ��ָ��*/
	static char*start_free;		 //�ڴ�ؿ�ʼλ��
	static char*end_free;		 //�ڴ�ؽ���λ��
	static size_t heap_size;     //�Ѿ��ֹ����ڴ��С
	/*�ӿ�*/
public:
	static void *allocate(size_t n);
	static void	deallocate(void*p, size_t n);
	static void *reallocate(void*p, size_t n);

};

//static��Ա�ĳ�ʼ��
//16��С�������ַ��ʼ��
template<int inst> typename __default_alloc_template<inst>::obj* volatile 
                	__default_alloc_template<inst>::free_lists[__NUMFREELIST] =
						{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//�ڴ����ʼλ��
template<int inst>
char* __default_alloc_template<inst>::start_free = 0;
//�ڴ�ؽ���λ��
template<int inst>
char* __default_alloc_template<inst>::end_free = 0;
//�ѷ���ĶѴ�С
template<int inst>
size_t __default_alloc_template<inst>::heap_size = 0;



template<int inst>
void * __default_alloc_template<inst>::refill(size_t n)
{
	int nobjs = __DEFAULT_NOBJS;        //���·���ռ䣬Ĭ��__DEFAULT_NOBJS��
	char*chunk = chunk_alloc(n, nobjs);  //�����ڴ�ػ���ڴ�
	obj*volatile*my_free_list;
	obj*result;
	obj*current_obj, *next_obj;   //������
	int i;
	if (nobjs == 1)return (chunk);

	my_free_list = free_lists + FREELIST_INDEX(n);   //��ò��������ֵ
	result = (obj*)chunk;	   //ȡ����������û���
	*my_free_list = next_obj = (obj*)(chunk + n); //��ʣ������鴮������洢��free_lists�У��Ա��´�ʹ��
	/*������������õ���水��size��С������*/
	for (i = 1; i < nobjs; ++i)//��һ������my_free_list�ˣ�ָʾ��һ�����е�С�����顣
	{
		current_obj = next_obj;
		next_obj = (obj*)((char*)next_obj + n);   //next�ڵ�����ƶ�
		if (nobjs == i + 1)
		{
			current_obj->free_list_link = 0;  //���һ�����ݿ�
		}
		else
		{
			current_obj->free_list_link = next_obj;
		}

		
	}
	return (result);

}

/*�ڴ�صĹ���*/
/*
*	������size С�������С;nobjs �����С���������
*/
template<int inst>
char*__default_alloc_template<inst>::chunk_alloc(size_t size, int&nobjs)
{
	char*result;
	size_t total_size = size*nobjs;
	size_t bytes_left = end_free - start_free;
	if (bytes_left >= total_size)
	{
		result = start_free;
		start_free += total_size;
		return (result);
	}
	else if (bytes_left >= size)	//ʣ�µ��ڴ�����ṩһ�������ϣ�����������Ҫ�����ô��
	{
		nobjs = bytes_left / size;	//������ṩ���ٸ�������С������
		total_size = nobjs*size;
		result = start_free;
		start_free += total_size;
		return (result);
	}
	else  
	{
		if (bytes_left > 0)//��ѯ�Ƿ���������С��������������ڴ棬������ԵĻ����Ͱ�����ڴ�������
		{
			obj*volatile *my_free_list = free_lists + FREELIST_INDEX(bytes_left);//��ѯ�ܲ��뵽�ĸ�С������
			((obj*)start_free)->free_list_link = *my_free_list;   //����ԭ��������
			*my_free_list = ((obj*)start_free);
		}
		size_t bytes_to_get = 2 * total_size + ROUND_UP(heap_size >> 4);  //���1��������ƫ����
		start_free = static_cast<char*>(malloc(bytes_to_get));				//�Ӷ������»���ڴ�
		if (start_free == 0)  //����ʧ��,�ѿռ䲻����.
		{
			int i = 0;
			obj*volatile*my_free_list, *p;
			/*
			*	����û�пռ��ˣ���ô��ѯ���е�free_list�ﻹ��û�ж���Ŀռ䣬����еĻ�����ȡ������
			*/
			for (i = size; i <= __MAX_BYTES; i += __ALIGN)
			{
				my_free_list = free_lists + FREELIST_INDEX(i);  //���С������Ŀ��пռ���ʼ��ַ
				p = *my_free_list;
				if (p != 0)//�ҵ�
				{
					*my_free_list = p->free_list_link;
					start_free = (char*)(p);
					end_free = start_free + i;
					return (chunk_alloc(size, nobjs));   //�ݹ���ã�
				}

			}
			end_free = 0;   //�Ѿ��Ҳ�����
			start_free = static_cast<char*>(malloc_alloc::allocate(bytes_to_get));//���ʧ�ܾͻᱨ��
		}
		heap_size = bytes_to_get;   //����ɹ��������ڴ棬�ͼ�¼�µõ��˶����ڴ�
		end_free = start_free + bytes_to_get;
		return(chunk_alloc(size, nobjs));
	}
	
}


/*�ռ����*/
template<int inst>
inline void * __default_alloc_template<inst>::allocate(size_t n)
{
	obj*volatile*my_free_list;
	obj*result;
	if (n > (size_t)__MAX_BYTES)
	{
		return (malloc_alloc::allocate(n));
	}
	my_free_list = free_lists + FREELIST_INDEX(n);
	result = *my_free_list;
	if (result == 0)
	{
		void*r = refill(ROUND_UP(n));
		return (r);
	}
	*my_free_list = result->free_list_link;
	return (result);

}



/*�ͷſռ�*/
template<int inst>
void __default_alloc_template<inst>::deallocate(void * p, size_t n)
{
	obj*q = (obj*)p;
	obj*volatile*my_free_list;
	if (n > ((size_t)__MAX_BYTES))
	{
		malloc_alloc::deallocate(p,n);
		return;
	}
	my_free_list = free_lists + FREELIST_INDEX(n);
	/*��p��ָ��Ŀռ�嵽����ͷ*/
	q->free_list_link = *my_free_list;
	*my_free_list = q;
}

/**********************************************************************************/
	typedef __malloc_alloc_template<0> malloc_alloc;
	typedef __default_alloc_template<0> default_alloc;   //ֱ�ӽ�ģ������趨Ϊ0

}






#endif