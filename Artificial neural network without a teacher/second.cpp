#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>
 
using namespace std;
 
class Neuron {
public:
    vector<double> weights;
 
    Neuron(int inputSize) {
        for (int i = 0; i < inputSize; i++) {
            weights.push_back((double)rand() / RAND_MAX * 2 - 1);
        }
    }
 
    //Вычисление расстояния между входом и весами (Евклидово расстояние)
    double calculateDistance(vector<double>& input) {
        double sum = 0.0;
 
        for (int i = 0; i < input.size(); i++) {
            sum += pow(input[i] - weights[i], 2);
        }
 
        return sqrt(sum);
    }
 
    //Обновление весов нейрона
    void updateWeights(vector<double>& input, double learningRate, vector<double>& winnerPosition) {
        for (int i = 0; i < weights.size(); i++) {
            //Вычисление значения функции окрестности и обновление весов
            double neighborhood = exp(-pow(winnerPosition[0] - i, 2) / (2 * pow(winnerPosition[1], 2)));
            weights[i] += learningRate * neighborhood * (input[i] - weights[i]);
        }
    }
 
    vector<double> getWeights() {
        return weights;
    }
};
 
class KohonenNetwork {
public:
    vector<Neuron> neurons;
 
    KohonenNetwork(int inputSize, int outputSize) {
        for (int i = 0; i < outputSize; i++) {
            neurons.push_back(Neuron(inputSize));
        }
    }
 
    //Поиск выигравшего нейрона с минимальным расстоянием до входных данных
    int findBestNeuron(vector<double>& input) {
        int bestNeuronIndex = 0;
        double minDistance = neurons[0].calculateDistance(input);
 
        for (int i = 1; i < neurons.size(); i++) {
            double distance = neurons[i].calculateDistance(input);
            if (distance < minDistance) {
                minDistance = distance;
                bestNeuronIndex = i;
            }
        }
 
        return bestNeuronIndex;
    }
 
    void train(vector<vector<double>>& trainingData, int maxIterations, double learningRate) {
        for (int i = 0; i < maxIterations; i++) {
            for (vector<double>& input : trainingData) {
                int winnerNeuronIndex = findBestNeuron(input);
                vector<double> winnerPosition = { (double)winnerNeuronIndex,(double)neurons.size() };
                neurons[winnerNeuronIndex].updateWeights(input, learningRate, winnerPosition);
            }
        }
    }
 
    vector<vector<double>> getWeights() {
        vector<vector<double>> result;
 
        for (Neuron& neuron : neurons) {
            result.push_back(neuron.getWeights());
        }
 
        return result;
    }
};
 
int main() {
    srand(time(NULL));
 
    vector<vector<double>> trainingData = {
        {0.1, 0.2},
        {0.3, 0.4},
        {0.5, 0.6},
        {0.7, 0.8},
        {0.9, 1.0},
    };
 
    KohonenNetwork network(2, 10);
    network.train(trainingData, 1000, 0.1);
 
    vector<vector<double>> weights = network.getWeights();
 
    for (int i = 0; i < weights.size(); i++) {
        cout << "Neuron " << i << ": ";
        for (int j = 0; j < weights[i].size(); j++) {
            cout << weights[i][j] << " ";
        }
        cout << endl;
    }
 
    return 0;
}
