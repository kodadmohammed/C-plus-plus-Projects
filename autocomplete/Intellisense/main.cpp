#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Intellisense.h"

using namespace std;

int main(){
    Intellisence correcteur;

    string mot;
    char *dico;
    string reponse;
    vector<string> tab;

    for ( ;; ){
      cout << "\n";
      cout << " Veillez entrez la langue du dictionnaire:  "<<endl;
      cout<<endl;
      cout << " > 1- pour le francais"<<endl;
      cout << " > 2- pour l'anglais"<<endl;
      cout << " > Tout autres touches pour quitter le programme"<<endl;
      cout<<endl;
      cout << "  -> ";
      cin >> reponse;
      if( reponse == "1")
        dico= "dictionnaires/fr.txt";
      else if ( reponse == "2")
        dico="dictionnaires/en.txt";
      else{
        cout << "Bye, a Bientot \n";
        exit(0);
      }
      cout<<endl;
      cout << " Veillez entrez saisie un mot:  ";
      cout<<endl;
      cout << "  -> ";
      cin >> mot;

      cout << " \n\t***Recherche dans le dictionnaire***"<<endl;
      tab = correcteur.Rechercher(mot, correcteur.charge_dicos( dico ), dico);
      cout<<endl;

      for(int i(0); i< (int)tab.size(); i++)
        cout<<" > "<<tab[i]<<endl;

      cout<<endl<<"   Voulez-vous continuez (oui/non) :   ";
      cin >> reponse;
      if( reponse=="oui")
        system("cls");
      else{
        cout<<endl<<endl<<"    Au revoir."<<endl<<endl;
        exit(0);
      }
    }
  return 0;
}
