
#include <iostream>
#include <vector>
#include<algorithm>
#include <unordered_map>
#include <tuple>
#include <sstream>
#include<chrono>

#https://codeforces.com/problemset/problem/160/D

using namespace std;
using namespace chrono;

typedef tuple<long long int , long long int , long long int , long long int> arista ;
typedef vector<long long int> vec;

const long long int UNDEF = -10;
const long long int NONE = -1;
const long long int AT_LEAST = 0;
const long long int ANY = 1;

int NO_LO_VI = -1;
int EMPECE_A_VER = 0;
int TERMINE_DE_VER = 1;

class DSU {
    vector<long long int> rank, parent;
public:
    DSU(long long int n) {
        rank.resize(n + 1, 0);
        parent.resize(n + 1);
        for(long long int i = 0; i < n + 1; i++){
            parent[i] = i;
        }
    }

    long long int find(long long int node){

        // En caso que nodo sea el representante
        if (node == parent[node]) return node;

        // Hago path compression
        return parent[node] = find(parent[node]);
    }

    void unionS(long long int u, long long int v) {
        int uRepresentative = find(u);
        int vRepresentative = find(v);

        // Si tienen el mismo representante, entonces pertenece al
        // mismo conjunto
        if (uRepresentative == vRepresentative) return;

        // Actualizamos el representante segun el caso del rank
        if (rank[uRepresentative] < rank[vRepresentative]) {
            parent[vRepresentative] = uRepresentative;
        } else if(rank[uRepresentative] > rank[vRepresentative]) {
            parent[uRepresentative] = vRepresentative;
        } else {
            parent[vRepresentative] = uRepresentative;
            rank[uRepresentative]++;
        }
    }
};

void dfs_visit(long long int v  ,vec& padre,vector<vec>& ady, vector<vec>& tree_edges,vec& back_sup_en, vec& back_inf_en, vec& desde,long long int& t, long long int p = -1 ) {
    t++;
    desde[v] = t;
    for (long long int u : ady[v]) {
        if (desde[u] ==UNDEF) {
            tree_edges[v].push_back(u);
            padre[u]=v;
            dfs_visit(u ,padre,ady, tree_edges, back_sup_en,back_inf_en,desde,t);
        }
        else if (u != padre[v] && desde[v]<desde[u]) { //Si no cuento 2 veces una backedge pero "transpuesta"
            back_inf_en[v]++;
            back_sup_en[u]++;
        }
    }
}


void dfs(vec& padre,vector<vec>& ady, vector<vec>& tree_edges,vec& back_sup_en, vec& back_inf_en,vec& representantes,long long int p = -1 ) {
    vec desde(ady.size()+1,UNDEF);
    long long int t = 0;
    for(long long int nodo : representantes ) {
    if(desde[nodo] == UNDEF) {
        padre[nodo] =  NONE;
        dfs_visit( nodo  ,padre,ady,tree_edges, back_sup_en, back_inf_en,desde,t);
    }
    }




}

long long int cubren( long long int v, vector<vector<long long int>>& tree_edges,
    vec& back_sup_en, vec& back_inf_en, vec& memo, long long  int p = -1) {
    if (memo[v] != -1) return memo[v];
   long long  int out = 0;
    out += back_inf_en[v];
    out -= back_sup_en[v];

    for (long long int hijo : tree_edges[v]) {
        if (hijo != p) {
            out += cubren(hijo, tree_edges, back_sup_en, back_inf_en, memo,v);
        }
    }

    memo[v] = out;
    return out;
}


/*IDEA: la de la practica dada por Ezequiel. Puentes para aristas candidatas y correr DFS en G'.
 *G' es el grafo resultante de tomar cada componente conexa del bosque kruskal y representarla con un unico vertice.
 *Las aristas de G' son las aristas candidatas.
 */



long long int es_any(arista e, vec& res, DSU& dsu) {
    if(dsu.find(get<0>(e)) == dsu.find(get<1>(e))) {
        res[get<3>(e)] = NONE;
        return 0;
    }
    res[get<3>(e)] =  ANY;
    dsu.unionS(get<0>(e), get<1>(e));
    return 1;
}


unordered_map<long long int, long long int> comprimir(vec& rep) {
    vec r ={};
    sort(rep.begin(),rep.end(), [](long long int a, long long int b) {
            return a < b;
        });
    unordered_map<long long int,long long int> dicc;
    dicc[rep[0]] = 0;
    r.push_back(0);
    long long int pos = 1;
    for(int i = 1; i < rep.size(); i++) {
        if(rep[i] != rep[i-1]) {
            dicc[rep[i]] = pos ;
            r.push_back(pos);
            pos++;
        }
    }
    rep = r;
    return dicc;
}


long long int CLASIFICAR(vector<arista>& E, vec& res, long long int n) {
    DSU dsu(n);
    long long int aristas = 0;  ;
    long long int i = 1; long long int j=0 ;
    unordered_map<long long int, long long int> mapa ={};
    while( i < res.size()) {
        vector<arista> candidatas = {}; j =i;
        while(get<2>(E[j]) == get<2>(E[i])&& j < E.size() ) { //Por cada peso clasifico entre NONE y candidatas
            if(dsu.find(get<0>(E[j])) == dsu.find(get<1>(E[j]))) { //Ya se que es None
                res[get<3>(E[j])] = NONE;
            }
            else { //Agrego para despues hacer G'
                candidatas.push_back( E[j]);
            }
            j++;
        }
        if(candidatas.size() == 1) {// Es NONE o ANY
            long long int k = es_any(candidatas[0],res,dsu);
            aristas += k;
            if(k==1) {
                dsu.unionS(get<0>(candidatas[0]),get<1>(candidatas[0]));
                aristas++;
            }
        }else if(candidatas.size()>1) { // 2 o mas aristas de mismo peso, armo G'.
            vec representantes = {};
            for(arista e : candidatas) {
                representantes.push_back(dsu.find(get<0>(e)));
                representantes.push_back(dsu.find(get<1>(e)));
            }
            mapa = comprimir(representantes );
            long long int n1 = mapa.size();
            vec back_sup_en( n1+1,0);
            vec back_inf_en(n1+1,0);
            vec padre(n1+1,UNDEF);
            vector<vec> ady(n1+1);
            vector<vec> tree_edges(n1+1);
            vec memo(n1+1,-1);

            for(arista e : candidatas) {
                ady[mapa.at(dsu.find(get<0>(e)))].push_back(mapa.at(dsu.find(get<1>(e))));
                ady[mapa.at(dsu.find(get<1>(e)))].push_back(mapa.at(dsu.find(get<0>(e))));
            }

            dfs(   padre, ady,  tree_edges,back_sup_en,back_inf_en,representantes);

            for(arista e : candidatas) {
                long long int u = dsu.find(get<0>(e));
                long long int w = dsu.find(get<1>(e));
                long long int dad  = UNDEF;
                long long int son = UNDEF;

                if(padre[mapa.at(u)] == mapa.at(w)) { //Para chequear si (v, padre[v]) es puente primero tengo que ver como me lo tomo el dfs, si (u,v) o (v,u)
                    dad = w; son = u;
                } else if(padre[mapa.at(w)] == mapa.at(u)){
                    dad = u; son = w;
                }else { // Puede que sea backedge, en tal caso seguro es at least
                    res[get<3>(e)] = AT_LEAST;
                }
                if(dad!= UNDEF) {
                    if(cubren(mapa.at(son), tree_edges, back_sup_en, back_inf_en,memo ) != 0) {
                        res[get<3>(e)] = AT_LEAST;
                    }
                    else { res[get<3>(e)] = ANY;}
                }
            }
                for(arista e : candidatas) {
                    if(dsu.find(get<0>(e)) != dsu.find(get<1>(e))) {
                        dsu.unionS(get<0>(e),get<1>(e));
                        aristas++;
                    }
                }

            }



    i = max(i+1,j); //Solo para el ultimo caso
        //cout << " PASE "<< endl;
    }
    return i;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long int n; long long int m;
    cin>>n>>m;
  /*  for(long long int test = 1; test<= m; test++) {
        cout<< test << " " << test +1 <<" " <<  1<< endl;
    }
*/    vector<arista> E(m+1); //{ai,bi,wi,pos_en_input}
    long long int ai ; long long int bi; long long int wi;
    for(long long int i=1;i<=m;i++) {
        cin >> ai >> bi >> wi;
        E[i] = make_tuple(ai,bi,wi,i);
    }

    sort(E.begin(),E.end(), [](arista a, arista b) {
        return get<2>(a) < get<2>(b);
    });//Ordeno por peso ascendente
    vec res(m+1,UNDEF);
  
    long long int  cota = CLASIFICAR(E, res,n);
  
    long long int size = res.size();
    cota = min(cota + 1, size );

    for (long long int i = 1; i < cota; i++) {
        if (res[i] < 0) cout << "none"<< endl;
        else if (res[i] == AT_LEAST) cout << "at least one"<< endl;
        else if (res[i] == ANY) cout << "any"<< endl;
    }
    while (cota < res.size()) {
        cout << "none"<< endl;
        cota++;
    }
    return 0;
}
