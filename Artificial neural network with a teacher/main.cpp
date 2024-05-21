#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>

using namespace std;

//Функция активации (сигмоидальная функция)
double sigmoid(double x) {
	return 1 / (1 + exp(-x));
}

//Производная функции активации
double sigmoidDerivative(double x) {
	return x * (1 - x);
}

class Neuron {
public:
	double input;
	double output;
	double delta;
	vector<double> weights;

	Neuron() {
		input = 0.0;
		output = 0.0;
		delta = 0.0;
	}

	void initializeWeights(int numInputs) {
		for (int i = 0; i < numInputs; i++) {
			weights.push_back((double)rand() / RAND_MAX);
		}
	}
};

class Layer {
public:
	vector<Neuron> neurons;

	Layer(int numNeurons, int numInputsPerNeuron) {
		for (int i = 0; i < numNeurons; i++) {
			if (numInputsPerNeuron == 0) {
				numInputsPerNeuron = 2;
			}
			Neuron new_neuron;
			new_neuron.initializeWeights(numInputsPerNeuron);
			neurons.push_back(new_neuron);
		}
	}
};

class NeuralNetwork {
public:
	vector<Layer> layers;
	double learningRate;

	NeuralNetwork(vector<int> sizes, double lr) {
		learningRate = lr;

		for (int i = 0; i < sizes.size(); i++) {
			if (i == 0) {
				layers.push_back(Layer(sizes[i], 0));
			}
			else {
				layers.push_back(Layer(sizes[i], sizes[i - 1]));
			}
		}
	}

	//Прямое распространение сигналов
	void forwardPropagate(vector<double>& input) {
		//Задает выходные значения входного слоя
		for (int i = 0; i < input.size(); i++) {
			layers[0].neurons[i].output = input[i] * layers[0].neurons[i].weights[i];
		}

		//Вычисляет выходные значения каждого последующего слоя
		for (int i = 1; i < layers.size(); i++) {
			for (int j = 0; j < layers[i].neurons.size(); j++) {
				double sum = 0.0;

				//Суммирует взвешенные выходы предыдущего слоя
				for (int k = 0; k < layers[i - 1].neurons.size(); k++) {
					sum += layers[i - 1].neurons[k].output * layers[i].neurons[j].weights[k];
				}

				//Применяет функцию активации к сумме
				layers[i].neurons[j].output = sigmoid(sum);
			}
		}
	}

	//Обратное распространение ошибки
	void backwardPropagate(vector<double>& target) {
		for (int i = 0; i < target.size(); i++) {
			//Ошибка выходного слоя = разница между реальным и ожидаемым выходом
			layers.back().neurons[i].delta = (target[i] - layers.back().neurons[i].output) * sigmoidDerivative(layers.back().neurons[i].output);
		}

		for (int i = layers.size() - 2; i >= 0; i--) {
			for (int j = 0; j < layers[i].neurons.size(); j++) {
				double error = 0.0;
				for (int k = 0; k < layers[i + 1].neurons.size(); k++) {
					//Ошибка скрытого слоя = сумма взвешенных ошибок следующего слоя
					error += layers[i + 1].neurons[k].delta * layers[i + 1].neurons[k].weights[j];
				}
				layers[i].neurons[j].delta = error * sigmoidDerivative(layers[i].neurons[j].output);
			}
		}

		//Обновляет веса
		for (int i = 1; i < layers.size(); i++) {
			for (int j = 0; j < layers[i].neurons.size(); j++) {
				for (int k = 0; k < layers[i - 1].neurons.size(); k++) {
					//Корректирует веса с учетом ошибки и скорости обучения
					double weight_change = learningRate * layers[i].neurons[j].delta * layers[i - 1].neurons[k].output;
					layers[i].neurons[j].weights[k] += weight_change;
				}
			}
		}
	}

	void train(vector<vector<double>> input, vector<vector<double>> output, int epochs) {
		for (int epochs = 0; epochs < 10000; epochs++) {
			double error = 0.0;
			for (int i = 0; i < input.size(); i++) {
				forwardPropagate(input[i]);
				error += calculateMSE(output[i]);
				backwardPropagate(output[i]);
			}

			cout << "Epoch: " << epochs << " Error: " << error << endl;
		}
	}

	//Вычисление ошибки по среднеквадратичному отклонению
	double calculateMSE(vector<double> target) {
		double error = 0;

		//Суммирует квадраты разностей между реальным и ожидаемым выходом
		for (int i = 0; i < target.size(); i++) {
			error += pow(target[i] - layers.back().neurons[i].output, 2);
		}

		return error / 2;
	}
};

int main() {
	srand(time(NULL));

	vector<int> sizes = { 2, 3, 1 };
	NeuralNetwork network(sizes, 0.3);

	vector<vector<double>> input = { {0.0, 0.0}, {0.0, 1.0}, {1.0, 0.0}, {1.0, 1.0} };
	vector<vector<double>> output = { {0.0}, {1.0}, {1.0}, {0.0} };



	network.train(input, output, 10000);

	for (int i = 0; i < input.size(); i++) {
		network.forwardPropagate(input[i]);
		cout << "Input: " << input[i][0] << ", " << input[i][1] << " ";
		cout << "Predicted Output: " << network.layers.back().neurons[0].output << " " << endl;
	}

	return 0;
}