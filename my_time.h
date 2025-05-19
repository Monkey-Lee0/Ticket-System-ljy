#ifndef MY_TIME_H
#define MY_TIME_H

struct Time
{
    int mon=0,day=0,hour=0,min=0;
    Time()=default;
    Time(const int Mon,const int Day,const int Hour,const int Min):mon(Mon),day(Day),hour(Hour),min(Min){}
    explicit Time(const std::string& str,const bool md=true,const bool hm=true)
    {
        if(md&&hm)
        {
            mon=std::stoi(str.substr(0,2));
            day=std::stoi(str.substr(3,2));
            hour=std::stoi(str.substr(6,2));
            min=std::stoi(str.substr(9,2));
        }
        else if(!md)
        {
            hour=std::stoi(str.substr(0,2));
            min=std::stoi(str.substr(3,2));
        }
        else
        {
            mon=std::stoi(str.substr(0,2));
            day=std::stoi(str.substr(3,2));
        }
    }
    static std::string to2(const int x)
    {
        if(x<=9)
            return "0"+std::to_string(x);
        return std::to_string(x);
    }
    void simplify()
    {
        hour+=min/60;
        min%=60;
        day+=hour/24;
        hour%=24;
        if(mon==6&&day>30)
            mon++,day-=30;
        while(mon>=7&&day>31)
            mon++,day-=31;
    }
    Time operator+(const int Min)const
    {
        Time tmp={mon,day,hour,min+Min};
        tmp.simplify();
        return tmp;
    }
    [[nodiscard]] std::string to_string(const bool md=true) const
    {
        if(md)
            return to2(mon)+"-"+to2(day)+" "+to2(hour)+":"+to2(min);
        return to2(hour)+":"+to2(min);
    }
    [[nodiscard]] int to_int_md() const
    {
        if(mon<6||mon>9)
            return -1;
        if(mon==6)
            return day-1;
        if(mon==7)
            return day+29;
        if(mon==8)
            return day+60;
        return day+91;
    }
    [[nodiscard]] int to_int() const
    {
        return hour*60+min+this->to_int_md()*1440;
    }
    bool operator==(const Time &other)const{return mon==other.mon&&day==other.day&&hour==other.hour&&min==other.min;}
    std::strong_ordering operator<=>(const Time &other)
    {
        if(mon!=other.mon)
        {
            if(mon<other.mon)
                return std::strong_ordering::less;
            return std::strong_ordering::greater;
        }
        if(day!=other.day)
        {
            if(day<other.day)
                return std::strong_ordering::less;
            return std::strong_ordering::greater;
        }
        if(hour!=other.hour)
        {
            if(hour<other.hour)
                return std::strong_ordering::less;
            return std::strong_ordering::greater;
        }
        if(min!=other.min)
        {
            if(min<other.min)
                return std::strong_ordering::less;
            return std::strong_ordering::greater;
        }
        return std::strong_ordering::equal;
    }
};

#endif
