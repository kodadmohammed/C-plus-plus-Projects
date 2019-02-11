#ifndef INTELLISENSE_H_INCLUDED
#define INTELLISENSE_H_INCLUDED

#include <string.h>
#include <iostream>
#include <vector> //permet l'utilisation des tableau dynamique; donc la taille peut varier
#include <fstream>

using namespace std;
    // Cette vérifie la validité d'un mot: s'il est correct ou non.
class Intellisence{
    //Essentiellement inspiré du code:   http://pascal.cormier.free.fr/correcteur/PHPcheck.tar.gz
    // Attributs
    private:
        string texte,mot;
        vector<string> dictionnaire;

    public:
        //Constructeurs
        Intellisence(){}

        //Méthode Principale: elle corrige un mot
        vector<string> Rechercher(string mot, vector<string> dictionnaire,char dico[]);

        //Fonction qui applique à chaque mot la fonction correct_word
        void correct_text(string texte);

        //Fonction qui charge les dictionnaires nécéssaires à la correction du texte
        vector<string> charge_dicos(char dico[]);

        //Fonction qui vérifie si les deux premiers caractères de chaque mot est bien une lettre ou un tiret
        bool is_correct(string mot);

        //Fonction qui enlève les accent d'un mot
        string no_accents(string mot);

        //La distance Levenshtein est définie comme le nombre minimal de caractères qu'il faut remplacer,
        //insérer ou modifier pour transformer la chaîne source en cible. La complexité de l'algorithme est
        //en O(m*n), où n et m sont les tailles respectives de source et cible.
        int Distance_Levenshtein(string source, string cible);
};

    //Classe pour la recherche d'Anagramme
class Anagramme{
    //Essentiellement inspiré du code:    http://people.sc.fsu.edu/~jburkardt/cpp_src/anagram/anagram.cpp

    typedef struct dictent{
      struct dictent *alph;
      struct dictent *len;
      struct dictent *gl;
      char *word;
      int long_Mot_Dico;
    } dent;
public:
    # define DE_SIZE    ( sizeof ( dent ) )
    dent *aptr[26];
    dent *cptr[26][40];
    dent *gptr[40];
    char  tab_mot[10][50];//tableau contenant le mot à rechercher les anagrammes
    int   tab_carac[26];  //tableau des caractères non valides d'un mot
    int   reql[10];
    int   rl;
    dent *sptr[26][40];
    int   stack[50];
    int   longueur_Mot;
    int   trl;

    void  do_search ( char *es, char *sw );
    void find_valid_words ( );
    int  analyse( string mot,char dico[]);
    void initialize ( );//initialise certaines données
    int  parse_args ( int nargs );
    void read_dict (char dico[] );//lit chaque mot du dictionnaire
    void search ( int suggp, int sugg );
};

    //Classe permettant la traduction d'un mot en français réciproquement en anglais
class Traduction{
    //Essentiellement inspiré du code:  http://www.cppfrance.com/telecharger.aspx?ID=17677
    public:
    /* Ci-dessous les variables globales */

    string resultat;       // tableau qui contient le resultat de la traduction
    char mot_trouve[26];       // tableau qui prend le mot trouvé pour le renvoyer à la fonction afficer_resultat
    int pointeur[26][2];            // tableau qui enregistre les pointeurs sur les lettres

    /* Déclaration des fonctions */
    string recherche(int langue, char *mot);
    string afficher_resultat(char *langue, char* mot);
    int recherche_ligne(char *ligne, char *mot);
    int aide_ligne(char *ligne, char *mot);
    void recherche_pointeur();// enregistre les pointeurs dans un tableau pointeur[26][2]
};
#endif // INTELLISENSE_H_INCLUDED
