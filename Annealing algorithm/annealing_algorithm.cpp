#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>

using namespace std;

const int numCities = 6; //Количество городов
const double T = 100; //Начальная температура
const double coolingRate = 0.97; //Коэффициент охлаждения

vector<vector<int>> distances = {
    {0, 10, 15, 11, 2, 15},
    {10, 0, 16, 18, 21, 13},
    {15, 16, 0, 19, 12, 3},
    {11, 18, 19, 0, 18, 15},
    {2, 21, 12, 18, 0, 2},
    {15, 13, 3, 15, 2, 0}
};

//Вычисление длины пути
int calculateDistance(vector<int>& path) {
    int distance = 0;

    for (int i = 0; i < path.size() - 1; i++) {
        distance += distances[path[i]][path[i + 1]];
    }

    distance += distances[path[path.size() - 1]][path[0]];

    return distance;
}

vector<int> annealingAlgorithm() {
    double temperature = T;

    //Задаем случайный начальный путь
    vector<int> currentPath(numCities);
    for (int i = 0; i < currentPath.size(); i++) {
        currentPath[i] = i;
    }

    random_shuffle(currentPath.begin() + 1, currentPath.end());
    double currentDistance = calculateDistance(currentPath);

    //Задаем лучший путь
    vector<int> bestPath = currentPath;
    double bestDistance = currentDistance;

    while (temperature > 1) {
        vector<int> newPath = currentPath;
        int firstCity = rand() % numCities;
        int secondCity = rand() % numCities;

        //Меняем местами случайные города
        swap(newPath[firstCity], newPath[secondCity]);

        double newDistance = calculateDistance(newPath);

        if (newDistance < currentDistance) {
            currentDistance = newDistance;
            currentPath = newPath;
        }
        else {
            //Рассчитываем вероятность принятия текущего значения
            double probability = exp(-(currentDistance - newDistance) / temperature);

            if (probability > rand() / RAND_MAX) {
                currentDistance = newDistance;
                currentPath = newPath;
            }
        }

        if (currentDistance < bestDistance) {
            bestDistance = currentDistance;
            bestPath = currentPath;
        }

        cout << "T: " << temperature << endl << "Path: ";
        for (int i = 0; i < numCities; i++) {
            cout << currentPath[i] << " ";
        }
        cout << currentDistance << endl << endl;

        temperature *= coolingRate;
    }

    return bestPath;
}

int main() {
    srand(time(NULL));

    vector<int> bestPath = annealingAlgorithm();

    cout << "Best path: ";
    for (int i = 0; i < numCities; i++) {
        cout << bestPath[i] << " ";
    }

    cout << "\nDistance: " << calculateDistance(bestPath);

    return 0;
}

