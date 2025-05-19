#ifndef TICKET_H
#define TICKET_H

#include "database.h"
#include "train.h"
#include "user.h"

inline B_Plus_Tree<char[21],int> ticket_tree("data/TicketTree");
inline DataInteractor ticket_info("data/TicketInfo");

struct Ticket
{
    int status=-2;
    char trainID[21]{};
    char depart[31]{};
    Time depart_time;
    char dest[31]{};
    Time dest_time;
    int price=0;
    int num=0;
    int timeID=-1;
    std::string to_string()
    {
        std::string str;
        if(status==-1)
            str="[refunded]";
        else if(status==0)
            str="[pending]";
        else
            str="[success]";
        return str+" "+std::string(trainID)+" "+std::string(depart)+" "+depart_time.to_string()+" -> "+
            std::string(dest)+" "+dest_time.to_string()+" "+std::to_string(price)+" "+std::to_string(num);
    }
};

inline int buy_ticket(const char* username,const char* trainID,const char* day_str,int num,const char* depart,
                      const char* dest,const bool queue)
{
    if(login_state.Find(my_c_str<21>(username)).empty())
        return -1;
    auto res=train_tree.Find(my_c_str<21>(trainID));
    if(res.empty())
        return -1;
    auto train=train_info.read_T<Train>(res[0]);
    if(!train.release)
        return -1;
    auto ticket=Ticket();
    strcpy(ticket.trainID,trainID);
    strcpy(ticket.depart,depart);
    strcpy(ticket.dest,dest);
    ticket.num=num;
    const Time today(day_str,true,false);
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
        return -1;
    now_time.mon=today.mon,now_time.day=today.day;
    ticket.depart_time=now_time;
    ticket.timeID=start_md;
    int total_time=0,total_price=0,max_seat=train.seatNum;
    for(int j=s;j<train.stationNum;j++)
    {
        if(strcmp(train.stations[j],dest)==0)
            break;
        now_time=now_time+train.travelTimes[j];
        total_time+=train.travelTimes[j];
        if(j>s)
            now_time=now_time+train.stopoverTimes[j-1],total_time+=train.stopoverTimes[j-1];
        total_price+=train.prices[j];
        max_seat=std::min(max_seat,train.seatNum-train.seats[start_md][j]);
    }
    ticket.price=total_price;
    ticket.dest_time=now_time;
    if(max_seat>=num)
    {
        ticket.status=1;
        for(int j=s;j<train.stationNum;j++)
        {
            if(strcmp(train.stations[j],dest)==0)
                break;
            train.seats[start_md][j]+=num;
        }
        train_info.update_T(train,res[0]);
        const auto pos=ticket_info.write_T(ticket);
        ticket_tree.Insert(my_c_str<21>(username),pos);
        return ticket.price*ticket.num;
    }
    if(!queue)
        return -1;
    ticket.status=0;
    train_info.update_T(train,res[0]);
    const auto pos=ticket_info.write_T(ticket);
    ticket_tree.Insert(my_c_str<21>(username),pos);
    const auto list_pos=wait_tree.Find(my_c_str<24>(std::string(trainID)+std::to_string(ticket.timeID)))[0];
    const auto list=wait_list.read_T<WaitList>(list_pos);
    auto nxt_list=wait_list.read_T<WaitList>(list.final);
    auto pre_list=wait_list.read_T<WaitList>(nxt_list.pre);
    const auto now_list=WaitList(nxt_list.pre,pos,list.final,list.final);
    pre_list.nxt=nxt_list.pre=wait_list.write_T(now_list);
    wait_list.update_T(pre_list,now_list.pre);
    wait_list.update_T(nxt_list,now_list.nxt);
    return 0;
}

inline std::pair<int,std::string*> query_order(const char* username)
{
    if(login_state.Find(my_c_str<21>(username)).empty())
        return {-1,nullptr};
    auto res=ticket_tree.Find_ordered(my_c_str<21>(username));
    const int cnt=res.cnt;
    auto* ans=new std::string[cnt];
    for(int i=0;i<cnt;i++)
        ans[cnt-i-1]=ticket_info.read_T<Ticket>(res[i]).to_string();
    return std::make_pair(cnt,ans);
}

inline bool refund_ticket(const char* username,const int n)
{
    if(login_state.Find(my_c_str<21>(username)).empty())
        return false;
    auto pos=ticket_tree.Find_kth(my_c_str<21>(username),n);
    if(!pos)
        return false;
    auto ticket=ticket_info.read_T<Ticket>(pos);
    if(ticket.status==-1)
        return false;
    if(ticket.status==0)
    {
        ticket.status=-1;
        ticket_info.update_T(ticket,pos);
        return false;
    }
    ticket.status=-1;
    ticket_info.update_T(ticket,pos);
    auto las=wait_list.read_T<WaitList>(
    wait_tree.Find(my_c_str<24>(std::string(ticket.trainID)+std::to_string(ticket.timeID)))[0]);
    auto now=wait_list.read_T<WaitList>(las.nxt);
    pos=train_tree.Find(my_c_str<21>(ticket.trainID))[0];
    auto train=train_info.read_T<Train>(pos);
    int ss=0;
    for(int j=0;j<train.stationNum;j++)
        if(strcmp(train.stations[j],ticket.depart)==0)
        {
            ss=j;
            break;
        }
    for(int j=ss;j<train.stationNum;j++)
    {
        if(strcmp(train.stations[j],ticket.dest)==0)
            break;
        train.seats[ticket.timeID][j]-=ticket.num;
    }
    while(now.pos!=-1)
    {
        ticket=ticket_info.read_T<Ticket>(now.pos);
        if(ticket.status)
        {
            las.nxt=now.nxt;
            const int pos_pre=now.pre;
            wait_list.update_T(las,pos_pre);
            now=wait_list.read_T<WaitList>(now.nxt);
            now.pre=pos_pre;
            wait_list.update_T<WaitList>(now,las.nxt);
        }
        else
        {
            int s=0;
            for(int j=0;j<train.stationNum;j++)
                if(strcmp(train.stations[j],ticket.depart)==0)
                {
                    s=j;
                    break;
                }
            int max_seat=train.seatNum;
            for(int j=s;j<train.stationNum;j++)
            {
                if(strcmp(train.stations[j],ticket.dest)==0)
                    break;
                max_seat=std::min(max_seat,train.seatNum-train.seats[ticket.timeID][j]);
            }
            if(max_seat>=ticket.num)
            {
                ticket.status=1;
                for(int j=s;j<train.stationNum;j++)
                {
                    if(strcmp(train.stations[j],ticket.dest)==0)
                        break;
                   train.seats[ticket.timeID][j]+=ticket.num;
                }
                ticket_info.update_T(ticket,now.pos);
            }
            las=now,now=wait_list.read_T<WaitList>(las.nxt);
        }
    }
    train_info.update_T(train,pos);
    return true;
}

#endif