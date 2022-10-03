# Grasp
Para el correcto funcionamiento del código se le entregará lo siguiente: \
* Carpeta "instancias" que contiene todo el dataset
* Carpeta "resultados" la que guardará los resultados entregados por el programa
* Archivos grasp.cpp, input.h y ejecutable grasp.

Para compilar basta con ejecutar\
* g++ grasp.cpp -o grasp

Para ejecutar el programa hay dos formas, la primera consiste de esta linea de comando\
* ./grasp -i 100-300-001.txt -t 1 -th 0.8 -d 0.9 -v 0.07\
* con los parametros
* -i nombreInstancia.txt
* -t tiempoEnSegundosEntero
* -th thresholdFlotante
* -d nivelDeterminismoFlotante
* -v porcentajeVecindarioLocalSearch\
* este corresponde a un porcentaje de las peores columnas a analizar en LocalSearch\
Ahora, la segunda forma de ejecutar es la siguiente\
* ./grasp -i all -t 1 -th 0.8 -d 0.9 -v 0.07

-se ha implementado el comando all, que le indica al programa que se ejecute para las 10 instancias de las variaciones del dataset.\
\
Los resultados son almacenados en la carpeta "resultados" con el siguiente nombre:\
FechaYHora_i-nombreInstancia_t TiempoEnSegundos_th Threshold_d NivelDeterminismo_v PorcentajeVecindario.txt\
\
ejemplo: Sun-Oct--2-19:43:25-2022_i-100-600-001_t7_th0.85_d0.9_v0.07.txt
