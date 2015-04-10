#include <cstdlib>
#include <limits.h>
#include <vector>
#include <ctime>
#include "jRand.h"

jRand& jRand::getInstance(){
    static jRand instance;
    return instance;
}

void jRand::init(int num_rands){
    _rands.clear();
    _rands.resize(num_rands);

    srand(time(NULL));
    for(int i =0 ;i < num_rands; ++i){
        _rands[i] = gen_rand();
    }
    _count = -1;
}

double jRand::operator()(){
    _count = (_count + 1) % _rands.size();
    return _rands[_count];
}

double jRand::gen_rand(){
    return rand()/(double)RAND_MAX;
}
