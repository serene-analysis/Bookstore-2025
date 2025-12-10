#include "book.h"
#include "checker.h"
/*
using Tbook = std::pair<String, std::tuple<String, String, String, String, double, long long > >;
*/

std::string turnback(String arr){
    std::string ret;
    for(int i=0;i<=60;i++){
        if(arr[i]){
            ret += arr[i];
        }
    }
    return ret;
}

bool BookSystem::show(String isbn, String bookname, String author, String keyword, AccountSystem &account){
    if(account.currentPrivilege() < 1)return false;
    Tbook given = Tbook();
    std::vector<Tbook> got;
    if(isbn != String()){
        given.first = isbn;
        got = isbn_.allSimilar(given);
    }
    else if(bookname != String()){
        given.first = bookname;
        got = bookname_.allSimilar(given);
    }
    else if(author != String()){
        given.first = author;
        got = author_.allSimilar(given);
    }
    else if(author != String()){
        given.first = author;
        got = author_.allSimilar(given);
    }
    else{
        got = isbn_.all();
    }
    for(Tbook now : got){
        double price;
        long long quantity;// A problem here
        std::tie(isbn, bookname, author, keyword, price, quantity) = now.second;
        std::cout << turnback(isbn) << '\t' << turnback(bookname) << '\t' << turnback(author) << '\t' <<
            turnback(keyword) << '\t' << price << '\t' << quantity << std::endl;
    }
    if(got.empty()){
        std::cout << std::endl;
    }
    return true;
}

std::vector<String> split(std::array<char, 61> keyword){
    std::string got, str = turnback(keyword);
    std::vector<String> mem;
    for(char now : str){
        if(now == '|'){
            mem.push_back(turn(got)), got.clear();
        }
        else{
            got += now;
        }
    }
    return mem;
}

void BookSystem::change(Tbook now, Tbook arr){
    String isbn, bookname, author, keyword;
    double price;
    long long quantity;
    std::tie(isbn, bookname, author, keyword, price, quantity) = now.second;
    Tbook misbn = std::make_pair(isbn, now.second),
        mbookname = std::make_pair(bookname, now.second),
        mauthor = std::make_pair(author, now.second);
    std::vector<String> fkeyword = split(keyword);
    std::tie(isbn, bookname, author, keyword, price, quantity) = arr.second;
    Tbook gisbn = std::make_pair(isbn, arr.second),
        gbookname = std::make_pair(bookname, arr.second),
        gauthor = std::make_pair(author, arr.second);
    std::vector<String> skeyword = split(keyword);
    isbn_.modify(misbn, gisbn);
    bookname_.modify(mbookname, gbookname);
    author_.modify(mauthor, gauthor);
    for(auto fir : fkeyword){
        Tbook del = std::make_pair(fir, now.second);
        keyword_.remove(del);
    }
    for(auto fir : skeyword){
        Tbook ins = std::make_pair(fir, arr.second);
        keyword_.insert(ins);
    }
    return;
}

void BookSystem::insert(Tbook arr){
    String isbn, bookname, author, keyword;
    double price;
    long long quantity;
    std::vector<String> fkeyword = split(keyword);
    std::tie(isbn, bookname, author, keyword, price, quantity) = arr.second;
    Tbook gisbn = std::make_pair(isbn, arr.second),
        gbookname = std::make_pair(bookname, arr.second),
        gauthor = std::make_pair(author, arr.second);
    std::vector<String> skeyword = split(keyword);
    isbn_.insert(gisbn);
    bookname_.insert(gbookname);
    author_.insert(gauthor);
    for(auto fir : skeyword){
        Tbook ins = std::make_pair(fir, arr.second);
        keyword_.insert(ins);
    }
    return;
}

bool BookSystem::buy(String isbn, long long quantity, AccountSystem &account, LogSystem &log){
    if(account.currentPrivilege() < 3)return false;
    if(quantity <= 0)return false;
    Tbook given = Tbook();
    given.first = isbn;
    Tbook arr = isbn_.findSimilar(given), mem = arr;
    if(arr.first != given.first)return false;
    if(std::get<5>(arr.second) < quantity)return false;
    double cost = std::get<4>(arr.second) * quantity;
    std::get<5>(arr.second) -= quantity;
    log.move(cost, true);
    change(mem, arr);
    return account.changeBook(mem, arr);
}

bool BookSystem::select(String isbn, AccountSystem &account){
    if(account.currentPrivilege() < 3)return false;
    Tbook given = Tbook();
    given.first = isbn;
    Tbook arr = isbn_.findSimilar(given), got = arr;
    if(arr.first != isbn){
        insert(given);
        got = given;
    }
    return account.select(got);
}

bool BookSystem::modify(String isbn, String bookname, String author,
    String keyword, double price, AccountSystem &account){
        if(account.currentPrivilege() < 3)return false;
        Tbook now = account.currentBook(), arr = now;
        if(now == Tbook())return false;
        if(isbn != String()){
            std::get<0>(arr.second) = isbn;
            if(isbn_.findSimilar(arr) != Tbook())return false;
        }
        if(isbn != String()){
            std::get<0>(arr.second) = isbn;
        }
        if(bookname != String()){
            std::get<1>(arr.second) = bookname;
        }
        if(author != String()){
            std::get<2>(arr.second) = author;
        }
        if(keyword != String()){
            std::get<3>(arr.second) = keyword;
        }
        if(price != -1){
            std::get<4>(arr.second) = price;
        }
        change(now, arr);
        return account.changeBook(now, arr);
    }

bool BookSystem::import(long long quantity, double totalcost, AccountSystem &account, LogSystem &log){
    if(account.currentPrivilege() < 3)return false;
    if(quantity <= 0 || totalcost <= 0)return false;
    Tbook now = account.currentBook(), arr = now;
    if(now == Tbook())return false;
    std::get<5>(arr.second) += quantity;
    log.move(totalcost, false);
    change(now, arr);
    return account.changeBook(now, arr);
}