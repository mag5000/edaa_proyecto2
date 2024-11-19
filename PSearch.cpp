#ifndef PSEARCH
#define PSEARCH

#include <algorithm>
#include <string>
#include <chrono>
#include <fstream>
#include <iostream>
#include <cstdint>
#include <vector>
#include "FMindex.cpp"
#include "LCP.cpp"
#include "sufix_array.cpp"

using namespace std;

//===================================== Funciones para tomar tiempo de creacion y tamaño ====================================================================

// Función que toma el tiempo de creación del Sufix Array normal
void SA_creation_time(string text){

    ofstream outfile("output/SA_creation_time_seg.csv");
    outfile << "Estructura,Segundos,Bytes" <<endl;

    auto start_time = chrono::high_resolution_clock::now();

    suffix_array sa(text);// creación suffix_array; 
   
    auto end_time = chrono::high_resolution_clock::now();
    long long duration = chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
    
    cout<< " SA Time: " << duration << endl;
    cout<< " SA Size: " << sa.getMemorySize() << endl << endl;

    
    outfile << "Sufix Array," << duration << "," << sa.getMemorySize();
    outfile.close(); 

}

// Función que toma el tiempo de creacion del Sufix Array con LCP
void LCP_creation_time(string text){

    ofstream outfile("output/LCP_creation_time_seg.csv");
    outfile << "Estructura,Segundos,Bytes" <<endl;

    auto start_time = chrono::high_resolution_clock::now();

    SuffixArrayLCP lcp(text);

    auto end_time = chrono::high_resolution_clock::now();
    long long duration = chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
    
    cout<< " LCP Time: " << duration << endl;
    cout<< " LCP Size: " << lcp.getMemorySize() << endl << endl; 
    
    outfile << "SA + LCP," << duration << "," << lcp.getMemorySize();
    outfile.close(); 

}

// Función que toma el tiempo de creacio del FM Index
void FMI_creation_time(string text){

    ofstream outfile("output/FMI_creation_time_seg.csv");
    outfile << "Estructura,Segundos,Bytes" <<endl;

    auto start_time = chrono::high_resolution_clock::now();
    
    FMIndex fmi(text);
    
    auto end_time = chrono::high_resolution_clock::now();
    long long duration = chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();

    cout<< " FMI Time: " << duration << endl;
    cout<< " FMI Size: " << fmi.getMemorySize() << endl << endl;

    outfile << "FMI," << duration << "," << fmi.getMemorySize();
    outfile.close(); 

}


//===================================== Funciones para tomar tiempo de búsqueda ====================================================================


// Función que toma el tiempo de búsqueda del Sufix Array normal
void SA_search_time(string text, vector<string> patterns){

    ofstream outfile("output/SA_search_time_nano.csv");
    outfile << "Estructura,Largo Patron,Tiempo" <<endl;

    suffix_array sa(text);// creación suffix_array; 
    int patterns_size = patterns.size();

    cout << "===== SA ===== " << endl;

    for (int i = 0 ; i < patterns_size ;i++){

        auto start_time = chrono::high_resolution_clock::now();

        sa.count(patterns[i]);
    
        auto end_time = chrono::high_resolution_clock::now();
        long long duration = chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
        
        outfile << "Sufix Array," << patterns[i].size() << "," << duration <<endl;

        cout<< "Pattern Size: " << patterns[i].size() << "-  Time: " << duration << endl;

    }
    
    outfile.close(); 

}

// Función que toma el tiempo de búsqueda del Sufix Array + LCP
void LCP_search_time(string text, vector<string> patterns){

    ofstream outfile("output/LCP_search_time_nano.csv");
    outfile << "Estructura,Largo Patron,Tiempo" <<endl;
    
    SuffixArrayLCP lcp(text);// creación estructura 
    int patterns_size = patterns.size();

    cout << "===== SA+LCP ===== " << endl;

    for (int i = 0 ; i < patterns_size ;i++){

        auto start_time = chrono::high_resolution_clock::now();

        lcp.countOccurrences(patterns[i]);
    
        auto end_time = chrono::high_resolution_clock::now();
        long long duration = chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
        
        outfile << "SA+LCP," << patterns[i].size() << "," << duration <<endl;
        cout<< "Pattern Size: " << patterns[i].size() << "-  Time: " << duration << endl;

    }
    outfile.close(); 
}

// Función que toma el tiempo de búsqueda del Sufix Array + LCP
void FMI_search_time(string text, vector<string> patterns){

    ofstream outfile("output/FMI_search_time_nano.csv");
    outfile << "Estructura,Largo Patron,Tiempo" <<endl;
    
    FMIndex fmi(text);// creación estructura 
    int patterns_size = patterns.size();

    cout << "===== FMI ===== " << endl;

    for (int i = 0 ; i < patterns_size ;i++){

        auto start_time = chrono::high_resolution_clock::now();

        fmi.countOccurrences(patterns[i]);
    
        auto end_time = chrono::high_resolution_clock::now();
        long long duration = chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
        
        outfile << "FMI," << patterns[i].size() << "," << duration <<endl;
        cout<< "Pattern Size: " << patterns[i].size() << "-  Time: " << duration << endl;

    }
    outfile.close(); 
}

#endif