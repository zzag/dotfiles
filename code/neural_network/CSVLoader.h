#ifndef CSVLOADER_H
#define CSVLOADER_H

#include <iostream>
#include <fstream>
#include <string>
#include "NetData.h"

#define _CRT_SECURE_NO_WARNINGS 0

using std::cout;
using std::endl;
using std::string;
using std::fstream;
using std::ios;
using std::getline;

void parseline(vector<TrainDataSet> &data, string line, int numInput, int numOutput) {

	float *inputs = new float[numInput];
	float *outputs = new float[numOutput];

	char *cStr = new char[line.size() + 1];
	strcpy(cStr, line.c_str());

	char *tmp;
	int index = 0;

	tmp = strtok(cStr, ",");

	while (tmp != NULL && index < numInput + numOutput) {
		if (index < numInput) inputs[index] = static_cast<float>(atof(tmp));
		else outputs[index - numInput] = static_cast<float>(atof(tmp));

		tmp = strtok(NULL, ",");
		index++;
	}

	// write to train data set
	TrainDataSet td;
	
	

	for (int i = 0; i < numInput; i++) {
		InputDataSet ids;
		ids.m_value = inputs[i];

		td.m_input.push_back(ids);
	}

	for (int i = 0; i < numOutput; i++) {
		OutputDataSet ods;
		ods.m_value = outputs[i];

		td.m_output.push_back(ods);
	}

	data.push_back(td);

	delete[] inputs;
	delete[] outputs;
}

vector<TrainDataSet> loadTrainDataSet(const char *filename, int numInput, int numOutput) {

	vector<TrainDataSet> retVal = vector<TrainDataSet>();

	// open file
	fstream file;

	file.open(filename, ios::in);

	if (!file.is_open()) {
		cout << " Path " << filename << " is invalid!" << endl;
		return vector<TrainDataSet>();
	}

	// read file
	while (!file.eof()) {
		string line = "";

		// get line
		getline(file, line);

		// parse line
		parseline(retVal, line, numInput, numOutput);
	}

	file.close();

	return retVal;
}

#endif /* CSVLOADER_H */