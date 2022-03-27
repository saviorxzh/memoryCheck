#ifndef _MEMCHECK_H_
#define _MEMCHECK_H_

#include <iostream>
#include <unordered_map>

//存储内存信息
struct MemInfo {
    MemInfo() = default;
    MemInfo(const char* mFile, unsigned long mLine, unsigned long mSize)
        : file(mFile), line(mLine), size(mSize)
    {

    }
    std::string file; //char file[100];需要malloc/free
    unsigned long line;
    unsigned long size;
};

//内存检查
class MemCheck {
private:
    static bool checkStatus;
    static std::unordered_map<void*, MemInfo> MemPool;
    static MemCheck memcheck; //声明1个静态自身类对象,类的所有对象都可用memcheck调用
    unsigned long totalMemSize;
public:
    MemCheck();
    ~MemCheck();
    static void setMemInfo(void* p, const char* file, unsigned long line, unsigned long size);
    static void deleteMemInfo(void* p);
};

void *operator new(size_t size, const char* file, unsigned long line);
void *operator new[](size_t size, const char* file, unsigned long line);
void operator delete(void* ptr);
void operator delete[](void* ptr);

//implementation执行
#ifndef _NEW_OVERLOAD_IMPLEMENTATION_ //此处条件编译的作用:未定义此宏的文件执行下行宏
#define new new(__FILE__, __LINE__) //new(b, c) A;等价于new(sizeof(A), b, c)
#endif

#endif