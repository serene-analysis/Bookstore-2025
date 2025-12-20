#pragma once
#include <algorithm>
#include <vector>
#include <iostream>
#include <array>
#include <utility>
#include <tuple>
#include <fstream>
#include <cassert>

using std::fstream;
using std::ifstream;
using std::ofstream;
using std::ios;

const static int Knext = 1, Ksize = 2;

//#define DEBUG

template<class T>
bool similar(T now, T oth){
#ifdef DEBUG
std::cout << "similar:" << std::endl;
for(int i=0;i<63;i++){
    std::cout << now.first.operator[](i);
}
std::cout << std::endl;
for(int i=0;i<63;i++){
    std::cout << oth.first.operator[](i);
}
std::cout << std::endl;
#endif
    return now.first == oth.first;
}

/*bool similar(std::pair<std::array<char,64>, unsigned int> now, std::pair<std::array<char,64>, unsigned int> oth){
    return now.first == oth.first;
}*/

template<class T, int info_len = 2, int block = 500>
class MemoryRiver {
public:
    /* your code here */
    fstream file;
    std::string file_name;
    int sizeofT = sizeof(T);
    int sizeofRiver = sizeof(int)*info_len + sizeofT * block;
    T pool[505];


    MemoryRiver() = default;

    MemoryRiver(const std::string file_name) : file_name(file_name) {}

    //初始化偏移量为 delta 的块

    void initialise(std::string FN = "", int delta = 0) {
        //std::cout << "MemoryRiver : initialise " << FN << ", delta = " << delta << std::endl;
        if (FN != "") file_name = FN;
        file.seekp(delta, ios::beg);
        int tmp = 0;
        for (int i = 0; i < info_len; ++i){
            file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        }
        T emp = T();
        for(int i=0;i<block;i++){
            file.write(reinterpret_cast<char*>(&emp), sizeofT);
        }
        return;
    }

    //读出第n个int的值赋给tmp，1_base
    int get_info(int n, int delta) {
        int tmp = 0;
        if (n > info_len){
            throw std::out_of_range("get_info");
        }
        file.seekg(delta + sizeof(int)*(n-1), ios::beg);
        file.read(reinterpret_cast<char*>(&tmp), sizeof(int));
#ifdef DEBUG
//std::cout << "get_info, type = " << ((n == 1) ? "next" : "size") << ", delta = " << delta << ", ret = " << tmp << std::endl;
#endif
        return tmp;
        /* your code here */
    }

    //将tmp写入第n个int的位置，1_base
    void write_info(int tmp, int n, int delta) {
        if (n > info_len){
            throw std::out_of_range("write_info");
        }
        file.seekp(delta + sizeof(int)*(n-1), ios::beg);
        file.write(reinterpret_cast<char*>(&tmp), sizeof(int));
#ifdef DEBUG
//std::cout << "write_info, type = " << ((n == 1) ? "next" : "size") << ", delta = " << delta << ", tmp = " << tmp << std::endl;
#endif
        return;
        /* your code here */
    }

    //是否满

    bool full(int delta){
#ifdef DEBUG
std::cout << "checkfull : " << delta << std::endl;
int msize = get_info(Ksize, delta);
std::cout << "msize = " << msize << ", block - 1 = " << block - 1 << std::endl;
#endif
        return get_info(Ksize, delta) == block - 1;
    }

    //是否空

    bool empty(int delta){
        return get_info(Ksize, delta) == 0;
    }

    //在文件合适位置写入类对象t
    void write(T &t, int delta) {
#ifdef DEBUG
std::cout << "write, delta = " << delta << std::endl;
similar(t,t);
#endif
        int nsize = get_info(Ksize, delta);
        file.seekg(delta + sizeof(int)*info_len, ios::beg);
        T nv;
        bool found = false;
        for(int i=1;i<=nsize;i++){
            file.read(reinterpret_cast<char*>(&nv), sizeofT);
            if(nv >= t){
                //std::cout << "write found, i = " << i << std::endl;
                T arr = t, mem = nv;
                pool[0] = t;
                file.seekg(-sizeofT, ios::cur);
                file.read(reinterpret_cast<char*>(&pool[1]),sizeofT * (nsize-i+1));
                file.seekp(-sizeofT * (nsize-i+1), ios::cur);
                file.write(reinterpret_cast<char*>(pool),sizeofT * (nsize-i+2));
                /*for(int j=1;j<=nsize-i+2;j++){
                    //std::cout << "i-1+j = " << i-1+j << std::endl;
                    //similar(arr, arr);
                    file.seekp(-sizeofT, ios::cur);
                    file.write(reinterpret_cast<char*>(&arr),sizeofT);
                    arr = mem;
                    if(j != nsize-i+2){
                        file.read(reinterpret_cast<char*>(&mem),sizeofT);
                    }
                }*/
                found = true;
                break;
            }
        }
        if(!found){
#ifdef DEBUG
std::cout << "append in the end" << std::endl;
#endif
            file.write(reinterpret_cast<char*>(&t), sizeofT);
        }
        file.seekg(delta + sizeof(int)*info_len + sizeofT*(block-1), ios::beg);
        T mx,imx;
        file.read(reinterpret_cast<char*>(&imx), sizeofT);
        if(nsize == 0){
            mx = t;
        }
        else{
            mx = std::max(imx, t);
        }
        //similar(mx, mx);
        file.seekp(-sizeofT, ios::cur);
        file.write(reinterpret_cast<char*>(&mx), sizeofT);
        /*if(nsize >= 1){
            T now = read(1, delta);
            similar(now, now);
            now = read(2, delta);
            similar(now, now);
        }*/
        write_info(nsize+1, Ksize, delta);
        //std::cout << "delta = " << delta << ", size = " << nsize+1 << std::endl;
        //mx = read(block, delta);
        //similar(mx, mx);
        return;
        /* your code here */
    }

    T read(const int index, int delta) {
        T t;
        file.seekg(delta + sizeof(int)*info_len + sizeofT * (index-1), ios::beg);
        file.read(reinterpret_cast<char*>(&t), sizeofT);
        return t;
        /* your code here */
    }

    bool remove(T &value, int delta) {
        /* your code here */
        int nsize = get_info(Ksize, delta);
        file.seekg(delta + sizeof(int)*info_len, ios::beg);
        T nv;
        for(int i=1;i<=nsize;i++){
            file.read(reinterpret_cast<char*>(&nv), sizeofT);
            if(nv == value){
#ifdef DEBUG
std::cout << "delta = " << delta << ", remove!" << std::endl;
#endif
                file.read(reinterpret_cast<char*>(pool),sizeofT * (nsize-i));
                file.seekp(-sizeofT * (nsize-i+1), ios::cur);
                file.write(reinterpret_cast<char*>(pool),sizeofT * (nsize-i));
                /*
                T nxt;
                for(int j=1;j<=nsize-i;j++){
                    file.read(reinterpret_cast<char*>(&nxt),sizeofT);
                    file.seekp(-2*sizeofT, ios::cur);
                    file.write(reinterpret_cast<char*>(&nxt),sizeofT);
                    if(j != nsize - i){
                        file.seekg(sizeofT, ios::cur);
                    }
                }*/
                if(i == nsize && nsize != 1){
                    file.seekp(delta + sizeof(int)*info_len + sizeofT*(block-1), ios::beg);
                    T nv;
                    file.seekg(delta + sizeof(int)*info_len + sizeofT*(nsize-2), ios::beg);
                    file.read(reinterpret_cast<char*>(&nv), sizeofT);
                    file.seekp(delta + sizeof(int)*info_len + sizeofT*(block-1), ios::beg);
                    file.write(reinterpret_cast<char*>(&nv), sizeofT);
                }
                write_info(nsize-1, Ksize, delta);
                return true;
            }
        }
        return false;
    }
    
    bool overall(T &value, int delta) {
        bool have = false;
        int nsize = get_info(Ksize, delta);
        /* your code here */
        file.seekg(delta + sizeof(int)*info_len, ios::beg);
        T nv;
        for(int i=1;i<=nsize;i++){
            file.read(reinterpret_cast<char*>(&nv), sizeofT);
            if(similar(value, nv)){
                have = true;
                std::cout << nv.second << ' ';
            }
        }
        return have;
    }

    
    T find(T &value, int delta) {
        bool have = false;
        int nsize = get_info(Ksize, delta);
        /* your code here */
        file.seekg(delta + sizeof(int)*info_len, ios::beg);
        T nv, ret = T();
        for(int i=1;i<=nsize;i++){
            file.read(reinterpret_cast<char*>(&nv), sizeofT);
            if(value == nv){
                ret = nv;
            }
        }
        return ret;
    }
    
    T findSimilar(T &value, int delta) {
        bool have = false;
        int nsize = get_info(Ksize, delta);
        /* your code here */
        file.seekg(delta + sizeof(int)*info_len, ios::beg);
        T nv, ret = T();
        for(int i=1;i<=nsize;i++){
            file.read(reinterpret_cast<char*>(&nv), sizeofT);
            if(similar(value, nv)){
                ret = nv;
            }
        }
        return ret;
    }

    std::vector<T> allSimilar(T &value, int delta) {
        int nsize = get_info(Ksize, delta);
        /* your code here */
        file.seekg(delta + sizeof(int)*info_len, ios::beg);
        T nv;
        std::vector<T> ret;
        for(int i=1;i<=nsize;i++){
            file.read(reinterpret_cast<char*>(&nv), sizeofT);
            if(similar(value, nv)){
                ret.push_back(nv);
            }
        }
        return ret;
    }

    std::vector<T> all(int delta) {
        int nsize = get_info(Ksize, delta);
        /* your code here */
        file.seekg(delta + sizeof(int)*info_len, ios::beg);
        T nv;
        std::vector<T> ret;
        for(int i=1;i<=nsize;i++){
            file.read(reinterpret_cast<char*>(&nv), sizeofT);
            ret.push_back(nv);
        }
        return ret;
    }
};


template<class T, int info_len = 2, int block = 500> // require T ==/<=/>=/</>/!= T
class BlockList {
public:
    std::string file_name;
    MemoryRiver<T,info_len,block> list;
    int sizeofRiver = sizeof(int)*info_len + sizeof(T)*block;
    int leftmemory = sizeof(int)*3;
    BlockList() = default;
    int size = 0, head = leftmemory, number = 0;

    BlockList(const std::string file_name) : file_name(file_name) {}

    ~BlockList(){
        list.file.seekp(0, ios::beg);
            //std::cerr << "program end" << std::endl;
            //std::cout << "size = " << size << std::endl;
            //std::cout << "head = " << head << std::endl;
            //std::cout << "number = " << number << std::endl;
        list.file.write(reinterpret_cast<char*>(&size), sizeof(int));
        list.file.write(reinterpret_cast<char*>(&head), sizeof(int));
        list.file.write(reinterpret_cast<char*>(&number), sizeof(int));
        list.file.close();
        return;
    }

    void initialise(std::string FN = "") {
        file_name = FN;
        list.file.open(file_name, ios::in | ios::out | ios::binary);
        if(!list.file.is_open()){
            list.file.open(file_name, ios::out | ios::binary);
            list.file.close();
            list.file.open(file_name, ios::in | ios::out | ios::binary);
            size = 1;
            list.initialise(file_name, head);
        }
        else{
            list.file.read(reinterpret_cast<char*>(&size), sizeof(int));
            list.file.read(reinterpret_cast<char*>(&head), sizeof(int));
            list.file.read(reinterpret_cast<char*>(&number), sizeof(int));
            //std::cerr << "continued" << std::endl;
            //std::cout << "size = " << size << std::endl;
            //std::cout << "head = " << head << std::endl;
            //std::cout << "number = " << number << std::endl;
        }
        //std::cout << "sizeofT = " << sizeof(T) << std::endl;
        return;
    }

    bool empty(){
        return number == 0;
    }

    int getnumber(){
        return number;
    }

    void insert(T &value) {
        number++;
        int now = head, nxt = 0;
        while(true){
            nxt = list.get_info(Knext, now);
#ifdef DEBUG
std::cout << "now = " << now << ", nxt = " << nxt << std::endl;
int msize = list.get_info(Ksize, now);
std::cout << "now.size = " << msize << std::endl;
#endif
            if(list.empty(now) || list.read(block, now) < value){
                if(list.empty(now)){
                    list.write(value, now);
                    break;
                }
                if(!nxt){
                    if(list.full(now)){
                        nxt = size * sizeofRiver + leftmemory, size++;
                        list.write_info(nxt, Knext, now);
                        list.initialise(file_name, nxt);
                        list.write(value, nxt);
                        break;
                    }
                    else{
                        list.write(value, now);
                        break;
                    }
                }
                else{
                    if(list.read(1, nxt) < value){
                        now = nxt;
                    }
                    else{
                        list.write(value, now);
                        break;
                    }
                }
            }
            else{
                list.write(value, now);
                break;
            }
        }
#ifdef DEBUG
std::cout << "after:(block = " << block << ")" << std::endl;
for(int i=1;i<=block-1;i++){
    std::pair<std::array<char,64>,unsigned int> nv = list.read(i, now);
    similar(nv, nv);
}
#endif
        if(list.full(now)){
            int arr = size * sizeofRiver + leftmemory;
            list.initialise(file_name, arr), size++;
#ifdef DEBUG
std::cout << "full, after:(block = " << block << ")" << std::endl;
#endif
            list.write_info(nxt, Knext, arr);
            list.write_info(arr, Knext, now);
            for(int i=1;i<=(block-1)/2;i++){
                T nv = list.read(block-i, now);
                list.write(nv, arr);
                list.remove(nv, now);
            }
        }
        return;
    }

    void remove(T &value) {
        int now = head, last = head;
        while(true){
            if(list.empty(now)){
                now = list.get_info(Knext, now);
                if(!now){
                    return;
                }
                continue;
            }
            if(list.read(block, now) >= value){
                number -= list.remove(value, now);
                if(list.empty(now)){
                    if(now == head){
                        if(number){
                            head = list.get_info(Knext, now);
                        }
                        else{
                            head = leftmemory;
                        }
                    }
                    else{
                        list.write_info(list.get_info(Knext, now), Knext, last);
                    }
                }
                break;
            }
            last = now, now = list.get_info(Knext, now);
            if(!now){
                return;
            }
        }
        return;
    }
    
    void pop_back(){
        int now = head, left = number;
        while(true){
            if(list.empty(now)){
                now = list.get_info(Knext, now);
                if(!now){
                    return;
                }
                continue;
            }
            /*similar(list.read(1, now), value);
            similar(list.read(block, now), value);*/
            int nsize = list.get_info(Ksize, now);
            //std::cout << "left = " << left << ", nsize = " << nsize << std::endl;
            if(nsize == left){
                T gv = list.read(block, now);
                remove(gv);
                return;
            }
            else{
                left -= nsize;
            }
            now = list.get_info(Knext, now);
            if(!now){
                return;
            }
        }
        return;
    }

    T findLast(){
        int now = head;
        T ret = T();
        while(true){
            if(list.empty(now)){
                now = list.get_info(Knext, now);
                if(!now){
                    return ret;
                }
                continue;
            }
            /*similar(list.read(1, now), value);
            similar(list.read(block, now), value);*/
            ret = list.read(block, now);
            now = list.get_info(Knext, now);
            if(!now){
                return ret;
            }
        }
        return ret;
    }

    T findSimilar(T &value){
        int now = head;
        std::vector<int>got;
        while(true){
            if(list.empty(now)){
                now = list.get_info(Knext, now);
                if(!now){
                    break;
                }
                continue;
            }
            if(list.read(block, now).first >= value.first && list.read(1, now).first <= value.first){
                got.push_back(now);
            }
            else{
                if(list.read(1, now).first > value.first){
                    break;
                }
            }
            now = list.get_info(Knext, now);
            if(!now){
                break;
            }
        }
        std::reverse(got.begin(), got.end());
        for(int nv : got){
            T ngot = list.findSimilar(value, nv);
            if(ngot != T()){
                return ngot;
            }
        }
        return T();
    }

    std::vector<T> allSimilar(T &value){
        int now = head;
        std::vector<int> got;
        std::vector<T> ret;
        while(true){
            if(list.empty(now)){
                now = list.get_info(Knext, now);
                if(!now){
                    break;
                }
                continue;
            }
            if(list.read(block, now).first >= value.first && list.read(1, now).first <= value.first){
                got.push_back(now);
            }
            else{
                if(list.read(1, now).first > value.first){
                    break;
                }
            }
            now = list.get_info(Knext, now);
            if(!now){
                break;
            }
        }
        for(int nv : got){
            std::vector<T> ngot = list.allSimilar(value, nv);
            for(T gv : ngot){
                ret.push_back(gv);
            }
        }
        return ret;
    }

    std::vector<T> all(){
        int now = head;
        std::vector<int> got;
        std::vector<T> ret;
        while(true){
            if(list.empty(now)){
                now = list.get_info(Knext, now);
                if(!now){
                    break;
                }
                continue;
            }
            got.push_back(now);
            now = list.get_info(Knext, now);
            if(!now){
                break;
            }
        }
        for(int nv : got){
            std::vector<T> ngot = list.all(nv);
            for(T gv : ngot){
                ret.push_back(gv);
            }
        }
        return ret;
    }

    void modify(T &given, T &arr){
        remove(given);
        insert(arr);
        return;
    }

};