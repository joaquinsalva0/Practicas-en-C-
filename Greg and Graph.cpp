#include <iostream>
#include <queue>
#include <vector>
using namespace std;

#https://codeforces.com/problemset/problem/295/B

/* La idea es usar el invariante del algoritmo de dantzig .
 * Dantzig asegura que en la iteracion k tenemos la distancia minima todos-todos
 * del digrado inducido por los primeros k vertices.
 * Aca queremos calcular la distancia minima todos-todos habiendo agregado un nodo y luego agregarlo
 * Iterando de fin a principio en el vector orden.
 * Para el valor a prlong long intear solo lo acumulo en una suma y luego asigno al vector res.
 */
void print_mat(vector<vector<long long int>> mat) {
 long long int n = mat.size()-1;
    for(long long int i = 1; i <= n; i++) {
        for(long long int j = 1; j <= n; j++) {
            cout << mat[i][j] << " ";
        }
        cout <<endl;
    }
cout << endl;
}
 
void actualizar_ij(long long int k_camara, vector<vector<long long int>>& D,vector<bool>& esta ) {
    for(long long int i = 1; i < esta.size(); i++) {
        if(esta[i]) {
            for(long long int j = 1; j < esta.size(); j++) {
                if(esta[j]){
                D[i][j] = min(D[i][j],D[i][k_camara] + D[k_camara][j]);
                }
            }
        }
    }
}
void actualizar_ik( long long int k, vector<vector<long long int>>& D,vector<bool>& esta ) {
    for(long long int i = 1; i < esta.size(); i++) {
        if(esta[i]) {
            for(long long int j = 1; j <esta.size(); j++) {
                if(esta[j] ){
                    D[i][k] = min(D[i][k],D[i][j] + D[j][k]);
                    D[k][i] = min(D[k][i], D[k][j] + D[j][i]);
                }
            }
        }
    }
}
 
long long int sumar(vector<vector<long long int>>& mat,vector<bool>& esta) {
    long long int suma = 0;
    for(long long int i = 1; i < mat.size(); i++) {
        for(long long int j =1; j < mat.size(); j++) {
            if(esta[j] && esta[i]) {
                suma+= mat[i][j];
            }
        }
    }
 
    return suma;
}

void dantzig(vector<vector<long long int>>& distancias, vector<long long int>& orden) {
    long long int n = distancias.size()-1;
    vector<bool>esta(n+1,false);
    vector<long long int> res(n+1);
    for(long long int k = n; k>= 1; k--) {
        long long int k_camara = orden[k];
        esta[k_camara] = true;
        actualizar_ik(k_camara,distancias,esta);
        actualizar_ij(k_camara,distancias, esta );
        res[k] = sumar(distancias,esta);
        }
for(long long int r = 1; r <= n; r++) {cout<< res[r]<<" ";}
}
 
int main() {
    long long int n;
    cin >> n;
 vector<long long int>orden(n+1);
    vector<vector<long long int>>distancias(n+1,vector<long long int>(n+1));
    vector<vector<long long int>>distancias_originales(n+1,vector<long long int>(n+1));
    for(long long int i =1 ; i <= n; i ++) {
        for(long long int j = 1; j <= n; j ++) {
            cin >> distancias[i][j];
            }
    }
    distancias_originales = distancias;
    for(long long int k =1; k <= n; k++) {
        cin >> orden[k];
    }
     dantzig(distancias,orden);

    return 0;
}
