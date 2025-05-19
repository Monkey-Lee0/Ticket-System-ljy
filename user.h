#ifndef USER_H
#define USER_H

#include"database.h"
#include"tools.h"

struct User
{
    char userName[21]{};
    char password[31]{};
    char name[16]{};
    char mailAddr[31]{};
    int privilege=-1;
    User(const char* userName,const char* password,const char* name,const char* mailAddr,const int privilege)
    {
        this->privilege=privilege;
        strcpy(this->userName,userName);
        strcpy(this->password,password);
        strcpy(this->name,name);
        strcpy(this->mailAddr,mailAddr);
    }
    User()=default;
};

inline B_Plus_Tree<char[21],bool,true> login_state("data/LoginState");

inline B_Plus_Tree<char[21],int> user_tree("data/UserTree");
inline DataInteractor user_info("data/UserInfo");

inline User query_profile(const char* curUserName,const char* userName,const bool check=true)
{
    if(check&&login_state.Find(my_c_str<21>(curUserName)).empty())
        return {};
    auto res=user_tree.Find(my_c_str<21>(userName));
    auto res_cur=user_tree.Find(my_c_str<21>(curUserName));
    if(res.empty()||res_cur.empty())
        return {};
    const auto profile=user_info.read_T<User>(res[0]);
    if(strcmp(curUserName,userName)!=0)
        if(const auto profile0=user_info.read_T<User>(res_cur[0]);
            profile.privilege>=profile0.privilege)
            return {};
    return profile;
}

inline bool add_user(const char* curUserName,const char* userName,const char* password,
                     const char* name,const char* mailAddr,int privilege)
{
    if(user_tree.empty())
        privilege=10;
    else if(query_profile(curUserName,curUserName).privilege<=privilege)
        return false;
    if(!strlen(userName)||!strlen(password)||!strlen(name)||!strlen(mailAddr)||privilege==-1)
        return false;
    if(!user_tree.Find(my_c_str<21>(userName)).empty())
        return false;
    const User new_user(userName,password,name,mailAddr,privilege);
    user_tree.Insert(my_c_str<21>(userName),user_info.write_T(new_user));
    return true;
}

inline bool login(const char* userName,const char* password)
{
    if(!login_state.Find(my_c_str<21>(userName)).empty())
        return false;
    const auto profile=query_profile(userName,userName,false);
    if(profile.privilege==-1||strcmp(profile.password,password)!=0)
        return false;
    login_state.Insert(my_c_str<21>(userName),true);
    return true;
}

inline bool logout(const char* userName)
{
    if(login_state.Find(my_c_str<21>(userName)).empty())
        return false;
    login_state.Delete(my_c_str<21>(userName),true);
    return true;
}

inline User modify_profile(const char* curUserName,const char* userName,const char* password,const char* name,
    const char* mailAddr,int privilege)
{
    if(login_state.Find(my_c_str<21>(curUserName)).empty())
        return {};
    auto res=user_tree.Find(my_c_str<21>(userName));
    auto res_cur=user_tree.Find(my_c_str<21>(curUserName));
    if(res.empty()||res_cur.empty())
        return {};
    auto profile=user_info.read_T<User>(res[0]);
    if(strcmp(curUserName,userName)!=0)
    {
        if(const auto profile0=user_info.read_T<User>(res_cur[0]);
            profile.privilege>=profile0.privilege||privilege>=profile0.privilege)
            return {};
    }
    else if(privilege>=profile.privilege)
            return {};
    if(strlen(password))
        strcpy(profile.password,password);
    if(strlen(name))
        strcpy(profile.name,name);
    if(strlen(mailAddr))
        strcpy(profile.mailAddr,mailAddr);
    if(privilege!=-1)
        profile.privilege=privilege;
    user_info.update_T(profile,res[0]);
    return profile;
}

#endif