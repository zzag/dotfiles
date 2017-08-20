#include <iostream>
#include <vector>
#include <utility>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::pair;

class Neuron {
protected:

	float m_activation;
	float m_output;
	vector<float> m_weights;

	friend class NeuralNetwork;

	float threshold(float x) {
		if (x < 0)
			return -1.0f;
		else
			return 1.0f;
	}

public:

	Neuron(vector<float> &weights) {

		///
		/// Init activation and output value
		///
		m_activation = 0.0f;
		m_output = 0.0f;

		///
		/// Init weights
		///
		for (auto tmp : weights) m_weights.push_back(tmp);
		
	}

	float feed(vector<float> &input) {

		if (input.size() != m_weights.size()) {
			cout << "Invalid size of the input vector!" << endl;
			return 0.0f;
		}

		m_activation = 0.0f;
		for (unsigned int i = 0; i < input.size(); i++) {
			m_activation += input[i] * m_weights[i];
		}

		m_output = threshold(m_activation);

		return m_output;
	}

};

class NeuralNetwork {
protected:

	///
	/// Layer A
	///
	vector<Neuron> m_fieldANeurons;

	///
	/// Layer B
	///
	vector<Neuron> m_fieldBNeurons;

public:

	enum field {
		A, B
	};

	NeuralNetwork(vector<float> &inputVectorA, vector<float> &inputVectorB) {

		if (!inputVectorA.size() || !inputVectorB.size()) {
			cout << "One from input vectors is empty!" << endl;
			exit(EXIT_FAILURE);
		}

		// get matrices
		vector<vector<float>> originalMatrix = vector<vector<float>>();
		for (unsigned int i = 0; i < inputVectorA.size(); i++) originalMatrix.push_back(vector<float>());
		for (vector<float> &tmp : originalMatrix) {
			for (unsigned int i = 0; i < inputVectorB.size(); i++) tmp.push_back(0.0f);
		}

		vector<vector<float>> transposeMatrix = vector<vector<float>>();
		for (unsigned int i = 0; i < inputVectorB.size(); i++) transposeMatrix.push_back(vector<float>());
		for (vector<float> &tmp : transposeMatrix) {
			for (unsigned int i = 0; i < inputVectorA.size(); i++) tmp.push_back(0.0f);
		}

		// multiply
		for (unsigned int i = 0; i < inputVectorA.size(); i++) {
			for (unsigned int j = 0; j < inputVectorB.size(); j++)
				originalMatrix[i][j] = inputVectorA[i] * inputVectorB[j];
		}

		// transpose
		for (unsigned int i = 0; i < inputVectorA.size(); i++) {
			for (unsigned int j = 0; j < inputVectorB.size(); j++)
				transposeMatrix[j][i] = originalMatrix[i][j];
		}

		///
		/// create neurons
		///

		// field A
		for (unsigned int i = 0; i < inputVectorA.size(); i++) {
			Neuron tmp = Neuron(originalMatrix[i]);
			m_fieldANeurons.push_back(tmp);
		}

		// field B
		for (unsigned int i = 0; i < inputVectorB.size(); i++) {
			Neuron tmp = Neuron(transposeMatrix[i]);
			m_fieldBNeurons.push_back(tmp);
		}
	}

	~NeuralNetwork() {
		m_fieldANeurons.clear();
		m_fieldBNeurons.clear();
	}

	vector<float> feed(int field, vector<float> &input) {

		if (field == A) {
			vector<float> retVal = vector<float>();

			for (auto &neuron : m_fieldBNeurons) {
				retVal.push_back(neuron.feed(input));
			}

			return retVal;
		}
		else if (field == B) {
			vector<float> retVal = vector<float>();

			for (auto &neuron : m_fieldANeurons) {
				retVal.push_back(neuron.feed(input));
			}

			return retVal;
		} else
			return vector<float>();
	}
};

int main() {

	vector<float> inputA = vector<float>();
	inputA.push_back(1.0f);
	inputA.push_back(-1.0f);
	inputA.push_back(-1.0f);
	inputA.push_back(1.0f);
	inputA.push_back(1.0f);

	vector<float> inputB = vector<float>();
	inputB.push_back(-1.0f);
	inputB.push_back(1.0f);
	inputB.push_back(-1.0f);

	NeuralNetwork net(inputA, inputB);

	/////////////////////////////////////////////////////////
	///
	///
	///
	/////////////////////////////////////////////////////////

	vector<float> inputAA = vector<float>();
	inputAA.push_back(1.0f);
	inputAA.push_back(-1.0f);
	inputAA.push_back(-1.0f);
	inputAA.push_back(1.0f);
	inputAA.push_back(1.0f);

	vector<float> inputBB = vector<float>();
	inputBB.push_back(-1.0f);
	inputBB.push_back(1.0f);
	inputBB.push_back(-1.0f);

	vector<float> out = net.feed(NeuralNetwork::A, inputAA);

	cout << "Test: ";
	for (auto tmp : inputAA) cout << tmp << " ";
	cout << "-> ";

	for (auto tmp : out) cout << tmp << " ";
	cout << endl;

	vector<float> out1 = net.feed(NeuralNetwork::B, inputBB);

	cout << "Test: ";
	for (auto tmp : inputBB) cout << tmp << " ";
	cout << "-> ";

	for (auto tmp : out1) cout << tmp << " ";
	cout << endl;

	char c; cin >> c;
	return 0;
}