#include <iostream>
#include <random>
#include <ctime>
#include <math.h>
#include <vector>
#include <algorithm>

using namespace std;

double generadorAleatorio() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 1000);
    return dis(gen);
}

float calculoDeDistancia(int** v1, int** v2, int dimension) {
    float resultado, sumatoria = 0;
    for (int* d1 = *v1, *d2 = *v2; d1 < *v1 + dimension; d1++, d2++)
        sumatoria = sumatoria + pow((*d1 - *d2), 2);
    resultado = sqrt(sumatoria);
    return resultado;
}

int main() {
    //CREACION DE VECTORES
    int cantidadVectores, dimension;
    cantidadVectores = 20000;
    cout << "\nIngresar dimensiones deseadas:\n";
    cin >> dimension;

    int** conjuntoDeVectores = new int* [cantidadVectores];
    for (int** i = conjuntoDeVectores; i < conjuntoDeVectores + cantidadVectores; i++) {
        *(i) = new int[dimension];
        for (int* j = *(i); j < *(i)+dimension; j++)
            *j = generadorAleatorio();
    }

    unsigned tInicio, tFin;        //para medir los tiempos
    tInicio = clock();           //inicio de medición
    //CALCULO DE DISTANCIA ENTRE VECTORES
    vector<float> distanciasCalculadas;
    for (int** j = conjuntoDeVectores + 1; j < conjuntoDeVectores + cantidadVectores; j++)
        distanciasCalculadas.push_back(calculoDeDistancia(conjuntoDeVectores, j, dimension));
    
    //for (vector<float>::iterator it = distanciasCalculadas.begin(); it !=distanciasCalculadas.end(); it++)    cout << *it << endl;
    float minDist = *min_element(distanciasCalculadas.begin(), distanciasCalculadas.end());
    float maxDist = *max_element(distanciasCalculadas.begin(), distanciasCalculadas.end());
    
    //RATIOS DE DISTANCIAS
    vector<float> ratios;
    for (vector<float>::iterator it = distanciasCalculadas.begin(); it != distanciasCalculadas.end(); it++) {
        float valor = floorf(((*it - minDist)/(maxDist-minDist)) * 10) / 10;
        ratios.push_back(valor);
    }

    tFin = clock();           //fin de medición
    double tiempoOrdenamiento = (double(tFin - tInicio) / CLOCKS_PER_SEC);
    //for (vector<float>::iterator it = ratios.begin(); it != ratios.end(); it++)    cout << *it << endl;
    //MOSTRAR RESULTADOS
    cout << "VALOR\t" << "CANTIDAD\n" << endl;
    cout << "1\t" << count(ratios.begin(), ratios.end(), 1.0) << endl;
    cout << "0.9\t" << count(ratios.begin(), ratios.end(), floorf(0.9*10)/10) << endl;
    cout << "0.8\t" << count(ratios.begin(), ratios.end(), floorf(0.8 * 10) / 10) << endl;
    cout << "0.7\t" << count(ratios.begin(), ratios.end(), floorf(0.7 * 10) / 10) << endl;
    cout << "0.6\t" << count(ratios.begin(), ratios.end(), floorf(0.6 * 10) / 10) << endl;
    cout << "0.5\t" << count(ratios.begin(), ratios.end(), floorf(0.5 * 10) / 10) << endl;
    cout << "0.4\t" << count(ratios.begin(), ratios.end(), floorf(0.4 * 10) / 10) << endl;
    cout << "0.3\t" << count(ratios.begin(), ratios.end(), floorf(0.3 * 10) / 10) << endl;
    cout << "0.2\t" << count(ratios.begin(), ratios.end(), floorf(0.2 * 10) / 10) << endl;
    cout << "0.1\t" << count(ratios.begin(), ratios.end(), floorf(0.1 * 10) / 10) << endl;
    cout << "0\t" << count(ratios.begin(), ratios.end(), 0.0) << endl;
    cout << "\nEl tiempo de ejecucion del algoritmo fue de:\t" << tiempoOrdenamiento << " s" << endl;
    //ELIMINACION DE VECTORES
    for (int** i = conjuntoDeVectores; i < conjuntoDeVectores + cantidadVectores; i++) {
        delete[] * i;
    }
    delete[] conjuntoDeVectores;

    return 0;
}