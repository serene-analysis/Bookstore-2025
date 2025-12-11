#include "log.h"
#include <array>
#include <utility>
#include <tuple>

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

bool LogSystem::show(int count, AccountSystem &account){
    if(account.currentPrivilege() < 7)return false;
    if(count == 0){
        std::cout << std::endl;
        return true;
    }
    Tfinance all = finance_.getnumber() == 0 ? Tfinance() : finance_.findLast(), want = Tfinance();
    want.first = all.first - count;
    if(want.first < 0)return false;
    if(count == -1 || want.first == 0){
        std::cout << "+ " << (long double)(all.second.first + 1e-2) / 100.0 << " - " << (long double)(all.second.second + 1e-2) / 100.0 << std::endl;
        return true;
    }
    Tfinance got = finance_.findSimilar(want);
    std::cout << "+ " << (long double)(all.second.first - got.second.first + 1e-2) / 100.0 << " - " << (long double)(all.second.second - got.second.second + 1e-2) / 100.0 << std::endl;
    return true;
}