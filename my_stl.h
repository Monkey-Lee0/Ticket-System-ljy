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
		auto pre_p=p;
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

	template<class T>
	void sort(typename vector<T>::iterator ll, typename vector<T>::iterator rr)
	{
		static bool init=false;
		if(!init)
			init=true,srand(time(nullptr));
		vector<std::pair<typename vector<T>::iterator, typename vector<T>::iterator>> sta;
		sta.push_back(std::make_pair(ll,rr));
		while(!sta.empty())
		{
			auto l=sta.back().first,r=sta.back().second;
			sta.pop_back();
			if(r-l<=0)
				continue;
			vector<T> L,M,R;
			int target=rand()%(r-l+1);
			T target_value=*(l+target);
			for(auto i=l;i!=r;++i)
				if(*i<target_value)
					L.push_back(*i);
				else if(*i==target_value)
					M.push_back(*i);
				else
					R.push_back(*i);
			for(auto i=l;i!=l+L.size();++i)
				*i=L[i-l];
			for(auto i=l+L.size();i!=r-R.size();++i)
				*i=M[i-l-L.size()];
			for(auto i=r-R.size();i!=r;++i)
				*i=R[r-i-1];
			sta.push_back(std::make_pair(l,l+L.size()));
			sta.push_back(std::make_pair(r-R.size(),r));

		}
	}
}

#endif