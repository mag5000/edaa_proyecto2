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
    std::unordered_map<char, int> C;
    std::vector<std::unordered_map<char, int>> Occ;

    // Calcula el BWT de manera eficiente
    std::string buildBWT(const std::string &text) {
        int n = text.size();
        std::vector<int> suffixArray(n);

        //std::cout << "Text size: " << n << std::endl;
        //std::cout << "BWT: ";


        // Construir el Suffix Array (usando solo índices)
        for (int i = 0; i < n; ++i) {
            suffixArray[i] = i;
        }
        std::sort(suffixArray.begin(), suffixArray.end(), [&text, n](int i, int j) {
            return text.substr(i) < text.substr(j);
        });

        // Generar el BWT
        std::string bwt_result(n, '\0');
        for (int i = 0; i < n; ++i) {
            bwt_result[i] = suffixArray[i] == 0 ? text[n - 1] : text[suffixArray[i] - 1];
        }


        //std::cout << "OK" << std::endl;

        return bwt_result;
    }

    // Construye las tablas C y Occ
    void buildTables() {

        //std::cout << "C: ";


        std::map<char, int> freq;
        for (char c : bwt) {
            freq[c]++;
        }

        int total = 0;
        for (auto &entry : freq) {
            C[entry.first] = total;
            total += entry.second;
        }

        //std::cout << "OK" << std::endl;
        //std::cout << "Occ: ";

        Occ.resize(bwt.size());
        std::unordered_map<char, int> currentCount;
        for (size_t i = 0; i < bwt.size(); ++i) {
            currentCount[bwt[i]]++;
            Occ[i] = currentCount;
        }

        //std::cout << "OK" << std::endl;
    }

public:
    FMIndex(const std::string &input_text) {
        text = input_text + '\x01';
        bwt = buildBWT(text);
        buildTables();
    }

    ~FMIndex() {}

    int countOccurrences(const std::string &pattern) {
        int m = pattern.size();
        int i = m - 1;
        char c = pattern[i];

        if (C.find(c) == C.end()) {
            return 0;
        }

        int sp = C[c];
        int ep = (c + 1 < 128 && C.find(c + 1) != C.end()) ? C[c + 1] - 1 : bwt.size() - 1;

        while (sp <= ep && i >= 1) {
            c = pattern[--i];
            sp = C[c] + (sp > 0 ? Occ[sp - 1][c] : 0);
            ep = C[c] + Occ[ep][c] - 1;
        }

        return (sp <= ep) ? (ep - sp + 1) : 0;
    }


    size_t getMemorySize() const {

        size_t size = sizeof(FMIndex);

        //size += text.capacity();
        //size += bwt.capacity();
        //size += C.size() * sizeof(std::pair<char, int>);

        for (const auto& map : Occ) {
            size += map.size() * sizeof(std::pair<char, int>);
        }

        //return size;

        return sizeof(std::int64_t) * (C.size() + bwt.size());

    }



};




