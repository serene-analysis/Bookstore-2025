#include "account.h"
#include "checker.h"
#include "book.h"
#include <cassert>
#include <array>
#include <utility>
#include <tuple>
/*
using Tselected = std::pair<String, std::tuple<String, String, String, String, double, long long > >;
using Taccount = std::pair<String,std::tuple<String, int, String, Tselected > >;

Tselected = pair<ISBN, tuple<ISBN,Name,Author,Keyword,Price,Quantity>>
Taccount = pair<UserId, tuple<Password,Privilege,Username,TSelected>>
Tstackaccount = pair<Id, Taccount>
Tmap = pair<UserId, Id>
*/

void print(Tselected book){
    std::cout << "sortkeyword = " << turnback(book.first) << std::endl;
    std::cout << "isbn = " << turnback(std::get<0>(book.second)) << std::endl;
    std::cout << "bookname = " << turnback(std::get<1>(book.second)) << std::endl;
    std::cout << "author = " << turnback(std::get<2>(book.second)) << std::endl;
    std::cout << "keyword = " << turnback(std::get<3>(book.second)) << std::endl;
    std::cout << "price = " << std::get<4>(book.second) << std::endl;
    std::cout << "quantity = " << std::get<5>(book.second) << std::endl;
    return;
}

void out(Taccount acc){
    std::cout << "userid = " << turnback(acc.first) << std::endl;
    std::cout << "password = " << turnback(std::get<0>(acc.second)) << std::endl;
    std::cout << "privilege = " << std::get<1>(acc.second) << std::endl;
    std::cout << "username = " << turnback(std::get<2>(acc.second)) << std::endl;
    return;
}

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
        stack_.pop_back(), map_.pop_back();
    }
    return true;
}

bool AccountSystem::su(String userid, String password){
    //std::cout << "su, userid = " << turnback(userid) << ", password = " << turnback(password) << std::endl;
    //    std::cout << "stack.size() = " << stack_.getnumber() << ", map.size() = " << map_.getnumber() << std::endl;
    Taccount given = Taccount();
    given.first = userid;
    //std::cout << "account_.number = " << account_.getnumber() << std::endl;
    Taccount now = currentAccount(), want = account_.findSimilar(given);
    if(want.first != userid){
        //std::cout << "user not found" << std::endl;
        return false;
    }
    if(password != String()){
        if(std::get<0>(want.second) != password)return false;
        //std::cout << "su success" << std::endl;
        Tstackaccount ins = std::make_pair(stack_.getnumber() + 1, want);
        //out(ins.second);
        Tmap nmap = std::make_pair(userid, stack_.getnumber() + 1);
        stack_.insert(ins), map_.insert(nmap);
        return true;
    }
    if(std::get<1>(want.second) >= std::get<1>(now.second))return false;
    Tstackaccount ins = std::make_pair(stack_.getnumber() + 1, want);
    Tmap nmap = std::make_pair(userid, stack_.getnumber() + 1);
    stack_.insert(ins), map_.insert(nmap);
    return true;
}


bool AccountSystem::logout(){
    Taccount now = currentAccount();
        //std::cout << "stack.size() = " << stack_.getnumber() << ", map.size() = " << map_.getnumber() << std::endl;
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
    //std::cout << "signup" << std::endl;
        //std::cout << "stack.size() = " << stack_.getnumber() << ", map.size() = " << map_.getnumber() << std::endl;
    if(forced){// root
        //std::cout << "forced" << std::endl;
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
    //std::cout << "useradd" << std::endl;
    //    std::cout << "stack.size() = " << stack_.getnumber() << ", map.size() = " << map_.getnumber() << std::endl;
    if(privilege != 0 && privilege != 1 && privilege != 3 && privilege != 7)return false;
    Taccount given = Taccount();
    given.first = userid;
    Taccount now = currentAccount(), want = account_.findSimilar(given), arr = given;
    std::get<0>(arr.second) = password;
    std::get<1>(arr.second) = privilege;
    std::get<2>(arr.second) = username;
    //out(now), out(want);
    if(std::get<1>(now.second) < 3)return false;
    if(want.first == userid)return false;
    if(privilege >= std::get<1>(now.second))return false;
    account_.insert(arr);
    return true;
}

bool AccountSystem::remove(String userid){
        //std::cout << "stack.size() = " << stack_.getnumber() << ", map.size() = " << map_.getnumber() << std::endl;
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
    //std::cout << "changeBook:" << std::endl;
    //print(from), print(arr);
    for(Tstackaccount now : all){
        if(std::get<3>(now.second.second) == from){
            Tstackaccount to = now;
            std::get<3>(to.second.second) = arr;
            stack_.modify(now, to);
        }
    }
    return true;
}