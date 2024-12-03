#include <iostream>
#include <vector>
using namespace std;

# https://www.spoj.com/problems/BORW/
int bw(vector<int>& v, int i, int ult_negro, int ult_blanco, vector<vector<vector<int>>>& Memo ){// ult_negro(blanco) son posiciones no los enteros en si.
    if(i == v.size()){ return 0;}
    if(Memo[i][ult_negro][ult_blanco] != -10){ return Memo[i][ult_negro][ult_blanco];}
    if( v[i] > v[ult_negro] && v[i] < v[ult_blanco] ){
        int b = bw(v, i+1,ult_negro, i, Memo );
        int n = bw(v, i+1,i, ult_blanco , Memo);
        int sp = bw(v, i+1,ult_negro, ult_blanco, Memo )+1;
        Memo[i][ult_negro][ult_blanco] = min(min(b,n),sp);
        return Memo[i][ult_negro][ult_blanco] ;
    }
    else if( v[i] > v[ult_negro]){
        int n = bw(v, i+1,i, ult_blanco, Memo );
        int sp = bw(v, i+1,ult_negro, ult_blanco, Memo )+1;
        Memo[i][ult_negro][ult_blanco] = min(n,sp);
        return Memo[i][ult_negro][ult_blanco];
    }
    else if(v[i] <v[ult_blanco]){
        int b = bw(v, i+1,ult_negro, i, Memo );
        int sp = bw(v, i+1,ult_negro, ult_blanco, Memo )+1;
        Memo[i][ult_negro][ult_blanco]  = min(b,sp);
        return Memo[i][ult_negro][ult_blanco];
    }
    else {
        Memo[i][ult_negro][ult_blanco] = bw(v, i + 1, ult_negro, ult_blanco, Memo) + 1;
        return  Memo[i][ult_negro][ult_blanco] ;
    }
}
 
 
//NEGRO ASCENDENTE BLANCO DESCENDENTE
 
int main() {
	int max_int = 2147483647;
    int N  ; int n;
    cin >> N;
    cout<< endl;
    while( N != -1){// Memoria: posicion i,ult_negro,ult_blanco va el min de numeros no pintados empezando en la pos i,
                    // con el ultimo numero negro y blanco en las posiciones ult_negro, ult_blanco respectivamente.
    vector<vector<vector<int>>>Memo (N+2,vector<vector<int>>(N+2,vector<int>(N+2,-10)));
        vector<int>v(N+2);
        v[0] = -1; v[1] = max_int;
        for(int i = 2; i < N+2; i++){
            cin>> n;
            v[i] = n;}
            cout << bw(v,2,0,1, Memo)<< endl;
        cin >> N;
    }
    return 0;
}
