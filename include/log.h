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
using Tfinance = std::pair<int,std::pair<long long, long long> >;
using Tfinanceinfo = std::tuple<String, String, String, long long, long long>;
using Tlogfinance = std::pair<int, Tfinanceinfo>;
using Tuserinfo = std::tuple<String, String, int, String, String, String, int, String>;
using Tloguser = std::pair<int, Tuserinfo>;
using Tbookinfo = std::tuple<String, String, int, String, String, String, String, long long>;
using Tlogbook = std::pair<int, Tbookinfo>;

struct AccountSystem;

struct LogSystem{
    BlockList<Tfinance> finance_;
    BlockList<Tlogfinance> logfinance_;
    BlockList<Tloguser> loguser_;
    BlockList<Tlogbook> logbook_;
    LogSystem(){
        finance_.initialise("finance.data");
        logfinance_.initialise("logfinance.data");
        loguser_.initialise("loguser.data");
        logbook_.initialise("logbook.data");
        return;
    }
    ~LogSystem() = default;
    void move(long long, bool);
    void log_move_finance(Tfinanceinfo);
    void log_move_user(Tuserinfo);
    void log_move_book(Tbookinfo);
    bool show(long long, AccountSystem &);
    bool report_finance(AccountSystem &);
    void report_user(AccountSystem &, int);
    void report_book(AccountSystem &, int);
    bool report_employee(AccountSystem &, int);
    bool log(AccountSystem &);
};