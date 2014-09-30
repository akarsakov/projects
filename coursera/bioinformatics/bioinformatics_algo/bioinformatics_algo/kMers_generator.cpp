#include "kMers_generator.h"
#include "DNA_processing.h"

using namespace std;

string number2pattern(int number, int k)
{
    string pattern;
    for (int i=0; i<k; i++)
    {
        pattern = int2nucl(number % 4) + pattern;
        number /= 4;
    }
    return pattern;
}

int pattern2number(string pattern)
{
    long number = 0, mult = 1;
    for (auto it=pattern.rbegin(); it!=pattern.rend(); it++)
    {
        number += nucl2int(*it)*mult;
        mult *= 4;
    }
    return number;
}

const kMerIterator& operator++(kMerIterator& it) 
{
    it.count++;
    return it;
}

const kMerIterator operator++(kMerIterator& i, int) 
{
    kMerIterator oldValue(i.count, i.k);
    i.count++;
    return oldValue;
}

bool operator==(const kMerIterator& left, const kMerIterator& right) {
    return left.count == right.count;
}

bool operator!=(const kMerIterator& left, const kMerIterator& right) {
    return left.count != right.count;
}

string operator*(const kMerIterator& it)
{
    return number2pattern(it.count, it.k);
}
