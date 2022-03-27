#include <iostream>
#include "memcheck.h"

using namespace std;

class A {};

int main() {
    A* pA = new A();
    int* n = new int[100];
    char* p = new char[100];
    delete[] p;
    return 0;
}