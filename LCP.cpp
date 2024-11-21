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
        int count = 0;

        // Búsqueda binaria para encontrar el primer sufijo que contiene el patrón
        while (left <= right) {
            int mid = left + (right - left) / 2;
            int suffixIndex = suffixArray[mid];
            std::string suffix = text.substr(suffixIndex, m);

            if (suffix < pattern) {
                left = mid + 1;
            } else if (suffix > pattern) {
                right = mid - 1;
            } else {
                // El patrón coincide con el sufijo en la posición `mid`
                count++;

                // Buscar hacia la izquierda usando LCP
                int leftIdx = mid - 1;
                while (leftIdx >= left && text.substr(suffixArray[leftIdx], m) == pattern) {
                    count++;
                    leftIdx--;
                }

                // Buscar hacia la derecha usando LCP
                int rightIdx = mid + 1;
                while (rightIdx <= right && text.substr(suffixArray[rightIdx], m) == pattern) {
                    count++;
                    rightIdx++;
                }
                break;
            }
        }

        return count;
    }

    int64_t getMemorySize() const {
        //size_t size = sizeof(SuffixArrayLCP);
        //size += text.capacity();
        //size += suffixArray.capacity() * sizeof(int);
        //size += lcpArray.capacity() * sizeof(int);
        //return size;

        return sizeof(std::int64_t) * (suffixArray.size() + lcpArray.size());

    }
};