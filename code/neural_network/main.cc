#include <iostream>
#include <cmath>
#include <time.h>
#include <vector>
#include "NetData.h"
#include "CSVLoader.h"

using std::cin;
using std::cout;
using std::endl;
using std::vector;

class NeuralNetwork {

protected:

	///
	/// Input layer
	///
	int m_numberInputNeurons;
	float *m_inputNeurons;

	///
	/// Hidden Layer
	///
	int m_numberHiddenNeurons;
	float *m_hiddenNeurons;

	///
	/// Output layer
	///
	int m_numberOutputNeurons;
	float *m_outputNeurons;

	///
	/// Weights
	///
	float **m_inputHiddenWeights;
	float **m_hiddenOutputWeights;

	///
	/// Train data
	///
	float m_learningRate;
	float m_momentum;
	float m_desiredAccuracy;
	unsigned m_maxEpochs;
	float *m_hiddenErrorGradients;
	float *m_outputErrorGradients;
	float **m_inputHiddenDeltaWeights;
	float **m_hiddenOutputDeltaWeights;

	/////////////////////////////////////////////////////////
	///
	/// 
	///
	/////////////////////////////////////////////////////////

	// randomize all weights
	void randomizeWeights() {

		srand((unsigned)time(0));

		// input - hidden layer
		for (int i = 0; i < m_numberInputNeurons; i++) {
			for (int j = 0; j < m_numberHiddenNeurons; j++) {
				m_inputHiddenWeights[i][j] = []() -> float {
					int tmp = rand() % 100;
					return (float(tmp) / 100.0f - 0.5f);
				}();

				m_inputHiddenDeltaWeights[i][j] = 0.0f;
			}
		}

		// hidden - output layer
		for (int i = 0; i < m_numberHiddenNeurons; i++) {
			for (int j = 0; j < m_numberOutputNeurons; j++) {
				m_hiddenOutputWeights[i][j] = []() -> float {
					int tmp = rand() % 100;
					return (float(tmp) / 100.0f - 0.5f);
				}();

				m_hiddenOutputDeltaWeights[i][j] = 0.0f;
			}
		}
	}

	// activation function
	float activationFunction(float x) {
		return 1.0f / (1.0f + exp(-x));
	}

	float getRoundedValue(float x) {
		if (x < 0.1f)
			return 0.0f;
		else if (x > 0.9f)
			return 1.0f;
		else
			return -1.0f;
	}

	// get accuracy
	float getAccuracy(vector<TrainDataSet> dataSet) {

		int countIncorrectResults = 0;

		for (unsigned int i = 0; i < dataSet.size(); i++) {

			// feed data
			feed(dataSet[i].m_input);

			// check
			for (int j = 0; j < m_numberOutputNeurons; j++) {
				if (getRoundedValue(m_outputNeurons[j]) != dataSet[i].m_output[j].m_value) {
					countIncorrectResults++;
					break;
				}
			}
		}

		return 100.0f - float(countIncorrectResults * 100.0f / dataSet.size());
	}

	// apply deltas
	void updateWeights() {

		// input - hidden
		for (int i = 0; i < m_numberInputNeurons; i++) {
			for (int j = 0; j < m_numberHiddenNeurons; j++)
				m_inputHiddenWeights[i][j] += m_inputHiddenDeltaWeights[i][j];
		}

		// hidden - output
		for (int i = 0; i < m_numberHiddenNeurons; i++) {
			for (int j = 0; j < m_numberOutputNeurons; j++)
				m_hiddenOutputWeights[i][j] += m_hiddenOutputDeltaWeights[i][j];
		}
	}

	// get output error gradient
	float getOutputErrorGradient(float desiredValue, float outputValue) {
		return outputValue * (1.0f - outputValue) * (desiredValue - outputValue);
	}

	// get hidden error gradient
	float getHiddenErrorGradient(int neuron) {
		float weightSum = 0.0f;

		for (int i = 0; i < m_numberOutputNeurons; i++) weightSum += m_hiddenOutputWeights[neuron][i] * m_outputErrorGradients[i];

		return m_hiddenNeurons[neuron] * (1.0f - m_hiddenNeurons[neuron]) * weightSum;
	}

	// backpropagate
	void backpropagate(vector<OutputDataSet> expectedValues) {

		// set delta values between hidden - output layer
		for (int i = 0; i < m_numberOutputNeurons; i++) {
			m_outputErrorGradients[i] = getOutputErrorGradient(expectedValues[i].m_value, m_outputNeurons[i]);

			for (int j = 0; j < m_numberHiddenNeurons; j++)
				m_hiddenOutputDeltaWeights[j][i] = m_learningRate * m_hiddenNeurons[j] * m_outputErrorGradients[i] +
					m_momentum * m_hiddenOutputDeltaWeights[j][i];
		}

		// set delta values between input - hidden layer
		for (int i = 0; i < m_numberHiddenNeurons; i++) {
			m_hiddenErrorGradients[i] = getHiddenErrorGradient(i);

			for (int j = 0; j < m_numberInputNeurons; j++)
				m_inputHiddenDeltaWeights[j][i] = m_learningRate * m_inputNeurons[j] * m_hiddenErrorGradients[i] + 
					m_momentum * m_inputHiddenDeltaWeights[j][i];
		}

		// update weights
		updateWeights();
	}

public:

	NeuralNetwork(
		int numberInputNeurons, int numberHiddenNeurons, int numberOutputNeurons) {

		// init input layer
		m_numberInputNeurons = numberInputNeurons;
		m_inputNeurons = new float[m_numberInputNeurons];

		for (int i = 0; i < m_numberInputNeurons; i++) m_inputNeurons[i] = 0.0f;

		// init hidden layer
		m_numberHiddenNeurons = numberHiddenNeurons;
		m_hiddenNeurons = new float[m_numberHiddenNeurons];

		for (int i = 0; i < m_numberHiddenNeurons; i++) m_hiddenNeurons[i] = 0.0f;

		// init output layer
		m_numberOutputNeurons = numberOutputNeurons;
		m_outputNeurons = new float[m_numberOutputNeurons];

		for (int i = 0; i < m_numberOutputNeurons; i++) m_outputNeurons[i] = 0.0f;

		// init weights

		m_inputHiddenWeights = new float*[m_numberInputNeurons];
		m_inputHiddenDeltaWeights = new float*[m_numberInputNeurons];

		for (int i = 0; i < m_numberInputNeurons; i++) {
			m_inputHiddenWeights[i] = new float[m_numberHiddenNeurons];
			m_inputHiddenDeltaWeights[i] = new float[m_numberHiddenNeurons];
		}

		m_hiddenOutputWeights = new float*[m_numberHiddenNeurons];
		m_hiddenOutputDeltaWeights = new float*[m_numberHiddenNeurons];

		for (int i = 0; i < m_numberHiddenNeurons; i++) {
			m_hiddenOutputWeights[i] = new float[m_numberOutputNeurons];
			m_hiddenOutputDeltaWeights[i] = new float[m_numberOutputNeurons];
		}

		randomizeWeights();

		// init error gradients

		m_hiddenErrorGradients = new float[m_numberHiddenNeurons];
		for (int i = 0; i < m_numberHiddenNeurons; i++) m_hiddenErrorGradients[i] = 0.0f;

		m_outputErrorGradients = new float[m_numberOutputNeurons];
		for (int i = 0; i < m_numberOutputNeurons; i++) m_outputErrorGradients[i] = 0.0f;

		// init training data
		m_learningRate = 1.0f;
		m_momentum = 1.0f;
		m_desiredAccuracy = 50.0f;
		m_maxEpochs = 100;
	}

	~NeuralNetwork() {

		// delete layers
		delete[] m_inputNeurons;
		delete[] m_hiddenNeurons;
		delete[] m_outputNeurons;

		// delete weights
		for (int i = 0; i < m_numberInputNeurons; i++) delete[] m_inputHiddenWeights[i];
		delete[] m_inputHiddenWeights;

		for (int i = 0; i < m_numberHiddenNeurons; i++) delete[] m_hiddenOutputWeights[i];
		delete[] m_hiddenOutputWeights;

		// delete deltas
		for (int i = 0; i < m_numberInputNeurons; i++) delete[] m_inputHiddenDeltaWeights[i];
		delete[] m_inputHiddenDeltaWeights;

		for (int i = 0; i < m_numberHiddenNeurons; i++) delete[] m_hiddenOutputDeltaWeights[i];
		delete[] m_hiddenOutputDeltaWeights;

		// delete error gradients
		delete[] m_hiddenErrorGradients;
		delete[] m_outputErrorGradients;
	}

	void setDesiredAccuracy(float accuracy) {
		if (accuracy <= 0.0f)
			return;

		m_desiredAccuracy = accuracy;
	}

	void setMaxEpochs(unsigned epochs) {
		m_maxEpochs = epochs;
	}

	void setTrainConstants(float learningRate, float momentum) {
		m_learningRate = learningRate;
		m_momentum = momentum;
	}

	vector<OutputDataSet> feed(vector<InputDataSet> input) {

		// check size
		if (input.size() != m_numberInputNeurons) {
			cout << "Input vector size is not equal to number of input neurons: " << 
				input.size() << " != " << m_numberInputNeurons << endl;
			return vector<OutputDataSet>();
		}

		// feed input layer
		for (int i = 0; i < m_numberInputNeurons; i++) m_inputNeurons[i] = input[i].m_value;

		// feed hidden layer
		for (int i = 0; i < m_numberHiddenNeurons; i++) {
			float sum = 0.0f;

			for (int j = 0; j < m_numberInputNeurons; j++) sum += m_inputNeurons[j] * m_inputHiddenWeights[j][i];

			m_hiddenNeurons[i] = activationFunction(sum);
		}

		// feed output layer
		for (int i = 0; i < m_numberOutputNeurons; i++) {
			float sum = 0.0f;

			for (int j = 0; j < m_numberHiddenNeurons; j++) sum += m_hiddenNeurons[j] * m_hiddenOutputWeights[j][i];

			m_outputNeurons[i] = activationFunction(sum);
		}

		vector<OutputDataSet> retVal = vector<OutputDataSet>();

		for (int i = 0; i < m_numberOutputNeurons; i++) {
			OutputDataSet tmp;
			tmp.m_value = m_outputNeurons[i];

			retVal.push_back(tmp);
		}

		return retVal;
	}

	void train(vector<TrainDataSet> trainData) {

		///
		/// Print some info
		///
		cout << "===============================================================================" << endl;
		cout << "                                    TRAINING                                   " << endl;
		cout << " Network configuration: " << endl;
		cout << "  > Layers: " << m_numberInputNeurons << " - " << m_numberHiddenNeurons << " - " << m_numberOutputNeurons << endl;
		cout << "  > Learning rate: " << m_learningRate << endl;
		cout << "  > Momentum: " << m_momentum << endl;
		cout << "  > Max epochs: " << m_maxEpochs << endl;
		cout << "  > Desired accuracy: " << m_desiredAccuracy << endl;
		cout << "  > Count patterns: " << trainData.size() << endl;
		cout << "===============================================================================" << endl;

		unsigned epoch = 1;
		float accuracy = getAccuracy(trainData);
		float previousAccuracy = accuracy;

		cout << " Initial accuracy: " << accuracy << endl;

		while (epoch <= m_maxEpochs && accuracy < m_desiredAccuracy) {

			for each (auto pattern in trainData)
			{
				feed(pattern.m_input);
				backpropagate(pattern.m_output);
			}

			// get accuracy
			accuracy = getAccuracy(trainData);

			// output
			if (ceil(accuracy) != ceil(previousAccuracy))
				cout << " Epoch #" << epoch << "\tAccuracy: " << accuracy << "%" << endl;

			previousAccuracy = accuracy;

			epoch++;
		}

		cout << endl;
		cout << " Training is complete!" << endl;
		cout << " Count epochs: " << epoch - 1 << endl;
		cout << " Final accuracy: " << accuracy << "%" << endl;
	}

};

int main() {

	cout << " Loading train data..." << endl;
	vector<TrainDataSet> trainData = loadTrainDataSet("train-data.csv", 16, 1);

	NeuralNetwork net(16, 8, 1);

	net.setDesiredAccuracy(100.0f);
	net.setTrainConstants(0.01f, 0.05f);
	net.train(trainData);

	char stop; cin >> stop;
	return 0;
}