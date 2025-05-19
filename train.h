#ifndef TRAIN_H
#define TRAIN_H

#include "my_time.h"
#include "database.h"
#include "tools.h"

inline B_Plus_Tree<char[21],int> train_tree("data/TrainTree");
inline B_Plus_Tree<char[62],int> from_to_tree("data/FromToTree");
inline B_Plus_Tree<char[24],int> wait_tree("data/WaitTree");
inline DataInteractor train_info("data/TrainInfo");
inline DataInteractor wait_list("data/WaitList");

struct Train
{
    char trainID[21]="";
    int stationNum=-1;
    char stations[101][31]{};
    int seatNum=0;
    int prices[101]{};
    Time startTime;
    int travelTimes[101]{};
    int stopoverTimes[101]{};
    Time saleDate_l,saleDate_r;
    char type=' ';
    bool release=false;
    int seats[101][101]{};
};

struct WaitList
{
    int pre;
    int pos;
    int nxt;
    int final;
};

inline bool add_train(const char* trainID,const int stationNum,const int seatNum,const char* stations_str,
                      const char* prices_str,const char* startTime_str,const char* travelTimes_str,
                      const char* stopoverTimes_str,const char* saleDate_str,const char type)
{
    if(!train_tree.Find(my_c_str<21>(trainID)).empty())
        return false;
    Train tmp;
    strcpy(tmp.trainID,trainID);
    tmp.stationNum=stationNum;
    tmp.seatNum=seatNum;

    int cnt=0,len=static_cast<int>(strlen(stations_str));std::string str;
    for(int i=0;i<len;i++)
    {
        if(stations_str[i]=='|')
        {
            strcpy(tmp.stations[cnt++],str.data());
            str.clear();
        }
        else
            str.push_back(stations_str[i]);
    }
    strcpy(tmp.stations[cnt++],str.data());

    cnt=0,len=static_cast<int>(strlen(prices_str)),str.clear();
    for(int i=0;i<len;i++)
    {
        if(prices_str[i]=='|')
        {
            tmp.prices[cnt++]=std::stoi(str);
            str.clear();
        }
        else
            str.push_back(prices_str[i]);
    }
    tmp.prices[cnt++]=std::stoi(str);

    tmp.startTime=Time(startTime_str,false);

    cnt=0,len=static_cast<int>(strlen(travelTimes_str)),str.clear();
    for(int i=0;i<len;i++)
    {
        if(travelTimes_str[i]=='|')
        {
            tmp.travelTimes[cnt++]=std::stoi(str);
            str.clear();
        }
        else
            str.push_back(travelTimes_str[i]);
    }
    tmp.travelTimes[cnt++]=std::stoi(str);

    if(stopoverTimes_str[0]!='_')
    {
        cnt=0,len=static_cast<int>(strlen(stopoverTimes_str)),str.clear();
        for(int i=0;i<len;i++)
        {
            if(stopoverTimes_str[i]=='|')
            {
                tmp.stopoverTimes[cnt++]=std::stoi(str);
                str.clear();
            }
            else
                str.push_back(stopoverTimes_str[i]);
        }
        tmp.stopoverTimes[cnt++]=std::stoi(str);
    }

    cnt=0,len=static_cast<int>(strlen(saleDate_str)),str.clear();
    for(int i=0;i<len;i++)
    {
        if(saleDate_str[i]=='|')
        {
            tmp.saleDate_l=Time(str,true,false);
            str.clear();
        }
        else
            str.push_back(saleDate_str[i]);
    }
    tmp.saleDate_r=Time(str,true,false);

    tmp.type=type;

    const int pos=train_info.write_T(tmp);
    train_tree.Insert(my_c_str<21>(trainID),pos);

    for(int i=0;i<stationNum;i++)
        for(int j=i;j<stationNum;j++)
            from_to_tree.Insert(my_c_str<62>(std::string(tmp.stations[i])+"-"+std::string(tmp.stations[j])),pos);

    return true;
}

inline Train query_train(const char* trainID)
{
    auto res=train_tree.Find(my_c_str<21>(trainID));
    if(res.empty())
        return {};
    return train_info.read_T<Train>(res[0]);
}

inline bool delete_train(const char* trainID)
{
    if(train_tree.Find(my_c_str<21>(trainID)).empty())
        return false;
    auto res=train_tree.Find(my_c_str<21>(trainID));
    if(res.empty())
        return {};
    if(train_info.read_T<Train>(res[0]).release)
        return false;
    const auto train=train_info.read_T<Train>(res[0]);
    for(int i=0;i<train.stationNum;i++)
        for(int j=i;j<train.stationNum;j++)
            from_to_tree.Delete(my_c_str<62>(std::string(train.stations[i])+"-"+std::string(train.stations[j])),
                res[0]);
    train_tree.Delete(my_c_str<21>(trainID),res[0]);
    return true;
}

inline bool release_train(const char* trainID)
{
    if(train_tree.Find(my_c_str<21>(trainID)).empty())
        return false;
    auto res=train_tree.Find(my_c_str<21>(trainID));
    if(res.empty())
        return false;
    auto train=train_info.read_T<Train>(res[0]);
    if(train.release)
        return false;
    train.release=true;
    train_info.update_T(train,res[0]);
    const int l_md=train.saleDate_l.to_int_md();
    const int r_md=train.saleDate_r.to_int_md();
    for(int i=l_md;i<=r_md;i++)
    {
        WaitList tmp1(-1,-1,0,0);
        const int pos1=wait_list.write_T(tmp1);
        WaitList tmp2(pos1,-1,-1,-1);
        const int pos2=wait_list.write_T(tmp2);
        tmp1.nxt=tmp1.final=pos2;
        wait_list.update_T(tmp1,pos1);
        wait_tree.Insert(my_c_str<24>(std::string(trainID)+std::to_string(i)),pos1);
    }

    return true;
}

inline std::pair<int,std::string*> query_ticket(const char* depart,const char* dest,
    const char* day_str,const char* key)
{
    auto res=from_to_tree.Find(my_c_str<62>(std::string(depart)+"-"+std::string(dest)));
    const Time today(day_str,true,false);
    int count=0;
    using T3=std::pair<std::pair<int,std::string>,std::string>;
    auto* tmp=new T3[res.cnt];
    for(int i=0;i<res.cnt;i++)
    {
        const auto train=train_info.read_T<Train>(res[i]);
        if(!train.release)
            continue;
        Time now_time=train.startTime;
        now_time.mon=today.mon,now_time.day=today.day;
        int s=0;
        for(int j=0;j<train.stationNum;j++)
        {
            if(strcmp(train.stations[j],depart)==0)
            {
                s=j;
                break;
            }
            now_time=now_time+train.stopoverTimes[j]+train.travelTimes[j];
        }
        const int start_md=2*today.to_int_md()-now_time.to_int_md();
        const int l_md=train.saleDate_l.to_int_md();
        const int r_md=train.saleDate_r.to_int_md();
        if(start_md<l_md||start_md>r_md)
            continue;
        now_time.mon=today.mon,now_time.day=today.day;
        std::string str=std::string(train.trainID)+" "+std::string(depart)+" "+now_time.to_string()+" -> "+
            std::string(dest)+" ";
        int total_time=0,total_price=0,max_seat=train.seatNum;
        for(int j=s;j<train.stationNum;j++)
        {
            if(strcmp(train.stations[j],dest)==0)
                break;
            now_time=now_time+train.travelTimes[j];
            total_time+=train.travelTimes[j];
            if(j>s)
                total_time+=train.stopoverTimes[j-1],now_time=now_time+train.stopoverTimes[j-1];
            total_price+=train.prices[j];
            max_seat=std::min(max_seat,train.seatNum-train.seats[start_md][j]);
        }
        str+=now_time.to_string()+" "+std::to_string(total_price)+" "+std::to_string(max_seat);
        if(key[0]=='t')
            tmp[count++]=std::make_pair(std::make_pair(total_time,std::string(train.trainID)),str);
        else
            tmp[count++]=std::make_pair(std::make_pair(total_price,std::string(train.trainID)),str);
    }
    my_stable_sort<T3>(tmp,tmp+count,[](const T3& a,const T3&b)->bool{
        if(a.first.first!=b.first.first)
            return a.first.first<b.first.first;
        return a.first.second<b.first.second;
    });
    auto* ans=new std::string[count];
    for(int i=0;i<count;i++)
        ans[i]=tmp[i].second;
    delete[] tmp;
    return std::make_pair(count,ans);
}

inline std::pair<std::string,std::string> query_transfer(const char* depart,const char* dest,
    const char* day_str,const char* key)
{
    auto res=from_to_tree.Find(my_c_str<62>(std::string(depart)+"-"+std::string(depart)));
    if(res.empty())
        return std::make_pair("","");
    const Time today(day_str,true,false);
    int ans1=1e9,ans2=1e9;
    std::string ans3,ans4;
    std::string str1,str2;
    for(int i=0;i<res.cnt;i++)
    {
        const auto train=train_info.read_T<Train>(res[i]);
        if(!train.release)
            continue;
        Time now_time=train.startTime;
        now_time.mon=today.mon,now_time.day=today.day;
        int s=0;
        for(int j=0;j<train.stationNum;j++)
        {
            if(strcmp(train.stations[j],depart)==0)
            {
                s=j;
                break;
            }
            now_time=now_time+train.stopoverTimes[j]+train.travelTimes[j];
        }
        const int start_md=2*today.to_int_md()-now_time.to_int_md();
        const int l_md=train.saleDate_l.to_int_md();
        const int r_md=train.saleDate_r.to_int_md();
        if(start_md<l_md||start_md>r_md)
            continue;
        now_time.mon=today.mon,now_time.day=today.day;
        std::string Str1_t=std::string(train.trainID)+" "+std::string(depart)+" "+now_time.to_string()+" -> ";
        int total_time=0,total_price=0,max_seat=train.seatNum;
        for(int j=s;j<train.stationNum;j++)
        {
            if(j!=s)
            {
                std::string Str1=Str1_t+std::string(train.stations[j])+" "+now_time.to_string()+" "+
                    std::to_string(total_price)+" "+std::to_string(max_seat);
                auto res_inner=from_to_tree.Find(my_c_str<62>(
                    std::string(train.stations[j])+"-"+std::string(dest)));
                for(int k=0;k<res_inner.cnt;k++)
                {
                    const auto train_inner=train_info.read_T<Train>(res_inner[k]);
                    if(!train_inner.release)
                        continue;
                    Time inner_time=train_inner.startTime;
                    inner_time.mon=now_time.mon,inner_time.day=now_time.mon;
                    int ss=0;
                    for(int p=0;p<train_inner.stationNum;p++)
                    {
                        if(strcmp(train_inner.stations[p],train.stations[j])==0)
                        {
                            ss=p;
                            break;
                        }
                        inner_time=inner_time+train_inner.stopoverTimes[p]+train_inner.travelTimes[p];
                    }
                    int start_md_inner=2*now_time.to_int_md()-inner_time.to_int_md();
                    inner_time.mon=now_time.mon,inner_time.day=now_time.day;
                    if(inner_time<now_time)
                        inner_time=inner_time+1440,start_md_inner++;
                    const int l_md_inner=train_inner.saleDate_l.to_int_md();
                    const int r_md_inner=train_inner.saleDate_r.to_int_md();
                    if(start_md_inner>r_md_inner)
                        continue;
                    if(start_md_inner<l_md_inner)
                        now_time=now_time+1440*(l_md_inner-start_md_inner);
                    std::string Str2=std::string(train_inner.trainID)+" "+std::string(train.stations[j])+" "+
                        inner_time.to_string()+" -> "+std::string(dest)+" ";
                    int max_seat_inner=train_inner.seatNum;
                    int total_price_inner=total_price;
                    for(int p=ss;p<train_inner.stationNum;p++)
                    {
                        if(strcmp(train_inner.stations[p],dest)==0)
                            break;
                        inner_time=inner_time+train_inner.travelTimes[p];
                        if(p>ss)
                            inner_time=inner_time+train_inner.stopoverTimes[p-1];
                        total_price_inner+=train_inner.prices[p];
                        max_seat_inner=std::min(max_seat_inner,train_inner.seatNum-
                            train_inner.seats[start_md_inner][p]);
                    }
                    int total_time_inner=total_time+(inner_time.to_int()-now_time.to_int());
                    Str2+=inner_time.to_string()+" "+std::to_string(total_price_inner-total_price)+" "+
                        std::to_string(max_seat_inner);
                    std::string ID1=std::string(train.trainID);
                    std::string ID2=std::string(train_inner.trainID);
                    if(key[0]=='t')
                        if(total_time_inner<ans1||(total_time_inner==ans1&&total_price_inner<ans2)||
                            (total_time_inner==ans1&&total_price_inner==ans2&&ID1<ans3)||
                            (total_time_inner==ans1&&total_price_inner==ans2&&ID1==ans3&&ID2<ans4))
                            ans1=total_time_inner,ans2=total_price_inner,ans3=ID1,ans4=ID2,str1=Str1,str2=Str2;
                    if(key[0]=='p')
                        if(total_price_inner<ans1||(total_price_inner==ans1&&total_time_inner<ans2)||
                            (total_price_inner==ans1&&total_time_inner==ans2&&ID1<ans3)||
                            (total_price_inner==ans1&&total_time_inner==ans2&&ID1==ans3&&ID2<ans4))
                            ans1=total_time_inner,ans2=total_price_inner,ans3=ID1,ans4=ID2,str1=Str1,str2=Str2;
                }
            }
            now_time=now_time+train.travelTimes[j];
            total_time+=train.travelTimes[j];
            if(j>s)
                now_time=now_time+train.stopoverTimes[j-1],total_time+=train.stopoverTimes[j-1];
            total_price+=train.prices[j];
            max_seat=std::min(max_seat,train.seatNum-train.seats[start_md][j]);
        }
    }
    return {str1,str2};
}

#endif
