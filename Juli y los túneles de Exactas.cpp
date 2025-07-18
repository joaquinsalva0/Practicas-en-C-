#include <iostream>
#include <vector>
#include <tuple>
#include<queue>

# https://github.com/joaquinsalva/Recursos/blob/main/Problem%20-%20C%20-%20Codeforces.pdf
using namespace std;

/*
 *IDEA: Nos piden distancia minima entre nodo v1 = 1 y cualquier nodo vi.
 *En clase vimos que el BFS Tree que nos queda luego de correr el BFS partiendo del nodo v1, nos queda en el vector
 *de distacias dist[vi] == d(v1,vi). Corremos BFS con las aristas atajos y las aristas que van de vi a vi+1 para todo i
 *entre 1 y n-1.
 *Correccion: puedo ir entre el aula i y el aula i+1 pero tambien entre i+1 e i.
 *Implica que a veces va a convenir tomar un atajo y retroceder.
 *Si tal fuera el caso, entonces la energia minima requerida es | atajo_mas_cercano_posterior_a_vi - vi| + P(v1,atajo_mas_cercano_posterior_a_vi),
 *que ya tengo calculado a P con el BFS. Busco el atajo mas cercano(hijo_mayor) en nlogn, estan ordenados. Quedaria (n^2)long n todo,costoso.
 */

/*
 *Para facilitar implementacion de BFS, como sabemos que todo vertice tiene 2 vecinos (pueden ser iguales) analizo casos a mano con 2 ifs.
 */

void BFS(long long int v1, vector<tuple< long long int, long long int>> &adj,vector<  long long int>& dist,    vector< long long int>& padre) {
    dist[v1] = 0;
    padre[v1] =-1;
    queue<long long int> frontera;
    frontera.push(v1);
    long long int visitando; long long int vecino1; long long int vecino2;
    while(!frontera.empty()) {
        visitando =  frontera.front();
        vecino1 = get<0>(adj[visitando]); vecino2 = get<1>(adj[visitando]);
        if( dist[vecino2] == -1 ) { // No lo visite
            dist[vecino2] = dist[visitando] + 1;
            padre[vecino2] = visitando;
            frontera.push(vecino2);
        }
        if( dist[vecino1] == -1 ) { // No lo visite
            dist[vecino1] = dist[visitando] + 1;
            padre[vecino1] = visitando;
            frontera.push(vecino1);
        }
        if(dist[visitando-1] == -1 && visitando-1 >= 0) {
            dist[visitando-1] = dist[visitando] + 1;
            padre[visitando-1] = visitando;
            frontera.push(visitando-1);

        }
            frontera.pop();
        }


}

 int main(){
     long long int N;  long long int atajo_i;
    cin >> N;
    vector<tuple<long long int,  long long int>> adyacencias(N+1);
    vector<long long int> atajos(N+1, -1);
    for(long long int i = 1; i <= N; i++) {
    cin>> atajo_i;
        atajos[i] = atajo_i;
    adyacencias[i] = {i+1, atajo_i};
    }
    adyacencias[N] = {N, N};
    vector< long long int> dist(N+1, -1);
    vector< long long int> padre(N+1, -1);
    BFS(1,adyacencias, dist, padre);
    long long int ind = 1; long long int k;
    while(atajos[ind] ==-1){ind++;}
    tuple<long long int,long long int>atajo_actual = {ind,atajos[ind]};
    for(long long int j = 1 ; j < dist.size(); j++) {
        cout << dist[j] << " ";
    }

    return 0;
}
