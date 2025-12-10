#include "account.h"
#include "checker.h"
#include <cassert>
/*
using Tselected = std::pair<String, std::tuple<String, String, String, String, double, long long > >;
using Taccount = std::pair<String,std::tuple<String, int, String, Tselected > >;

Tselected = pair<ISBN, tuple<ISBN,Name,Author,Keyword,Price,Quantity>>
Taccount = pair<UserId, tuple<Password,Privilege,Username,TSelected>>
Tstackaccount = pair<Id, Taccount>
Tmap = pair<UserId, Id>
*/

Taccount AccountSystem::currentAccount(){
    if(stack_.empty()){
        return std::make_pair(turn("serene_analysis"), std::make_tuple(turn("070304gao"), 0, turn("serene_analysis"), Tselected()) );
    }
    return stack_.findLast().second;
}

int AccountSystem::currentPrivilege(){
    Taccount now = currentAccount();
    return std::get<1>(now.second);
}

Tselected AccountSystem::currentBook(){
    Taccount now = currentAccount();
    return std::get<3>(now.second);
}

bool AccountSystem::exit(){
    while(!stack_.empty() || !map_.empty()){
        assert(!stack_.empty() && !map_.empty());
        stack_.pop_back();
    }
    return true;
}

bool AccountSystem::su(String userid, String password){
    Taccount given = Taccount();
    given.first = userid;
    Taccount now = currentAccount(), want = account_.findSimilar(given);
    if(want.first != userid)return false;
    if(password != String()){
        if(std::get<0>(want.second) != password)return false;
        Tstackaccount ins = std::make_pair(stack_.number() + 1, want);
        Tmap nmap = std::make_pair(userid, stack_.number() + 1);
        stack_.insert(ins), map_.insert(nmap);
        return true;
    }
    if(std::get<1>(want.second) >= std::get<1>(now.second))return false;
    Tstackaccount ins = std::make_pair(stack_.number() + 1, want);
    stack_.insert(ins);
    return true;
}


bool AccountSystem::logout(){
    Taccount now = currentAccount();
    if(std::get<1>(now.second) < 1)return false;
    if(now.first == turn("serene_analysis"))return false;
    Tstackaccount slast = stack_.findLast();
    Tmap nmap = std::make_pair(std::get<0>(slast.second), 0),
        found = map_.findSimilar(nmap);
    map_.remove(found);
    stack_.pop_back();
    return true;
}

bool AccountSystem::signup(String userid, String password, String username, bool forced){
    if(forced){// root
        Taccount acc = std::make_pair(userid, std::make_tuple(password, 7, username, Tselected()));
        account_.insert(acc);
        return true;
    }
    Taccount given = Taccount();
    given.first = userid;
    Taccount now = currentAccount(), want = account_.findSimilar(given);
    if(want.first == userid)return false;
    Taccount ins = std::make_pair(userid, std::make_tuple(password, 1, username, Tselected()));
    account_.insert(ins);
    return true;
}

bool AccountSystem::passwd(String userid, String currentPassword, String newPassword){
    Taccount given = Taccount();
    given.first = userid;
    Taccount now = currentAccount(), want = account_.findSimilar(given), arr = want;
    if(std::get<1>(now.second) < 1)return false;
    if(want.first != userid)return false;
    if(currentPassword != String()){
        if(std::get<0>(want.second) != currentPassword)return false;
        std::get<0>(arr.second) = newPassword;
        account_.modify(want, arr);
        return true;
    }
    if(std::get<1>(now.second) != 7)return false;
    std::get<0>(arr.second) = newPassword;
    account_.modify(want, arr);
    return true;
}

bool AccountSystem::useradd(String userid, String password, int privilege, String username){
    Taccount given = Taccount();
    given.first = userid;
    Taccount now = currentAccount(), want = account_.findSimilar(given), arr = given;
    std::get<0>(arr.second) = password;
    std::get<1>(arr.second) = privilege;
    std::get<2>(arr.second) = username;
    if(std::get<1>(now.second) < 3)return false;
    if(want.first == userid)return false;
    if(privilege >= std::get<1>(now.second))return false;
    account_.insert(given);
    return true;
}

bool AccountSystem::remove(String userid){
    Taccount given = Taccount();
    given.first = userid;
    Taccount now = currentAccount(), want = account_.findSimilar(given);
    if(std::get<1>(now.second) < 7)return false;
    if(want.first != userid)return false;
    Tmap nmap = std::make_pair(userid, 0);
    if(map_.findSimilar(nmap) != Tmap())return false;
    account_.remove(want);
    return true;
}

bool AccountSystem::select(Tselected book){
    Tstackaccount now = stack_.findLast(), arr = now;
    std::get<3>(arr.second.second) = book;
    stack_.modify(now, arr);
    return true;
}

bool AccountSystem::changeBook(Tselected from, Tselected arr){
    std::vector<Tstackaccount> all = stack_.all();
    for(Tstackaccount now : all){
        if(std::get<3>(now.second.second) == from){
            Tstackaccount to = now;
            std::get<3>(to.second.second) = arr;
            stack_.modify(now, to);
        }
    }
    return true;
}