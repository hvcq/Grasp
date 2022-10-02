#include "input.h"

using namespace std;

//ofstream tiempos_txt;

bool eleccion_determinista;
int MAYOR_CARDINALIDAD_ITERACION;
int MAYOR_CARDINALIDAD_GLOBAL;

string MEJOR_S;

typedef struct{
    int posicion;
    char caracter_menos_frecuente;
    int frecuencia;
}columna;

int calcula_threshold_string(string S_aux, string str){
    int cuantos_distintos = 0;
    for(int i = 0; i < S_aux.size(); ++i)
        if(S_aux.at(i) != str.at(i))
            ++cuantos_distintos;
    return cuantos_distintos;
}

bool caracteres_son_distintos(char S_aux_char, char b){
    if((S_aux_char != b) && (S_aux_char != 'X'))
        return true;
    else
        return false;
}

int posicion_menor_frecuencia(int frecuencia_caracteres_aux[4]){
    int menor_frecuencia = 9999;
    int pos = 0;
    for(int i = 0; i < 4; ++i){
        if(frecuencia_caracteres_aux[i] < menor_frecuencia){
            menor_frecuencia = frecuencia_caracteres_aux[i];
            pos = i;
        }
    }
    return pos;
}

void setup_columnas(int n, int m, vector<string> & vec_strings, vector<columna> & vec_columnas){
    columna columna_aux;
    int posicion_del_menor;
    int frecuencia_caracteres_aux[4]; // [0]='A';[1]='G';[2]='C';[3]='T'
    for(int i = 0; i < m; ++i){
        frecuencia_caracteres_aux[0] = 0;
        frecuencia_caracteres_aux[1] = 0;
        frecuencia_caracteres_aux[2] = 0;
        frecuencia_caracteres_aux[3] = 0;
        for(int j = 0; j < n; ++j){
            if(vec_strings.at(j).at(i) == 'A') ++frecuencia_caracteres_aux[0];
            if(vec_strings.at(j).at(i) == 'G') ++frecuencia_caracteres_aux[1];
            if(vec_strings.at(j).at(i) == 'C') ++frecuencia_caracteres_aux[2];
            if(vec_strings.at(j).at(i) == 'T') ++frecuencia_caracteres_aux[3];
        }
        posicion_del_menor = posicion_menor_frecuencia(frecuencia_caracteres_aux);
        columna_aux.posicion = i;
        columna_aux.frecuencia = frecuencia_caracteres_aux[posicion_del_menor];
        if(posicion_del_menor == 0) columna_aux.caracter_menos_frecuente = 'A';
        if(posicion_del_menor == 1) columna_aux.caracter_menos_frecuente = 'G'; 
        if(posicion_del_menor == 2) columna_aux.caracter_menos_frecuente = 'C'; 
        if(posicion_del_menor == 3) columna_aux.caracter_menos_frecuente = 'T';  
        vec_columnas.push_back(columna_aux);
    }
}

string setup_S(int m){
    string S;
    for(int i = 0;i < m; ++i){
        S.push_back('X');
    }
    return S;
}

bool compara_columnas(columna c1, columna c2){
    return (c1.frecuencia < c2.frecuencia);
}

int columna_a_asignar(float nivel_de_determinismo, int size_vector_columnas){
    int random = rand()%100;
    int determinismo_int = nivel_de_determinismo*100;
    /*Retorna la mejor columna o la que contiene el caracter menos frecuente hasta el momento*/
    if(random < determinismo_int){
        eleccion_determinista = true;
        return 0;
    /*Retorna una columna aleatoria del conjunto*/
    }else{
        eleccion_determinista = false;
        return rand()%size_vector_columnas;
    }
}

string greedy_aleatorizado(int n, int m, float th, vector<string> vec_strings, vector<columna> vec_columnas, vector<int> & vec_t_strings,float nivel_de_determinismo){
    string S = setup_S(m);
    int threshold = m*th;
    int posicion_columna_a_asignar;
    int caracter_a_asignar = 0;
    string S_aux;
    vector<char> posibilidades {'A','G','C','T'};
    vector<int> vec_t_strings_aux = vec_t_strings;
    int mayor_t_actual = 0;
    int mayor_t_global = -1;
    int cantidad_mayores_t_aux;

    /*En el caso determinista solo se fija en S el mejor caracter de la mejor columna*/

    /*
    Aqui se revisan threshold - 1 columnas, la heuristica utilizada es para el caso no determinista, 
    ir fijando aquellos caracteres que maximizan la cardinalidad al superar la mayor distancia de
    hamming (t) actual.
    */
    while(vec_columnas.size() > (m - threshold)){
        S_aux = S;
        posicion_columna_a_asignar = columna_a_asignar(nivel_de_determinismo, vec_columnas.size());
        if(!eleccion_determinista){
            for(int j = 0; j < 4; ++j){
                cantidad_mayores_t_aux = 0;
                S_aux.at(vec_columnas.at(posicion_columna_a_asignar).posicion) = posibilidades.at(j);
                for(int k = 0; k < n; ++k){
                    if(caracteres_son_distintos(S_aux.at(vec_columnas.at(posicion_columna_a_asignar).posicion),vec_strings.at(k).at(vec_columnas.at(posicion_columna_a_asignar).posicion)))
                        ++vec_t_strings_aux.at(k);
                    if(vec_t_strings_aux.at(k) > mayor_t_actual)
                        ++cantidad_mayores_t_aux;
                }
                if(cantidad_mayores_t_aux > mayor_t_global){
                    mayor_t_global = cantidad_mayores_t_aux;
                    caracter_a_asignar = j;
                }
                vec_t_strings_aux.clear();
                vec_t_strings_aux = vec_t_strings;
                S_aux.at(vec_columnas.at(posicion_columna_a_asignar).posicion) = 'X';
            }
        }
        if(eleccion_determinista){
            cantidad_mayores_t_aux = 0;
            S.at(vec_columnas.at(posicion_columna_a_asignar).posicion) = vec_columnas.at(posicion_columna_a_asignar).caracter_menos_frecuente;
        }else
            S.at(vec_columnas.at(posicion_columna_a_asignar).posicion) = posibilidades.at(caracter_a_asignar);
        for(int i = 0; i < n; ++i){
            if(caracteres_son_distintos(S.at(vec_columnas.at(posicion_columna_a_asignar).posicion),vec_strings.at(i).at(vec_columnas.at(posicion_columna_a_asignar).posicion)))
                ++vec_t_strings_aux.at(i);
            if(eleccion_determinista){
                if(vec_t_strings_aux.at(i) > mayor_t_actual)
                    ++cantidad_mayores_t_aux;
            }
        }
        if(eleccion_determinista){
            if(cantidad_mayores_t_aux > mayor_t_global){
                mayor_t_global = cantidad_mayores_t_aux;
            }
        }
        vec_t_strings = vec_t_strings_aux;
        /*El t a usar es la mayor distancia de hamming encontrada en los strings hasta el momento (greedy)*/
        for(int i = 0; i < n; ++i)
            if(vec_t_strings.at(i) > mayor_t_actual)
                mayor_t_actual = vec_t_strings.at(i);
        vec_columnas.erase(vec_columnas.begin()+posicion_columna_a_asignar);
    }
    int mayor_cardinalidad = -1;
    int cardinalidad_aux;
    /*
    Aqui se revisan las columnas restantes, la heuristica utilizada es para el caso no determinista, 
    ir fijando aquellos caracteres que maximizan la cardinalidad al superar o empatar el threshold actual
    */
    while ((!vec_columnas.empty())&&(mayor_cardinalidad < n)){
        S_aux = S;
        posicion_columna_a_asignar = columna_a_asignar(nivel_de_determinismo, vec_columnas.size());
        if(!eleccion_determinista){
            for(int j = 0; j < 4; ++j){
                cardinalidad_aux = 0;
                S_aux.at(vec_columnas.at(posicion_columna_a_asignar).posicion) = posibilidades.at(j);
                for(int k = 0; k < n; ++k){
                    if(caracteres_son_distintos(S_aux.at(vec_columnas.at(posicion_columna_a_asignar).posicion),vec_strings.at(k).at(vec_columnas.at(posicion_columna_a_asignar).posicion)))
                        ++vec_t_strings_aux.at(k);
                    if(vec_t_strings_aux.at(k) >= threshold)
                        ++cardinalidad_aux;
                }
                if(cardinalidad_aux > mayor_cardinalidad){
                    mayor_cardinalidad = cardinalidad_aux;
                    caracter_a_asignar = j;
                }
                vec_t_strings_aux.clear();
                vec_t_strings_aux = vec_t_strings;
                S_aux.at(vec_columnas.at(posicion_columna_a_asignar).posicion) = 'X';
            }
        }
        if(eleccion_determinista){
            cardinalidad_aux = 0;
            S.at(vec_columnas.at(posicion_columna_a_asignar).posicion) = vec_columnas.at(posicion_columna_a_asignar).caracter_menos_frecuente;
        }else
            S.at(vec_columnas.at(posicion_columna_a_asignar).posicion) = posibilidades.at(caracter_a_asignar);
        for(int i = 0; i < n; ++i){
            if(caracteres_son_distintos(S.at(vec_columnas.at(posicion_columna_a_asignar).posicion),vec_strings.at(i).at(vec_columnas.at(posicion_columna_a_asignar).posicion)))
                ++vec_t_strings_aux.at(i);
            if(eleccion_determinista){
                if(vec_t_strings_aux.at(i) >= threshold)
                    ++cardinalidad_aux;
            }
        }
        if(eleccion_determinista){
            if(cardinalidad_aux > mayor_cardinalidad){
                mayor_cardinalidad = cardinalidad_aux;
            }
        }
        vec_t_strings = vec_t_strings_aux;
        vec_columnas.erase(vec_columnas.begin()+posicion_columna_a_asignar);
    }
    /*Si con thershold caracteres ya se abarca todo el conjunto omega, se rellena con 'A' las columnas sobrantes*/
    if(mayor_cardinalidad == n)
        for(int i = 0; i < m; ++i)
            if(S.at(i) == 'X') S.at(i) = 'A';
            
    MAYOR_CARDINALIDAD_ITERACION = mayor_cardinalidad;
    return S;
    /*tiempos_txt<<"S: "<<S<<endl;
    tiempos_txt<<"Cardinalidad de P^S: "<<mayor_cardinalidad<<endl;
    tiempos_txt<<"Tiempo de respuesta: "<<time<<" [seconds]"<<endl;*/
}

/*Utilizar un parámetro con el porcentaje de peores columnas de vec_columnas
las cuales seran el vecindario h realizar cambios en ellos*/

//MAYOR_CARDINALIDAD_ITERACION es global por lo que tenemos la cardinalidad que se tenia de greedy aleatorizado

string LocalSearch(int n,int m, int threshold, string S, vector<string> vec_strings, vector<columna> vec_columnas, vector<int> vec_t_strings, float porcentaje_vecindario){
    int size_vecindario =  m*porcentaje_vecindario;
    vector<char> posibilidades {'A','G','C','T'};

    // Dónde es true significa que el caracter es distinto al de la solucion y false que son iguales
    bool matriz_caracteres[n][m];
    for(int i = 0; i < n; ++i){
        for(int j = 0 ; j < m; ++j){
            if(vec_strings.at(i).at(j) != S.at(j))
                matriz_caracteres[i][j] = true;
            else
                matriz_caracteres[i][j] = false;
        }
    }

    vector<int> vec_t_strings_aux = vec_t_strings;
    int cardinalidad_aux = 0;
    string S_aux = S;
    string mejor_S_aux = S;
    bool encontre_mejor_cardinalidad = false;
    int posicion_columna_a_asignar = m - 1;
    for(int ii = m - 1; ii >= m - size_vecindario; --ii){
        for(int i = m - 1; i >= m - size_vecindario; --i){
            S_aux = S;    
            for(int j = 0; j < 4 ; ++j){
                cardinalidad_aux = 0;
                S_aux.at(vec_columnas.at(i).posicion) = posibilidades.at(j);
                for(int k = 0; k < n; ++k){
                    if(S_aux.at(vec_columnas.at(i).posicion) != vec_strings.at(k).at(vec_columnas.at(i).posicion)){
                        if(!matriz_caracteres[k][vec_columnas.at(i).posicion]){
                            ++vec_t_strings_aux.at(k);
                        }
                    }else{
                        if(matriz_caracteres[k][vec_columnas.at(i).posicion]){
                            --vec_t_strings_aux.at(k);
                        }
                    }
                    if(vec_t_strings_aux.at(k) >= threshold)
                        ++cardinalidad_aux;
                }
                if(cardinalidad_aux > MAYOR_CARDINALIDAD_ITERACION){
                    MAYOR_CARDINALIDAD_ITERACION = cardinalidad_aux;
                    mejor_S_aux = S_aux;
                    encontre_mejor_cardinalidad = true;
                    posicion_columna_a_asignar = i;
                }
                vec_t_strings_aux = vec_t_strings;
            }  
        }

        if(encontre_mejor_cardinalidad){
            for(int k = 0; k < n; ++k){
                if(mejor_S_aux.at(vec_columnas.at(posicion_columna_a_asignar).posicion) != vec_strings.at(k).at(vec_columnas.at(posicion_columna_a_asignar).posicion)){
                    if(!matriz_caracteres[k][vec_columnas.at(posicion_columna_a_asignar).posicion]){
                        matriz_caracteres[k][vec_columnas.at(posicion_columna_a_asignar).posicion] = true;
                        ++vec_t_strings_aux.at(k);
                    }
                }else{
                    if(matriz_caracteres[k][vec_columnas.at(posicion_columna_a_asignar).posicion]){
                        matriz_caracteres[k][vec_columnas.at(posicion_columna_a_asignar).posicion] = false;
                        --vec_t_strings_aux.at(k);
                    }
                }
            }
            vec_t_strings = vec_t_strings_aux;
            encontre_mejor_cardinalidad = false;
            S = mejor_S_aux;
        }
    }
    return mejor_S_aux;
}

void GRASP(vector<string> & vec_strings, int t, float th, float nivel_de_determinismo, float porcentaje_vecindario){
    int n = vec_strings.size();
    int m = vec_strings.at(0).size();
    vector<columna> vec_columnas;
    
    /*Inicializa el vector con las columnas*/
    setup_columnas(n, m, vec_strings, vec_columnas);
    
    /*Ordena las columnas de menor a mayor frecuencia*/
    sort(vec_columnas.begin(),vec_columnas.end(),compara_columnas);

    /*Este vector guarda el t que lleva cada string hasta el momento debido al S que se esta generando*/
    vector<int> vec_t_strings(n);

    string S;
    string new_S;
    double time = 0;
    unsigned t0, t1;
    int iteracion = 0;
    MAYOR_CARDINALIDAD_GLOBAL = -1;
    while(time < t){
        fill(vec_t_strings.begin(), vec_t_strings.end(), 0); // inicia el t de cada string en 0
        //Aqui comienza a medir el tiempo
        t0 = clock();
        S = greedy_aleatorizado(n, m, th, vec_strings, vec_columnas, vec_t_strings, nivel_de_determinismo);
        cout<<"--------------"<<endl;
        cout<<"cardinalidad greedy_a: "<<MAYOR_CARDINALIDAD_ITERACION<<endl;
        cout<<"solucion greedy_a S: "<<S<<endl;
        new_S = LocalSearch(n, m, th*m , S, vec_strings, vec_columnas, vec_t_strings, porcentaje_vecindario);
        cout<<"cardinalidad greedy_a + localSearch: "<<MAYOR_CARDINALIDAD_ITERACION<<endl;
        if(MAYOR_CARDINALIDAD_ITERACION > MAYOR_CARDINALIDAD_GLOBAL){
            MAYOR_CARDINALIDAD_GLOBAL = MAYOR_CARDINALIDAD_ITERACION;
            MEJOR_S = new_S;
        }
        //Aqui termina de medir el tiempo
        t1 = clock();
        time += (double(t1-t0)/CLOCKS_PER_SEC);
    
        //cout<<"Resultado iteracion "<<iteracion<<endl;
        cout<<"(greedy_a + localSearch) new_S: "<<new_S<<endl;
        //cout<<"Cardinalidad de P^S: "<<MAYOR_CARDINALIDAD_ITERACION<<endl;
        //cout<<"Tiempo de respuesta: "<<time<<" [seconds]"<<endl<<endl;
        ++iteracion;
    }  
    cout<<"_________________________"<<endl;
    cout<<"  Resultado Final GRASP  "<<endl; 
    cout<<"S: "<<MEJOR_S<<endl;
    cout<<"Cardinalidad de P^S: "<<MAYOR_CARDINALIDAD_GLOBAL<<endl;
    cout<<"Tiempo de respuesta: "<<time<<" [seconds]"<<endl<<endl;
    
}

int main(int argc, char *argv[]){
    vector<string> vec_strings;
    srand(time(NULL));
    if(argc < 11){
        cout<<endl<<"ERROR: No se encuentran todos los argumentos necesarios"<<endl;
        throw;
    }
    parsear_entrada(argv[2], vec_strings);
    GRASP(vec_strings,stoi(argv[4]),stof(argv[6]),stof(argv[8]),stof(argv[10]));
    /*tiempos_txt.open("resultados_GP_0.85.txt");
    string archivo;
    for(int i = 1; i <= 10; ++i){
        archivo = "100-300-0";
        if(i<10){
            archivo.push_back('0');
            archivo.push_back(to_string(i).c_str()[0]);
        }else{
            archivo.push_back('1');
            archivo.push_back('0');
        }
        archivo.push_back('.');
        archivo.push_back('t');
        archivo.push_back('x');
        archivo.push_back('t');

        parsear_entrada(archivo, vec_strings);
        tiempos_txt<<"------ instancia "<<archivo<<" ------"<<endl;
        tiempos_txt<<"th: "<<argv[4]<<endl;
        tiempos_txt<<"determinismo: "<<argv[6]<<endl;
        greedy_aleatorizado(vec_strings.size(), vec_strings.at(0).size(), stof(argv[4]), vec_strings, stof(argv[6]));
        tiempos_txt<<endl;
        vec_strings.clear();
    }
    tiempos_txt.close();*/
    return 0;
}