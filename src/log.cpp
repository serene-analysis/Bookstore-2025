#include "log.h"
#include "account.h"
#include "book.h"
#include <array>
#include <utility>
#include <tuple>
/*
using Tfinanceinfo = std::tuple<String, String, String, long long, long long>;
using Tlogfinance = std::pair<int, Tfinanceinfo>;
using Tuserinfo = std::tuple<String, String, int, String, String, String, int, String>;
using Tloguser = std::pair<int, Tuserinfo>;
using Tbookinfo = std::tuple<String, String, String, String, String, String, long long>;
using Tlogbook = std::pair<int, Tbookinfo>;
*/

std::string getStrings(long long v){
    std::string ret;
    if(v < 100){
        ret = "0.";
        ret += (char)(v / 10 + '0');
        ret += (char)(v % 10 + '0');
    }
    else{
        long long x = v / 100;
        while(x){
            ret += (char)(x % 10 + '0');
            x /= 10;
        }
        v %= 100;
        std::reverse(ret.begin(), ret.end());
        ret += '.';
        ret += (char)(v / 10 + '0');
        ret += (char)(v % 10 + '0');
    }
    return ret;
}

void LogSystem::move(long long v, bool incomed){
    int id = 1;
    long long income = 0.0, expense = 0.0;
    if(finance_.getnumber() != 0){
        Tfinance got = finance_.findLast();
        id = got.first + 1, income = got.second.first, expense = got.second.second;
    }
    if(incomed){
        income += v;
    }
    else{
        expense += v;
    }
    Tfinance to = std::make_pair(id, std::make_pair(income, expense));
    finance_.insert(to);
    return;
}

bool LogSystem::show(long long count, AccountSystem &account){
    if(account.currentPrivilege() < 7)return false;
    if(count == 0){
        std::cout << std::endl;
        return true;
    }
    Tfinance all = finance_.getnumber() == 0 ? Tfinance() : finance_.findLast(), want = Tfinance();
    want.first = all.first - count;
    if(want.first < 0)return false;
    if(count == -1 || want.first == 0){
        std::cout << "+ " << getStrings(all.second.first) << " - " << getStrings(all.second.second) << std::endl;
        return true;
    }
    Tfinance got = finance_.findSimilar(want);
    std::cout << "+ " << getStrings(all.second.first - got.second.first) << " - " << getStrings(all.second.second - got.second.second) << std::endl;
    return true;
}

void LogSystem::log_move_finance(Tfinanceinfo info){
    Tlogfinance now = std::make_pair(logfinance_.getnumber() + 1, info);
    logfinance_.insert(now);
    return;
}

void LogSystem::log_move_book(Tbookinfo info){
    Tlogbook now = std::make_pair(logbook_.getnumber() + 1, info);
    logbook_.insert(now);
    return;
}

void LogSystem::log_move_user(Tuserinfo info){
    Tloguser now = std::make_pair(loguser_.getnumber() + 1, info);
    loguser_.insert(now);
    return;
}

bool LogSystem::report_finance(AccountSystem &account){
    if(account.currentPrivilege() < 7)return false;
    std::vector<Tlogfinance> all = logfinance_.all();
    for(Tlogfinance now : all){
        String userid, op, isbn;
        long long quantity, totalcost;
        std::tie(userid, op, isbn, quantity, totalcost) = now.second;
        std::cout << "Userid = " << turnback(userid) << '\t' << "op = " << turnback(op) << '\t' << "isbn = " << turnback(isbn) << '\t' << "quantity*100 = " << quantity << '\t';
        if(totalcost != -1)std::cout << "totalcost*100 = " << totalcost << '\t';
        std::cout << std::endl;
    }
    return true;
}

void LogSystem::report_user(AccountSystem &account, int minimum_privilege = 3){
    std::vector<Tloguser> all = loguser_.all();
    for(Tloguser now : all){
        String userid, op, givenuserid, password, newpassword, username;
        int userprivilege, givenprivilege;
        std::tie(userid, op, userprivilege, givenuserid, password, newpassword, givenprivilege, username) = now.second;
        if(userprivilege < minimum_privilege)continue;
        std::cout << "Userid = " << turnback(userid) << '\t';
        std::cout << "op = " << turnback(op) << '\t';
        std::cout << "current privilege = " << userprivilege << '\t';
        if(password != String())std::cout << "given password = " << turnback(password) << '\t';
        if(newpassword != String())std::cout << "new password = " << turnback(newpassword) << '\t';
        if(givenprivilege != -1)std::cout << "given privilege = " << userprivilege << '\t';
        if(username != String())std::cout << "given username = " << turnback(username) << '\t';
        std::cout << std::endl;
    }
}

void LogSystem::report_book(AccountSystem &account, int minimum_privilege = 3){
    std::vector<Tlogbook> all = logbook_.all();
    for(Tlogbook now : all){
        String userid, op, isbn, bookname, author, keyword;
        int userprivilege;
        long long price;
        std::tie(userid, op, userprivilege, isbn, bookname, author, keyword, price) = now.second;
        if(userprivilege < minimum_privilege)continue;
        std::cout << "Userid = " << turnback(userid) << '\t';
        std::cout << "op = " << turnback(op) << '\t';
        std::cout << "current privilege = " << userprivilege << '\t';
        if(isbn != String())std::cout << "isbn = " << turnback(isbn) << '\t';
        if(bookname != String())std::cout << "bookname = " << turnback(bookname) << '\t';
        if(author != String())std::cout << "author = " << turnback(author) << '\t';
        if(keyword != String())std::cout << "keyword = " << turnback(keyword) << '\t';
        if(price != -1)std::cout << "price*100 = " << price << '\t';
        std::cout << std::endl;
    }
}

bool LogSystem::report_employee(AccountSystem &account, int minimum_privilege = 3){
    if(account.currentPrivilege() < 7)return false;
    report_user(account, minimum_privilege);
    report_book(account, minimum_privilege);
    return true;
}

bool LogSystem::log(AccountSystem &account){
    if(account.currentPrivilege() < 7)return false;
    report_finance(account);
    report_employee(account, 0);
    return true;
}