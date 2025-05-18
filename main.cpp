#pragma GCC optimize("Ofast")
#include"database.h"
#include"ticket.h"
#include"user.h"
#include"tools.h"
#include"train.h"
std::string str;
int main()
{
    while(true)
    {
        std::getline(std::cin,str);
        std::string r_str;
        while(!str.empty())
            r_str.push_back(str.back()),str.pop_back();
        std::string cmd=decompose(r_str);
        if(cmd=="exit")
        {
            std::cout<<"bye"<<std::endl;
            break;
        }
        if(cmd=="add_user")
        {
            std::string curUserName,userName,password,name,mailAddr;
            int privilege=-1;
            while(!r_str.empty())
            {
                const auto opt=decompose(r_str);
                if(opt=="-c")
                    curUserName=decompose(r_str);
                else if(opt=="-u")
                    userName=decompose(r_str);
                else if(opt=="-p")
                    password=decompose(r_str);
                else if(opt=="-n")
                    name=decompose(r_str);
                else if(opt=="-m")
                    mailAddr=decompose(r_str);
                else if(opt=="-g")
                    privilege=stoi(decompose(r_str));
            }
            const int state=add_user(curUserName.data(),userName.data(),password.data(),
                name.data(),mailAddr.data(),privilege);
            if(!state)
                std::cout<<"-1"<<std::endl;
            else
                std::cout<<"0"<<std::endl;
        }
        else if(cmd=="login")
        {
            std::string userName,password;
            while(!r_str.empty())
            {
                const auto opt=decompose(r_str);
                if(opt=="-u")
                    userName=decompose(r_str);
                else if(opt=="-p")
                    password=decompose(r_str);
            }
            const int state=login(userName.data(),password.data());
            if(!state)
                std::cout<<"-1"<<std::endl;
            else
                std::cout<<"0"<<std::endl;
        }
        else if(cmd=="logout")
        {
            std::string userName;
            while(!r_str.empty())
                if(decompose(r_str)=="-u")
                    userName=decompose(r_str);
            const int state=logout(userName.data());
            if(!state)
                std::cout<<"-1"<<std::endl;
            else
                std::cout<<"0"<<std::endl;
        }
        else if(cmd=="query_profile")
        {
            std::string curUserName,userName;
            while(!r_str.empty())
            {
                const auto opt=decompose(r_str);
                if(opt=="-c")
                    curUserName=decompose(r_str);
                else if(opt=="-u")
                    userName=decompose(r_str);
            }
            const auto state=query_profile(curUserName.data(),userName.data());
            if(state.privilege==-1)
                std::cout<<"-1"<<std::endl;
            else
                std::cout<<state.userName<<" "<<state.name<<" "<<state.mailAddr<<" "<<state.privilege<<std::endl;
        }
        else if(cmd=="modify_profile")
        {
            std::string curUserName,userName,password,name,mailAddr;
            int privilege=-1;
            while(!r_str.empty())
            {
                const auto opt=decompose(r_str);
                if(opt=="-c")
                    curUserName=decompose(r_str);
                else if(opt=="-u")
                    userName=decompose(r_str);
                else if(opt=="-p")
                    password=decompose(r_str);
                else if(opt=="-n")
                    name=decompose(r_str);
                else if(opt=="-m")
                    mailAddr=decompose(r_str);
                else if(opt=="-g")
                    privilege=stoi(decompose(r_str));
            }
            const User state=modify_profile(curUserName.data(),userName.data(),password.data(),
                name.data(),mailAddr.data(),privilege);
            if(state.privilege==-1)
                std::cout<<"-1"<<std::endl;
            else
                std::cout<<state.userName<<" "<<state.name<<" "<<state.mailAddr<<" "<<state.privilege<<std::endl;
        }
        else if(cmd=="add_train")
        {
            std::string trainID,stations_str,prices_str,startTime_str,travelTimes_str,stopoverTimes_str,saleDate_str;
            char type;
            int stationNum=0,seatNum=0;
            while(!r_str.empty())
            {
                const auto opt=decompose(r_str);
                if(opt=="-i")
                    trainID=decompose(r_str);
                else if(opt=="-n")
                    stationNum=std::stoi(decompose(r_str));
                else if(opt=="-m")
                    seatNum=std::stoi(decompose(r_str));
                else if(opt=="-s")
                    stations_str=decompose(r_str);
                else if(opt=="-p")
                    prices_str=decompose(r_str);
                else if(opt=="-x")
                    startTime_str=decompose(r_str);
                else if(opt=="-t")
                    travelTimes_str=decompose(r_str);
                else if(opt=="-o")
                    stopoverTimes_str=decompose(r_str);
                else if(opt=="-d")
                    saleDate_str=decompose(r_str);
                else if(opt=="-y")
                    type=decompose(r_str)[0];
            }
            const int state=add_train(trainID.data(),stationNum,seatNum,stations_str.data(),prices_str.data(),
                startTime_str.data(),travelTimes_str.data(),
                stopoverTimes_str.data(),saleDate_str.data(),type);
            if(!state)
                std::cout<<"-1"<<std::endl;
            else
                std::cout<<"0"<<std::endl;
        }
        else if(cmd=="delete_train")
        {
            std::string trainID;
            while(!r_str.empty())
                if(decompose(r_str)=="-i")
                    trainID=decompose(r_str);
            const int state=delete_train(trainID.data());
            if(!state)
                std::cout<<"-1"<<std::endl;
            else
                std::cout<<"0"<<std::endl;
        }
        else if(cmd=="release_train")
        {
            std::string trainID;
            while(!r_str.empty())
                if(decompose(r_str)=="-i")
                    trainID=decompose(r_str);
            const int state=release_train(trainID.data());
            if(!state)
                std::cout<<"-1"<<std::endl;
            else
                std::cout<<"0"<<std::endl;
        }
        else if(cmd=="query_train")
        {
            std::string trainID,day_str;
            while(!r_str.empty())
            {
                const std::string opt=decompose(r_str);
                if(opt=="-i")
                    trainID=decompose(r_str);
                else if(opt=="-d")
                    day_str=decompose(r_str);
            }
            const Train train=query_train(trainID.data());
            if(!train.seatNum)
                std::cout<<"-1"<<std::endl;
            else
            {
                Time today(day_str,true,false);
                int cnt=today.to_int_md();
                if(today>train.saleDate_r||today<train.saleDate_l)
                    std::cout<<"-1"<<std::endl;
                else
                {
                    std::cout<<train.trainID<<" "<<train.type<<std::endl;
                    Time now_time=train.startTime;
                    now_time.mon=today.mon,now_time.day=today.day;
                    int price=0;
                    std::cout<<train.stations[0]<<" xx-xx xx:xx -> "<<now_time.to_string()<<" 0 "<<train.seatNum-
                        train.seats[cnt][0]<<std::endl;
                    for(int i=1;i<train.stationNum-1;i++)
                    {
                        now_time=now_time+train.travelTimes[i-1];
                        std::cout<<train.stations[i]<<" "<<now_time.to_string()<<" -> ";
                        now_time=now_time+train.stopoverTimes[i-1];
                        price+=train.prices[i-1];
                        std::cout<<now_time.to_string()<<" "<<price<<" "<<train.seatNum-train.seats[cnt][i]<<std::endl;
                    }
                    now_time=now_time+train.travelTimes[train.stationNum-2];
                    price+=train.prices[train.stationNum-2];
                    std::cout<<train.stations[train.stationNum-1]<<" "<<now_time.to_string()<<" -> xx-xx xx:xx "<<
                        price<<" x"<<std::endl;
                }
            }
        }
        else if(cmd=="query_ticket")
        {
            std::string depart,dest,day_str,key="time";
            while(!r_str.empty())
            {
                const std::string opt=decompose(r_str);
                if(opt=="-s")
                    depart=decompose(r_str);
                else if(opt=="-t")
                    dest=decompose(r_str);
                else if(opt=="-d")
                    day_str=decompose(r_str);
                else if(opt=="-p")
                    key=decompose(r_str);
            }
            const auto [fi, se]=query_ticket(depart.data(),dest.data(),
                day_str.data(),key.data());
            std::cout<<fi<<std::endl;
            for(int i=0;i<fi;i++)
                std::cout<<se[i]<<std::endl;
            delete[] se;
        }
        else if(cmd=="query_transfer")
        {
            std::string depart,dest,day_str,key="time";
            while(!r_str.empty())
            {
                const std::string opt=decompose(r_str);
                if(opt=="-s")
                    depart=decompose(r_str);
                else if(opt=="-t")
                    dest=decompose(r_str);
                else if(opt=="-d")
                    day_str=decompose(r_str);
                else if(opt=="-p")
                    key=decompose(r_str);
            }
            const auto [fi,se]=query_transfer(depart.data(),dest.data(),
                day_str.data(),key.data());
            if(fi.empty())
                std::cout<<"0"<<std::endl;
            else
                std::cout<<fi<<std::endl<<se<<std::endl;
        }
        else if(cmd=="buy_ticket")
        {
            std::string username,trainID,day_str,depart,dest;
            int num=0;
            bool queue=false;
            while(!r_str.empty())
            {
                const std::string opt=decompose(r_str);
                if(opt=="-u")
                    username=decompose(r_str);
                else if(opt=="-i")
                    trainID=decompose(r_str);
                else if(opt=="-d")
                    day_str=decompose(r_str);
                else if(opt=="-n")
                    num=std::stoi(decompose(r_str));
                else if(opt=="-f")
                    depart=decompose(r_str);
                else if(opt=="-t")
                    dest=decompose(r_str);
                else if(opt=="-q")
                    queue=decompose(r_str)[0]=='t';
            }
            const auto res=buy_ticket(username.data(),trainID.data(),day_str.data(),num,
                depart.data(),dest.data(),queue);
            if(res)
                std::cout<<res<<std::endl;
            else
                std::cout<<"queue"<<std::endl;
        }
        else if(cmd=="query_order")
        {
            std::string username;
            while(!r_str.empty())
                if(decompose(r_str)=="-u")
                    username=decompose(r_str);
            const auto [fi,se]=query_order(username.data());
            std::cout<<fi<<std::endl;
            for(int i=0;i<fi;i++)
                std::cout<<se[i]<<std::endl;
            delete[] se;
        }
        else if(cmd=="refund_ticket")
        {
            std::string username;
            int n=1;
            while(!r_str.empty())
            {
                const std::string opt=decompose(r_str);
                if(opt=="-u")
                    username=decompose(r_str);
                else if(opt=="-n")
                    n=std::stoi(decompose(r_str));
            }
            const auto res=refund_ticket(username.data(),n);
            if(res)
                std::cout<<0<<std::endl;
            else
                std::cout<<-1<<std::endl;
        }
        else if(cmd=="clean")
        {
            auto clean=[](std::string file)
            {
                std::ofstream f;
                f.open(file);
                f.close();
            };
            login_state.reload();
            user_tree.reload();
            user_info.reload();
            ticket_tree.reload();
            ticket_info.reload();
            train_tree.reload();
            from_to_tree.reload();
            wait_tree.reload();
            train_info.reload();
            wait_list.reload();
        }
    }

    return 0;
}