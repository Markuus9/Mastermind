#include <iostream>
#include <vector>
#include <random>
using namespace std;

typedef vector< vector<int> >Matriznum;
typedef vector< vector<char> >Matrizfich;
typedef vector<int> Vec;

struct Jugador {
    int Intents = 1;
    vector<int> Jugades;
    vector<string> Resultat;
};

Vec transform(int c){
    //transform = trasforma el codigo a vector.
    //Pre: c es un numero entero positivo de 4 digitos.
    //Post: Devuelve un vector con cada digito del numero c.
    Vec v(4);
    for(int i=0;i<4;++i){
        //Inv: El vector v contiende los digitos [4-i] del numero c en la posicion [3-(i-1)]
        v[3-i]=c%10;
        c/=10;
    }
    return v;
}

bool codigo_correcto(Vec &comb,int cod){
    //codigo_correcto = comprueba si el codigo/intento es valido.
    //Pre: comb es un vector de enteros vacio y cod en un numero entero positivo.
    /*Post: Devuelve verdadero si 1234<=cod<=9876 y no se repite ningun numero y si el codigo no contiene 9 o 0.
            En caso contrario, devuelve falso.*/
    bool incorrect=false;
    if(cod>=1234 and cod<=9876){
        comb=transform(cod);
        int i=0;
        while(not incorrect and i<4){
            //Inv: no se repite ningun numero en el codigo e i-1<4
            int j=i+1;
            while(not incorrect and j<4){
                //Inv: no se repite ningun numero en el codigo, i-1<4, j-1<4 y además en la posicion del vector j-1 no se encuentra ningún 0.
                if(comb[i]==comb[j] or comb[j]==0) incorrect=true;
                else ++j;
            }
            if(not incorrect) ++i;
        }
    }
    else incorrect=true;
    return not incorrect;
}

string visualizacion(const Vec &jugada, const Vec comb, bool &final){
    string resultado;
    bool no_esta;
    int aciertos = 0;
    for(int i=0; i<4; ++i){
        no_esta = true;
        if(jugada[i]==comb[i]){
            resultado.push_back('X');
            no_esta = false;
            aciertos += 1;
        } else {
            for(int j=0; j<4; ++j){
                if(jugada[i]==comb[j]){
                    resultado.push_back('O');
                    no_esta = false;
                } 
            }
        }
        if (no_esta == true){
           resultado.push_back('-'); 
        }
        if (aciertos==4){
            final = true;
        }
    }
    return resultado;
}

void jugador_b(Vec &jugada, Jugador &C, Vec &comb, bool &final){
    //Pre: comb es un vector de enteros vacio.
    int cod;
    bool continuar =false;
    while(not continuar){
        cout<<"Jugador B, intent "<< C.Intents << ':'<<endl;
        cin>>cod;
        if(codigo_correcto(jugada,cod)){
            continuar=true;
        } else{
            cout<<"Error, codi incorrecte."<<endl;
            jugador_b(jugada, C, comb, final);
        } 
    }
    C.Resultat.push_back(visualizacion(jugada, comb, final));
    if (continuar == true){
        C.Jugades.push_back(cod);
        for(int i = 0; i<C.Intents; ++i){
            if (C.Intents==10){
                cout << 10 << "   " << C.Jugades[i] << "   " << C.Resultat[i] << endl;
            } else {
            cout << '0' << i+1 << "   " << C.Jugades[i] << "   " << C.Resultat[i] << endl;
            }
        }
        C.Intents += 1;
    }    
}

void jugador_a(Vec &comb){
    //Pre: comb es un vector de enteros vacio.
    int cod;
    bool continuar =false;
    while(not continuar){
        cout<<"Jugador A, escull el codi secret:"<<endl;
        cin>>cod;
        if(codigo_correcto(comb,cod)) continuar=true;
        else cout<<"Error, codi incorrecte."<<endl;
    }
}

bool triar_mode(bool &Mode){
    //Detecta quin mode es vol jugar.
    //Pre: Mode es un bolea.
    //Post: Torna el resultat escollit pel jugador, el qual pot ser Manual o Automatic.
    char input;
    cin >> input;
    if (input == 'A'){
        Mode = false;
    } else if(input == 'M'){
        Mode = true;
    } else {
       cout << "Error: Mode de joc incorrecte." << endl << "Quin mode de joc vols triar, Manual (M)/ Aleatori (A)? :"<< endl;
       triar_mode(Mode);
    }
    return Mode;
}

int aleatori(int min, int max){
    static random_device device{};
    static default_random_engine engine{ device()};
    uniform_int_distribution<int> distribution{min, max};
    return distribution(engine);
}

int main(){
    bool Manual, finalizado = false;
    Jugador B;
    Vec combinacion(4);
    Vec jugadas;
    cout << "BENVINGUT AL JOC MASTERMIND!!" << endl << "Quin mode de joc vols triar, Manual (M)/ Aleatori (A)? :" << endl;
    triar_mode(Manual);
    jugador_a(combinacion);
    while (not finalizado){
        jugador_b(jugadas, B, combinacion, finalizado);
    }
}
