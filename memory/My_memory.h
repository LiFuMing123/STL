#ifndef _MEMORY_H
#define _MEMORY_H

namespace MyCppSTL
{

	class shared_count_base  //����
	{
		//��Ա����
	private:
		unsigned int _count;
	public://���캯��
		shared_count_base() :_count(1) {}
		virtual ~shared_count_base() {}
		virtual void dispose() = 0;  //�ͷ���Դ��������ֵΪ0ʱ
		virtual void destory()       //�����ͷ�
		{
			delete this;
		}
		void add_ref_count() { ++_count; }
		void decr_ref_count() { --_count; }  //��ʱ����
		long use_count() { return _count; }
		bool expired() { return _count == 0; }

	};

	template<typename T>
	class shared_count_ptr :public shared_count_base
	{
		//��Ա����
	private:
		T* _ptr;
	public:
		//���캯��
		shared_count_ptr() :_ptr(0) {}
		shared_count_ptr(T* p) :_ptr(p) {}
		//��Ա
		virtual void dispose(){ delete _ptr;}
		virtual void destory() { delete this; }
		virtual shared_count_ptr() {}
	};

	template<typename T,typename Deleter>
	class shared_count_deleter :public shared_count_base
	{
	private:
		T*_ptr;
		Deleter _D;
	public:
		shared_count_deleter(T*p, Deleter d) :_ptr(p), _D(d) {}
		//��Ա����
		virtual void dispose() { _D(_ptr); } //ʹ���û��ṩ��ɾ����
		virtual void destory() { delete this; }
		virtual ~shared_count_deleter() {}

	};


	template<typename T>
	class shared_count      //���ü���
	{
	 public: //��Ƕ�ͱ�
		 using value_type = T;
	private: //��Ա����
		shared_count_base *_count_ptr;
	public: //���캯��
		shared_count() :_count_ptr(0) {}
		shared_count(T*p) :_count_ptr(0)
		{
			_count_ptr = new shared_count_ptr<T>(p);
		}
		template<typename Tp=T,typename Deleter>
		shared_count(Tp*p, Deleter d) : _count_ptr(0)
		{
			p = new shared_count_deleter<Tp, Deleter>(p, d);
		}

		~shared_count()
		{
			if (_count_ptr->use_count() != 0)
			{
				_count_ptr->decr_ref_count();
			}
			else
				_count_ptr->dispose();
		}

		shared_count(shared_count&rhs) :_count_ptr(rhs._count_ptr)  //copy construct
		{
			if (_count_ptr != NULL)_count_ptr->add_ref_count();   //�������ü���
		}

		shared_count&operator=(shared_count&rhs)  //assignment
		{
			if (this != &rhs)
			{
				if (_count_ptr != NULL)_count_ptr->decr_ref_count();
				if (_count_ptr->use_count == 0)_count_ptr->dispose();   //�ͷ�ָ����Դ
				if (rhs._count_ptr != NULL)rhs._count_ptr->add_ref_count();
				_count_ptr = rhs._count_ptr;
			}

			return *this;
		}

		void swap(shared_count&rhs)
		{
			shared_count_base* temp = rhs._count_ptr;
			rhs._count_ptr = _count_ptr;
			_count_ptr = temp;
		}

		long use_count()
		{
			return _count_ptr == NULL ? 0 : _count_ptr->use_count();
		}
		bool unique()
		{
			return this->use_count == 0;
		}

	};

	template<typename T>
	class shared_ptr
	{
		//��Ƕ�ͱ�
	public:
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using const_pointer = const T*;
		using const_reference = const T&;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
	private: //��Ա
		T* _Ptr;
		shared_count<T> count;
	public:  //���캯��
		shared_ptr() :_Ptr(0), count() {}
		shared_ptr(T* p) : _Ptr(p), count(p) {}
		template<typename _Tp=T,typename Deleter>   //����һ��ɾ������
		shared_ptr(_Tp* p, Deleter d) : _Ptr(p), count(p, d) {}
		shared_ptr&operator=(shared_ptr&rhs)
		{
			_Ptr = rhs._Ptr;
			count = rhs.count;
			return *this;
		}

		void swap(shared_ptr<T>&rhs)
		{
			std::swap(_Ptr, rhs._Ptr);
			rhs.swap(count);
		}

	};


}

#endif // !_MEMORY_H

