#ifndef NETDATA_H
#define NETDATA_H

#include <vector>

using std::vector;

///
/// typedef input and output data set type
///
typedef struct {
	float m_value;
} InputDataSet, OutputDataSet;

///
/// typedef train data set type
///
typedef struct {
	vector<InputDataSet> m_input;
	vector<OutputDataSet> m_output;
} TrainDataSet;

#endif /* NETDATA_H */