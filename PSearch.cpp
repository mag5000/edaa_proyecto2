#ifndef PSEARCH
#define PSEARCH

#include <algorithm>
#include <string>
#include <chrono>
#include <fstream>
#include <iostream>
#include <cstdint>
#include <vector>
#include "fm_index.cpp"
#include "LCP.cpp"
#include "sufix_array.cpp"

using namespace std;

//===================================== Funciones para tomar tiempo de creacion y tamaño ====================================================================

// Función que toma el tiempo de creación del Sufix Array normal
void SA_creation_time(string text_original, int num_exp){

    ofstream outfile("output/SA_creation_time_seg.csv");
    outfile << "Estructura,Tamaño,Milisegundos,Bytes" <<endl;
    
    string text;
    int target_size = 1000000;
    //para textos de largo 1millon a 10millones
    for (int i = 1 ; i <= 10 ; i ++){

        int text_size = target_size*i;    

        if (text_original.length() > text_size) {
            text = text_original.substr(0, text_size);
        } else {
            text = text_original; // Caso en que no hay que cortar
        }

        for(int i = 0; i < num_exp ; i++){

            auto start_time = chrono::high_resolution_clock::now();
            suffix_array sa(text);// creación suffix_array; 
            auto end_time = chrono::high_resolution_clock::now();
            long long duration = chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
                
            outfile << "Sufix Array," << text_size << "," << duration << "," << sa.getMemorySize()  << endl;  
            cout<< " Text Size: " << text_size << endl;
            cout<< " SA Time: " << duration << endl;
            cout<< " SA Size: " << sa.getMemorySize() << endl << endl;
        
        }

    }

    outfile.close(); 

}

// Función que toma el tiempo de creacion del Sufix Array con LCP
void LCP_creation_time(string text_original, int num_exp){

    ofstream outfile("output/LCP_creation_time_seg.csv");
    outfile << "Estructura,Tamaño,Milisegundos,Bytes" <<endl;

    string text;
    int target_size = 1000000;
    //para textos de largo 1millon a 10millones
    for (int i = 1 ; i <= 10 ; i ++){

        int text_size = target_size*i;    

        if (text_original.length() > text_size) {
            text = text_original.substr(0, text_size);
        } else {
            text = text_original; // Caso en que no hay que cortar
        }

        for(int i = 0; i < num_exp ; i++){

            auto start_time = chrono::high_resolution_clock::now();
            SuffixArrayLCP lcp(text);
            auto end_time = chrono::high_resolution_clock::now();
            long long duration = chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
            
            outfile << "SA + LCP," << text_size << "," << duration << "," << lcp.getMemorySize() << endl;
            cout<< " Text Size: " << text_size << endl;
            cout<< " LCP Time: " << duration << endl;
            cout<< " LCP Size: " << lcp.getMemorySize() << endl << endl; 

        }
    
    }

    outfile.close(); 

}

// Función que toma el tiempo de creacio del FM Index
void FMI_creation_time(string text_original, int num_exp){

    ofstream outfile("output/FMI_creation_time_seg.csv");
    outfile << "Estructura,Tamaño,Milisegundos,Bytes" <<endl;

    string text;
    int target_size = 1000000;
    //para textos de largo 1millon a 10millones
    for (int i = 1 ; i <= 10 ; i ++){

        int text_size = target_size*i;    

        if (text_original.length() > text_size) {
            text = text_original.substr(0, text_size);
        } else {
            text = text_original; // Caso en que no hay que cortar
        }

        for(int i = 0; i < num_exp ; i++){

            auto start_time = chrono::high_resolution_clock::now();
            FMIndex fmi(text);        
            auto end_time = chrono::high_resolution_clock::now();
            long long duration = chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
            
            outfile << "FMI," << text_size << "," << duration << "," << fmi.getMemorySize()  << endl;
            cout<< " Text Size: " << text_size << endl;
            cout<< " FMI Time: " << duration << endl;
            cout<< " FMI Size: " << fmi.getMemorySize() << endl << endl;
        
        }
   
    }

    outfile.close(); 

}


//===================================== Funciones para tomar tiempo de búsqueda ====================================================================


// Función que toma el tiempo de búsqueda del Sufix Array normal
void SA_search_time(string text, vector<string> patterns, int num_exp){

    ofstream outfile("output/SA_search_time_nano.csv");
    outfile << "Estructura,Largo Patron,Tiempo" <<endl;

    suffix_array sa(text);// creación suffix_array; 
    int patterns_size = patterns.size();
    long long duration = 0;

    cout << "--- SA --- " << endl;

    for ( int i = 0 ; i < patterns_size ;i++){

        for ( int j = 0 ; j < num_exp ; j++ ) { 

            auto start_time = chrono::high_resolution_clock::now();
            sa.countOcurrences(patterns[i]);
            auto end_time = chrono::high_resolution_clock::now();
            duration = chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
            
            outfile << "Sufix Array," << patterns[i].size() << "," << duration <<endl;

        }
        
        cout<< "Pattern Size: " << patterns[i].size() << "  -  Time: " << duration << endl;

    }
    
    outfile.close(); 

}

// Función que toma el tiempo de búsqueda del Sufix Array + LCP
void LCP_search_time(string text, vector<string> patterns, int num_exp){

    ofstream outfile("output/LCP_search_time_nano.csv");
    outfile << "Estructura,Largo Patron,Tiempo" <<endl;
    
    SuffixArrayLCP lcp(text);// creación estructura 
    int patterns_size = patterns.size();
    long long duration = 0;

    cout << "--- SA+LCP --- " << endl;

    for (int i = 0 ; i < patterns_size ;i++){

        for ( int j = 0 ; j < num_exp ; j++ ) { 

            auto start_time = chrono::high_resolution_clock::now();
            lcp.countOccurrences(patterns[i]);
            auto end_time = chrono::high_resolution_clock::now();
            duration = chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
            
            outfile << "SA+LCP," << patterns[i].size() << "," << duration <<endl;

        }

        
        cout<< "Pattern Size: " << patterns[i].size() << "  -  Time: " << duration << endl;

    }
    outfile.close(); 
}

// Función que toma el tiempo de búsqueda del Sufix Array + LCP
void FMI_search_time(string text, vector<string> patterns, int num_exp){

    ofstream outfile("output/FMI_search_time_nano.csv");
    outfile << "Estructura,Largo Patron,Tiempo" <<endl;
    
    FMIndex fmi(text);// creación estructura 
    int patterns_size = patterns.size();
    long long duration = 0;

    cout << "--- FMI --- " << endl;

    for (int i = 0 ; i < patterns_size ;i++){

        for ( int j = 0 ; j < num_exp ; j++ ) { 

            auto start_time = chrono::high_resolution_clock::now();
            fmi.countOccurrences(patterns[i]);
            auto end_time = chrono::high_resolution_clock::now();
            duration = chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

            outfile << "FMI," << patterns[i].size() << "," << duration <<endl;

        }
        
        cout<< "Pattern Size: " << patterns[i].size() << "  -  Time: " << duration << endl;

    }
    outfile.close(); 
}

#endif