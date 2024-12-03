#include <algorithm>
#include <iostream>
#include <vector>
#include <stack>

#https://github.com/joaquinsalva/Recursos/blob/main/Problem%20-%20B%20-%20Codeforces.pdf

//Creamos la clase vertice para parsear la entrada en un vector de pilas de adyacencias donde
//la posicion i del vector da Adj(vi).
 
using namespace std;
//Visito el vector de adjyacencias, pintando el vertice v de color c, y a sus vecinos del color opuesto (recursiono)
/*
 *IDEA: Un arbol es conexo, por lo cual si comienzo asignando un color a un vertice v1, y luego
 *asigno el color opuesto a todos sus vecinos, y lo mismo para los vecinos de los vecinos de v1 y asi,
 *quedan dos conjuntos Azul y Rojo, por ponerles nombre, que es la unica forma de armar un grafo bipartito (segun vimos en clase)
 *El maximo de aristas es que todo azul este conectado  con todo rojo, es decir cantidad_rojos * cantidad de azules.
 *Maximo = aristas_que_ya_estan - aristas_faltantes <-> #rojos * #azules - aristas_que_ya_estan = aristas_faltantes.
 */
 
void pintar( vector<stack<long long int>>& adj, vector<int>& color,long long int v, int c) {
    if(color[v] != 0){return;} //Ya lo pinte.
    color[v] = c;
    while( !adj[v].empty()) {
        long long int vecino = adj[v].top();
        adj[v].pop();
        pintar(adj,color,vecino, -c);
 
    }
}
 
 
int main() {
 
        long long int N;
        cin >> N;
        // Creamos el vector de pilas de adyacencias
        vector<stack<long long int>> adyacencias(N+1);
        vector<int>color(N+1,0); //Si es -1 es rojo, si es 1 es azul, 0 es no visitado.
 
        long long int u; long long  int v;
        for(int i =1; i <= N-1; i++) {
            cin >> u >> v;
            adyacencias[u].push(v);
            adyacencias[v].push(u);
        }
 
  /*  vector<tuple<int,int>> adj(N-1);
    for (int i = 0; i < adj.size(); i++) {
        cin >> u >> v;
        adj[i] = {u,v};
    }
    ordenar_asignar(adj, color);
*/
 
        if (N <= 3){cout << 0 << endl;}else {
            pintar(adyacencias,color,1,-1);
         //   pintar_iterativo(adyacencias,color);
            long long int cantidad_rojos = 0 ;
            for(long long int j = 1; j < color.size(); j++) {if(color[j] == -1){cantidad_rojos++;} }
            long long int cantidad_azules = N - cantidad_rojos;
            long long int max_aristas_bipartito = cantidad_rojos * cantidad_azules;
            cout << max_aristas_bipartito - N +1 << endl;
        }
 
        return 0;
}
