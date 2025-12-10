#include "account.h"
#include "checker.h"
/*
using Tselected = std::pair<std::array<char,61>, std::tuple<std::array<char,61>, std::array<char,61>, std::array<char,61>, std::array<char,61>, double, long long > >;
using Taccount = std::pair<std::array<char,61>,std::tuple<std::array<char,61>, int, std::array<char,61>, Tselected > >;

Tselected = pair<ISBN, tuple<ISBN,Name,Author,Keyword,Price,Quantity>>
Taccount = pair<UserId, tuple<Password,Privilege,Username,TSelected>>
Tstackaccount = pair<Id, Taccount>
*/

Taccount AccountSystem::currentAccount(){
    if(stack_.empty()){
        return std::make_pair(turn("serene_analysis"), std::make_tuple(turn("070304gao"), 0, turn("serene_analysis"), Tselected()) );
    }
    return stack_.findLast().second;
}

bool AccountSystem::su(std::array<char,61> userid, std::array<char,61> password){
    Taccount given = Taccount();
    given.first = userid;
    Taccount now = currentAccount(), want = account_.findSimilar(given);
    if(want.first != userid)return false;
    if(password != std::array<char,61>()){
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

bool AccountSystem::signup(std::array<char,61> userid, std::array<char,61> password, std::array<char,61> username, bool forced){
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

bool AccountSystem::passwd(std::array<char,61> userid, std::array<char,61> currentPassword, std::array<char,61> newPassword){
    Taccount given = Taccount();
    given.first = userid;
    Taccount now = currentAccount(), want = account_.findSimilar(given), arr = want;
    if(std::get<1>(now.second) < 1)return false;
    if(want.first != userid)return false;
    if(currentPassword != std::array<char,61>()){
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

bool AccountSystem::useradd(std::array<char,61> userid, std::array<char,61> password, int privilege, std::array<char,61> username){
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

bool AccountSystem::remove(std::array<char,61> userid){
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