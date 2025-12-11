#include "checker.h"
#include "interactor.h"
#include <fstream>
#include <iomanip>
#include <cmath>
#include <map>

String turn(std::string str = ""){
    int len = str.length();
    String ret = String();
    for(int i=0;i<len;i++){
        ret[i] = str[i];
    }
    return ret;
}

bool is_invisible(char c) {
    unsigned char uc = static_cast<unsigned char>(c);
    return uc <= 31 || uc == 127;
}

long long getInt(std::string str){
    long long ret = 0;
    bool doted = false;
    int counter = 0;
    for(char now : str){
        if(now != '.'){
            ret = ret * 10 + now - '0';
            if(doted){
                counter++;
            }
        }
        else{
            doted = true;
        }
    }
    if(counter == 0){
        ret *= 100;
    }
    if(counter == 1){
        ret *= 10;
    }
    //std::cerr << "ret = " << ret << std::endl;
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
    if(type == Privilege){
        if(str.length() > 1){
            return false;
        }
        for(char now : str){
            if(!isdigit(now)){
                return false;
            }
        }
        return true;
    }
    if(type == Username){
        if(str.length() > 30){
            return false;
        }
        for(char now : str){
            if(is_invisible(now)){
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
            if(is_invisible(now)){
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
            if(is_invisible(now) || now == '\"'){
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
            if(is_invisible(now) || now == '\"'){
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
        if(getInt(str) > 2147483647){
            return false;
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
        if(getInt(str) > 2147483647){
            return false;
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
    if(apr[got]){
        return true;
    }
    return false;
}

std::string remove_pre_suf(std::string str, Infotype type){
    int len = str.length();
    if(type == ISBN){
        return str.substr(6, len - 6);
    }
    if(type == BookName){
        return str.substr(7, len - 8);
    }
    if(type == Author){
        return str.substr(9, len - 10);
    }
    if(type == Keyword){
        return str.substr(10, len - 11);
    }
    if(type == Price){
        return str.substr(7, len - 7);
    }
    throw -1;
    return "";
}

bool Checker::pre_suf_valid(std::string str, Infotype type){
    int len = str.length();
    if(type == ISBN){
        if(str.length() <= 6){
            return false;
        }
        return str.substr(0, 6) == "-ISBN=" && valid(str.substr(6, len - 6), ISBN);
    }
    if(type == BookName){
        if(str.length() <= 8){
            return false;
        }
        return str.substr(0, 7) == "-name=\"" && str.back() == '\"' && valid(str.substr(7, len - 8), BookName);
    }
    if(type == Author){
        if(str.length() <= 10){
            return false;
        }
        return str.substr(0, 9) == "-author=\"" && str.back() == '\"' && valid(str.substr(9, len - 10), Author);
    }
    if(type == Keyword){
        if(str.length() <= 11){
            return false;
        }
        return str.substr(0, 10) == "-keyword=\"" && str.back() == '\"' && valid(str.substr(10, len - 11), Keyword);
    }
    if(type == Price){
        if(str.length() <= 7){
            return false;
        }
        return str.substr(0, 7) == "-price=" && valid(str.substr(7, len - 7), Price);
    }
    throw -1;
    return false;
}

bool Checker::operate(std::vector<std::string> info, AccountSystem &account, BookSystem &book, LogSystem &log, Interactor &interactor){
    static int counter = 0;
    counter++;
    if(info.empty()){
        return true;
    }
    std::string fir = *info.begin();
    int size = info.size();
    //std::cout << "counter = " << counter << ",fir = " << fir << std::endl;
    if(fir == "quit" || fir == "exit"){
        if(size != 1){
            return false;
        }
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
            return book.show(turn(), turn(), turn(), turn(), account);
        }
        if(info[1] == "finance"){
            if(size >= 4){
                return false;
            }
            if(size == 2){
                return log.show(-1, account);
            }
            if(!valid(info[2], Count))return false;
            return log.show(std::stoi(info[2]), account);
        }
        else{
            if(size != 2){
                return false;
            }
            bool can[4] = {pre_suf_valid(info[1], ISBN), pre_suf_valid(info[1], BookName), pre_suf_valid(info[1], Author), pre_suf_valid(info[1], Keyword)};
            if(can[0]){
                //std::cout << "show by ISBN" << std::endl;
                return book.show(turn(remove_pre_suf(info[1], ISBN)), turn(), turn(), turn(), account);
            }
            if(can[1]){
                //std::cout << "show by bookname" << std::endl;
                return book.show(turn(), turn(remove_pre_suf(info[1], BookName)), turn(), turn(), account);
            }
            if(can[2]){
                //std::cout << "show by author" << std::endl;
                return book.show(turn(), turn(), turn(remove_pre_suf(info[1], Author)), turn(), account);
            }
            if(can[3]){
                //std::cout << "show by keyword" << std::endl;
                if(!single_keyword(info[1]))return false;
                return book.show(turn(), turn(), turn(), turn(remove_pre_suf(info[1], Keyword)), account);
            }
            return false;
        }
    }
    if(fir == "buy"){
        if(size != 3){
            return false;
        }
        if(!valid(info[1], ISBN) || !valid(info[2], Quantity))return false;
        return book.buy(turn(info[1]), std::stoll(info[2]), account, log);
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
                if(duplicated_keyword(remove_pre_suf(info[wc], Keyword))){
                    return false;
                }
                keyword = remove_pre_suf(info[wc], Keyword), gkeyword = true;
            }
            if(pre_suf_valid(info[wc], Price)){
                if(gprice){
                    return false;
                }
                price = remove_pre_suf(info[wc], Price), gprice = true;
            }
            return false;
        }
        return book.modify(turn(isbn), turn(bookname), turn(author), turn(keyword), gprice ? getInt(price) : -1, account);
    }
    if(fir == "import"){
        if(size != 3){
            return false;
        }
        if(!valid(info[1], Quantity) || !valid(info[2], TotalCost))return false;
        return book.import(std::stoll(info[1]), getInt(info[2]), account, log);
    }
    return false;
}
