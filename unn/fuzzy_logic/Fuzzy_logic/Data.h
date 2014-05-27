#ifndef __DATA__
#define __DATA__

#include <string>
#include <vector>
#include <map>

class Data
{
private:
    int numFeatures;
    int numExamples;

    std::vector<std::string> header;
    std::vector<std::vector<float>> data;
    std::map<int, std::string> class_map;
public:
    Data(std::string filename);
    virtual ~Data() { };

    int getNumFeatures() { return (int) header.size(); };
    int getNumExamples() { return (int) data.size(); };
    int getNumClasses() { return (int) class_map.size(); };

    float getFeatureValue(int example, int feature);
    std::vector<float> getExampleVector(int example);
    std::string getClassName(int class_id);
};

#endif /*__DATA__*/