#pragma once
#include "blocklist.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using String = std::array<char,61>;
using Tselected = std::pair<String, std::tuple<String, String, String, String, double, long long > >;
using Taccount = std::pair<String,std::tuple<String, int, String, Tselected > >;
using Tstackaccount = std::pair<int, Taccount>;
using Tmap = std::pair<String, int>;

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
    ~AccountSystem() = default;
    Taccount currentAccount();
    int currentPrivilege();
    Tselected currentBook();
    bool exit();
    bool su(String, String);
    bool logout();
    bool signup(String, String, String, bool);
    bool passwd(String, String, String);
    bool useradd(String, String, int, String);
    bool remove(String);
    bool select(Tselected);
    bool changeBook(Tselected, Tselected);
};