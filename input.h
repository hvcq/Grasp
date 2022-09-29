#include <bits/stdc++.h>

using namespace std;

void parsear_entrada(string nombre_archivo_instancia, vector<string> & vec_strings){
    string ruta_archivo = "instancias/";
    ruta_archivo += nombre_archivo_instancia;
    ifstream instancia_problema(ruta_archivo.c_str());
    string linea;
    while (getline(instancia_problema, linea)) {
        linea.erase(linea.size()-1,linea.size()-1);
        vec_strings.push_back(linea);
    }
    instancia_problema.close();
}