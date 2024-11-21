#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <cstdint>
#include <numeric>

class FMIndex {
private:
    std::string text;
    std::string bwt;  // BWT como cadena
    std::vector<int> C;  // Tabla C (frecuencia acumulada de caracteres)
    std::vector<std::vector<int>> Occ;  // Tabla Occ (frecuencias acumuladas por posición)

    // Construye el BWT usando un Suffix Array simple
    void buildBWT() {

        // std::cout << "in bwt" <<std::endl;

        int n = text.size();
        std::vector<int> sa(n);
        
        // Construcción del Suffix Array (SA)
        for (int i = 0; i < n; ++i) {
            sa[i] = i;
        }
       std::sort(sa.begin(), sa.end(), [this](int i, int j) {
             return text.compare(i, text.size() - i, text, j, text.size() - j) < 0;
        });

        bwt.resize(n);
        for (int i = 0; i < n; ++i) {
            int sa_index = sa[i];
            bwt[i] = sa_index == 0 ? text[n - 1] : text[sa_index - 1];

        }

        // std::cout << "bwt ok " << std::endl;

    }

   void buildTables() {
    // std::cout << "creando c " << std::endl;

    const int alphabet_size = 256;  // Supone un alfabeto de 256 caracteres ASCII
    C.resize(alphabet_size, 0);

    // Calcular frecuencias de caracteres en el BWT
    for (char c : bwt) {
        C[c]++;
    }

    // Convertir C en acumulada
    for (int i = 1; i < alphabet_size; ++i) {
        C[i] += C[i - 1];
    }

    // std::cout << "c ok " << std::endl;
    // std::cout << "Creando Occ de manera optimizada" << std::endl;

    // Usar una estructura que mantenga solo el conteo de ocurrencias en cada paso
    std::vector<int> occ_current(alphabet_size, 0);  // Almacenar las ocurrencias de la fila actual

    // Proceso de acumulación para la tabla Occ
    for (size_t i = 0; i < bwt.size(); ++i) {
        if (i > 0) {
            // Solo actualizar el conteo de la posición actual (sin copiar toda la fila)
            occ_current[bwt[i - 1]]++;
        }
        
        // Aquí puedes usar `occ_current` para obtener las ocurrencias de cualquier carácter hasta la posición `i`
        // Si necesitas realizar consultas específicas de ocurrencias para cada carácter de `bwt[i]`, puedes hacerlo con occ_current[bwt[i]].

        // Opcionalmente, si necesitas almacenar algo para cada iteración de `bwt`, lo puedes hacer aquí.
    }

    // std::cout << "Occ optimizada ok" << std::endl;
}

public:
    FMIndex(const std::string &input_text) {
        text = input_text + '\x01'; // Agregar un carácter especial para asegurar que el sufijo array tenga una terminación
        buildBWT();
        buildTables();
    }

    // Contar ocurrencias de un patrón utilizando el FM-index
    int countOccurrences(const std::string &pattern) {
        int m = pattern.size();
        int i = m - 1;
        char c = pattern[i];

        // Inicializar sp y ep usando la tabla C
        int sp = (c > 0) ? C[c - 1] : 0;
        int ep = C[c] - 1;

        while (sp <= ep && i >= 1) {
            c = pattern[--i];

            // Actualizar sp y ep usando la tabla Occ
            sp = (c > 0 ? C[c - 1] : 0) + (sp > 0 ? Occ[sp - 1][c] : 0);
            ep = C[c] - 1 + (ep >= 0 ? Occ[ep][c] : 0);
        }

        return (sp <= ep) ? (ep - sp + 1) : 0;
    }

    int64_t getMemorySize() const {
        //size_t size = sizeof(SuffixArrayLCP);
        //size += text.capacity();
        //size += suffixArray.capacity() * sizeof(int);
        //size += lcpArray.capacity() * sizeof(int);
        //return size;

        return sizeof(int64_t) * (bwt.size());

    }

};