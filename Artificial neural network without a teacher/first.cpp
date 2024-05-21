#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>
 
using namespace std;
 
class HebbNetwork {
public:
    HebbNetwork(int input, int output, double learning = 0.1) {      
        inputSize = input;
        outputSize = output;
        learningRate = learning;
 
        //Инициализация весов случайными значениями [0,1]
        for (int i = 0; i < inputSize; i++) {
            weights.push_back(vector<double>(outputSize));
            for (int j = 0; j < outputSize; j++) {
                weights[i][j] = (rand() % 1000) / 1000.0;
            }
        }
    }
 
    void train(vector<vector<double>>& inputs, int numIterations) {
        for (int iter = 0; iter < numIterations; iter++) {
            for (vector<double>& input : inputs) {
                //Прямое распространение
                vector<double> outputs;
                for (int i = 0; i < outputSize; i++) {
                    double sum = 0.0;
                    for (int j = 0; j < inputSize; j++) {
                        sum += input[j] * weights[j][i]; //Взвешенная сумма входов
                    }
                    outputs.push_back(activationFunction(sum));
                }
                //Обновление весов
                for (int i = 0; i < outputSize; i++) {
                    for (int j = 0; j < inputSize; j++) {
                        weights[j][i] += learningRate * outputs[i] * input[j];
                    }
                }
            }
        }
    }
 
    void printWeights() {
        cout << "\nTrained weights:" << endl;
        for (int i = 0; i < inputSize; i++) {
            for (int j = 0; j < outputSize; j++) {
                cout << "w[" << i << "]= " << weights[i][j] << endl;
            }
        }
    }
 
private:
    int inputSize;
    int outputSize;
    double learningRate;
    vector<vector<double>> weights;
 
    double activationFunction(double x) {
        return 1.0 / (1.0 + exp(-x));
    }
};
 
int main() {
    srand(time(NULL));
 
    HebbNetwork network(10, 1);
    vector<vector<double>> inputs(4, vector<double>(10));
 
    for (int i = 0; i < inputs.size(); i++) {
        for (int j = 0; j < inputs[i].size(); j++) {
            inputs[i][j] = (rand() % 1000) / 1000.0;
        }
    }
 
    cout << "Input: " << endl;
    for (int i = 0; i < inputs.size(); i++) {
        for (int j = 0; j < inputs[i].size(); j++) {
            cout << inputs[i][j] << " ";
        }
        cout << endl;
    }
 
    network.train(inputs, 1000);
    network.printWeights();
 
    return 0;
}
