#include <iostream>
#include <vector>
#include <random>
using namespace std;

struct Jugador {
    int Intents = 1;            //Inv: Intents<=10
    vector<int> Jugades;        //Inv: Jugades.size()<=10.
    vector<string> Resultat;    //Inv: Resultat.size()<=10.
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

string visualizacio(const Vec &jugada, const Vec &comb, bool &final, int &correctes){
     /* Visualització = retorna un string amb la visualització de les posicions ben col·locades, mal col·locades, i les que no hi son.
     A més, modifica final i correctes per actualitzar l'estat del joc.*/
    // Pre: Jugada i comb són dos vectors de sencers no buits, final és un boleà.
    // Post: Retorna un string amb la visualització de les posicions ben col·locades, mal col·locades, i les que no estan.
    string resultat;
    bool no_hi_es;
    int encerts = 0;
    for(int i=0; i<4; ++i){
        no_hi_es = true;
        if(jugada[i]==comb[i]){
            resultat.push_back('X');
            no_hi_es = false;
            encerts += 1;
        } else {
            for(int j=0; j<4; ++j){
                if(jugada[i]==comb[j]){
                    resultat.push_back('O');
                    no_hi_es = false;
                } 
            }
        }
        if (no_hi_es == true){
           resultat.push_back('-'); 
        }
        if (encerts==4){
            final = true;
            correctes=encerts;
        }
    }
    return resultat;
}

bool triar_mode(bool &Mode){
    //Detecta quin mode es vol jugar.
    //Pre: Mode es un bolea.
    //Post: Torna el resultat escollit pel jugador, el qual pot ser Manual o Automatic.
    char input;
    cout << "Quin mode de joc vols triar, Manual (M)/ Aleatori (A)? : " << endl;
    cin >> input;
    if (input == 'A') Mode = false;
    else if(input == 'M') Mode = true;
    else {
       cout << "Error: Mode de joc incorrecte." << endl;
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
    unsigned int size=comb.size(),i=0;
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
    cout<<endl;
    while(not continuar){
        //Inv: no s'ha complert la condicio del bolea
        cout<<"Jugador A, escull el codi secret: "<<endl;
        cin>>codi;
		if(codiEsCorrecte(comb,codi)) continuar = true;
		else cout<<"Error: Codi secret incorrecte."<<endl;
    }
}

void jugador_b(Vec &jugada, Jugador &C, Vec &comb, bool &final,int &correctes){
    // jugador_b = mostra per pantalla (si la jugada és correcta), l'historial amb totes les jugades i resultats. 
    // Pre: jugada i comb són dos vectors de sencers no buits, C és una tupla de tipus Jugador, final és un boleà i correctes és un sencer.
    // Post: mostra per pantalla l'historial amb totes les jugades i els resultats.
    bool continuar =false;
    int cod;
    while(not continuar){
        cout<<"Jugador B, intent "<< C.Intents << ": "<<endl;
        cin>>cod;
        if(codiEsCorrecte(jugada,cod))continuar=true;
        else cout<<"Error: Intent incorrecte."<<endl;
    }
    C.Resultat.push_back(visualizacio(jugada, comb, final,correctes));
    if (continuar == true){
        C.Jugades.push_back(cod);
        cout<<"Jugades:"<<endl;
        for(int i = 0; i<C.Intents; ++i){
            if (i+1==10){
                cout << 10 << "   " << C.Jugades[i] << "   " << C.Resultat[i] << endl;
                final=true;
            } else {
            cout << '0' << i+1 << "   " << C.Jugades[i] << "   " << C.Resultat[i] << endl;
            }
        }
        C.Intents += 1;
        cout<<endl;
    }    
}

int main(){
    bool Manual, finalitzat = false;
    int correctes=0;
    Jugador B;
    Vec combinació(4);
    Vec jugades;
    cout << "BENVINGUT AL JOC MASTERMIND!!" << endl<<endl;
    if(triar_mode(Manual)) jugador_a(combinació);
    else modeAutomatic(combinació);
    cout<<endl;
    while(not finalitzat) jugador_b(jugades, B, combinació, finalitzat, correctes);
    if(correctes==4) cout << "Felicitats jugador B!! Has guanyat!!" << endl;
    else cout << "Has esgotat els 10 intents." << endl << "El codi secret era " <<invert_transform(combinació)<<endl;
}