#pragma once
#include "blocklist.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using Tselected = std::pair<std::array<char,61>, std::tuple<std::array<char,61>, std::array<char,61>, std::array<char,61>, std::array<char,61>, double, long long > >;
using Taccount = std::pair<std::array<char,61>,std::tuple<std::array<char,61>, int, std::array<char,61>, Tselected > >;
using Tstackaccount = std::pair<int, Taccount>;
using Tmap = std::pair<std::array<char,61>, int>;

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
    bool exit();
    bool su(std::array<char,61>, std::array<char,61>);
    bool logout();
    bool signup(std::array<char,61>, std::array<char,61>, std::array<char,61>, bool);
    bool passwd(std::array<char,61>, std::array<char,61>, std::array<char,61>);
    bool useradd(std::array<char,61>, std::array<char,61>, int, std::array<char,61>);
    bool remove(std::array<char,61>);
};