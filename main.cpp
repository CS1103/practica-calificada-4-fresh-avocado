#include <iostream>
#include <thread>
#include <vector>
#include <future>
#include <cassert>

using namespace std;

// dividir el vector en "num_threads" sectores, cada thread debe iterar sobre ese vector
// y retornar el mayor valor que encuentre, luego, se comparan los valores retornados por cada thread
// cuando se agarre el future de cada uno, y se retorna el mayor

template <class T>
void find_max_i(vector<T>& vec, const int& start, const int& stop, promise<T>& promesa) {
    T max = vec[start];
    for (int i = start+1; i < stop; ++i) {
        if (vec[i] > max) {
            max = vec[i];
        }
    }
    promesa.set_value(max);
}

template <class T>
T find_max(vector<T>& vec, const int& num_threads) {
    vector<T> starts(num_threads);
    vector<T> stops(num_threads);

    for (int k = 0; k < num_threads; ++k) {
        starts[k] = k*(vec.size()/num_threads);
        stops[k] = (k+1)*(vec.size()/num_threads);
    }

    vector<thread> threads;
    vector<future<T>> futures(num_threads);
    vector<promise<T>> promises(num_threads);

    for (int i = 0; i < num_threads; ++i) {
        futures[i] = promises[i].get_future();
    }

    for (int m = 0; m < num_threads; ++m) {
        threads.emplace_back(&find_max_i<T>, ref(vec), ref(starts[m]), ref(stops[m]), ref(promises[m]));
    }

    for (int j = 0; j < num_threads; ++j) {
        threads[j].join();
    }

    T max = futures[0].get();

    for (int l = 1; l < num_threads; ++l) {
        T valor = futures[l].get();
        if (valor > max) {
            max = valor;
        }
    }

    return max;
}

int main() {

    /// Probando con enteros
    vector<int> ivec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    assert(find_max(ivec, 5) == 10);
    assert(find_max(ivec, 2) == 10);
    assert(find_max(ivec, 10) == 10);

    /// Probando con doubles
    vector<double> dvec = {3.14, 2.71, 9.9, 3.3, 5.6, 2.1};

    assert(find_max(dvec, 3) == 9.9);
    assert(find_max(dvec, 6) == 9.9);
    assert(find_max(dvec, 2) == 9.9);

    /// Probando con char
    vector<char> cvec = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};

    assert(find_max(cvec, 5) == 'j');
    assert(find_max(cvec, 10) == 'j');
    assert(find_max(cvec, 2) == 'j');

    return 0;
}