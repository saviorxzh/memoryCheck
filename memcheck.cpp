#define _NEW_OVERLOAD_IMPLEMENTATION_
#include "memcheck.h"


//运算符重载new的第一个参数必须是size
void *operator new(size_t size, const char* file, unsigned long line) {
    if (size == 0)
        size = 1; //空类时size=1
    void* ptr = malloc(size);
    if (ptr == nullptr)
        std::cout << "ERROR NEW!" << std::endl;
    else
        MemCheck::setMemInfo(ptr, file, line, size);
    return ptr;
}

void *operator new[](size_t size, const char* file, unsigned long line) {
    if (size == 0)
        size = 1;
    void* ptr = malloc(size);
    if (ptr == nullptr)
        std::cout << "ERROR NEW[]!" << std::endl;
    else
        MemCheck::setMemInfo(ptr, file, line, size);
    return ptr;
}

void operator delete(void* ptr) {
    MemCheck::deleteMemInfo(ptr);
    if (ptr) free(ptr);
}

void operator delete[](void* ptr) {
    MemCheck::deleteMemInfo(ptr);
    if (ptr) free(ptr); //此时ptr已经计算为整个数组大小
}


//static类成员变量先定义的后析构
bool MemCheck::checkStatus = true;
std::unordered_map<void*, MemInfo> MemCheck::MemPool;
MemCheck MemCheck::memcheck; //生成1个常量区对象,程序结束时释放

MemCheck::MemCheck() {
    std::cout << "================Memory constructor==============" << std::endl;
}

MemCheck::~MemCheck() {
    checkStatus = false;
    std::cout << "================Memory destructor===============" << std::endl;
    if (MemPool.size() > 0) {
        std::cout << "---------------leak memory info begin-------------" << std::endl;
        for (auto it : MemPool) {
            std::cout << "Leak Memory Address [" << it.first << "], ";
            std::cout << "File [" << it.second.file << "], ";
            std::cout << "Line [" << it.second.line << "], ";
            std::cout << "Size [" << it.second.size << "]";
            std::cout << std::endl;
        }
        std::cout << "---------------leak memory info end---------------" << std::endl;        
    }
}

void MemCheck::setMemInfo(void* p, const char* file, unsigned long line, unsigned long size) {
    if(!checkStatus) return ;
    std::cout << "Malloc Address [" << p << "], ";
    std::cout << "size[" << size << "]";
    std::cout << std::endl;
    MemInfo pInfo(file, line, size); //栈区
    memcheck.totalMemSize += size;
    memcheck.MemPool[p] = pInfo; //全局区
    std::cout << "Total Memory Size = " << memcheck.totalMemSize << std::endl;
}

void MemCheck::deleteMemInfo(void* p) {
    if(!checkStatus) return ;
    std::cout << "Delete address [" << p << "]" << std::endl;
    auto pInfo = memcheck.MemPool.find(p);
    if (pInfo != memcheck.MemPool.end()) {
        memcheck.totalMemSize -= pInfo->second.size;
        std::cout << "Total Memory Size = " << memcheck.totalMemSize << std::endl;
        memcheck.MemPool.erase(pInfo); //STL的erase底层调用delete,会输出PInfo自身的address
    }
}