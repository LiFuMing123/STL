#ifndef RB_TREE_H
#define RB_TREE_H

#include"Allocator.h"
#include"Iterator.h"

namespace MyCppSTL
{
	enum { RED = 0,BLACK = 1};
	template<typename T>    //������ڵ�Ķ���
	struct rb_node
	{
		using _Node= rb_node < T >;
		using _NodePtr= rb_node<T>*;
		using value_type= T;
		rb_node():left(NULL), right(NULL), parent(NULL), value(T()), COLOR(RED) {}
		_NodePtr left;
		_NodePtr right;
		_NodePtr parent;
		T value;
		int COLOR;

	};

	//���������
	template<typename T>
	struct RB_Tree_const_iterator :public iterator<bidirectional_iterator_tag, T>
	{
		//��Ƕ�ͱ���
		using value_type = const T;
		using reference = const T&;
		using pointer = const T*;
		using myIter = RB_Tree_const_iterator<T>;
		//��Ա
		rb_node<T>*_node;

		//���캯��
		RB_Tree_const_iterator() {}
		RB_Tree_const_iterator(rb_node<T>* x) :_node(x) {}
		RB_Tree_const_iterator(myIter&right) :_node(right._node) {}

		//��Ա����
		reference operator*() const
		{
			return _node->value;
		}
		pointer operator->() const
		{
			return &(operator*());
		}

		myIter& operator++() //ǰ�õ���
		{
			if (_node->right->parent == _node) //˵�����Һ���
			{
				_node = _node->right;
				while (_node->left->parent == _node)_node = _node->left;
			}
			else //û���Һ��ӣ�����
			{
				rb_node<T>* temp = _node->parent;
				while (_node == temp->right)
				{
					_node = temp;
					temp = temp->parent;
				}//��Ѱ�����ڵ㣬��ôtemp->right=null !=_node
				_node = temp;
			}

			
			return *this;
		}

		myIter operator++(int) //���õ���
		{
			auto temp = *this;
			++*this;
			return temp;
		}

		myIter& operator--() //ǰ�õݼ�
		{
			if (_node->left->parent == _node) //������
			{
				_node = _node->left;
				while (_node->right->parent == _node)_node = _node->right;
			}
			else
			{
				rb_node<T>* temp = _node->parent;
				while (temp->left == _node)
				{
					_node = temp;
					temp = temp->parent;
				}   
				//���ñ�ʾbegin
			}
			return *this;
		}

		myIter operator--(int) //���õݼ�
		{
			auto temp = *this;
			--*this;
			return temp;
		}

		//�߼�����
		bool operator==(myIter&right) const
		{
			return _node == right._node;
		}
		bool operator!=(myIter&right) const
		{
			return !(*this == right);
		}
	};

	template<typename T>
	struct RB_Tree_iterator :public RB_Tree_const_iterator<T>
	{
		//��Ƕ�ͱ���
		using value_type = T;
		using reference = T&;
		using pointer = T*;
		using myIter = RB_Tree_iterator<T>;
		using myBase = RB_Tree_const_iterator<T>;

		RB_Tree_iterator() :RB_Tree_const_iterator(){}
		RB_Tree_iterator(rb_node<T>* x) :RB_Tree_const_iterator(x) {}   //����һ��rb_node�ڵ������ת��
		RB_Tree_iterator(myIter&right) :RB_Tree_const_iterator(right) {}

		//��Ա����
		reference operator*() const
		{
			return _node->value;
		}

		pointer operator->() const
		{
			return &(operator*());
		}

		myIter& operator++()
		{
			++*(myBase*)this;
			//++((myBase)this);
			return *this;
		}

		myIter operator++(int)
		{
			auto temp = *this;
			++*this;
			return temp;
		}

		myIter& operator--()
		{
			--*(myBase*)this;
			return *this;
		}

		myIter operator--(int)
		{
			auto temp = *this;
			--*this;
			return temp;
		}
	};


	/*
	��һ��nil�ڵ㣬�ڵ��parentָ��NULL,��leftָ�����ҵ�Ԫ�أ�����end()��ʵ�֡�
	*/
	template<typename T,typename alloc=MyCppSTL::allocator<T>>
	class RB_Tree
	{
	public:
		using value_type = T;
		using reference = T&;
		using _Node = rb_node<T>;
		using _NodePtr = rb_node<T>*;
		using iterator = RB_Tree_iterator<T>;
		using rb_tree_node_alloc = MyCppSTL::allocator<rb_node<T>>;
		using const_iterator = RB_Tree_const_iterator<T>;
		using size_type = std::size_t;
		using myTree = RB_Tree<T>;
	private:
		_NodePtr root;         //�����ڵ�
		_NodePtr nil;          //�ڱ��ڵ�
		size_type node_count;  //�ڵ����
		
	public:
		//
		RB_Tree()
		{
			nil=make_node(T());
			nil->COLOR=BLACK;
			root=nil;
			root->parent = nil;
		}
		RB_Tree(RB_Tree<T>&right) //copy construct
		{
			nil = make_node(T());
			nil->COLOR = BLACK;
			root = nil;
			root->parent = nil;
			tree_clone(right.getRoot(), nil);
			node_count = right.node_count;
		}


		//����������
		myTree operator=(const myTree&right);
		
		_NodePtr& getRoot(){return root;}
		void tree_delete(_NodePtr position);
		iterator insert_equal(const T&key);    //����Ԫ�أ��������ظ�Ԫ��
		iterator insert_unique(const T& key);  //����Ԫ�أ��������ظ�Ԫ��
		_NodePtr RB_tree_successor(_NodePtr position) const;  //���Һ��
		_NodePtr tree_minnum(_NodePtr position) const;  

		//������
		iterator begin() { return iterator(tree_minnum(getRoot())); }
		const_iterator begin() const { return const_iterator(tree_minnum(getRoot())); }
		const_iterator cbegin() const {return const_iterator(tree_minnum(getRoot()));}
		iterator end() { return iterator(nil); }
		const_iterator end() const { return const_iterator(nil); }
		const_iterator cend() const { return const_iterator(nil); }
	private:
		//˽�г�Ա����
		//��ת
		iterator __insert(_NodePtr&root,const T& key);   //����Ԫ��
		void left_rotate(_NodePtr&position);   //����ת
		void right_rotate(_NodePtr&position);  //����ת
		_NodePtr make_node(const T&key);     //����ڵ�
		void destory_node(_NodePtr position);  //ɾ���ڵ�
		void RB_insert_fixup(_NodePtr& position); //ά�������������
		void RB_delete_fixup(_NodePtr& position);//
		_NodePtr node_clone(_NodePtr position); //�ڵ�Ŀ���
		_NodePtr tree_clone(_NodePtr position, _NodePtr parent); //���Ŀ���
		
		void clear(_NodePtr position);  //��������


	}; //end of class

	template<typename T,typename alloc>
	inline typename RB_Tree<T, alloc>::_NodePtr RB_Tree<T, alloc>::node_clone(_NodePtr position)
	{
		_NodePtr temp = make_node(position->value);
		temp->COLOR = position->COLOR;
		temp->left = temp->right = temp->parent = NULL;
		return temp;
	}

	template<typename T, typename alloc>
	typename RB_Tree<T,alloc>::_NodePtr RB_Tree<T,alloc>::make_node(const T&key)
	{
		/*
		_NodePtr temp=new _Node;
		temp->left=temp->right=temp->parent=NULL;
		temp->COLOR=RED;
		temp->value=key;
		*/
		_NodePtr temp = rb_tree_node_alloc::allocate();
		temp->left = temp->right = temp->parent = NULL;
		temp->COLOR = RED;
		MyCppSTL::construct(&temp->value,key);
		return temp;
	}

	template<typename T, typename alloc>
	void RB_Tree<T,alloc>::destory_node(_NodePtr position)
	{
		if(position)
		{
			//free(position);
			MyCppSTL::destroy(&position->value);
			rb_tree_node_alloc::deallocate(position);
		}
	}

    //����ת
	template<typename T, typename alloc>
	void RB_Tree<T,alloc>::left_rotate(_NodePtr&position)   //����ͼ�����
	{
		_NodePtr rotate_temp=position->right;   //����position->right��һ�����ڵģ�����Ҳ��û��Ҫ��ת��

		position->right = rotate_temp->left; 
		if(rotate_temp->left!=nil)rotate_temp->left->parent=position;   //��y���������ӵ�position��

		rotate_temp->parent=position->parent;
		if(position->parent==nil)root=rotate_temp;  //���ø��ڵ�
		else if(position==position->parent->left)position->parent->left=rotate_temp;
		else if(position==position->parent->right)position->parent->right=rotate_temp;
		

		rotate_temp->left=position;
		position->parent=rotate_temp;


	}

	//����ת
	template<typename T, typename alloc >
	void RB_Tree<T,alloc>::right_rotate(_NodePtr&position)
	{
		_NodePtr rotate_temp=position->left;

		position->left=rotate_temp->right;
		if(rotate_temp->right!=nil)rotate_temp->right->parent=position;

		rotate_temp->parent=position->parent;
		if(position->parent==nil)root=rotate_temp;
		else if(position==position->parent->left)position->parent->left=rotate_temp;
		else if(position==position->parent->right)position->parent->right=rotate_temp;

		rotate_temp->right=position;
		position->parent=rotate_temp;
	}

	template<typename T, typename alloc = MyCppSTL::allocator<T>>
	typename RB_Tree<T,alloc>::iterator RB_Tree<T,alloc>::__insert(_NodePtr&root, const T& key)  //����Ԫ��
	{
		_NodePtr pre=root;
		_NodePtr cur=root;  
		_NodePtr insert_node=make_node(key);
		while(cur!=nil)
		{
			pre=cur;
			if(key<cur->value)cur=cur->left;
			else
				cur=cur->right;
		}
		insert_node->parent=pre;
		if(pre==nil)root=insert_node;
		else if(key<pre->value)pre->left=insert_node;
		else 
		{
			pre->right=insert_node;
		}
		insert_node->left=nil;
		insert_node->right=nil;
		insert_node->COLOR=RED;
		//������ɣ�����ά�������������
		RB_insert_fixup(insert_node);
		++node_count;
		

		return iterator(insert_node);

	}

	template<typename T, typename alloc >   //ά����������
	void RB_Tree<T,alloc>::RB_insert_fixup(_NodePtr& position)
	{
		while(position->parent->COLOR==RED)  //�����ܹ���֤���ڵ����ɫ�Ǻ�ɫ��
		{
			if(position->parent==position->parent->parent->left)  
			{
				if(position->parent->parent->right->COLOR==RED)  //case1,��ڵ�Ҳ�Ǻ�ɫ
				{
					position->parent->parent->right->COLOR=BLACK;
					position->parent->COLOR=BLACK;
					position->parent->parent->COLOR=RED;
					position=position->parent->parent;   //������һ�ֵ�ѭ��
				}
				else if(position==position->parent->right)      //case 2 Ҫ����ת��Ϊcase3
				{
					position=position->parent;
					left_rotate(position);  //����תת��Ϊ��case3
				}
				else if(position==position->parent->left)//��ڵ�϶��Ǻ�ɫ��
				{
					position->parent->COLOR=BLACK;
					position->parent->parent->COLOR=RED;
					position=position->parent->parent;
					right_rotate(position);    //����ת
				}
			}
			else if(position->parent=position->parent->parent->right)
			{
				if(position->parent->parent->left->COLOR==RED) //��ڵ�
				{
					 position->parent->parent->left->COLOR=BLACK;
					 position->parent->parent->COLOR=RED;
					 position->parent->COLOR=BLACK;
					 position=position->parent->parent;
				}
				else if(position==position->parent->left)   //case 2
				{
					position=position->parent;
					right_rotate(position);    //����ת  ,ת��Ϊcase3
				}
				else if(position==position->parent->right)
				{
					position->parent->COLOR=BLACK;
					position->parent->parent->COLOR=RED;
					position=position->parent->parent;
					left_rotate(position);   //����
				}
			}
		}

		root->COLOR=BLACK; //���ָ�Ϊ��ɫ
	}

	template<typename T, typename alloc>
    typename RB_Tree<T,alloc>::_NodePtr RB_Tree<T,alloc>::tree_minnum(_NodePtr position) const
    {
    	if(position->left==nil)return position;
    	return tree_minnum(position->left);
    }


	template<typename T, typename alloc >
	typename RB_Tree<T,alloc>::_NodePtr RB_Tree<T,alloc>::RB_tree_successor(_NodePtr position) const
	{
		if(position->right!=nil)return tree_minnum(position->right);
		while(position!=nil&&position==position->parent->right)position=position->parent;
		return position;
	}

	template<typename T,typename alloc>
	void RB_Tree<T,alloc>::tree_delete(_NodePtr position)
	{
		_NodePtr del_temp=nil;  //ʵ��ɾ���Ľڵ�
		_NodePtr repl_temp=nil;
		if(position->left==nil||position->right==nil)del_temp=position;
		else
		{
			del_temp=RB_tree_successor(position);  //ɾ������
		}
		if(del_temp->left!=nil)repl_temp=del_temp->left;
		else
			repl_temp=del_temp->right;
		repl_temp->parent=del_temp->parent;     
		if(del_temp->parent==nil)root=repl_temp;
		else if(del_temp==del_temp->parent->left)del_temp->parent->left=repl_temp;
		else if(del_temp==del_temp->parent->right)del_temp->parent->right=repl_temp;

		if(del_temp!=position)
			{
				position->value=del_temp->value;
				//position->COLOR=del_temp->COLOR;  //��ȷ�ϣ�
			}

		//��������ɫ��ά��
		if(del_temp->COLOR==BLACK)
		{
			RB_delete_fixup(repl_temp);
		}

		//�ͷŽڵ�ռ�
		destory_node(del_temp);

	}

	template<typename T,typename alloc>
	void RB_Tree<T,alloc>::RB_delete_fixup(_NodePtr& position)
	{
		while(position!=root&&position->COLOR==BLACK) //ֻ��position��color����ɫ��black(˫�غ�ɫ)�����ۣ���Ϊ�����red�������ֱ������Ϊblack
		{
			if(position==position->parent->left)
			{
				_NodePtr bro=position->parent->right;
				if(bro->COLOR==RED) //case 1,�ֵܽڵ���������ɫ����
				{
					bro->COLOR=BLACK;
					position->parent->COLOR=RED;
					left_rotate(position->parent); //convert to case2/3/4
					bro=position->parent->right;
				}
				if(bro->right->COLOR==BLACK&&bro->left->COLOR==BLACK) //case2,3�����Ǻ�ɫ�ģ���ôposition��bro����ȥ��ɫ
				{
					bro->COLOR=RED;
					position=position->parent;//�൱�ڰ���һ���ɫת�Ƶ�position�ĸ��ڵ��ˣ������丸�ڵ���ʲô��ɫ������Ǻ�ɫ����Ϊ��ڣ��˳�ѭ��������Ǻ�ɫ�����Ǻںڣ�����ѭ�����������ɫ�������ϲ��ƶ�
					
				}
				else if(bro->right->COLOR==BLACK) //case 3,�����ӵ���ɫ�Ǻ�ɫ�ģ������Ǻ�ɫ convert to case 4
				{
					bro->left->COLOR=BLACK;
					bro->COLOR=RED;
					right_rotate(bro);
					bro=position->parent->right;
				}
				//bro->right->COLOR=RED      case 4  //�Һ����Ǻ�ɫ��

				bro->COLOR=position->parent->COLOR;
				position->parent->COLOR=BLACK;
				bro->right->COLOR=BLACK;
				left_rotate(position->parent);
				position=root;
			}
			else if(position==position->parent->right)//�Һ���
			{
				 _NodePtr bro=position->parent->left;
				 if(bro->COLOR==RED)  //�뷨��Ҫά������5
				 {
				 	bro->COLOR=BLACK;
				 	position->parent->COLOR=RED;
				 	right_rotate(position->parent);
				 	bro=position->parent->left;
				 }
				 if(bro->left->COLOR==BLACK&&bro->right->COLOR==BLACK)//case 2  bro->COLOR==BLACK
				 {
				 	bro->COLOR=RED;
				 	position=position->parent;
				 }
				 else if(bro->left->COLOR==BLACK) //bro->right->COLOR=RED; case 3
				 {
				 	bro->right->COLOR=BLACK;
				 	bro->COLOR=RED;
				 	left_rotate(bro);
				 	bro=position->parent->left;
				 }
				 //bro->left->COLOR==RED,bro->right->COLOR=RED   case 4
				 bro->COLOR=position->parent->COLOR;  //�����ֵܽڵ����ɫ���丸�ڵ����ɫ���ֵܽڵ���ɫ�϶��Ǻ�ɫ��
				 position->parent->COLOR=BLACK;
				 bro->left->COLOR=BLACK;
				 right_rotate(position->parent);
				 position=root;
			}
		}

		position->COLOR=BLACK;

	}


	//����Ԫ�أ��������ظ�Ԫ��
	template<typename T,typename alloc>
	inline typename RB_Tree<T, alloc>::iterator RB_Tree<T, alloc>::insert_equal(const T&key)
	{
		return __insert(getRoot(), key);
	}


	//���Ŀ���
	template<typename T,typename alloc>
	typename RB_Tree<T, alloc>::_NodePtr RB_Tree<T, alloc>::tree_clone(_NodePtr position, _NodePtr p)
	{
		_NodePtr temp = node_clone(position);
		temp->parent = p;      //���ø��ڵ�
		if (root == nil)
			root = temp;   //���ø��ڵ�
	

		if (position->right->parent == position)  //��������
		{
			temp->right = tree_clone(position->right, temp);
		}
		else
		{
			temp->right = nil;  //û��������������Ҷ�ӽڵ�
		}
		p = temp;
		//position = position->left;   //ת��������
	
		while (position->left->parent == position)
		{
			position = position->left;
			_NodePtr temp2 = tree_clone(position, p);
			p->left = temp2;
			temp2->parent = p;
			if (position->right->parent==position)
				temp2->right = tree_clone(position->right, temp2);
			p = temp2;
		//	position = position->left;
		}
		p->left = nil;//û���������ˣ�����Ҷ�ӽڵ�


	
		return temp; //ע�ⷵ�صĽڵ���temp,������p
	}

	//�����������
	/*
	template<typename T,typename alloc>
	typename RB_Tree<T, alloc>::myTree RB_Tree<T, alloc>::operator=(const myTree&right)
	{
		if (this != &right)
		{
			//
		}
	}
	*/

	//��������
	template<typename T,typename alloc>
	void RB_Tree<T, alloc>::clear(_NodePtr position)
	{
		if (position)
		{
			auto it = iterator(position);  
			auto it2 = ++it;
			while (it2 != nil)
			{
				destory_node(it._node);
				it = it2;
				++it2;
			}
		}
	}



} //end of namespace





#endif
