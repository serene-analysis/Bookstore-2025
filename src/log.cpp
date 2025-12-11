#include "log.h"
#include <array>
#include <utility>
#include <tuple>

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