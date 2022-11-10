#include <iostream>
#include <vector>
using namespace std;

typedef vector< vector<int> >Matriznum;
typedef vector< vector<char> >Matrizfich;

bool repetits(const vector<int> &v){
//Pre: v es un vector no vacio con la combinación secreta del jugador A.
//Post: si dentro del vector v se repite algún valor, devuelve verdadero.
    bool rep=false;
    int i=0;
    while(not rep and i<4){
        int j=0;
        while(not rep and j<4){
            if(i!=j){
                if(v[i]==v[j]){rep=true;}
            }
            ++j;
        }
        ++i;
    }
    return rep;
}

int main(){
    int i=0; //contador de jugadas
    int e=0; //numero selector de jugada(fila de la matriz)
    const int Blau = 1, Groc = 2, Vermell = 3, Verd = 4; 
    const int Marro = 5, Rosa = 6, Violeta = 7, Taronja = 8;
    char Negre='X',Blanques='O';
    bool correct =false;
    vector<int> combinacio(4);
    cout<<"Jugador A, introdueix la combinació secreta."<<endl;
    while(not correct){
    int cod;    //codigo secreto
    cin>>cod;
    int aux=cod;
        if (cod>8765 or cod<1234){
            cout<<"La combinació "<<cod<<" no és correcte..."<<endl;
            cout<<"Recorda que la combinacio ha de tenir 4 numeros del 1 al 8 no repetits."<<endl;
        }else{
            int j=0;
            while(aux!=0){
                combinacio[3-j]=aux%10;
                aux/=10;
                ++j;
            }
            if(repetits(combinacio)){
                cout<<"La combinació "<<cod<<" no és correcte..."<<endl;
                cout<<"Recorda que la combinacio ha de tenir 4 numeros del 1 al 8 no repetits."<<endl;
            }else{correct=true;}
        } 
    }
    /*cout<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl;
    cout<<"Jugador B, és el teu torn. Esbrina el codi secret"<<endl;*/

}