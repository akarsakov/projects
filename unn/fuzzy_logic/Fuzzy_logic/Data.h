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
	int class_column;

    std::vector<std::string> header;
    std::vector<std::vector<float>> data;
    std::map<int, std::string> class_map;
public:
    Data(std::string filename);
	Data(int numE, int numF, int cl_col);
    virtual ~Data() { };

    int getNumFeatures() { return (int) header.size(); };  
    int getNumExamples() { return (int) data.size(); };
    int getNumClasses() { return (int) class_map.size(); };

    float getFeatureValue(int example, int feature);
    std::vector<float> getExampleXVector(int example);
	float getExampleY(int example);
    std::string getClassName(int class_id);

    void normalize(float a, float b);
	void shuffleExamples(float percentPermutation);
	Data getSubset(std::vector<int> indexes);
};

#endif /*__DATA__*/