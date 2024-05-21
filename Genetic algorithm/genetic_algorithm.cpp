#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>

using namespace std;

const int numCities = 10; //Количество городов
const int populationSize = 10; //Размер популяции
const int generationSize = 100; //Макс. число поколений
const double mutationRate = 0.1; //Вероятность мутации

vector<vector<int>> distances = {
    {0, 16, 16, 17, 6, 3, 7, 19, 10, 5},
    {16, 0, 17, 7, 6, 11, 12, 5, 18, 10},
    {16, 17, 0, 5,19, 18, 7, 14, 15, 18},
    {17, 7, 5, 0, 10, 1, 11, 16, 12, 12},
    {6, 6, 19, 10, 0, 19, 10, 10, 20, 12},
    {3, 11, 18, 1, 19, 0, 12, 3, 9, 13},
    {7, 12, 7, 11, 10, 12, 0, 4, 8, 5},
    {19, 5, 14, 16, 10, 3, 4, 0, 9, 6},
    {10, 18, 15, 12, 20, 9, 8, 9 , 0, 16},
    {5, 10, 18, 12, 12, 13, 5, 6, 16, 0}
};

//Вычисление приспособленности (длины пути)
int calculateFitness(vector<int>& chromosome) {
    int distance = 0;

    for (int i = 0; i < chromosome.size() - 1; i++) {
        distance += distances[chromosome[i]][chromosome[i + 1]];
    }

    distance += distances[chromosome[chromosome.size() - 1]][chromosome[0]];

    return distance;
}

//Генерация случайного пути (хромосомы)
vector<int> generateRandomChromosome() {
    vector<int> chromosome(numCities);

    for (int i = 0; i < chromosome.size(); i++) {
        chromosome[i] = i;
    }

    random_shuffle(chromosome.begin(), chromosome.end());

    return chromosome;
}

//Мутация (перестановка генов)
void mutate(vector<int>& chromosome) {
    int firstPos = rand() % numCities;
    int secondPos = rand() % numCities;
    swap(chromosome[firstPos], chromosome[secondPos]);
}

//Скрещивание
vector<int> crossover(vector<int>& firstParent, vector<int>& secondParent) {
    vector<int> child(numCities);
    int start = rand() % numCities;

    copy(firstParent.begin(), firstParent.begin() + start, child.begin());

    for (int i = 0; i < numCities; i++) {
        if (find(child.begin(), child.end(), secondParent[i]) == child.end()) {
            child[start++] = secondParent[i];
        }
    }

    return child;
}

vector<int> geneticAlgorithm() {
    vector<vector<int>> population;
    for (int i = 0; i < populationSize; i++) {
        population.push_back(generateRandomChromosome());
    }

    for (int i = 0; i < generationSize; i++) {
        vector<vector<int>> newPopulation;

        //Сортируем популяцию по приспособленности и добавляем лучшую хромосому
        sort(population.begin(), population.end(), [](vector<int>& a, vector<int>& b) {return calculateFitness(a) < calculateFitness(b); });
        newPopulation.push_back(population[0]);

        while (newPopulation.size() < populationSize) {
            //Скрещивание
            int parent1 = rand() % populationSize;
            int parent2 = rand() % populationSize;

            vector<int> child = crossover(population[parent1], population[parent2]);

            //Мутация
            if (rand() / RAND_MAX < mutationRate) {
                mutate(child);
            }

            newPopulation.push_back(child);
        }

        population = newPopulation;
        sort(population.begin(), population.end(), [](vector<int>& a, vector<int>& b) {return calculateFitness(a) < calculateFitness(b); });

        //Вывод всех маршрутов
        cout << "Generation: " << i + 1 << endl;
        for (int i = 0; i < populationSize; i++) {
            cout << "Path " << i << ": ";

            for (int j = 0; j < numCities; j++) {
                cout << population[i][j] << " ";
            }

            cout << "\tDistance: " << calculateFitness(population[i]) << endl;
        }

        cout << endl;
    }

    return population[0];
}

int main() {
    srand(time(NULL));

    vector<int> bestPath = geneticAlgorithm();

    cout << "Best path: ";
    for (int i = 0; i < numCities; i++) {
        cout << bestPath[i] << " ";
    }

    cout << "\nDistance: " << calculateFitness(bestPath);

    return 0;
}
