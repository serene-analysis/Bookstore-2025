#include "book.h"
#include "checker.h"
#include <array>
#include <utility>
#include <tuple>
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

void out(Tbook book){
    std::cout << "sortkeyword = " << turnback(book.first) << std::endl;
    std::cout << "isbn = " << turnback(std::get<0>(book.second)) << std::endl;
    std::cout << "bookname = " << turnback(std::get<1>(book.second)) << std::endl;
    std::cout << "author = " << turnback(std::get<2>(book.second)) << std::endl;
    std::cout << "keyword = " << turnback(std::get<3>(book.second)) << std::endl;
    std::cout << "price = " << std::get<4>(book.second) << std::endl;
    std::cout << "quantity = " << std::get<5>(book.second) << std::endl;
    return;
}

bool BookSystem::show(String isbn, String bookname, String author, String keyword, AccountSystem &account){
    //std::cout << "show book" << std::endl;
    Tbook given = Tbook();
    std::vector<Tbook> got;
    if(account.currentPrivilege() < 1){
        //std::cout << "currentPrivilege() = " << account.currentPrivilege() << std::endl;
        return false;
    }
    if(isbn != String()){
        //std::cout << "isbn not empty" << std::endl;
        given.first = isbn;
        //std::cout << turnback(isbn) << std::endl;
        got = isbn_.allSimilar(given);
    }
    else if(bookname != String()){
        //std::cout << "bookname not empty" << std::endl;
        given.first = bookname;
        got = bookname_.allSimilar(given);
    }
    else if(author != String()){
        //std::cout << "author not empty" << std::endl;
        given.first = author;
        got = author_.allSimilar(given);
    }
    else if(keyword != String()){
        //std::cout << "keyword not empty" << std::endl;
        given.first = keyword;
        //std::cout << "given:" << std::endl;
        //out(given);
        got = keyword_.allSimilar(given);
    }
    else{
        got = isbn_.all();
    }

    //out(given);

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
    //std::cout << "book.modify:"  << std::endl;
    //out(now), out(arr);
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
    //std::cout << "book.insert:"  << std::endl;
    //out(arr);
    String isbn, bookname, author, keyword;
    double price;
    long long quantity;
    std::vector<String> fkeyword = split(keyword);
    std::tie(isbn, bookname, author, keyword, price, quantity) = arr.second;
    Tbook gisbn = std::make_pair(isbn, arr.second),
        gbookname = std::make_pair(bookname, arr.second),
        gauthor = std::make_pair(author, arr.second);
    isbn_.insert(gisbn);
    bookname_.insert(gbookname);
    author_.insert(gauthor);
    for(auto fir : fkeyword){
        Tbook ins = std::make_pair(fir, arr.second);
        keyword_.insert(ins);
    }
    return;
}

bool BookSystem::buy(String isbn, long long quantity, AccountSystem &account, LogSystem &log){
    //std::cout << "privilege = " << account.currentPrivilege() << std::endl;
    if(account.currentPrivilege() < 1)return false;
    if(quantity <= 0)return false;
    Tbook given = Tbook();
    given.first = isbn;
    Tbook arr = isbn_.findSimilar(given), mem = arr;
    //std::cout << "price = " << std::get<4>(arr.second) << ", quantity = " << std::get<5>(arr.second) << std::endl;
    if(arr.first != given.first)return false;
    if(std::get<5>(arr.second) < quantity)return false;
    double cost = std::get<4>(arr.second) * quantity;
    std::cout << cost << std::endl;
    std::get<5>(arr.second) -= quantity;
    log.move(cost, true);
    change(mem, arr);
    return account.changeBook(mem, arr);
}

bool BookSystem::select(String isbn, AccountSystem &account){
    //std::cout << "select, book.number = " << isbn_.getnumber() << std::endl;
    if(account.currentPrivilege() < 3)return false;
    Tbook given = Tbook();
    given.first = isbn;
    std::get<0>(given.second) = isbn;
    Tbook arr = isbn_.findSimilar(given), got = arr;
    if(arr.first != isbn){
        insert(given);
        got = given;
    }
    return account.select(got);
}

bool BookSystem::modify(String isbn, String bookname, String author,
    String keyword, double price, AccountSystem &account){
        //std::cout << "modify, book.number = " << isbn_.getnumber() << std::endl;
        //std::cout << "privilege = " << account.currentPrivilege() << std::endl;
        if(account.currentPrivilege() < 3)return false;
        Tbook now = account.currentBook(), arr = now;
        if(now == Tbook()){
            //std::cout << "not selected" << std::endl;
            return false;
        }
        if(isbn != String()){
            std::get<0>(arr.second) = isbn;
            arr.first = isbn;
            if(isbn_.findSimilar(arr) != Tbook())return false;
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