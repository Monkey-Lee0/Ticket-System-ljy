#ifndef DATABASE_H
#define DATABASE_H

#include<fstream>
#include<iostream>
#include<utility>
#include<cstring>
// #include<vector>
// #include<algorithm>
// #include<unordered_map>

constexpr int MAXN=24;
constexpr int CACHESIZE=2048;
using name60=char[61];
using name30=char[31];
using name20=char[21];
using name10=char[11];
using name13=char[14];

template<class T>
void my_stable_sort(T* l,T* r,bool (*cmp)(const T& l,const T& r))
{
	int siz=r-l;
	if(!siz)
		return ;
	auto* tmp=new T[siz];
	for(int i=2;(i>>1)<siz;i<<=1)
	{
		for(int j=0;j<siz;j+=i)
		{
			int L=j,R=std::min(L+i,siz),mid=std::min(L+(i>>1),siz);
			if(mid==R)
				continue;
			int pos1=L,pos2=mid;
			while(pos1!=mid||pos2!=R)
			{
				if(pos1==mid)
					tmp[pos1+pos2-L-mid]=l[pos2],++pos2;
				else if(pos2==R)
					tmp[pos1+pos2-L-mid]=l[pos1],++pos1;
				else if(cmp(l[pos2],l[pos1]))
					tmp[pos1+pos2-L-mid]=l[pos2],++pos2;
				else
					tmp[pos1+pos2-L-mid]=l[pos1],++pos1;
			}
			for(int k=0;k<R-L;k++)
				l[L+k]=tmp[k];
		}
	}
	delete[] tmp;
}

template<class T>
class Node
{
public:
	int fa=-1;
	T key[MAXN]{};
	int son[MAXN]{-1};
	Node(){memset(son,-1,sizeof(son));}
};

template<class T>
class List
{
public:
	std::pair<bool,T>* val;
	int cnt=0,ms=0;
	List()=default;
	explicit List(const bool tim,const T& a):ms(4)
	{
		val=new std::pair<bool,T>[4];
		val[cnt++]=std::make_pair(tim,a);
	}
	List(const List<T> &other):cnt(other.cnt),ms(other.ms)
	{
		if(ms)
		{
			val=new std::pair<bool,T>[ms];
			memcpy(val,other.val,sizeof(std::pair<bool,T>)*ms);
		}
	}
	void doubleSpace()
	{
		auto odd=val;
		val=new std::pair<bool,T>[ms<<1];
		if(odd!=nullptr)
			memcpy(val,odd,sizeof(std::pair<bool,T>)*ms),delete[] odd;
		ms<<=1;
	}
	void push_back(const T &p)
	{
		if(!ms)
			val=new std::pair<bool,T>[4],ms=4;
		else if(cnt==ms)
			doubleSpace();
		val[cnt++]=std::make_pair(1,p);
	}
	bool empty()const{return !cnt;}
	T& operator[](int pos){return val[pos].second;}
	~List(){;if(ms)delete[] val;}
};

// An advanced Memoryriver for any normal types and lists.
template<int info_len=4>
class DataInteractor
{
private:
	std::fstream file;
	std::string file_name;
	int info[info_len]{};
	// std::unordered_map<int,std::string> cache;
	// std::unordered_map<int,bool> cache_state;
	// std::unordered_map<int,int> cache_times;
	// std::vector<int> cache_stack[10];
	// void download()// Erase block from cache
	// {
	// 	int index=-1;
	// 	for(int i=0;i<=9&&index==-1;i++)
	// 		while(!cache_stack[i].empty())
	// 		{
	// 			if(!cache_times.contains(cache_stack[i].back()))
	// 				exit(0);
	// 			const auto real=std::min(9,cache_times[cache_stack[i].back()]);
	// 			if(real!=i)
	// 				cache_stack[real].push_back(cache_stack[i].back());
	// 			else
	// 				index=cache_stack[i].back();
	// 			cache_stack[i].pop_back();
	// 			if(index!=-1)
	// 				break;
	// 		}
	// 	const int state=cache_state[index];cache_state.erase(index);
	// 	const auto data=cache[index];cache.erase(index);
	// 	cache_times.erase(index);
	// 	if(!state)
	// 		return ;
	// 	file.seekp(index);
	// 	file.write(data.data(),CACHESIZE);
	// }
	// void upload(const int block)// Push block to cache  (if overcrowded, delete the element earliest visited)
	// {
	// 	if(block+CACHESIZE>info[0])
	// 	{
	// 		file.seekp(info[0]);
	// 		char a[block+CACHESIZE-info[0]];
	// 		file.write(a,block+CACHESIZE-info[0]);
	// 	}
	// 	if(cache.contains(block))
	// 		return cache_times[block]++,void();
	// 	if(cache.size()>1048576)
	// 		download();
	// 	std::string p;
	// 	p.resize(CACHESIZE);
	// 	file.seekg(block);
	// 	file.read(p.data(),CACHESIZE);
	// 	cache.emplace(block,p);
	// 	cache_state.emplace(block,false);
	// 	cache_times[block]=0;
	// 	cache_stack[0].push_back(block);
	// }
	// void new_block(const int block)// create a block
	// {
	// 	file.seekp(block);
	// 	char str[CACHESIZE];
	// 	file.write(str,CACHESIZE);
	// }
	// std::string read_block(const int block)// read a block
	// {
	// 	upload(block);
	// 	return cache[block];
	// }
	// void write_block(const int block,const std::string& str)// write to a block
	// {
	// 	upload(block);
	// 	cache[block]=str;
	// 	cache_state[block]=true;
	// }
	// static int belonged_block(const int index)// to get which block the index is in
	// {
	// 	return (index-info_len*sizeof(int))/CACHESIZE*CACHESIZE+info_len*sizeof(int);
	// }
	// std::string read_data(const int l,const int r)// to get data from an interval
	// {
	// 	const int bl=belonged_block(l),br=belonged_block(r-1);
	// 	std::string total_str;
	// 	for(int i=bl;i<=br;i+=CACHESIZE)
	// 		total_str+=read_block(i);
	// 	return total_str.substr(l-bl,r-l);
	// }
	// void write_data(const int l,const int r,const std::string& str)// to write data to an interval
	// {
	// 	if(const int bl=belonged_block(l),br=belonged_block(r-1); bl==br)
	// 	{
	// 		std::string tmp=read_block(bl);
	// 		for(int i=l;i<r;++i)
	// 			tmp[i-bl]=str[i-l];
	// 		write_block(bl,tmp);
	// 	}
	// 	else
	// 	{
	// 		std::string tmp=read_block(bl);
	// 		for(int i=l;i<bl+CACHESIZE;++i)
	// 			tmp[i-bl]=str[i-l];
	// 		write_block(bl,tmp);
	// 		for(int i=bl+CACHESIZE;i<br;i+=CACHESIZE)
	// 			write_block(i,str.substr(i-l,CACHESIZE));
	// 		tmp=read_block(br);
	// 		for(int i=br;i<r;++i)
	// 			tmp[i-br]=str[i-l];
	// 		write_block(br,tmp);
	// 	}
	// }
public:
	// services for constructor and destructor
	explicit DataInteractor(const std::string &FILE,const int clear=false):file_name(FILE)
	{
		system("mkdir data");
		std::ifstream tmp_file;tmp_file.open(FILE);
		const int mode=!tmp_file.good()||clear;
		tmp_file.close();
		if(mode)
		{
			file.open(FILE,std::ios::out);
			int tmp=0,initial=info_len*sizeof(int);
			file.write(reinterpret_cast<char *>(&initial),sizeof(int));
			file.write(reinterpret_cast<char *>(&initial),sizeof(int));
			info[0]=info[1]=initial;
			for(int i=2;i<info_len;++i)
				file.write(reinterpret_cast<char *>(&tmp),sizeof(int)),info[i]=0;
			file.close();
			file.open(FILE,std::ios::in|std::ios::out);
		}
		else
		{
			file.open(FILE,std::ios::in|std::ios::out);
			for(int i=0;i<info_len;i++)
				file.read(reinterpret_cast<char*>(&info[i]),sizeof(int));
		}
	}
	void reload()
	{
		file.close();
		file.open(file_name,std::ios::out);
		int tmp=0,initial=info_len*sizeof(int);
		file.write(reinterpret_cast<char *>(&initial),sizeof(int));
		file.write(reinterpret_cast<char *>(&initial),sizeof(int));
		info[0]=info[1]=initial;
		for(int i=2;i<info_len;++i)
			file.write(reinterpret_cast<char *>(&tmp),sizeof(int)),info[i]=0;
		file.close();
		file.open(file_name,std::ios::in|std::ios::out);
	}
	~DataInteractor()
	{
		file.seekp(0);
		for(int i=0;i<info_len;i++)
			file.write(reinterpret_cast<char*>(&info[i]),sizeof(int));
		// while(!cache.empty())
		// 	download();
		file.close();
	}

	// services for information
	int read_info(const int index){return info[index];}
	void update_info(const int tmp,const int index){info[index]=tmp;}

	// services for normal type T
	template<class T>
	T read_T(const int &index)
    {
        file.seekg(index);
        T tmp;
        file.read(reinterpret_cast<char*>(&tmp),sizeof(T));
        return tmp;
        // return *reinterpret_cast<T*>(read_data(index,index+sizeof(T)).data());
    }
	template<class T>
	void update_T(T &a,const int index)
	{
        file.seekp(index);
        file.write(reinterpret_cast<const char*>(&a),sizeof(T));
		// std::string str;
		// str.resize(sizeof(T));
		// memcpy(str.data(),&a,sizeof(T));
		// write_data(index,index+sizeof(T),str);
	}
	template<class T>
	int write_T(T &a)
	{
        const int index=info[0];info[0]+=sizeof(T);
        file.seekp(index);
        file.write(reinterpret_cast<const char*>(&a),sizeof(T));
        return index;
		// const int index=info[0];
		// info[0]+=sizeof(T);
		// while(info[1]<info[0])
		// {
		// 	new_block(info[1]);
		// 	info[1]+=CACHESIZE;
		// }
		// update_T(a,index);
		// return index;
	}

	// services for type List<T>
	template<class T>
	List<T> read_list(const int index)
	{
		List<T> res;
        file.seekg(index);
        file.read(reinterpret_cast<char*>(&res.cnt),sizeof(int));
        file.read(reinterpret_cast<char*>(&res.ms),sizeof(int));
        res.val=new std::pair<bool,T>[res.ms];
        file.read(reinterpret_cast<char*>(res.val),res.ms*sizeof(std::pair<bool,T>));
		// res.cnt=*reinterpret_cast<int*>(read_data(index,index+sizeof(int)).data());
		// res.ms=*reinterpret_cast<int*>(read_data(index+sizeof(int),index+2*sizeof(int)).data());
		// res.val=new std::pair<bool,T>[res.ms];
		// memcpy(reinterpret_cast<char*>(res.val),read_data(index+2*sizeof(int),
		// 	index+2*sizeof(int)+res.ms*sizeof(std::pair<bool,T>)).data(),res.ms*sizeof(std::pair<bool,T>));
		return res;
	}
	template<class T>
	void update_list(List<T> &a,const int index)
	{
        file.seekp(index);
        file.write(reinterpret_cast<char*>(&a.cnt),sizeof(int));
        file.write(reinterpret_cast<char*>(&a.ms),sizeof(int));
        file.write(reinterpret_cast<char*>(a.val),a.ms*sizeof(std::pair<bool,T>));
		// update_T(a.cnt,index);
		// update_T(a.ms,index+sizeof(int));
		// std::string str;
		// str.resize(a.ms*sizeof(std::pair<bool,T>));
		// memcpy(str.data(),reinterpret_cast<char*>(a.val),a.ms*sizeof(std::pair<bool,T>));
		// write_data(index+2*sizeof(int),index+2*sizeof(int)+a.ms*sizeof(std::pair<bool,T>),str);
	}
	template<class T>
	int write_list(List<T> &a)
	{
		const int index=info[0];
		info[0]+=2*sizeof(int)+a.ms*sizeof(std::pair<bool,T>);
        file.seekp(index);
        file.write(reinterpret_cast<char*>(&a.cnt),sizeof(int));
        file.write(reinterpret_cast<char*>(&a.ms),sizeof(int));
        file.write(reinterpret_cast<char*>(a.val),a.ms*sizeof(std::pair<bool,T>));
		// while(info[1]<info[0])
		// {
		// 	new_block(info[1]);
		// 	info[1]+=CACHESIZE;
		// }
		// update_list<T>(a,index);
		return index;
	}
};

template<class T>
int COUNT_OF(const Node<T> &a,bool op=false)
{
	for(int i=0;i<MAXN;i++)
		if(a.son[i]==-1)
			return i;
	return MAXN;
}

template<class T,class T0,bool clear=false>
class B_Plus_Tree
{
	DataInteractor<> file;
	int search_to_leaf(const T key)
	{
		int now=file.read_info(2),depth=file.read_info(3);
		while(depth!=1)
		{
			--depth;
			const auto tmp=file.read_T<Node<T>>(now);
			for(int i=1;i<=MAXN;i++)
				if(i==MAXN||tmp.son[i]==-1||strcmp(tmp.key[i],key)>0)
				{
					now=tmp.son[i-1];
					break;
				}
		}
		return now;
	}
	void split(const int &index,Node<T> left)
	{
		Node<T> right;
		for(int i=MAXN/2+1;i<MAXN;i++)
		{
			strcpy(right.key[i-MAXN/2],left.key[i]);
			right.son[i-MAXN/2]=left.son[i],left.son[i]=-1;
		}
		right.son[0]=left.son[MAXN/2];left.son[MAXN/2]=-1;
		right.fa=left.fa;
		file.update_T(left,index);
		int index_r=file.write_T(right);
		for(int i=0;i<MAXN/2;i++)
			file.update_T(index_r,right.son[i]);
		if(left.fa==-1)
		{
			Node<T> new_root;
			strcpy(new_root.key[1],left.key[MAXN/2]);
			new_root.son[0]=index,new_root.son[1]=index_r;
			left.fa=right.fa=file.write_T(new_root);
			file.update_T(left,index);
			file.update_T(right,index_r);
			const int depth=file.read_info(3)+1;
			file.update_info(left.fa,2);
			file.update_info(depth,3);
		}
		else
		{
			auto fa=file.read_T<Node<T>>(left.fa);
			const int cnt=COUNT_OF(fa);
			int pos=0;
			for(int i=0;i<cnt;i++)
				if(fa.son[i]==index)
				{
					pos=i+1;
					break;
				}
			for(int i=cnt-1;i>=pos;--i)
				strcpy(fa.key[i+1],fa.key[i]),fa.son[i+1]=fa.son[i];
			strcpy(fa.key[pos],left.key[MAXN/2]),fa.son[pos]=index_r;
			if(cnt+1>=MAXN)
				split(left.fa,fa);
			else
				file.update_T(fa,left.fa);
		}
	}
public:
	explicit B_Plus_Tree(const std::string &s1):file(s1,clear){}
	void reload(){file.reload();}
	bool empty(){return !file.read_info(3);}
	void Insert(const T key,const T0 val)
	{
		if(int depth=file.read_info(3); !depth)
		{
			Node<T> a;List<T0> b(true,val);
			strcpy(a.key[0],key);
			a.son[0]=file.write_list(b);
			file.update_info(file.write_T(a),2);
			++depth;
			file.update_info(depth,3);
			return ;
		}
		int now=search_to_leaf(key);
		auto tmp=file.read_T<Node<T>>(now);
		const auto cnt=COUNT_OF(tmp);
		int pos=cnt;
		for(int i=0;i<cnt;i++)
		{
			const int ok=strcmp(tmp.key[i],key);
			if(ok==0)
			{
				if(tmp.son[i]>=0)
				{
					const auto ms=file.read_T<int>(tmp.son[i]+sizeof(int));
					if(const auto CNT=file.read_T<int>(tmp.son[i])+1; CNT<=ms)
					{
						file.update_T(CNT,tmp.son[i]);
						auto tmp_pair=std::make_pair(true,val);
						file.update_T(tmp_pair,tmp.son[i]+2*sizeof(int)+(CNT-1)*sizeof(std::pair<bool,T0>));
					}
					else
					{
						auto X=file.read_list<T0>(tmp.son[i]);
						X.doubleSpace();
						X.val[X.cnt++]=std::make_pair(true,val);
						tmp.son[i]=file.write_list(X);
						file.update_T(tmp,now);
					}
					return ;
				}
				List<T0> X(true,val);
				tmp.son[i]=file.write_list(X);
				file.update_T(tmp,now);
				return ;
			}
			if(ok>0)
			{
				pos=i;
				break;
			}
		}
		for(int i=cnt-1;i>=pos;--i)
			strcpy(tmp.key[i+1],tmp.key[i]),tmp.son[i+1]=tmp.son[i];
		List<T0> X(true,val);
		strcpy(tmp.key[pos],key),tmp.son[pos]=file.write_list(X);
		if(cnt+1>=MAXN)
		{
			Node<T> right;
			for(int i=MAXN/2;i<MAXN;i++)
			{
				strcpy(right.key[i-MAXN/2],tmp.key[i]);
				right.son[i-MAXN/2]=tmp.son[i],tmp.son[i]=-1;
			}
			right.fa=tmp.fa;
			file.update_T(tmp,now);
			const int index_r=file.write_T(right);
			if(tmp.fa==-1)
			{
				Node<T> new_root;
				strcpy(new_root.key[1],right.key[0]);
				new_root.son[0]=now,new_root.son[1]=index_r;
				tmp.fa=right.fa=file.write_T(new_root);
				file.update_T(tmp,now);
				file.update_T(right,index_r);
				const int Depth=file.read_info(3)+1;
				file.update_info(tmp.fa,2);
				file.update_info(Depth,3);
			}
			else
			{
				auto fa=file.read_T<Node<T>>(tmp.fa);
				const int cnt1=COUNT_OF(fa);
				int pos1=0;
				for(int i=0;i<cnt1;i++)
					if(fa.son[i]==now)
					{
						pos1=i+1;
						break;
					}
				for(int i=cnt1-1;i>=pos1;--i)
					strcpy(fa.key[i+1],fa.key[i]),fa.son[i+1]=fa.son[i];
				strcpy(fa.key[pos1],right.key[0]),fa.son[pos1]=index_r;
				if(cnt1+1>=MAXN)
					split(tmp.fa,fa);
				else
					file.update_T(fa,tmp.fa);
			}
		}
		else
			file.update_T(tmp,now);
	}
	List<T0> Find(const T key)
	{
		if(!file.read_info(3))
			return {};
		const int now=search_to_leaf(key);
		auto tmp=file.read_T<Node<T>>(now);
		const auto cnt=COUNT_OF(tmp);
		for(int i=0;i<cnt;i++)
		{
			const int ok=strcmp(tmp.key[i],key);
			if(ok==0)
			{
				if(tmp.son[i]<0)
					return {};
				auto X=file.read_list<T0>(tmp.son[i]);
				my_stable_sort<std::pair<bool,T0>>(X.val,X.val+X.cnt,
					[](const std::pair<bool,T0> &a,const std::pair<bool,T0> &b)->bool{return a.second<b.second;});
				if(!X.cnt)
					return {};
				List<T0> res;
				T0 las=X.val[0].second;int count=0;
				for(int j=0;j<X.cnt;j++)
					if(X.val[j].second==las)
					{
						if(X.val[j].first)
							count++;
						else if(count)
							count--;
					}
					else
					{
						if(count)
							res.push_back(las);
						las=X.val[j].second;
						if(X.val[j].first)
							count=1;
						else
							count=0;
					}
				if(count)
					res.push_back(las);
				if(res.cnt!=X.cnt)
				{
					X.cnt=0;
					for(int j=0;j<res.cnt;j++)
						X.val[X.cnt++]=res.val[j];
					file.update_list(X,tmp.son[i]);
				}
				return res;
			}
			if(ok>0)
				return{};
		}
		return {};
	}
	void Delete(const T key,const T0 val)
	{
		if(!file.read_info(3))
			return ;
		int now=search_to_leaf(key);
		auto tmp=file.read_T<Node<T>>(now);
		const auto cnt=COUNT_OF(tmp);
		for(int i=0;i<cnt;i++)
		{
			const int ok=strcmp(tmp.key[i],key);
			if(ok==0&&tmp.son[i]>=0)
			{
				const auto ms=file.read_T<int>(tmp.son[i]+sizeof(int));
				if(const auto CNT=file.read_T<int>(tmp.son[i])+1; CNT<=ms)
				{
					file.update_T(CNT,tmp.son[i]);
					const auto tmp_pair=std::make_pair(false,val);
					file.update_T(tmp_pair,tmp.son[i]+2*sizeof(int)+(CNT-1)*sizeof(std::pair<bool,T0>));
				}
				else
				{
					auto X=file.read_list<T0>(tmp.son[i]);
					X.doubleSpace();
					X.val[X.cnt++]=std::make_pair(false,val);
					tmp.son[i]=file.write_list(X);
					file.update_T(tmp,now);
				}
				return ;
			}
			if(ok>=0)
				return ;
		}
	}
	List<T0> Find_ordered(const T key)
	{
		if(!file.read_info(3))
			return {};
		const int now=search_to_leaf(key);
		auto tmp=file.read_T<Node<T>>(now);
		const auto cnt=COUNT_OF(tmp);
		for(int i=0;i<cnt;i++)
		{
			const int ok=strcmp(tmp.key[i],key);
			if(ok==0)
			{
				if(tmp.son[i]<0)
					return {};
				return file.read_list<T0>(tmp.son[i]);
			}
			if(ok>0)
				return{};
		}
		return {};
	}
	T0 Find_kth(const T key,const int k)
	{
		if(!file.read_info(3))
			return {};
		const int now=search_to_leaf(key);
		auto tmp=file.read_T<Node<T>>(now);
		const auto cnt=COUNT_OF(tmp);
		for(int i=0;i<cnt;i++)
		{
			const int ok=strcmp(tmp.key[i],key);
			if(ok==0)
			{
				if(tmp.son[i]<0)
					return {};
				const int count=file.read_T<int>(tmp.son[i]);
				if(count<k)
					return T0();
				return file.read_T<std::pair<bool,T0>>(tmp.son[i]+2*sizeof(int)+
					(count-k)*sizeof(std::pair<bool,T0>)).second;
			}
			if(ok>0)
				return T0();
		}
		return {};
	}
};

#endif