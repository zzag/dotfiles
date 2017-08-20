#include <cstdio>
#include <cmath>
#include <iostream>

using std::cout;
using std::endl;

class Network;

class Neuron {
protected:
	double activation;
	friend class Network;

public:
	double weights[4];

	Neuron() {}
	Neuron(double *w) {
		for (int index = 0; index < 4; index++)
			weights[index] = w[index];
	}

	double activate(int countInputValues, double *values) {
		double tmp = 0;

		for (int index = 0; index < countInputValues; index++)
			tmp += values[index] * weights[index];

		return tmp;
	}
};

class Network {
public:
	Neuron neurons[4];
	double output[4];

	Network(double *weightsA, 
		double *weightsB, 
		double *weightsC, 
		double *weightsD) {
		neurons[0] = Neuron(weightsA);
		neurons[1] = Neuron(weightsB);
		neurons[2] = Neuron(weightsC);
		neurons[3] = Neuron(weightsD);
	}

	int threshold(int val) { 

		if (val >= 0)
			return 1;

		return 0;
	}

	void activate(double *pattern) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++)
				cout << "Neuron " << i << " Weight[" << j << "] = " << neurons[i].weights[j] << endl;

			neurons[i].activation = neurons[i].activate(4, pattern);
			output[i] = threshold(neurons[i].activation);

			cout << "Neuron " << i << " Activation value: " <<
				neurons[i].activation << " Output value: " <<
				output[i] << endl;
		}
	}
};

int main() {

	double pattern[] = {0, 0, 1, 0};
	double weigthsA[] = {0, -1, -1, 1};
	double weightsB[] = {-1, 0, 1, -1};
	double weightsC[] = {-1, 1, 0, -1};
	double weightsD[] = {1, -1, -1, 0};

	Network net(weigthsA, weightsB, weightsC, weightsD);
	net.activate(pattern);

	return 0;
}