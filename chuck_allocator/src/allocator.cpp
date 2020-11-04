#include "allocator.h"

using namespace task;

int main() {
    Allocator<int>* al = new Allocator<int>;
    int* p = al->allocate(1); // Is allocated in the 1st chunk
    al->construct(p);
    *p = 1221;
    int* p1 = al->allocate(2); // Is allocated in the 2nd chunk
    al->construct(p1);
    *p1 = -14521;
    int* p2 = p1 + 1;
    al->construct(p2);
    *p2 = 454;
    al->allocate(1); // Is allocated in the 1st chunk
    al->allocate(1); // Is allocated in the 3rd chunk

    auto* al1 = new Allocator<int>(*al);
    al->allocate(2);
    al1->allocate(1);
    delete al;
    delete al1;
    return 0;
}