#ifndef MY_VECTOR_HPP
#define MY_VECTOR_HPP

#include <cstddef>
#include <cstring>
#include <random>

namespace sjtu
{
template<typename T>
class vector
{
	char *p;
	size_t maxs;
	size_t reals;
	void doubleSpace()
	{
		const auto pre_p=p;
		p=new char[2*maxs*sizeof(T)];
		memcpy(p,pre_p,maxs*sizeof(T));
		delete[] pre_p;
		maxs<<=1;
	}
public:
	class const_iterator;
	class iterator
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

	private:
		size_t pos;
		vector<T>* obj;
	public:
		iterator(int p,vector<T>* q):pos(p),obj(q){}
		size_t POS() const {return pos;}
		iterator operator+(const int &n) const{return iterator(pos+n,obj);}
		iterator operator-(const int &n) const{return iterator(pos-n,obj);}
		int operator-(const iterator &rhs) const{return pos-rhs.pos;}
		iterator& operator+=(const int &n)
		{
			pos+=n;
			return *this;
		}
		iterator& operator-=(const int &n)
		{
			pos-=n;
			return *this;
		}
		iterator operator++(int) 
		{
			auto pre=iterator(pos,obj);
			pos++;
			return pre;
		}
		iterator& operator++() 
		{
			pos++;
			return *this;
		}
		iterator operator--(int) 
		{
			auto pre=iterator(pos,obj);
			pos--;
			return pre;
		}
		iterator& operator--() 
		{
			pos--;
			return *this;	
		}
		T& operator*() const{return obj->at(pos);}
		bool operator==(const iterator &rhs) const {return pos==rhs.pos&&obj==rhs.obj;}
		bool operator==(const const_iterator &rhs) const {return pos==rhs.pos&&obj==rhs.obj;}
		bool operator!=(const iterator &rhs) const {return pos!=rhs.pos||obj!=rhs.obj;}
		bool operator!=(const const_iterator &rhs) const {return pos!=rhs.pos||obj!=rhs.obj;}
	};
	class const_iterator
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

	private:
		size_t pos;
		const vector<T>* obj;
	public:
		const_iterator(int p,const vector<T>* q):pos(p),obj(q){}
		const_iterator operator+(const int &n) const{return const_iterator(pos+n);}
		const_iterator operator-(const int &n) const{return const_iterator(pos-n);}
		int operator-(const const_iterator &rhs) const{return pos-rhs.pos;}
		const_iterator& operator+=(const int &n)
		{
			pos+=n;
			return *this;
		}
		const_iterator& operator-=(const int &n)
		{
			pos-=n;
			return *this;
		}
		const_iterator operator++(int) 
		{
			auto pre=const_iterator(pos);
			pos++;
			return pre;
		}
		const_iterator& operator++() 
		{
			pos++;
			return *this;
		}
		const_iterator operator--(int) 
		{
			auto pre=const_iterator(pos);
			pos--;
			return pre;
		}
		const_iterator& operator--() 
		{
			pos--;
			return *this;	
		}
		const T& operator*() const{return obj->at(pos);}
		bool operator==(const iterator &rhs) const {return pos==rhs.pos&&obj==rhs.obj;}
		bool operator==(const const_iterator &rhs) const {return pos==rhs.pos&&obj==rhs.obj;}
		bool operator!=(const iterator &rhs) const {return pos!=rhs.pos||obj!=rhs.obj;}
		bool operator!=(const const_iterator &rhs) const {return pos!=rhs.pos||obj!=rhs.obj;}
	};
	vector():p(new char[2*sizeof(T)]),maxs(2),reals(0){}
	vector(const vector &other):maxs(other.maxs),reals(other.reals)
	{
		p=new char[maxs*sizeof(T)];
		for(int i=0;i<reals;i++)
			new(reinterpret_cast<T*>(p+i*sizeof(T)))T(reinterpret_cast<T*>(other.p)[i]);
	}
	~vector()
	{
		for (int i=0;i<reals;i++)
			reinterpret_cast<T*>(p)[i].~T();
		delete[] p;
	}
	vector &operator=(const vector &other) 
	{
		if(this==&other)
			return *this;
		for(int i=0;i<reals;i++)
			reinterpret_cast<T*>(p)[i].~T();
		delete[] p;
		maxs=other.maxs;
		reals=other.reals;
		p=new char[maxs*sizeof(T)];
		for(int i=0;i<reals;i++)
			new(reinterpret_cast<T*>(p+i*sizeof(T)))T(reinterpret_cast<T*>(other.p)[i]);
		return *this;
	}
	T & at(const size_t &pos){return *reinterpret_cast<T*>(p+pos*sizeof(T));}
	const T & at(const size_t &pos) const {return *reinterpret_cast<T*>(p+pos*sizeof(T));}
	T & operator[](const size_t &pos){return *reinterpret_cast<T*>(p+pos*sizeof(T));}
	const T & operator[](const size_t &pos) const {return *reinterpret_cast<const T*>(p+pos*sizeof(T));}
	const T & front() const {return at(0);}
	const T & back() const {return at(reals-1);}
	iterator begin() {return iterator(0,this);}
	const_iterator begin() const {return const_iterator(0,this);}
	const_iterator cbegin() const {return const_iterator(0,this);}
	iterator end() {return iterator(reals,this);}
	const_iterator end() const {return const_iterator(reals,this);}
	const_iterator cend() const {return const_iterator(reals,this);}
	bool empty() const {return !reals;}
	size_t size() const {return reals;}
	void clear()
	{
		for(int i=0;i<reals;i++)
			reinterpret_cast<T*>(p)[i].~T();
		reals=0;
	}
	iterator insert(iterator pos, const T &value) 
	{
		if(reals==maxs)
			doubleSpace();
		reals++;
		for(size_t i=reals-1;i>pos.POS();i--)
			memcpy(p+i*sizeof(T),p+(i-1)*sizeof(T),sizeof(T));
		new(reinterpret_cast<T*>(p+pos.POS()*sizeof(T)))T(value);
		return pos;
	}
	iterator insert(const size_t &ind, const T &value)
	{
		if(reals==maxs)
			doubleSpace();
		reals++;
		for(size_t i=reals-1;i>ind;i--)
			memcpy(p+i*sizeof(T),p+(i-1)*sizeof(T),sizeof(T));
		new(reinterpret_cast<T*>(p+ind*sizeof(T)))T(value);
		return iterator(ind,this);
	}
	iterator erase(iterator pos) 
	{
		reals--;
		reinterpret_cast<T*>(p)[pos.POS()].~T();
		for(size_t i=pos.POS();i<reals;++i)
			memcpy(p+i*sizeof(T),p+(i+1)*sizeof(T),sizeof(T));
		return pos;
	}
	iterator erase(const size_t &ind) 
	{
		reals--;
		reinterpret_cast<T*>(p)[ind].~T();
		for(size_t i=ind;i<reals;i++)
			memcpy(p+i*sizeof(T),p+(i+1)*sizeof(T),sizeof(T));
		return iterator(ind,this);
	}
	void push_back(const T &value) {const size_t pos=reals;insert(pos,value);}
	void pop_back() {reinterpret_cast<T*>(p)[--reals].~T();}
	void resize(const int s)
	{
		const auto q=p;
		p=new char[s*sizeof(T)];
		memcpy(p,q,sizeof(T)*reals);
		delete[] q;
		maxs=reals=s;
	}
	char* data() const {return p;}
};
	template<typename T, class Compare = std::less<T>>
	class priority_queue {
		Compare cmp;
		struct Node
		{
			const T val;
			Node *son=nullptr, *to=nullptr;
			Node(const T& val,Node *s,Node *t):val(val),son(s),to(t){}
		};
		Node *root;
		size_t cnt_of_node=0;
		Node* dfs_copy(Node *x)
		{
			if(x==nullptr)
				return nullptr;
			return new Node(x->val,dfs_copy(x->son),dfs_copy(x->to));
		}
		void dfs_delete(Node *x)
		{
			if(x==nullptr)
				return;
			dfs_delete(x->son);
			dfs_delete(x->to);
			delete x;
		}
		Node* merge_sibling(Node *x, const int cnt=0)
		{
			if(x==nullptr||x->to==nullptr)
				return x;
			Node *y=x->to;
			Node *z=merge_sibling(y->to,cnt+1);
			x->to=y->to=nullptr;

			if(!cmp(x->val,y->val))
			{
				y->to=x->son;
				x->son=y;
			}
			else
			{
				x->to=y->son;
				y->son=x;
				std::swap(x,y);
			}
			if(z==nullptr)
				return x;

			if(!cmp(x->val,z->val))
			{
				z->to=x->son;
				x->son=z;
			}
			else
			{
				x->to=z->son;
				z->son=x;
				std::swap(x,z);
			}
			x->to=nullptr;

			return x;
		}
	public:
		priority_queue():root(nullptr){}
		priority_queue(const priority_queue &other):root(dfs_copy(other.root)),cnt_of_node(other.cnt_of_node){}
		~priority_queue() {dfs_delete(root);}
		priority_queue &operator=(const priority_queue &other)
		{
			if(&other==this)
				return *this;
			dfs_delete(root);
			root=dfs_copy(other.root);
			cnt_of_node=other.cnt_of_node;
			return *this;
		}
		const T & top() const {return root->val;}
		void push(const T &e)
		{
			if(root==nullptr)
			{
				cnt_of_node++;
				root=new Node(e,nullptr,nullptr);
				return ;
			}
			bool chk=!cmp(e,root->val);
			cnt_of_node++;
			if(chk)
			{
				Node *new_root=new Node(e,root,nullptr);
				root=new_root;
			}
			else
			{
				Node *new_son=new Node(e,nullptr,root->son);
				root->son=new_son;
			}
		}

		/**
		 * @brief delete the top element from the priority queue.
		 * @throws container_is_empty if empty() returns true
		 */
		void pop()
		{
			Node *new_root=merge_sibling(root->son);
			cnt_of_node--;
			delete root;
			root=new_root;
		}
		size_t size() const {return cnt_of_node;}
		bool empty() const {return !cnt_of_node;}
		void merge(priority_queue &other)
		{
			if(root==nullptr)
				return root=other.root,cnt_of_node=other.cnt_of_node,other.root=nullptr,other.cnt_of_node=0,void();
			if(other.root==nullptr)
				return ;
			bool chk=cmp(other.root->val,root->val);
			if(!chk)
			{
				root->to=other.root->son;
				other.root->son=root;
				root=other.root;
			}
			else
			{
				other.root->to=root->son;
				root->son=other.root;
			}
			cnt_of_node+=other.cnt_of_node;
			other.root=nullptr,other.cnt_of_node=0;
		}
	};

	template<class T>
	void sort(typename vector<T>::iterator l, typename vector<T>::iterator r)
	{
		if(l==r)
			return ;
		priority_queue<T> p;
		auto ll=l;
		while(ll!=r)
			p.push(*ll),++ll;
		ll=r,--ll;
		while(!p.empty())
		{
			*ll=p.top();
			p.pop();
			if(!p.empty())
				--ll;
		}
	}
}

#endif