#include <iostream>
#include <queue>
#include <vector>
#include <tuple>
#include <functional>
#include <algorithm>
using namespace std;

#https://vjudge.net/problem/Kattis-flowerytrails

typedef  long long int lint;
typedef tuple<lint, lint> arista;
lint inf = INT_MAX;
lint undef = -1;

struct vertice {
    lint v;
    lint d;
};


bool Compare(arista a , arista b )
{
    return get<1>(a) >= get<1>(b);
}

void relax(lint u, lint v,  lint peso,vector<lint>& dist, priority_queue<arista, vector<arista>, function<bool(arista, arista)>>& Q,vector<lint>& padres ) {
    if(dist[v] > dist[u]+peso ) {
        dist[v] = dist[u]+peso;
        padres[v] = u;
        Q.push(make_tuple(v,dist[v]));
    }
}
// Mi lista de adyacencias tiene en el lugar Adj[v] la lista de tuplas (w,c), con w el nodo vecino y c el costo de la calle.
vector<lint> DIJKSTRA (vector<vector<tuple<lint,lint>>>& Adj, lint s) {
    vector<lint> dist_n0(Adj.size(), inf);
    dist_n0[s] = 0;
    vector<lint>padres(Adj.size(),undef);
 priority_queue<arista, vector<arista>, function<bool(arista, arista)>> Q(Compare);
    //En mi cola de prioridad pongo v, con v.d = dist(0,v). Ordenado: vi < vi+1 si vi.d < vi+1.d
    //vertice n0; n0.v = 0;n0.d = 0;
    Q.push(make_tuple(s,0));
    for(lint i = 1; i < Adj.size(); i++) {
        Q.push(make_tuple(i,inf) );
    }
    tuple<lint,lint> a; lint padre = undef;lint hijo = undef; lint peso = undef;
    while(!Q.empty()) {
            a = Q.top() ;
             padre = get<0>(a);
            Q.pop();
            for(arista vecina :Adj[padre] ) {
                 hijo = get<0>(vecina);
                peso = get<1>(vecina);
                relax(padre, hijo, peso,dist_n0, Q, padres);
            }

    }

    return dist_n0;
}

lint verificar(vector<vector<tuple<lint,lint>>>& Adj, vector<lint>& d0, vector<lint>&dn, lint c_min) {
    //Vimos en clase que una arista e esta en un camino min de 0 a n-1, d(0,e1) + c(e) + d(e2,n-1) = |c_min|,
    // con e = (e1,e2)
    lint costo = 0;
    lint padre= undef;
    lint hijo= undef;
    lint peso = undef;
    for(lint i = 0 ; i < Adj.size(); i++) {
        for(lint j = 0 ; j < Adj[i].size(); j++){
            padre = i;
            hijo = get<0>(Adj[i][j]);
            peso = get<1>(Adj[i][j]);
            if(d0[padre]+ peso + dn[hijo] == c_min){
                costo += peso;
            }
        }
    }
    return costo;
}



vector<vector<tuple<lint,lint>>> alivianar(vector<vector<tuple<lint,lint>>>& Adj) {
    vector<vector<tuple<lint,lint>>> Adj_sin_repes(Adj.size());
    for(lint i = 0 ; i < Adj.size(); i++) {//Pongo aristas v->w repetidas juntas
        sort(Adj[i].begin(),Adj[i].end(), [](tuple<lint,lint> a, tuple<lint,lint> b) {
            if (get<0>(a) == get<0>(b))
                return get<1>(a) < get<1>(b);
            return get<0>(a) < get<0>(b);     });

        if(Adj[i].size()>0){Adj_sin_repes[i].push_back(Adj[i][0]);}//Siempre pongo la primera
        for(lint j = 1 ; j < Adj[i].size(); j++){// Pongo las no repetidas de menor peso
            if(get<0>(Adj[i][j]) != get<0>(Adj[i][j-1])){
                Adj_sin_repes[i].push_back(Adj[i][j]);
            }
        }

    }
    return(Adj_sin_repes);
}


int main()
{// Como las aristas son de un multigrafo y quiero el camino minimo, parseo entrada,
    // ordeno cada Adj[v] respecto a los vertices de N(v) y escojo la arista de menor peso, si hay mas de una.
    //Elimino las que son v->v. Mentira, si tengo mas de una arista Dikstra toma la minima. La arista v->v la desestimo con un if.
    lint n; lint m;
    cin >> n >> m;
    lint v; lint w; lint c;
    vector<vector<arista>>ady(n);
    for(lint i = 1; i <= m;i++) {
        cin >> v >> w >> c;
        if(v != w) {
            ady[v].push_back({w,2*c});
            ady[w].push_back({v,2*c});
        }
    }
    lint costo;
    // vector<vector<arista>> ady = alivianar(ady_inicial);
    vector< lint> d0 =  DIJKSTRA(ady,0);
    vector< lint> dn =  DIJKSTRA(ady,n-1);
    lint c_min = d0[n-1];
    costo = verificar(ady,d0,dn, c_min);
    cout << costo;
    return 0;
}
