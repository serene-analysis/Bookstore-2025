#pragma once

#include "blocklist.h"
#include "account.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <utility>
#include <tuple>
using Tfinance = std::pair<int,std::pair<long long, long long> >;
using Tlog = std::pair<int, std::string>;

struct LogSystem{
    BlockList<Tfinance> finance_;
    BlockList<Tlog> log_;
    LogSystem(){
        finance_.initialise("finance.data");
        log_.initialise("log.data");
        return;
    }
    ~LogSystem() = default;
    void move(long long, bool);
    bool show(long long, AccountSystem &);
    bool report_finance(AccountSystem &);
    bool report_employee(AccountSystem &);
    bool log(AccountSystem &);
};