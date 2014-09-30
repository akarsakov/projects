#ifndef KMERS_GENERATOR
#define KMERS_GENERATOR

#include <string>
#include <math.h>

std::string number2pattern(int number, int k);
int pattern2number(std::string pattern);

class kMerIterator
{
private:
    int count;
    int k;
public:
    kMerIterator(int _c, int _k): count(_c), k(_k) { };

    friend std::string operator*(const kMerIterator& it);
    friend const kMerIterator& operator++(kMerIterator& it);
    friend const kMerIterator operator++(kMerIterator& it, int);
    friend bool operator==(const kMerIterator& first, const kMerIterator& second);
    friend bool operator!=(const kMerIterator& first, const kMerIterator& second);
};

class kMersGenerator
{
private:
    int k;

public:
    kMersGenerator(int _k): k(_k) { };

    kMerIterator begin() { return kMerIterator(0, k); };
    kMerIterator end() { return kMerIterator((int) pow(4, k), k); };
};

#endif /* KMERS_GENERATOR */