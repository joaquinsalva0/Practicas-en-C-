#include <iostream>
#include <vector>
 
#https://codeforces.com/problemset/problem/546/E

using namespace std;
typedef long long  lint;
lint undef = -1;
 
typedef vector<lint> vec;
void dfs_visit( lint v  ,vector<tuple<lint,lint>>& padre,vector<vector<tuple<lint,lint>>>& ady  ) {
  for ( tuple<lint,lint> U : ady[v]) {
   lint u = get<0>(U);  lint u_ij = get<1>(U);
    if(get<0>(padre[u]) == undef) {
    padre[u]= make_tuple(v, u_ij);//v
    dfs_visit(u ,padre,ady);
  }
  }
}
 
 
vector<tuple<lint,lint>> dfs(vector<vector<tuple<lint,lint>>>& ady ) {
vector<tuple<lint,lint>> padre(ady.size(),make_tuple(undef,undef)); //
  dfs_visit( ady.size()-2 ,padre,ady);
  return padre;
  }
 
vector<vector<tuple<lint,lint>>> Residual(vector<vector<tuple<lint,lint>>>& ady, vector<vector<lint>>& flujo ) {
  lint j; lint u_ij; //Devuelve lista de ady, en el tuple1 es TRUE si ij y tuple2 la CAPACIDAD
  vector<vector<tuple<lint,lint>>>ady_residual(ady.size());
  for(lint i = 1; i < ady.size(); i++) {//Itero sobre todas las aristas
    for( tuple<lint,lint>U  : ady[i]) {
       j = get<0>(U);  u_ij = get<1>(U);
      if(flujo[i][j] < u_ij) {
      ady_residual[i].push_back(make_tuple(j,u_ij));
      }
      if(flujo[i][j]>0) {
        ady_residual[j].push_back(make_tuple(i,u_ij));
      }
    }
  }
  return ady_residual;
}
 
lint delta_camino(vector<tuple<lint,lint>>& P, lint t, vector<vector<lint>>&flujo,vector<vector<bool>>esta) {
int delta_min = INT_MAX ;
  int delta_i = undef;
  lint padre = get<0>(P[t]);
  lint hijo = t;
  while(hijo!= t-1) {//Mientras no llegue a la fuente
    lint u_ij = get<1>(P[hijo]);
    if (esta[padre][hijo]) {
      delta_i = u_ij - flujo[padre][hijo];
    }
    if(esta[hijo][padre]) {
      delta_i = flujo[hijo][padre];
    }
 
    delta_min = min(delta_min, delta_i); // Actualiza el mínimo
    hijo = padre;
    padre = get<0>(P[padre]);
  }
return delta_min;
}
 
void actualizar_flujo(vector<vec>& flujo, vector<tuple<lint,lint>>&P, lint delta, vector<vector<bool>>& esta) {
 
  lint hijo = flujo.size()-1;
  lint padre =get<0>(P[hijo]) ;
  while( hijo != flujo.size()-2) {//Mientras no llegue a la fuente
    if(esta[padre][hijo]){flujo[padre][hijo] += delta ;}
    if(esta[hijo][padre]){flujo[hijo][padre] -= delta;}
    hijo = padre;
    padre = get<0>(P[padre]);
  }
 
}
void imprimir(vector<vec> flujo,lint n) {
  for(lint i = 1; i < n+1; i++) {
    for(lint j = n+1; j <= 2*n; j++) {
      cout << flujo[i][j] << " ";
    }
    cout << endl;
  }
}
 
lint ff_dfs(  vector<vector<tuple<lint,lint>>>& ady,vector<vector<lint>>&flujo,lint n ,vector<vector<bool>>&esta) {
  lint flujito = 0;lint sumidero = ady.size()-1;
  lint delta_P = undef;
  vector<vector<tuple<lint,lint>>> ady_residual = Residual(ady, flujo);
vector<tuple<lint,lint>> padres =  dfs(ady_residual);
 
 while(get<0>(padres[sumidero]) != undef) {//Existe camino de aumento
   ady_residual = Residual(ady, flujo);
   delta_P = delta_camino(padres, sumidero,flujo,esta);
 
   actualizar_flujo(flujo,padres,delta_P,esta);
   //imprimir(flujo,n); cout << endl;
   padres =  dfs(ady_residual);
 }
  for(lint i = n+1; i <= 2*n ; i++) {
    flujito += flujo[i][flujo.size()-1];
  }
  return flujito;
}
 
 
int main() {
  //Hago una matriz de adyacencias y de flujo, facilita mucho el calculo.
  //Hacemos submit inicial para 2 matrices de 202x202 para ver si si no me paso de memoria
  //Pareciera ser que estoy bien para usarlas.
  lint n; lint m; lint ai; lint bi;
  cin >> n >> m;
  //Fila y columna 0 vacia, por comodidad
  //En la fila i, 0<i<n,n<=i_prima<2n, i_prima!=i tengo las aulas, ady[i][i_prima] = ai  <->  ady[i_prima][i] = aj <-> (i,i_prima) o (i_prima,i) mov valido.
  //SUMIDERO: Columna 2n+2, ady[i_prima][2n+1] = bi, n<=i_prima<2n, sumidero. Solo le llegan las capacidades de las aulas.
  //FUENTE: En la fila 2n+1, ady[n][i_prima] = ai. nodo n es fuente, unicas aristas salen hacia cada aula Ai.
lint tam_red = (2*n)+3;
  lint s =2*n+1;lint t = 2*n+2;
  vector<vector<tuple<lint,lint>>>ady(tam_red);
  vector<vector<bool>>esta(tam_red,vector<bool>(tam_red,false));
  vec Ai(n+1); vec Bi(n+1);
  vector<vector<lint>>flujo(tam_red, vector<lint>(tam_red,0));
  lint flujo_maximo_bis = 0; lint flujo_maximo_ais = 0;
  for(lint i = 1; i <= n; i++) {
    cin >> ai;
    Ai[i] = ai; //AGREGO CAPACIDAD s->ai y ai -> ai_prima pues se pueden quedar los que quieran
    flujo_maximo_ais += ai;
    ady[s].push_back(make_tuple(i,ai));
    ady[i].push_back(make_tuple(i+n,ai));
    esta[s][i] = true;esta[i][i+n] = true;
  }
  for(lint i = 1; i <= n; i++) {
    cin >> bi;
    flujo_maximo_bis +=bi;
    Bi[i] = bi;// AGREGO CAPACIDAD ai_prima -> t
    ady[n+i].push_back(make_tuple(t,bi));
    esta[n+i][t] = true;
  }
  lint p; lint q;lint p_prima;lint q_prima;
for(lint i= 1; i <= m; i++) {//AGREGO CAPACIDADES NODOS lintERMEDIOS.
  cin >> p >> q; p_prima = p+n;q_prima = q+n;
  ady[p].push_back(make_tuple(q_prima,Ai[p]));
  ady[q].push_back(make_tuple(p_prima,Ai[q]));
  esta[p][q_prima] = true;esta[q][p_prima] = true;
}
//Corramos DFS y si no llego en tiempo hago BFS.
  lint res = ff_dfs(ady,flujo,n,esta);
  if(res == flujo_maximo_bis && flujo_maximo_bis == flujo_maximo_ais) {//no deberia hacer falta, ver  
    cout << "YES"<< endl;
    imprimir(flujo,n);
  }else{cout << "NO"<< endl;}
  cout << endl;
 // imprimir(flujo);
  return 0;
 
}
