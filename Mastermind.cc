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

/* Genera un nombre aleatori entre els nombres min i max (ambdos 
 * inclosos).*/
// Pre: min < max
// Post: retorna un nombre generat aleat`oriament entre min i max.
int aleatori(int min, int max){
    static random_device device{};
    static default_random_engine engine{ device()};
    uniform_int_distribution<int> distribution{min, max};
    return distribution(engine);
}

// Genera un vector a partir d'un codi de 4 digits.
// Pre: c es un nombre enter de 4 digits.
// Post: Retorna un vector amb cadascun dels digits de c.
Vec transforma(int c){
    Vec v(4);
    for(int i=0;i<4;++i){
        /* Inv: El vector v conte els digits [4-i] del numero c a la 
         * 		posicio [3-(i-1)]*/
        v[3-i]=c%10;
        c/=10;
    }
    return v;
}

/* Genera un numero a partir de la concatenació dels nombres del vector 
 * comb.*/
// Pre: comb es un vector d'enters no buit.
// Post: Retorna la concatenació d'aquest vector en un enter.
int invert_transform(const Vec &comb){
    int codi=0;
    for(int i=0;i<3;++i){
        //Inv: S'emmagatzema la concatenacio de comb a codi fins i-1.
        codi+=comb[i];
        codi*=10;
    }
    codi+=comb[3];
    return codi;
}

/* Comprova que el codi manual no presenti un numero repetit, sigui de 
 * 4 digits i no contingui cap 0.*/
// Pre: comb es un vector d'enters buit i cod es un nombre enter.
/* Post: Retorna true si 1234<=cod<=9876, no es repeteix cap numero 
 * 		y si el codi no conte el numero 0. En cas contrari, retorna 
 * 		false.*/
bool codiEsCorrecte(Vec &comb,int cod){
    bool incorrect=false;
    if(cod>=1234 and cod<=9876){
        comb=transforma(cod);
        int i=0;
        while(not incorrect and i<4){
            // Inv: No es repeteix cap numero al codi i (i-1<4)
            int j=i+1;
            while(not incorrect and j<4){
                /* Inv: No es repeteix cap numero al codi i-1<4, j-1<4 i 
                 * ames en la posicio del vector j-1 no hi ha cap numero 
                 * 0.*/
                if(comb[i]==comb[j] or comb[j]==0) incorrect = true;
                else ++j;
            }
            if(not incorrect) ++i;
        }
    }
    else incorrect=true;
    return not incorrect;
}

/* Visualització = retorna un string amb la visualització de les 
 * posicions ben col·locades, mal col·locades, i les que no hi son.
 * A més, modifica final i correctes per actualitzar l'estat del joc.*/
/* Pre: Jugada i comb són dos vectors de sencers no buits, final és un 
 * 		boleà.*/
/* Post: Retorna un string amb la visualització de les posicions ben 
 * col·locades, mal col·locades, i les que no estan.*/
string visualizacio(const Vec &jugada, const Vec &comb, bool &final, int &correctes){
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

// Detecta quin mode es vol jugar.
// Pre: Mode es un bolea.
/* Post: Torna el resultat escollit pel jugador, el qual pot ser 
 * 		Manual o Automatic.*/
bool triar_mode(bool &Mode){
    string input;
    cout<<"Quin mode de joc vols triar, Manual (M)/ Aleatori (A)? : ";
    cout<<endl;
    cin>>input;
    if(input[0] == 'A' and input.size()==1) Mode = false;
    else if(input[0] == 'M' and input.size()==1) Mode = true;
    else {
       cout << "Error: Mode de joc incorrecte." << endl;
       triar_mode(Mode);
    }
    return Mode;
}

/* Comprova que el codi automatic no presenti un numero repetit, sigui 
 * de 4 digits i no contingui cap 0.*/
// Pre: comb es un vector d'enters buit. i i num son nombres enters.
/* Post: retorna true si es repeteix el valor num en el vector comb fins 
 * 		la posició iesima.*/
bool esCorrecte(Vec &comb,const int &i,const int &num){
    bool repetit=false;
    int j=0;
    while(not repetit and j<i){
        /* Inv: No s'ha complit la condicio i a mes s'ha recorregut el 
         * 		vector fins j-1.*/
        if(num==comb[j]) repetit=true;
        else ++j;
    }
    return not repetit;
}

// Genera el codi secret quan el mode de joc es automatic.
// Pre: comb es un vector d'enters buit.
/* Post: Crea una combinacio aleatoria de 4 digits amb numeros no 
 * 		repetits [1,...,9].*/
void modeAutomatic(Vec &comb){
    unsigned int size=comb.size(),i=0;
    while(i<size){
        /* Inv: S'emmagatzema en comb un numero aleatori no repetit en 
         * 		l'interval[1,...,9] al vector comb fins i-1.*/
        comb[i]=aleatori(1,9);
        if(esCorrecte(comb,i,comb[i])) ++i;
    }
}
/* Mostra per pantalla si el codi secret introduit manualment es 
 * incorrecte.*/
// Pre: comb es un vector de enteros vacio.
/* Post: Introdueix en comb una combinacio aleatoria de numeros no 
 * 		repetits de 1 a 9.*/
void jugador_a(Vec &comb){
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

/* Mostra per pantalla (si la jugada és correcta), l'historial amb 
 * totes les jugades i resultats.*/ 
/* Pre: jugada i comb són dos vectors de sencers no buits, C és una 
 * 		tupla de tipus Jugador, final és un boleà i correctes és un 
 * 		sencer.*/
/* Post: mostra per pantalla l'historial amb totes les jugades i els 
 * 		resultats.*/
void jugador_b(Vec &jugada, Jugador &C, Vec &comb, bool &final,int &correctes){
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
                cout<<10<< "   "<<C.Jugades[i]<<"   "<<C.Resultat[i];
                final=true;
            } else {
            cout<<'0'<<i+1<<"   "<<C.Jugades[i]<<"   "<<C.Resultat[i];
            }
            cout<<endl;
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
    cout<<"BENVINGUT AL JOC MASTERMIND!!"<<endl<<endl;
    if(triar_mode(Manual)) jugador_a(combinació);
    else modeAutomatic(combinació);
    cout<<endl;
    while(not finalitzat){ 
		jugador_b(jugades, B, combinació, finalitzat, correctes);
	}
    if(correctes==4) cout<<"Felicitats jugador B!! Has guanyat!!";
    else{
		cout<<"Has esgotat els 10 intents."<<endl; 
		cout<<"El codi secret era "<<invert_transform(combinació);
	}
	cout<<endl;
}
