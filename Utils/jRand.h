#ifndef J_RAND_H_
#define J_RAND_H_

#include <vector>

class jRand{
public:
    static jRand& getInstance();

    void init(int num_rands);
    double operator()();
    double gen_rand();

protected:
    std::vector<double> _rands;
    int _count;

private:
    jRand(){}
    jRand(const jRand& other);
    void operator=(jRand const& other);
};

#endif
