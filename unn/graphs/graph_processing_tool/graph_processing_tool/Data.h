#ifndef __DATA__
#define __DATA__

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iterator>
#include <boost\filesystem.hpp>

/*
 * Base class for data representation
 */
template <typename T>
class Data
{
private:
    int numFeatures;
    int numExamples;
    std::vector<std::string> featureNames;
    T* data;
public:
    Data(std::string filename, bool hasHeader=false);
	Data(size_t numF, size_t numE);
	virtual ~Data() { delete[] data; };

    inline size_t getNumFeatures() { return numFeatures; }
    inline size_t getNumExamples() { return numExamples; }

	std::vector<std::string> getFeatureNames() { return featureNames; };
	void setFeatureNames(std::vector<std::string> fNames) { featureNames = fNames; };

    T* getFeatureVector(size_t index) { return data + index*numExamples; };
	void setFeatureVector(size_t index, T* features);

    T getFeatureValue(size_t feature_index, size_t example_index) { return data[feature_index*numExamples + example_index]; };
	void setFeatureValue(size_t feature_index, size_t example_index, T value) { data[feature_index*numExamples + example_index] = value; };

	void writeToFile(std::string filename);
	void writeExampleToFile(std::string filename, size_t ex_index);

private:
	void openStreamFromPath(std::string filepath, std::ofstream& file);
};

template<typename S>
void Data<S>::openStreamFromPath(std::string filepath, std::ofstream& file)
{
	boost::filesystem::path path(filepath);
	boost::filesystem::path parent = path.parent_path();

	if(!(boost::filesystem::exists(parent)) && !parent.empty())
	{
        if (!boost::filesystem::create_directory(parent))
		{
			char error_msg[100];
			sprintf_s(error_msg, "Cannot create path - %s!", filepath.c_str());
			throw std::logic_error(error_msg);
		}
    }
	file.open(filepath, std::ifstream::out);
}

static std::string trim(std::string str)
{
	std::string res = str;
	res.erase(0, res.find_first_not_of(' '));
	return res;
}

template<typename T>
Data<T>::Data(std::string filename, bool hasHeader)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        char error_msg[100];
        sprintf_s(error_msg, "Cannot open file - %s!", filename.c_str());
		throw std::logic_error(error_msg);
    }

    // Dirty
    string line;
    if (getline(file, line))
    {
        stringstream line_stream(line);
        string name;
        
        if (hasHeader) {
            while (getline(line_stream, name, ','))
                featureNames.push_back(trim(name));
        } else {
            int index = 0;
            char buf[80];
            while (getline(line_stream, name, ',')) {
                itoa(index++, buf, 10);
                featureNames.push_back(std::string(buf));
            }
        }
    }
    numFeatures = (int) featureNames.size();

    int numLines = hasHeader ? 0 : 1;
    while (getline(file, line))
        ++numLines;
    numExamples = numLines;

    data = new T[numExamples*numFeatures];
	
	// read file again
	file.close();
	file.open(filename);
    getline(file, line);

	for (int i=0; i<numExamples; i++)
	{
		getline(file, line);
		stringstream line_stream(line);
		string val;
		for (int j=0; j<numFeatures; j++)
		{
			getline(line_stream, val, ',');
			data[j*numExamples + i] = (T) atof(val.c_str());
		}
	}
}

template<typename T>
Data<T>::Data(size_t numF, size_t numE): numFeatures(numF), numExamples(numE)
{
	data = new T[numExamples*numFeatures];
	std::fill(data, data + numExamples*numFeatures, (T) 0.0);
}

template<typename T>
void Data<T>::setFeatureVector(size_t index, T* features)
{
	T* ptr = data + index*numExamples;
#pragma warning(push) 
#pragma warning(disable:4996) 
	std::copy(features, features + numFeatures, ptr);
#pragma warning(pop) 
}

template<typename T>
void Data<T>::writeToFile(std::string filename)
{
	ofstream file;
	openStreamFromPath(filename, file);
	if (!file.is_open())
    {
        char error_msg[100];
        sprintf_s(error_msg, "Cannot open file - %s!", filename.c_str());
		cout << error_msg << endl;
		throw std::logic_error(error_msg);
    }

	// create header
	if (featureNames.size() > 0)
	{
		for (int i=0; i<numFeatures; i++)
		{
			if (i == 0) file << featureNames[i];
			else {
				file << ", " << featureNames[i];
			}
		}
		file << "\n";
	}

	// write data
	for (int i=0; i<numExamples; i++)
	{
		for (int j=0; j<numFeatures; j++)
		{
			if (j == 0) file << getFeatureValue(j, i);
			else {
				file << ", " << getFeatureValue(j, i);
			}
		}
		file << "\n";
	}
	file.close();
}

template<typename T>
void Data<T>::writeExampleToFile(std::string filename, size_t ex_index)
{
	ofstream file;
	if (ex_index != 0)
		file.open(filename, std::ifstream::app);
	else
		openStreamFromPath(filename, file);

	if (!file.is_open())
    {
        char error_msg[100];
        sprintf_s(error_msg, "Cannot open file - %s!", filename.c_str());
		throw std::logic_error(error_msg);
    }

	if (ex_index == 0)
	{
		// create header
		if (featureNames.size() > 0)
		{
			for (int i=0; i<numFeatures; i++)
			{
				if (i == 0) file << featureNames[i];
				else {
					file << ", " << featureNames[i];
				}
			}
			file << "\n";
		}
	}

	for (int j=0; j<numFeatures; j++)
	{
		if (j == 0) file << getFeatureValue(j, ex_index);
		else {
			file << ", " << getFeatureValue(j, ex_index);
		}
	}
	file << "\n";

	file.close();
}

#endif /*__DATA__*/