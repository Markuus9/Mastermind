#include <iostream>
#include <vector>
using namespace std;

int main(){
    int i = 0; // contador de jugadas
    int e = 0; // numero para hacer calculos diversos
    int correctes = 0, incorrectes = 0, descolocades = 0;
    const int Blau = 1, Groc = 2, Vermell = 3, Verd = 4, Marro = 5, Rosa = 6, Violeta = 7, Taronja = 8;
    vector<int> combinacio(4);
    vector<int> jugada(4);
    char Negre = 'X', Blanques = 'O';
    bool finalitzat = false;
    while(i<15 and not finalitzat){
        if (i == 0){
            cout << "Torn del jugador 1" << endl << "Escolleix una combinació de 4 fiches: (Blau = 1, Groc = 2, Vermell = 3, Verd = 4, Marro = 5, Rosa = 6, Violeta = 7, Taronja = 8)" << endl;
            cin >> e;
            if (e<=9999){
                for(int l = 0; l<4; ++l){
                    combinacio[l] = e%10;
                    e = e/10;
                } 
            } else {
                cout << "Combinació incorrecta, torna a provar. " << endl;
                i = -1;
            }
        } else if(i == 1){
            cout << "Torn del jugador 2" << endl << "Intenta esbrinar la combinació oculta!" << endl << "????"<< endl;
            cin >> e;
            if (e<=9999){
                for(int k = 0; k<4; ++k){
                    jugada[k] = e%10;
                    e = e/10;
                }
                for(int f = 0; f<4; ++f){
                    if (f==3){
                        if(jugada[f]==combinacio[f]){
                            cout << Negre << endl;
                            correctes += 1;
                        } else {
                            for(int d = 0; d<4; ++d){
                                for(int s = 0; s<4; ++s){
                                    if(combinacio[d]==jugada[s]){
                                        cout << Blanques << endl;
                                        descolocades += 1;
                                    } else {
                                        cout << '-' << endl;
                                        incorrectes +=1;
                                    }
                                }
                            }
                        }
                    } else if(jugada[f]==combinacio[f]){
                        cout << Negre;
                        correctes += 1;
                    } else {
                        for(int d = 0; d<4; ++d){
                            for(int s = 0; s<4; ++s){
                                if(combinacio[d]==jugada[s]){
                                    cout << Blanques;
                                    descolocades += 1;
                                } else {
                                    cout << '-';
                                    incorrectes +=1;
                                }
                            }
                        }
                    }
                } 
            } else {
                cout << "Combinació incorrecta, torna a provar. " << endl;
            }
        } else{
            cout << "Mala sort, torna a provar. " << endl;
            finalitzat = true;
        }
        ++i;
    }
}
