// Código obtenido de las clases y modificado con con  ChatGPT de OpenAI

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class suffix_array {
    
private:
    std::string text;
    std::vector<int> suffixArray;

    void buildSuffixArray() {
        int n = text.size();
        suffixArray.resize(n);

        // Inicializar el arreglo de sufijos
        for (int i = 0; i < n; ++i) {
            suffixArray[i] = i;
        }

        // Ordenar los sufijos usando la comparación directa en el texto
        std::sort(suffixArray.begin(), suffixArray.end(), [this](int a, int b) {
            return text.compare(a, text.size() - a, text, b, text.size() - b) < 0;
        });
    }

    // busca la primer posición en la que el patrón podría aparecer en el arreglo de sufijos,
    int lowerBound(const std::string& pattern) const {
        int left = 0, right = suffixArray.size() - 1;
        int m = pattern.size();

        while (left <= right) {
            int mid = left + (right - left) / 2;
            int suffixIndex = suffixArray[mid];

            // Comparar el patrón con el sufijo actual
            if (text.compare(suffixIndex, m, pattern) < 0) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }

        return left;
    }

    // encuentra la primera posición en el arreglo de sufijos donde el patrón sería mayor que el sufijo actual
    int upperBound(const std::string& pattern) const {
        int left = 0, right = suffixArray.size() - 1;
        int m = pattern.size();

        while (left <= right) {
            int mid = left + (right - left) / 2;
            int suffixIndex = suffixArray[mid];

            // Comparar el patrón con el sufijo actual
            if (text.compare(suffixIndex, m, pattern) <= 0) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }

        return left;
    }

public:
    // Constructor
    suffix_array(const std::string& t) : text(t) {
        buildSuffixArray();
    }

    // Destructor
    ~suffix_array() {
        suffixArray.clear();
        //std::cout << "Destructor llamado, memoria liberada." << std::endl;
    }

    // Función que cuenta el número de ocurrencias del patrón
    int countOcurrences(const std::string& pattern) const {
        if (pattern.empty()) return 0;

        int start = lowerBound(pattern);
        int end = upperBound(pattern);

        return end - start;
    }

    //Función que retorna el tamaño utilizado
    int64_t getMemorySize() const {
        
        return sizeof(std::int64_t) * suffixArray.size();

    }

};