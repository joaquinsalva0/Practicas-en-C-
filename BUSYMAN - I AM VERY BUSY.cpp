#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

# https://www.spoj.com/problems/BUSYMAN/
using namespace std;
 
int busy(vector<tuple<int,int>>& act){
    // Ordeno segun  hora comienzo (desc) porque es de segunda importancia para el problema
    sort(act.begin(), act.end(), [](tuple<int,int> a, tuple<int,int> b) {
        return get<0>(a) > get<0>(b);
    });
    // Ordeno segun  hora fin (asc)
    sort(act.begin(), act.end(), [](tuple<int,int> a, tuple<int,int> b) {
        return get<1>(a) < get<1>(b);
    });
    //Ahora el algoritmo en si
    //h_fin es la hora a la que termina la ultima actividad que sume.
    //Agrego una actividad si la
    int h_fin = 0 ; int i=0; int contador = 0;
    for(int i = 0; i < act.size(); i++){
        if(get<0>(act[i]) >= h_fin){
            h_fin = get<1>(act[i]);
            contador++;
        }
    }
return contador;
}
//IDEA:
/* Si tomo respuesta optima (del ejemplo) {pelicula, dotA, dormir}
 * y sustraigo alguna actividad, digamos pelicula, la respuesta optima tendra 3-1 = 2 actividades +
 * el maximo de actividades posibles dentro de las horas  0 a 6.
 * Tomo de  todas las actividades que empiezan luego 0, la que termina antes (y antes de 6). Empate? La que comienza despues.
 * Ordeno tuplas por hora comienzo (desc) y luego por hora de fin  (asc).
 * Ordenar es O(nlog(n))
 * Aplicamos algoritmo desde el comienzo, cuando agrego una actividad tomo su hora de fin como mi nueva hora de comienzo.
 * En cada paso itero sobre las actividades sin pasar por alguna 2 veces, O(n)
 */
int main() {
    int Tests; int j=0; int cant_act;int inicio; int fin;
    cin>>Tests;
    while(j<Tests){
        cin>> cant_act;
        vector<tuple<int,int>>act(cant_act);
        int k = 0;
        while(cant_act>0){
            cin>> inicio >> fin;
            act[k] = {inicio,fin};
        cant_act--;k++;
        }
        cout << busy(act)<< endl;
        j++;
    }
    return 0;
}
