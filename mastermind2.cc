#include <iostream>
#include <vector>
#include <random>
using namespace std;

struct Jugador {
    int Intents = 1;            //intents<=10
    vector<int> Jugades;        //Jugades.size()<=10.
    vector<string> Resultat;    //Resultat.size()<=10.
};

typedef vector<int> Vec;

int aleatori(int min, int max){
    //Pre: min i max son nombres enters.
    //Post: Retorna un numero aleatori entre min i max.
    static random_device device{};
    static default_random_engine engine{ device()};
    uniform_int_distribution<int> distribution{min, max};
    return distribution(engine);
}

Vec transforma(int c){
    //Pre: c es un nombre enter de 4 digits.
    //Post: Retorna un vector amb cadascun dels digits de c.
    Vec v(4);
    for(int i=0;i<4;++i){
        //Inv: El vector v conte els digits [4-i] del numero c a la posicio [3-(i-1)]
        v[3-i]=c%10;
        c/=10;
    }
    return v;
}

int invert_transform(const Vec &comb){
    //Pre: comb es un vector d'enters no buit.
    //Post: Retorna la concatenació d'aquest vector en un enter.
    int codi=0;
    for(int i=0;i<3;++i){
        //Inv: S'emmagatzema la concatenacio de comb a codi fins i-1.
        codi+=comb[i];
        codi*=10;
    }
    codi+=comb[3];
    return codi;
}

bool codiEsCorrecte(Vec &comb,int cod){
    //Pre: comb es un vector d'enters buit i cod es un nombre enter.
    /*Post: Retorna true si 1234<=cod<=9876, no es repeteix cap numero y si el codi no conte el numero 0.
            En cas contrari, retorna false.*/
    bool incorrect=false;
    if(cod>=1234 and cod<=9876){
        comb=transforma(cod);
        int i=0;
        while(not incorrect and i<4){
            //Inv: No es repeteix cap numero al codi i (i-1<4)
            int j=i+1;
            while(not incorrect and j<4){
                //Inv: No es repeteix cap numero al codi i-1<4, j-1<4 i ames en la posicio del vector j-1 no hi ha cap numero 0.
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
     /* visualitzacion = devuelve un string con la visualizacion de las posiciones bien colocadas, mal colocadas, y las que no estan.
    Ademas modifica final y correctas para actualizar el estado del juego.*/
    // Pre: jugada y comb son dos vectores de enteros no vacios, final es un boleano.
    // Post: Devuelve un string con la visualizacion de las posiciones bien colocadas, mal colocadas, y las que no estan.
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

bool esCorrecte(Vec &comb,const int &i,const int &num){
    //Pre: comb es un vector d'enters buit. i i num son nombres enters.
    //Post: retorna true si es repeteix el valor num en el vector comb fins la posició iesima.
    bool repetit=false;
    int j=0;
    while(not repetit and j<i){
        //Inv: No s'ha complit la condicio i a mes s'ha recorregut el vector fins j-1.
        if(num==comb[j]) repetit=true;
        else ++j;
    }
    return not repetit;
}

void modeAutomatic(Vec &comb){
    //Pre: comb es un vector d'enters buit.
    //Post: Crea una combinacio aleatoria de 4 digits amb numeros no repetits [1,...,9].
    unsigned int size=comb.size();
    int i=0;
    while(i<size){
        //Inv: S'emmagatzema en comb un numero aleatori no repetit en l'interval[1,...,9] al vector comb fins i-1.
        comb[i]=aleatori(1,9);
        if(esCorrecte(comb,i,comb[i])) ++i;
    }
}

void jugador_a(Vec &comb){
    //Pre: comb es un vector de enteros vacio.
    //Post: Introdueix una combinacio aleatoria de numeros no repetits en l'interval [1,...,9].
    int codi;
    bool continuar = false;
    while(not continuar){
        //Inv: no s'ha complert la condicio del bolea
        cout<<"Jugador A, escull el codi secret:"<<endl;
        cin>>codi;
        if(codiEsCorrecte(comb,codi)) continuar = true;
        else cout<<"Error, codi incorrecte."<<endl;
    }
}

void jugador_b(Vec &jugada, Jugador &C, Vec &comb, bool &final,int &correctas){
    // jugador_b = muestra por pantalla (si la jugada es correcta), el historial con todas las jugadas y resultados. 
    // Pre: jugada y comb son dos vectores de enteros no vacios, C es una tupla de tipo Jugador, final es un boleano y correctas es un entero.
    // Post: muestra por pantalla el historial con todas las jugadas y resultados.
    int cod;
    bool continuar =false;
    while(not continuar){
        cout<<"Jugador B, intent "<< C.Intents << ':'<<endl;
        cin>>cod;
        if(codiEsCorrecte(jugada,cod))continuar=true;
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
    else modeAutomatic(combinacion);
    while(not finalizado) jugador_b(jugadas, B, combinacion, finalizado, correctas);
    if(correctas==4) cout << "Felicitats jugador B!! Has guanyat!!" << endl;
    else cout << "Has esgotat els 10 intents." << endl << "El codi secret era: " <<invert_transform(combinacion)<<endl;
}