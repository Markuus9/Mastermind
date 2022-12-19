#include <iostream>
#include <vector>
#include <random>
using namespace std;

typedef vector<int> Vec;

struct Jugador {
    int Intents = 1;
    vector<int> Jugades;    
    vector<string> Resultat;
};

int aleatori(int min, int max){
    static random_device device{};
    static default_random_engine engine{ device()};
    uniform_int_distribution<int> distribution{min, max};
    return distribution(engine);
}

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

int invert_transform(const Vec &comb){
    //Pre: comb es un vector de enteros no vacio.
    //Post: devuelve el valor de la concatenacion de cada posicion del vector.
    unsigned int size= comb.size();
    int codigo=0;
    for(int i=0;i<size-1;++i){
        //Inv: Se almacena en codigo la concatencacion de los valores de comb hasta i-1.
        codigo+=comb[i];
        codigo*=10;
    }
    codigo+=comb[3];
    return codigo;
}

bool codigo_correcto(Vec &comb,int cod){
    //codigo_correcto = comprueba si el codigo/intento es valido.
    //Pre: comb es un vector de enteros vacio y cod en un numero entero positivo.
    /*Post: Devuelve verdadero si 1234<=cod<=9876 y no se repite ningun numero y si el codigo no contiene 0.
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
                if(comb[i]==comb[j] or comb[j]==0) incorrect = true;
                else ++j;
            }
            if(not incorrect) ++i;
        }
    }
    else incorrect=true;
    return not incorrect;
}

string visualizacion(const Vec &jugada, const Vec &comb, bool &final, int &correctas){
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
            correctas=aciertos;
        }
    }
    return resultado;
}

bool triar_mode(bool &Mode){
    //Detecta quin mode es vol jugar.
    //Pre: Mode es un bolea.
    //Post: Torna el resultat escollit pel jugador, el qual pot ser Manual o Automatic.
    char input;
    cin >> input;
    if (input == 'A') Mode = false;
    else if(input == 'M') Mode = true;
    else {
       cout << "Error: Mode de joc incorrecte." << endl << "Quin mode de joc vols triar, Manual (M)/ Aleatori (A)? :"<< endl;
       triar_mode(Mode);
    }
    return Mode;
}

bool corrector_automatico(Vec &comb,int i,int num){
    //Pre: comb es un vector de enteros no vacio, i y num son enteros positivos.
    //Post: Devuelve true si no aparece num en comb hasta i, en caso contrario devuelve false.
    bool repetit=false;
    int j=0;
    while(not repetit and j<i){
        //Inv: no se ha cumplido la condicion y se ha recorrido comb hasta j-1.
        if(num==comb[j]) repetit=true;
        else ++j;
    }
    return not repetit;
}

void mod_automatic(Vec &comb){
    //Pre: comb es un vector de enteros vacio.
    //Post: Guarda comb con numeros aleatorios [1,..,9] en cada posicion del vector.
    unsigned int size=comb.size();
    int i=0;
    while(i<size){
        comb[i]=aleatori(1,9);
        if(corrector_automatico(comb,i,comb[i])) ++i;
    }
}

void jugador_a(Vec &comb){
    //Pre: comb es un vector de enteros vacio.
    //Post:-----------------------------------
    int cod;
    bool continuar = false;
    while(not continuar){
        cout<<"Jugador A, escull el codi secret:"<<endl;
        cin>>cod;
        if(codigo_correcto(comb,cod)) continuar = true;
        else cout<<"Error, codi incorrecte."<<endl;
    }
}

void jugador_b(Vec &jugada, Jugador &C, Vec &comb, bool &final,int &correctas){
    //Pre: comb es un vector de enteros vacio.
    int cod;
    bool continuar =false;
    while(not continuar){
        cout<<"Jugador B, intent "<< C.Intents << ':'<<endl;
        cin>>cod;
        if(codigo_correcto(jugada,cod))continuar=true;
        else cout<<"Error, codi incorrecte."<<endl;
    }
    C.Resultat.push_back(visualizacion(jugada, comb, final,correctas));
    if (continuar == true){
        C.Jugades.push_back(cod);
        for(int i = 0; i<C.Intents; ++i){
            if (C.Intents==10){
                cout << 10 << "   " << C.Jugades[i] << "   " << C.Resultat[i] << endl;
                final=true;
            } else {
            cout << '0' << i+1 << "   " << C.Jugades[i] << "   " << C.Resultat[i] << endl;
            }
        }
        C.Intents += 1;
    }    
}

int main(){
    bool Manual, finalizado = false;
    int correctas=0;
    Jugador B;
    Vec combinacion(4);
    Vec jugadas;
    cout << "BENVINGUT AL JOC MASTERMIND!!" << endl << "Quin mode de joc vols triar, Manual (M)/ Aleatori (A)? :" << endl;
    if(triar_mode(Manual)) jugador_a(combinacion);
    else mod_automatic(combinacion);
    while(not finalizado) jugador_b(jugadas, B, combinacion, finalizado, correctas);
    if(correctas==4) cout << "Felicitats jugador B!! Has guanyat!!" << endl;
    else cout << "Has esgotat els 10 intents." << endl << "El codi secret era: " <<invert_transform(combinacion)<<endl;
}
