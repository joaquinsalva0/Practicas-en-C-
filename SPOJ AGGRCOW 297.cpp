#include <iostream>
#include  <vector>
#include <algorithm>

# https://www.spoj.com/problems/AGGRCOW/

using namespace std;

/*
 * IDEA: acoto el conjunto de valores posibles para maximizar la menor distancia entre cualesquieras 2 vacas.
 * Caso ideal: establos 1-distantes uno de otro (tengo todas las posibles posiciones disponibles), entonces
 * pongo a las vacas a la misma distancia (Xn/C).
 * Entonces sol <= (Xn/C).
 * Entonces ahora podria hacer una busqueda binaria en el intervalo (0,Xn/C), donde lo que busco es el maximo valor k
 * para el cual haya una asignacion valida de establos donde la minima distancia entre 2 vacas es k.
 * Costo: buscar el k es aprox log(1,000,000,000) = 30 y por cada k recorro las posiciones en O(N).
 * Entonces para el algoritmo, dado un k quedaria:
 * Parte Greedy: busco la primer posicion que este a distancia >= k de la anterior vaca (siempre pongo una en la primera
 * posicion).
 * Backtracking: es obvio que si Xn - Xi  < Ci * k entonces podo porque no hay solucion factible.
 * Xi = (posicion de la ultima vaca), Ci = vacas que me faltan asignar.
 */

bool k_distanciable(vector<int>& posiciones, int k, int C){
    //int cota = posiciones[posiciones.size()-1]/ C;
    //if( k > cota){ return false;}

    C --; //Primer vaca en primer establo es intuitivo
    int pos_ult_vaca = posiciones[0];
    for(int i = 1; i < posiciones.size() && C >0 ; i++){
        if( posiciones[i] - pos_ult_vaca >= k){
            pos_ult_vaca = posiciones[i];
            C--;
        }
    }
    return (C ==0); //Unica forma de que exista tal posible asignacion.
}
int busqueda_vaquera(vector<int>& posiciones, int desde, int hasta, int  C){
    if(desde +1 == hasta){ return desde;}
    int pos_mid = (hasta + desde) /2 ;
    bool mid = k_distanciable(posiciones, pos_mid,C);
    if(!mid){
        hasta = pos_mid;
        return(busqueda_vaquera(posiciones,desde,hasta,C));
    }
    else{
        desde = pos_mid;
        return busqueda_vaquera(posiciones,desde, hasta , C);
    }

}

int cows(vector<int>& posiciones, int C){
    //Primero ordenamos en O(NlogN), sino no puedo hacer nada.
    sort(posiciones.begin(), posiciones.end(), [](int a, int b) {
        return a < b;
    });
    int cota = posiciones[posiciones.size()-1]/ (C-1);
    if(k_distanciable(posiciones,cota,C)){return cota;}
    //Como busco un maximo, me guardo el mayor k donde dio False, entonces cuando llego a desde + 1 = hasta, y desde es
    // True pero hasta False, returneo desde.

    int mejor_k = busqueda_vaquera(posiciones,0,cota ,C);
    bool mejor_k1 = k_distanciable(posiciones, mejor_k +1 , C);
    if(mejor_k1) { return mejor_k + 1;}
    return mejor_k;

}

int main() {

int tests; int xi; int N; int C;
cin>> tests;
for(int i = 0; i < tests; i++){
    cin >> N >> C;
    if(N < C){ cout <<0;}
    else {
        vector<int> posiciones(N, -1);//VECTOR DE POSICIONES.
        for (int j = 0; j < N; j++) {
            cin >> xi;
            posiciones[j] = xi;

        }
        cout << cows(posiciones, C) << endl;
    }
}
    return 0;
}
