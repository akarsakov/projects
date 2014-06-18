#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <time.h>

#include "Data.h"
#include "utils.h"

using namespace std;

Data::Data(string filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        stringstream msg;
        msg << "Can't open file '" << filename << "'";
        Error(msg.str());
    }

    // read header
	class_column = -1;
    string line;
    if (getline(file, line))
    {
        stringstream line_stream(line);
        string name;
        
        while (getline(line_stream, name, ','))
        {
            if (name == "class")
                class_column = header.size();
            header.push_back(name);
        }
    } else
    {
        stringstream msg;
        msg << "Empty file '" << filename << "'";
        Error(msg.str());
    }

    int current_class_id = 0;
    while (getline(file, line))
    {
        stringstream line_stream(line);
        string value;
        vector<float> example;
        int index = 0;
        while (getline(line_stream, value, ','))
        {
            if (index != class_column)
            {
                if (value == "?")
                    example.push_back(-1.f);
                else
                    example.push_back( (float) atof(value.c_str()));
            }
            else
            {
                int class_id = -1;
                for (auto it=class_map.begin(); it!=class_map.end(); it++)
                {
                    if (it->second == value)
                    {
                        class_id = it->first;
                        break;
                    }
                }
                if (class_id < 0)
                {
                    class_id = current_class_id++;
                    class_map[class_id] = value;
                }
                example.push_back((float) class_id);
            }
            index++;
        }
        if (example.size() != header.size())
        {
            stringstream msg;
            msg << "Incorrect line " << data.size() + 1 << " in '" << filename;
            Error(msg.str());
        }
        data.push_back(example);
    }

    file.close();

    cout << "Summary (" << filename << "): " << endl;
    cout << "Number of examples: " << data.size() << endl;
    cout << "Number of features: " << header.size() << endl;
    cout << "Number of classes: " << class_map.size() << endl;
}

Data::Data(int numE, int numF, int cl_col): numExamples(numE), numFeatures(numF), class_column(cl_col)
{
}

float Data::getFeatureValue(int example, int feature)
{
    if (example < 0 || example >= getNumExamples() || feature < 0 || feature >= getNumFeatures())
    {
        stringstream msg;
        msg << "Incorrect indexes (" << example << "," << feature << ")";
        Error(msg.str());
    }
    return data[example][feature];
}

std::vector<float> Data::getExampleXVector(int example)
{
    if (example < 0 || example >= getNumExamples())
    {
        stringstream msg;
        msg << "Incorrect index (" << example << ")";
        Error(msg.str());
    }
	vector<float> res = data[example];
	if (class_column >= 0)
		res.erase(res.begin() + class_column);
    return res;
}

float Data::getExampleY(int example)
{
	return data[example][class_column];
}

string Data::getClassName(int class_id)
{
    auto f = class_map.find(class_id);
    if (f == class_map.end())
    {
        stringstream msg;
        msg << "Incorrect class id: " << class_id;
        Error(msg.str());
    }
    return f->second;
}

void Data::normalize(float a, float b)
{
    for (int i=0; i<getNumFeatures(); i++)
    {
        if (header[i] != "class")
        {
            float min_value = FLT_MAX;
            float max_value = FLT_MIN;
            for (int j=0; j<getNumExamples(); j++)
            {
                if (data[j][i] < 0)
                    continue;
                min_value = min(min_value, data[j][i]);
                max_value = max(max_value, data[j][i]);
            }

            for (int j=0; j<getNumExamples(); j++)
            {
                if (data[j][i] < 0)
                    continue;
                data[j][i] = a + (data[j][i] - min_value)*(b - a) / (max_value - min_value);
            }
        }
    }
}

void Data::shuffleExamples(float percentPermutation)
{
	srand((unsigned int) time(NULL));
	int numPermutations = (int) (getNumExamples()*percentPermutation);
    int numExamples = getNumExamples();
	for (int i=0; i<numPermutations; i++)
	{
        int first = rand()*numExamples/RAND_MAX;
		int second = rand()*numExamples/RAND_MAX;
		std::swap(data[first], data[second]);
	}
}

Data Data::getSubset(vector<int> indexes)
{
	int numExamples = indexes.size();
	Data result(numExamples, numFeatures, class_column);
    for (auto it=indexes.begin(); it!=indexes.end(); it++)
    {
        result.data.push_back(data[*it]);
    }
	result.class_map = class_map;
	result.header = header;
	return result;
}