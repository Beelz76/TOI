#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>

using namespace std;

const int iterations = 100; //Количество итераций
const double alpha = 1.0; //Параметр влияния феромонов
const double beta = 2.0; //Параметр влияния расстояния
const int numCities = 6; //Количество городов
const int numAnts = 10; //Количество муравьев
const double Q = 1.0; //Количество феромона, оставляемого муравьем
const double evaporationRate = 0.5; //Коэффициент испарения феромонов

vector<vector<double>> distances = {
    {0, 10, 15, 11, 2, 15},
    {10, 0, 16, 18, 21, 13},
    {15, 16, 0, 19, 12, 3},
    {11, 18, 19, 0, 18, 15},
    {2, 21, 12, 18, 0, 2},
    {15, 13, 3, 15, 2, 0}
};

vector<vector<double>> pheromones(numCities, vector<double>(numCities, 1.0));

//Расчет вероятности перехода муравья в следующий город
double calculateProbability(int i, int j, vector<int>& notVisited) {
    double numerator = pow(pheromones[i][j], alpha) * pow(1.0 / distances[i][j], beta);
    double denominator = 0.0;

    for (int city : notVisited) {
        denominator += pow(pheromones[i][city], alpha) * pow(1.0 / distances[i][city], beta);
    }

    return numerator / denominator;
}

//Построения маршрута муравьем
vector<int> createRoute(int ant) {
    vector<int> route;
    vector<int> notVisited(numCities);

    for (int i = 0; i < numCities; i++) {
        notVisited[i] = i;
    }

    int startCity = rand() % numCities;
    route.push_back(startCity);

    notVisited.erase(remove(notVisited.begin(), notVisited.end(), startCity), notVisited.end());

    while (!notVisited.empty()) {
        double maxProbability = 0.0;
        int selectedCity = 0;

        for (int city : notVisited) {
            double probability = calculateProbability(route.back(), city, notVisited);

            if (probability > maxProbability) {
                maxProbability = probability;
                selectedCity = city;
            }
        }

        route.push_back(selectedCity);
        notVisited.erase(remove(notVisited.begin(), notVisited.end(), selectedCity), notVisited.end());
    }

    return route;
}

//Обновление матрицы феромонов
void updatePheromones(vector<vector<int>>& routes, vector<double>& lengths) {
    for (int i = 0; i < numAnts; ++i) {
        for (int j = 0; j < numCities - 1; ++j) {
            int from = routes[i][j];
            int to = routes[i][j + 1];

            //Обновление феромона (с учетом испарения)
            pheromones[from][to] = (1 - evaporationRate) * pheromones[from][to] + Q / lengths[i];
            pheromones[to][from] = pheromones[from][to];
        }

        //Обновление феромона от последнего к первому городу (с учетом испарения)
        pheromones[routes[i].back()][routes[i].front()] = (1 - evaporationRate) * pheromones[routes[i].back()][routes[i].front()] + Q / lengths[i];
        pheromones[routes[i].front()][routes[i].back()] = pheromones[routes[i].back()][routes[i].front()];
    }
}

//Поиск лучшего маршрута
vector<int> findBestRoute(vector<vector<int>>& routes, vector<double>& lengths) {
    int minIndex = 0;
    double minLength = lengths[0];

    for (int i = 1; i < lengths.size(); i++) {
        if (lengths[i] < minLength) {
            minLength = lengths[i];
            minIndex = i;
        }
    }

    return routes[minIndex];
}

//Вычисление длины маршрута
double calculateRouteLength(const vector<int>& route, const vector<vector<double>>& distances) {
    double length = 0.0;

    for (int i = 0; i < numCities - 1; i++) {
        length += distances[route[i]][route[i + 1]];
    }

    length += distances[route.back()][route.front()];

    return length;
}

int main() {
    vector<vector<int>> antRoutes(numAnts);
    vector<double> routeLengths(numAnts);

    for (int iter = 0; iter < iterations; iter++) {
        for (int ant = 0; ant < numAnts; ant++) {
            antRoutes[ant] = createRoute(ant);
            routeLengths[ant] = calculateRouteLength(antRoutes[ant], distances);
        }

        updatePheromones(antRoutes, routeLengths);
    }

    for (int i = 0; i < antRoutes.size(); i++) {
        cout << i + 1 << " Ant - ";
        for (int j = 0; j < antRoutes[i].size(); j++) {
            cout << antRoutes[i][j] << " ";
        }
        cout << endl;
    }

    vector<int> bestRoute = findBestRoute(antRoutes, routeLengths);
    double bestLength = calculateRouteLength(bestRoute, distances);

    cout << "\nBest route: ";

    for (int city : bestRoute) {
        cout << city << " ";
    }

    cout << bestRoute.front();
    cout << "\nLenght: " << bestLength << endl;

    return 0;
}