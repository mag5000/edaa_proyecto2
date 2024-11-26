#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

class FMIndex {
private:
    std::string bwt;                // Burrows-Wheeler Transform
    std::map<char, int> C;          // Tabla C
    std::vector<std::map<char, int>> Occ; // Tabla Occ

    // Construir el Suffix Array en tiempo O(n log n)
    std::vector<int> buildSuffixArray(const std::string& text) {
        int n = text.size();
        std::vector<int> suffixArray(n), rank(n), tempRank(n);

        // Inicialización inicial
        for (int i = 0; i < n; ++i) {
            suffixArray[i] = i;
            rank[i] = text[i];
        }

        // Ordenar usando el algoritmo de rango-doblamiento
        for (int k = 1; k < n; k *= 2) {
            auto comparator = [&](int i, int j) {
                if (rank[i] != rank[j])
                    return rank[i] < rank[j];
                int rank_i_k = (i + k < n) ? rank[i + k] : -1;
                int rank_j_k = (j + k < n) ? rank[j + k] : -1;
                return rank_i_k < rank_j_k;
            };
            std::sort(suffixArray.begin(), suffixArray.end(), comparator);

            // Actualizar los rangos
            tempRank[suffixArray[0]] = 0;
            for (int i = 1; i < n; ++i) {
                tempRank[suffixArray[i]] = tempRank[suffixArray[i - 1]];
                if (comparator(suffixArray[i - 1], suffixArray[i]))
                    tempRank[suffixArray[i]]++;
            }
            rank = tempRank;
        }

        return suffixArray;
    }

    // Generar la BWT usando el Suffix Array
    std::string buildBWT(const std::string& text, const std::vector<int>& suffixArray) {
        int n = text.size();
        std::string bwtResult(n, ' ');

        for (int i = 0; i < n; ++i) {
            int j = suffixArray[i];
            bwtResult[i] = (j == 0) ? text[n - 1] : text[j - 1];
        }

        return bwtResult;
    }

    // Construye las tablas C y Occ
    void buildTables() {
        // Inicializar C
        for (char c : bwt) {
            C[c]++;
        }

        int sum = 0;
        for (auto& pair : C) {
            int temp = pair.second;
            pair.second = sum;
            sum += temp;
        }

        // Inicializar Occ
        Occ.resize(bwt.size(), std::map<char, int>());
        for (size_t i = 0; i < bwt.size(); ++i) {
            if (i > 0) {
                Occ[i] = Occ[i - 1];
            }
            Occ[i][bwt[i]]++;
        }
    }

public:
    FMIndex(const std::string& filepath) {
        // Leer archivo grande
        std::ifstream file(filepath, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("No se pudo abrir el archivo.");
        }

        std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        // Asegurarnos de que termine con un carácter delimitador
        if (text.back() != '$') {
            text += '$';
        }

        // Construir SA y BWT
        std::vector<int> suffixArray = buildSuffixArray(text);
        bwt = buildBWT(text, suffixArray);

        // Construir tablas C y Occ
        buildTables();
    }

    // Cuenta las ocurrencias de un patrón
    int countOccurrences(const std::string& pattern) {
        int m = pattern.size();
        int i = m - 1;
        char c = pattern[i];
        int top = C[c];
        int bottom = (C.find(c) != C.end()) ? C[c + 1] : bwt.size();

        while (top < bottom && i >= 0) {
            c = pattern[i];
            top = C[c] + ((top > 0) ? Occ[top - 1][c] : 0);
            bottom = C[c] + Occ[bottom - 1][c];
            i--;
        }

        return (top < bottom) ? bottom - top : 0;
    }

    int64_t getMemorySize() const {
        //size_t size = sizeof(SuffixArrayLCP);
        //size += text.capacity();
        //size += suffixArray.capacity() * sizeof(int);
        //size += lcpArray.capacity() * sizeof(int);
        //return size;

        return sizeof(std::int64_t) * (bwt.size() + C.size());

    }

};




