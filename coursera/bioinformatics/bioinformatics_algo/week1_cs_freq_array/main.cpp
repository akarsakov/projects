#include <iostream>
#include <fstream>
#include <string_processing.h>
#include <kMers_generator.h>
#include <DNA_processing.h>
#include <InfInt.h>

using namespace std;
using namespace bio;
using namespace bio::week1;

InfInt pattern2number_long(string pattern)
{
    InfInt number = 0, mult = 1;
    for (auto it=pattern.rbegin(); it!=pattern.rend(); it++)
    {
        number += (InfInt) nucl2int(*it)*mult;
        mult *= 4;
    }
    return number;
}

int main()
{
    InfInt s = pattern2number_long("CCATGGCCCTTAAATAAGGGTTCTCAACCGATGTTAATTTCTCAAGCAAGCCTGAG");
    cout << s << endl;

    return 0;
}