#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
//#include "FMindex.cpp"
//#include "LCP.cpp"
#include "PSearch.cpp"
//#include "sufix_array.cpp"

using namespace std;

// Funcion que sirve para leer el dataset.
// recibe la ruta como parametro.
string readFile(const string& filePath) {
    ifstream file(filePath); // Abrir el archivo
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo: " << filePath << std::endl;
        return {}; // Retorna un vector vacío si no se puede abrir el archivo
    }

    string content;
    string line;
    while (getline(file, line)) { // Leer línea por línea
        content += line + '\n'; // Agregar cada línea al contenido total
    }

    file.close(); // Cerrar el archivo
    return content; // Retornar el vector de fragmentos
}




string getRandomSubstring(const string& str, int substringLength) {

    int strLength = str.length();
    if (strLength < substringLength) {
        return ""; // Retornar vacío si la longitud del string es menor que el tamaño del substring
    }

    // Generar una posición inicial aleatoria
    int startPos = rand() % (strLength - substringLength + 1); // Asegurar que hay espacio para el substring
    string sub = str.substr(startPos, substringLength);

    //cout << "Poisición inicio: " << startPos << " y str: " << sub <<endl;

    // Extraer el substring de tamaño `substringLength`
    return sub;
}


// main
int main() {

    std::string test_text = "bananananana nananan ananana nanana bana banaba nbanabanabnabanababa banana bana kdfgkajshd iuawhe db ana nanananana nananan ananana nanana bana banaba nbanabanabnabanababa banana bana kdfgkajshd iuawhe db    nanananana nananan ananana nanana bana banaba nbanabanabnabanababa banana bana kdfgkajshd iuawhe db nanananana nananan ananana nanana bana banaba nbanabanabnabanababa banana bana kdfgkajshd iuawhe dbnanananana nananan ananana nanana bana banaba nbanabanabnabanababa banana bana kdfgkajshd iuawhe db    nanananana nananan ananana nanana bana banaba nbanabanabnabanababa banana bana kdfgkajshd iuawhe db    nanananana nananan ananana nanana bana banaba nbanabanabnabanababa banana bana kdfgkajshd iuawhe db    nanananana nananan ananana nanana bana banaba nbanabanabnabanababa banana bana kdfgkajshd iuawhe db    nanananana nananan ananana nanana bana banaba nbanabanabnabanababa banana bana kdfgkajshd iuawhe db    nanananana nananan ananana nanana bana banaba nbanabanabnabanababa banana bana kdfgkajshd iuawhe db    nanananana nananan ananana nanana bana banaba nbanabanabnabanababa banana bana kdfgkajshd iuawhe db    nanananana nananan ananana nanana bana banaba nbanabanabnabanababa banana bana kdfgkajshd iuawhe db    nanananana nananan ananana nanana bana banaba nbanabanabnabanababa banana bana kdfgkajshd iuawhe db";
    std::string filePath = "proteins";
    std::string text = readFile(filePath);
    cout<< "Lectura de texto: OK" << endl << endl;
    
    cout<< " =========== Creación ========== " << endl;
    SA_creation_time(test_text);
    LCP_creation_time(test_text);
    FMI_creation_time(test_text);

    vector<string> patterns;

    for (int i = 10 ; i<=100 ; i=i+10 ){

        patterns.push_back(getRandomSubstring(test_text,i));

    }

    cout<< " ========== Búsqueda =========== " << endl;
    SA_search_time(test_text, patterns);
    LCP_search_time(test_text, patterns);
    FMI_search_time(test_text, patterns);

    return 0;
}
