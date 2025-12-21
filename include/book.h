#pragma once

#include "blocklist.h"
#include "account.h"
#include "log.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <utility>
#include <tuple>

using String = std::array<char,61>;
using Tbook = std::pair<String, std::tuple<String, String, String, String, long long, long long > >;

std::vector<String> split(std::array<char, 61>);
std::string turnback(String arr);
struct BookSystem{
    BlockList<Tbook> isbn_, bookname_, author_, keyword_;
    BookSystem(){
        isbn_.initialise("isbn.data");
        bookname_.initialise("bookname.data");
        author_.initialise("author.data");
        keyword_.initialise("keyword.data");
        return;
    }
    ~BookSystem() = default;
    void change(Tbook, Tbook);
    void insert(Tbook);
    bool show(String, String, String, String, AccountSystem &, LogSystem &);
    bool buy(String, long long, AccountSystem &, LogSystem &);
    bool select(String, AccountSystem &, LogSystem &);
    bool modify(String, String, String, String, long long, AccountSystem &, LogSystem &);
    bool import(long long, long long, AccountSystem &, LogSystem &);
};