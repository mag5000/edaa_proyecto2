// Codigo generado con ChatGPT de OpenAI

#include <sdsl/suffix_arrays.hpp>
#include <string>
#include <iostream>
#include <algorithm>

using namespace sdsl;
using namespace std;

class FMIndex {
private:
    csa_wt<wt_huff<rrr_vector<127>>, 512, 1024> fm_index; // FM-Index
    string text; // Texto original

public:
    // Constructor: Construye el FM-Index a partir del texto
    explicit FMIndex(const string& input_text) : text(input_text) {
        construct_im(fm_index, text, 1); // Construye el índice en memoria
        // cout << "FM-Index construido con éxito para el texto dado." << endl;
    }

    // Método para contar las ocurrencias de un patrón
    size_t countOccurrences(const string& pattern) const {
        size_t occurrences = sdsl::count(fm_index, pattern.begin(), pattern.end());
        return occurrences;
    }

    // Método para obtener el tamaño de la estructura en memoria (en bytes)
    size_t getMemorySize() const {
        return size_in_bytes(fm_index);
    }

    // Método opcional: Imprime información del índice
    void printIndexInfo() const {
        cout << "El FM-Index ocupa " << size_in_mega_bytes(fm_index) << " MiB en memoria." << endl;
    }
};
