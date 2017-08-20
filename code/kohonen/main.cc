#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <cmath>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

class KohonenNetwork {
protected:

	///
	/// Input layer
	///
	int m_numberInputNeurons;
	float *m_inputNeurons;

	///
	/// Kohonen layer
	///
	int m_numberKohonenNeurons;
	float *m_kohonenNeurons;

	///
	/// Weights
	///
	float **m_weights;

	///
	/// Kohonen data
	///
	int m_winner;
	int m_neighborhoodSize;
	float m_alpha; // wNew = wOld + m_alpha * (Input - wOld);
	unsigned int m_cycles;
	float m_desiredDistance;
	int m_reducePeriod;

	//////////////////////////////////////////////////////////////
	///
	///
	///
	//////////////////////////////////////////////////////////////

	///
	/// Set random values of weights
	///
	void randomizeWeights() {

		srand((unsigned)time(0));

		for (int i = 0; i < m_numberInputNeurons; i++) {
			for (int j = 0; j < m_numberKohonenNeurons; j++) {
				m_weights[i][j] = 
					[]() -> float {
						int tmp = rand() % 100;
						return (float)tmp / 100.0f - 0.5f;
				} ();
			}
		}
	}

	///
	/// Initialize weights
	///
	void initializeWeights() {

		randomizeWeights();

		// normalize weights

		for (int i = 0; i < m_numberKohonenNeurons; i++) {

			float squareSum = 0.0f;

			for (int j = 0; j < m_numberInputNeurons; j++) {
				squareSum += m_weights[j][i] * m_weights[j][i];
			}

			squareSum = 1.0f / static_cast<float>(sqrt(static_cast<double>(squareSum)));

			for (int j = 0; j < m_numberInputNeurons; j++) {
				m_weights[j][i] *= squareSum;
			}
		}
	}

	float getWinnerDistance() {
		float distance = 0.0f;

		for (int i = 0; i < m_numberInputNeurons; i++)
			distance += (m_inputNeurons[i] - m_weights[i][m_winner]) * (m_inputNeurons[i] - m_weights[i][m_winner]);

		return static_cast<float>(sqrt(static_cast<double>(distance)));

	}

public:

	KohonenNetwork(int numberInputNeurons, int numberKohonenNeurons) {

		///
		/// Init input layer
		///
		m_numberInputNeurons = numberInputNeurons;
		m_inputNeurons = new float[m_numberInputNeurons];

		for (int i = 0; i < m_numberInputNeurons; i++) m_inputNeurons[i] = 0.0f;

		///
		/// Init kohonen layer
		///
		m_numberKohonenNeurons = numberKohonenNeurons;
		m_kohonenNeurons = new float[m_numberKohonenNeurons];

		for (int i = 0; i < m_numberKohonenNeurons; i++) m_kohonenNeurons[i] = 0.0f;

		///
		/// Init weights
		///
		m_weights = new float*[m_numberInputNeurons];
		for (int i = 0; i < m_numberInputNeurons; i++) m_weights[i] = new float[m_numberKohonenNeurons];

		initializeWeights();

		///
		/// Set default values
		///
		m_alpha = 1.0f;
		m_neighborhoodSize = 0;
		m_winner = -1;
		m_cycles = 50;
		m_desiredDistance = 1.0f;
		m_reducePeriod = 10;
	}

	~KohonenNetwork() {

		// delete input and activation data
		delete[] m_inputNeurons;
		delete[] m_kohonenNeurons;

		// delete weights
		for (int i = 0; i < m_numberInputNeurons; i++) delete[] m_weights[i];
		delete[] m_weights;

	}

	void setNeighborhoodSize(int size) {
		m_neighborhoodSize = size;
	}

	void setAlpha(float alpha) {
		m_alpha = alpha;
	}

	void setCycles(unsigned int cycles) {
		m_cycles = cycles;
	}

	void setDesiredDistance(float distance) {
		m_desiredDistance = distance;
	}

	vector<float> feed(vector<float> input) {

		if (input.size() != m_numberInputNeurons) {
			cout << " Invalid size of the input vector!" << endl;
			return vector<float>();
		}

		m_winner = -1;
		float winnersSum;

		for (int i = 0; i < m_numberInputNeurons; i++)
			m_inputNeurons[i] = input[i];

		for (int i = 0; i < m_numberKohonenNeurons; i++) {

			float sum = 0.0f;
			for (int j = 0; j < m_numberInputNeurons; j++)
				sum += m_inputNeurons[j] * m_weights[j][i];

			m_kohonenNeurons[i] = sum;

			if (m_winner < 0) {
				winnersSum = sum;
				m_winner = i;
			}
			else if (sum > winnersSum) {
				winnersSum = sum;
				m_winner = i;
			}
		}

		m_kohonenNeurons[m_winner] = 1.0f;

		for (int i = 0; i < m_numberKohonenNeurons; i++) {
			if (i == m_winner)
				continue;

			m_kohonenNeurons[i] = 0.0f;
		}

		vector<float> retVal = vector<float>();
		for (int i = 0; i < m_numberKohonenNeurons; i++) {
			retVal.push_back(m_kohonenNeurons[i]);
		}

		return retVal;
	}

	void updateWeights() {

		int startIndex = m_winner - m_neighborhoodSize;
		int stopIndex = m_winner + m_neighborhoodSize;

		if (startIndex < 0)
			startIndex = 0;

		if (stopIndex > m_numberKohonenNeurons)
			stopIndex = m_numberKohonenNeurons;

		for (int i = 0; i < m_numberInputNeurons; i++) {
			for (int j = startIndex; j < stopIndex; j++)
				m_weights[i][j] += 
					m_alpha * (m_inputNeurons[i] - m_weights[i][j]);
		}
	}

	void train(vector<vector<float>> patterns) {

		cout << "==========================================================================" << endl;
		cout << "                                    TRAINING                              " << endl;

		cout << " Network: " << endl;
		cout << "  > Inputs: " << m_numberInputNeurons << endl;
		cout << "  > Outputs: " << m_numberKohonenNeurons << endl;
		cout << "  > Desired distance: " << m_desiredDistance << endl;
		cout << "  > Max cycles: " << m_cycles << endl;
		cout << "  > Alpha: " << m_alpha << endl;
		cout << "  > Period: " << m_reducePeriod << endl;
		
		cout << " Number of patterns: " << patterns.size() << endl;

		cout << "==========================================================================" << endl;

		unsigned int cycle = 1;
		float distance = 10000.0f;

		while (cycle <= m_cycles && distance > m_desiredDistance) {

			distance = 0.0f;

			for each (auto pattern in patterns)
			{
				feed(pattern);
				distance += getWinnerDistance();

				updateWeights();
			}

			distance /= patterns.size();

			if (cycle % m_reducePeriod == 0) {
				if (m_alpha > 0.01f)
					m_alpha -= 0.1f;

				if (m_neighborhoodSize > 0)
					m_neighborhoodSize--;
			}

			cout << " Cycle #" << cycle << "\t Winner: " << m_winner << "\t Average distance: " << distance << endl;

			cycle++;
		}

		cout << endl;
		cout << " Train is complete!" << endl;
	}
};

void tmpFeed(vector<float> input, KohonenNetwork &net) {
	vector<float> ret = net.feed(input);

	for each (auto out in input)
	{
		cout << out << " ";
	}
	cout << "-> ";

	for each (auto out in ret)
	{
		cout << out << " ";
	}
	cout << endl;
}

int main() {

	KohonenNetwork net(10, 10);

	vector<float> p1 = vector<float>();
	p1.push_back(1.0f);
	p1.push_back(0.0f);
	p1.push_back(0.0f);
	p1.push_back(1.0f);
	p1.push_back(1.0f);
	p1.push_back(1.0f);
	p1.push_back(0.0f);
	p1.push_back(0.0f);
	p1.push_back(0.0f);
	p1.push_back(0.0f);

	vector<float> p2 = vector<float>();
	p2.push_back(1.0f);
	p2.push_back(0.0f);
	p2.push_back(1.0f);
	p2.push_back(0.0f);
	p2.push_back(1.0f);
	p2.push_back(0.0f);
	p2.push_back(1.0f);
	p2.push_back(0.0f);
	p2.push_back(1.0f);
	p2.push_back(0.0f);


	vector<vector<float>> patterns = vector<vector<float>>();
	patterns.push_back(p1);
	patterns.push_back(p2);

	net.setAlpha(0.9f);
	net.setDesiredDistance(0.00001f);
	net.setCycles(200);
	net.setNeighborhoodSize(2);
	net.train(patterns);

	cout << endl;
	cout << " Some tests:" << endl;
	
	tmpFeed(p1, net);
	tmpFeed(p2, net);

	vector<float> p3 = vector<float>();
	p3.push_back(0.0f);
	p3.push_back(0.0f);
	p3.push_back(1.0f);
	p3.push_back(0.0f);
	p3.push_back(1.0f);
	p3.push_back(0.0f);
	p3.push_back(0.0f);
	p3.push_back(0.0f);
	p3.push_back(0.0f);
	p3.push_back(0.0f);

	tmpFeed(p3, net);

	char c; cin >> c;
	return 0;
}