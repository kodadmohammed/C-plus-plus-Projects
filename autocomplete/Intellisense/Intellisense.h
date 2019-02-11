#ifndef INTELLISENSE_H_INCLUDED
#define INTELLISENSE_H_INCLUDED

#include <string.h>
#include <iostream>
#include <vector> //permet l'utilisation des tableau dynamique; donc la taille peut varier
#include <fstream>

using namespace std;
    // Cette v�rifie la validit� d'un mot: s'il est correct ou non.
class Intellisence{
    //Essentiellement inspir� du code:   http://pascal.cormier.free.fr/correcteur/PHPcheck.tar.gz
    // Attributs
    private:
        string texte,mot;
        vector<string> dictionnaire;

    public:
        //Constructeurs
        Intellisence(){}

        //M�thode Principale: elle corrige un mot
        vector<string> Rechercher(string mot, vector<string> dictionnaire,char dico[]);

        //Fonction qui applique � chaque mot la fonction correct_word
        void correct_text(string texte);

        //Fonction qui charge les dictionnaires n�c�ssaires � la correction du texte
        vector<string> charge_dicos(char dico[]);

        //Fonction qui v�rifie si les deux premiers caract�res de chaque mot est bien une lettre ou un tiret
        bool is_correct(string mot);

        //Fonction qui enl�ve les accent d'un mot
        string no_accents(string mot);

        //La distance Levenshtein est d�finie comme le nombre minimal de caract�res qu'il faut remplacer,
        //ins�rer ou modifier pour transformer la cha�ne source en cible. La complexit� de l'algorithme est
        //en O(m*n), o� n et m sont les tailles respectives de source et cible.
        int Distance_Levenshtein(string source, string cible);
};

    //Classe pour la recherche d'Anagramme
class Anagramme{
    //Essentiellement inspir� du code:    http://people.sc.fsu.edu/~jburkardt/cpp_src/anagram/anagram.cpp

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
    char  tab_mot[10][50];//tableau contenant le mot � rechercher les anagrammes
    int   tab_carac[26];  //tableau des caract�res non valides d'un mot
    int   reql[10];
    int   rl;
    dent *sptr[26][40];
    int   stack[50];
    int   longueur_Mot;
    int   trl;

    void  do_search ( char *es, char *sw );
    void find_valid_words ( );
    int  analyse( string mot,char dico[]);
    void initialize ( );//initialise certaines donn�es
    int  parse_args ( int nargs );
    void read_dict (char dico[] );//lit chaque mot du dictionnaire
    void search ( int suggp, int sugg );
};

    //Classe permettant la traduction d'un mot en fran�ais r�ciproquement en anglais
class Traduction{
    //Essentiellement inspir� du code:  http://www.cppfrance.com/telecharger.aspx?ID=17677
    public:
    /* Ci-dessous les variables globales */

    string resultat;       // tableau qui contient le resultat de la traduction
    char mot_trouve[26];       // tableau qui prend le mot trouv� pour le renvoyer � la fonction afficer_resultat
    int pointeur[26][2];            // tableau qui enregistre les pointeurs sur les lettres

    /* D�claration des fonctions */
    string recherche(int langue, char *mot);
    string afficher_resultat(char *langue, char* mot);
    int recherche_ligne(char *ligne, char *mot);
    int aide_ligne(char *ligne, char *mot);
    void recherche_pointeur();// enregistre les pointeurs dans un tableau pointeur[26][2]
};
#endif // INTELLISENSE_H_INCLUDED
