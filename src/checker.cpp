#include "checker.h"
#include "interactor.h"
#include <fstream>
#include <iomanip>
#include <map>

std::array<char,61> turn(std::string str = ""){
    int len = str.length();
    std::array<char,61> ret = std::array<char,61>();
    for(int i=0;i<len;i++){
        ret[i] = str[i];
    }
    return ret;
}

bool Checker::valid(std::string str, Infotype type){
    if(type == UserID || type == Password || type == CurrentPassword || type == NewPassword){
        if(str.length() > 30){
            return false;
        }
        for(char now : str){
            if(!(isdigit(now) || isalpha(now) || now == '_')){
                return false;
            }
        }
        return true;
    }
    if(type == ISBN){
        if(str.length() > 20){
            return false;
        }
        for(char now : str){
            if(isblank(now)){
                return false;
            }
        }
        return true;
    }
    if(type == BookName || type == Author){
        if(str.length() > 60){
            return false;
        }
        for(char now : str){
            if(isblank(now) || now == '\"'){
                return false;
            }
        }
        return true;
    }
    if(type == Keyword){
        if(str.length() > 60){
            return false;
        }
        for(char now : str){
            if(isblank(now) || now == '\"'){
                return false;
            }
        }
        int lasttype = 1;// In fact -1, but this is also right
        for(char now : str){
            if(now == '|'){
                if(lasttype != 0){
                    return false;
                }
                lasttype = 1;
            }
            else{
                lasttype = 0;
            }
        }
        if(lasttype == 1){
            return false;
        }
        return true;
    }
    if(type == Quantity){
        if(str.length() > 10){
            return false;
        }
        for(char now : str){
            if(!isdigit(now)){
                return false;
            }
        }
        return true;
    }
    if(type == Price || type == TotalCost){
        if(str.length() > 13){
            return false;
        }
        for(char now : str){
            if(!(isdigit(now) || now == '.')){
                return false;
            }
        }
        return true;
    }
    if(type == Count){
        if(str.length() > 10){
            return false;
        }
        for(char now : str){
            if(!isdigit(now)){
                return false;
            }
        }
        return true;
    }
    throw false;
    return false;
}

bool single_keyword(std::string str){
    for(char now : str){
        if(now == '|'){
            return false;
        }
    }
    return true;
}

bool duplicated_keyword(std::string str){
    std::map<std::string,bool> apr;
    std::string got;
    for(char now : str){
        if(now == '|'){
            if(apr[got]){
                return true;
            }
            apr[got] = true, got.clear();
        }
        else{
            got += now;
        }
    }
    return false;
}

std::string remove_pre_suf(std::string str, Infotype type){
    int len = str.length();
    if(type == ISBN){
        return str.substr(6, len - 1);
    }
    if(type == BookName){
        return str.substr(7, len - 2);
    }
    if(type == Author){
        return str.substr(9, len - 2);
    }
    if(type == Keyword){
        return str.substr(10, len - 2);
    }
    if(type == Price){
        return str.substr(7, len - 1);
    }
    throw -1;
    return "";
}

bool Checker::pre_suf_valid(std::string str, Infotype type){
    if(type == ISBN){
        if(str.length() <= 6){
            return false;
        }
        return str.substr(0, 5) == "-ISBN=";
    }
    if(type == BookName){
        if(str.length() <= 8){
            return false;
        }
        return str.substr(0, 6) == "-name=\"" && str.back() == '\"';
    }
    if(type == Author){
        if(str.length() <= 10){
            return false;
        }
        return str.substr(0, 8) == "-author=\"" && str.back() == '\"';
    }
    if(type == Keyword){
        if(str.length() <= 11){
            return false;
        }
        return str.substr(0, 9) == "-keyword=\"" && str.back() == '\"';
    }
    if(type == Price){
        if(str.length() <= 7){
            return false;
        }
        return str.substr(0, 6) == "-price=";
    }
    throw -1;
    return false;
}

bool Checker::operate(std::vector<std::string> info, AccountSystem &account, BookSystem &book, LogSystem &log, Interactor &interactor){
    if(info.empty()){
        return false;
    }
    std::string fir = *info.begin();
    int size = info.size();
    if(fir == "quit" || fir == "exit"){
        if(size != 1){
            return false;
        }
        interactor.end_ = true;
        return account.exit();// always return true
    }
    if(fir == "su"){
        if(size != 2 && size != 3){
            return false;
        }
        if(!valid(info[1], UserID))return false;
        if(size == 2){
            return account.su(turn(info[1]), turn());
        }
        else{
            if(!valid(info[2], Password))return false;
            return account.su(turn(info[1]), turn(info[2]));
        }
    }
    if(fir == "logout"){
        if(size != 1){
            return false;
        }
        return account.logout();
    }
    if(fir == "register"){
        if(size != 4){
            return false;
        }
        if(!valid(info[1], UserID) || !valid(info[2], Password) || !valid(info[3], Username))return false;
        return account.signup(turn(info[1]), turn(info[2]), turn(info[3]), false);
    }
    if(fir == "passwd"){
        if(size != 3 && size != 4){
            return false;
        }
        if(size == 3){
            if(!valid(info[1], UserID) || !valid(info[2], NewPassword))return false;
            return account.passwd(turn(info[1]), turn(), turn(info[2]));
        }
        else{
            if(!valid(info[1], UserID) || !valid(info[2], CurrentPassword) || !valid(info[3], Password))return false;
            return account.passwd(turn(info[1]), turn(info[2]), turn(info[3]));
        }
    }
    if(fir == "useradd"){
        if(size != 5){
            return false;
        }
        if(!valid(info[1], UserID) || !valid(info[2], Password) || !valid(info[3], Privilege) || !valid(info[4], Username))return false;
        return account.useradd(turn(info[1]), turn(info[2]), std::stoi(info[3]), turn(info[4]));
    }
    if(fir == "delete"){
        if(size != 2){
            return false;
        }
        if(!valid(info[1], UserID))return false;
        return account.remove(turn(info[1]));
    }
    if(fir == "show"){
        if(size == 1){
            return book.show(turn(), turn(), turn(), turn());
        }
        if(info[1] == "finance"){
            if(size != 3){
                return false;
            }
            if(!valid(info[2], Count))return false;
            return log.show(std::stoi(info[2]));
        }
        else{
            if(size != 2){
                return false;
            }
            bool can[4] = {pre_suf_valid(info[1], ISBN), pre_suf_valid(info[1], BookName), pre_suf_valid(info[1], Author), pre_suf_valid(info[1], Keyword)};
            if(can[0]){
                return book.show(turn(remove_pre_suf(info[1], ISBN)), turn(), turn(), turn());
            }
            if(can[1]){
                return book.show(turn(), turn(remove_pre_suf(info[1], BookName)), turn(), turn());
            }
            if(can[2]){
                return book.show(turn(), turn(), turn(remove_pre_suf(info[1], Author)), turn());
            }
            if(can[3]){
                if(!single_keyword(info[1]))return false;
                return book.show(turn(), turn(), turn(), turn(remove_pre_suf(info[1], Keyword)));
            }
            return false;
        }
    }
    if(fir == "buy"){
        if(size != 3){
            return false;
        }
        if(!valid(info[1], ISBN) || !valid(info[2], Quantity))return false;
        return book.buy(turn(info[1]), std::stoll(info[2]), account);
    }
    if(fir == "select"){
        if(size != 2){
            return false;
        }
        if(!valid(info[1], ISBN))return false;
        return book.select(turn(info[1]), account);
    }
    if(fir == "modify"){
        if(size == 1){
            return false;
        }
        std::string isbn, bookname, author, keyword, price;
        bool gisbn = false, gbookname = false, gauthor = false, gkeyword = false, gprice = false;
        for(int wc=1;wc<info.size();wc++){
            if(pre_suf_valid(info[wc], ISBN)){
                if(gisbn){
                    return false;
                }
                isbn = remove_pre_suf(info[wc], ISBN), gisbn = true;
            }
            if(pre_suf_valid(info[wc], BookName)){
                if(gbookname){
                    return false;
                }
                bookname = remove_pre_suf(info[wc], BookName), gbookname = true;
            }
            if(pre_suf_valid(info[wc], Author)){
                if(gauthor){
                    return false;
                }
                author = remove_pre_suf(info[wc], Author), gauthor = true;
            }
            if(pre_suf_valid(info[wc], Keyword)){
                if(gkeyword){
                    return false;
                }
                if(duplicated_keyword(info[wc])){
                    return false;
                }
                keyword = remove_pre_suf(info[wc], ISBN), gkeyword = true;
            }
            if(pre_suf_valid(info[wc], Price)){
                if(gprice){
                    return false;
                }
                price = remove_pre_suf(info[wc], Price), gprice = true;
            }
        }
        return book.modify(turn(isbn), turn(bookname), turn(author), turn(keyword), std::stod(price), account);
    }
    if(fir == "import"){
        if(size != 3){
            return false;
        }
        if(!valid(info[1], Quantity) || !valid(info[2], TotalCost))return false;
        return book.import(std::stoll(info[1]), std::stod(info[2]), account);
    }
    throw "error";
    return false;
}