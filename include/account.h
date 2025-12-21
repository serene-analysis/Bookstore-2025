#pragma once
#include "blocklist.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <utility>
#include <tuple>
using String = std::array<char,61>;
using Tselected = std::pair<String, std::tuple<String, String, String, String, long long, long long > >;
using Taccount = std::pair<String,std::tuple<String, int, String, Tselected > >;
using Tstackaccount = std::pair<int, Taccount>;
using Tmap = std::pair<String, int>;

struct LogSystem;

struct AccountSystem{
    BlockList<Taccount> account_;
    BlockList<Tstackaccount> stack_;
    BlockList<Tmap> map_;
    AccountSystem(){
        account_.initialise("account.data");
        stack_.initialise("stack.data");
        map_.initialise("map.data");
        return;
    }
    ~AccountSystem(){
        exit();
        return;
    }
    Taccount currentAccount();
    int currentPrivilege();
    String currentUserID();
    Tselected currentBook();
    String currentISBN();
    bool exit();
    bool su(String, String, LogSystem &);
    bool logout(LogSystem &);
    bool signup(String, String, String, bool, LogSystem &);
    bool passwd(String, String, String, LogSystem &);
    bool useradd(String, String, long long, String, LogSystem &);
    bool remove(String, LogSystem &);
    bool select(Tselected);
    bool changeBook(Tselected, Tselected);
};
