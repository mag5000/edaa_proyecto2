#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
//#include "fm_index.cpp"
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



// Funcion que obtiene substrings del texto recibido por parametro
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

    // Lectura del archivo a utilizar --------------------------------------------------------
    std::string filePath = "dna";   
    std::string text_original = readFile(filePath);

    int times = 0;
    int target_size = 10000000;
    string text;
    
    if (text_original.length() > target_size) {
        text = text_original.substr(0, target_size);
    } else {
        text = text_original; // Caso en que no hay que cortar
    }
    
    // Experimentos de creación --------------------------------------------------------------
    cout<< " =========== Creación ========== " << endl;
    SA_creation_time(text, 100);
    LCP_creation_time(text, 100);
    FMI_creation_time(text, 100);
    
    // Experimentos de búsqueda --------------------------------------------------------------
    vector<string> patterns;

//  Creacion de los patrones a buscar que si están en el texto  
    for (int i = 10 ; i<=100 ; i=i+10 ){
        patterns.push_back(getRandomSubstring(text,i));
    }

//  para crear valores que no estan en el texto
//    for (int i = 10; i <= 100; i += 10) {
//        patterns.push_back(std::string(i, 'z')); // Crea un string de longitud 'i' con 'z'
//    }

    cout<< " ========== Búsqueda =========== " << endl;
    SA_search_time(text, patterns, 100);
    LCP_search_time(text, patterns, 100);
    FMI_search_time(text, patterns, 100);
    
    return 0;
}
