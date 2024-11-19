#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class SuffixArrayLCP {
private:
    std::string text;
    std::vector<int> suffixArray;
    std::vector<int> lcpArray;

    void buildSuffixArray() {
        int n = text.size();
        suffixArray.resize(n);
        for (int i = 0; i < n; ++i) {
            suffixArray[i] = i;
        }

        std::sort(suffixArray.begin(), suffixArray.end(), [this](int a, int b) {
            return text.compare(a, text.size() - a, text, b, text.size() - b) < 0;
        });

        // Construcción del arreglo LCP después de ordenar los sufijos
        buildLCPArray();
    }

    void buildLCPArray() {
        int n = text.size();
        lcpArray.resize(n - 1);  // El arreglo LCP tendrá n - 1 elementos
        std::vector<int> rank(n, 0);

        // Crear el arreglo de rangos de sufijos
        for (int i = 0; i < n; ++i) {
            rank[suffixArray[i]] = i;
        }

        // Calcular el LCP
        int lcpLength = 0;
        for (int i = 0; i < n; ++i) {
            if (rank[i] > 0) {
                int prevSuffixIndex = suffixArray[rank[i] - 1];
                while (i + lcpLength < n && prevSuffixIndex + lcpLength < n &&
                       text[i + lcpLength] == text[prevSuffixIndex + lcpLength]) {
                    lcpLength++;
                }
                lcpArray[rank[i] - 1] = lcpLength;

                // Si lcpLength > 0, decrecerlo para la siguiente comparación
                if (lcpLength > 0) {
                    lcpLength--;
                }
            }
        }
    }

public:
    SuffixArrayLCP(const std::string& t) : text(t) {
        buildSuffixArray();
    }

    // Función count: Cuenta el número de ocurrencias del patrón usando LCP
    int countOccurrences(const std::string& pattern) {
        int left = 0, right = suffixArray.size() - 1;
        int m = pattern.size();

        // Buscar el límite inferior utilizando LCP
        while (left <= right) {
            int mid = left + (right - left) / 2;
            int suffixIndex = suffixArray[mid];
            int lcp = lcpArray[mid > 0 ? mid - 1 : 0]; // LCP con el sufijo anterior

            if (lcp < m) {
                std::string suffix = text.substr(suffixIndex, m);

                if (suffix < pattern) {
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            } else {
                // Si LCP >= m, significa que el patrón está contenido en los sufijos cercanos
                return right - left + 1;
            }
        }

        return 0;
    }

    size_t getMemorySize() const {
        size_t size = sizeof(SuffixArrayLCP);
        size += text.capacity();
        size += suffixArray.capacity() * sizeof(int);
        size += lcpArray.capacity() * sizeof(int);
        return size;
    }
};