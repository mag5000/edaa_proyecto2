#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <unordered_map>

class FMIndex {
private:
    std::string text;
    std::string bwt;
    std::map<char, int> C;
    std::vector<std::unordered_map<char, int>> Occ;
    int checkpointInterval;

    // Construcción eficiente del suffix array usando algoritmo SA-IS
    std::vector<int> buildSuffixArray() {
        int n = text.size();
        std::vector<int> suffixArray(n);
        std::vector<int> rank(n), tmp(n);

        for (int i = 0; i < n; ++i) {
            suffixArray[i] = i;
            rank[i] = text[i];
        }

        for (int len = 1;; len *= 2) {
            auto compare = [&](int i, int j) {
                if (rank[i] != rank[j]) return rank[i] < rank[j];
                int ri = (i + len < n) ? rank[i + len] : -1;
                int rj = (j + len < n) ? rank[j + len] : -1;
                return ri < rj;
            };
            std::sort(suffixArray.begin(), suffixArray.end(), compare);

            tmp[suffixArray[0]] = 0;
            for (int i = 1; i < n; ++i) {
                tmp[suffixArray[i]] = tmp[suffixArray[i - 1]] + (compare(suffixArray[i - 1], suffixArray[i]) ? 1 : 0);
            }

            rank = tmp;
            if (rank[suffixArray[n - 1]] == n - 1) break;
        }
        return suffixArray;
    }

    // Construcción de la BWT usando el suffix array
    std::string buildBWT() {
        std::vector<int> suffixArray = buildSuffixArray();
        std::string bwtResult;

        for (int i : suffixArray) {
            bwtResult += (i == 0) ? '$' : text[i - 1];
        }

        return bwtResult;
    }

    // Construcción del array C
    void buildCArray() {
        for (char ch : bwt) {
            C[ch]++;
        }

        int total = 0;
        for (auto& pair : C) {
            int count = pair.second;
            pair.second = total;
            total += count;
        }
    }

    // Construcción del array Occ con checkpoints
    void buildOccArray() {
        checkpointInterval = 32; // Ajusta según memoria y tamaño del texto
        std::unordered_map<char, int> count;
        Occ.reserve((bwt.size() / checkpointInterval) + 1);

        for (size_t i = 0; i < bwt.size(); ++i) {
            count[bwt[i]]++;
            if (i % checkpointInterval == 0) {
                Occ.push_back(count);
            }
        }
    }

    // Mapeo de LF con cálculos intermedios
    int LFMapping(int index, char ch) const {
        if (C.find(ch) == C.end()) return 0;

        int rank = 0;
        int checkpoint = index / checkpointInterval;

        if (checkpoint > 0) {
            rank += Occ[checkpoint - 1].at(ch);
        }

        for (int i = checkpoint * checkpointInterval; i < index; ++i) {
            if (bwt[i] == ch) rank++;
        }

        return C.at(ch) + rank;
    }

public:
    FMIndex(const std::string& t) : text(t) {
        text += "$"; // Agregar delimitador único
        bwt = buildBWT();
        buildCArray();
        buildOccArray();
    }

    ~FMIndex() {
        Occ.clear(); // Liberar memoria de Occ
    }

    int countOccurrences(const std::string& pattern) {
        int left = 0;
        int right = bwt.size() - 1;

        for (int i = pattern.size() - 1; i >= 0 && left <= right; --i) {
            char ch = pattern[i];
            if (C.find(ch) == C.end()) {
                return 0; // El carácter no existe en T
            }

            left = LFMapping(left, ch);
            right = LFMapping(right + 1, ch) - 1;
        }

        if (left > right) return 0;
        return right - left + 1;
    }

    size_t getMemorySize() const {
        size_t size = sizeof(FMIndex); // Tamaño estático de la clase

        // Agregar tamaño del string `text`
        //size += text.capacity(); // Usamos `capacity()` para incluir la memoria reservada dinámicamente

        // Agregar tamaño de la cadena `bwt`
        size += bwt.capacity(); // Usamos `capacity()` para incluir la memoria reservada

        // Agregar tamaño del mapa `C`
        size += C.size() * sizeof(std::pair<char, int>); // `C` es un `std::map`

        // Agregar tamaño del vector de mapas `Occ`
        for (const auto& map : Occ) {
            size += map.size() * sizeof(std::pair<char, int>); // Cada `unordered_map` en `Occ`
        }

        return size;
    }



};
