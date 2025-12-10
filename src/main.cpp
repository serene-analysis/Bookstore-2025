#include "interactor.h"
#include "checker.h"
#include "account.h"
#include "book.h"
#include "log.h"
#include <algorithm>
#include <cstdio>
#include <vector>
AccountSystem account;
BookSystem book;
LogSystem log;
Checker checker;
Interactor interactor;
signed main(){
    interactor.tian(account, book, log, checker);
    return 0;
}