#include "Intellisense.h"
#include <string>
#include <vector>
#include <algorithm>
#include <stdlib.h>
using namespace std;

vector<string> Intellisence::Rechercher(string mot, vector<string> dictionnaire, char dico[]){

    string mot_entre;
    string mot_dico;//mot inclus dans un dictionnaire lors de son parcours
    bool faute,correction; //Déterminerons si un mot contient oui
    //ou non une faute et s'il lui est associé une éventuelle correction.

    vector<string> tableau_De_Chaines;//vecteur contenat les éventuelles proposition de correction d'un mot
    if(is_correct( no_accents( mot) ) ){
        //On parcourt le mot pour le convertir en minuscule
        for(int i(0); i<(int)mot.size(); ++i)//le préfixe (int) permet de signé la variable qui le suit
            mot[i] = tolower(mot[i]);

        std::vector<string>::iterator iter;
        iter = std::find(dictionnaire.begin(),dictionnaire.end(), mot);
        if ( iter != dictionnaire.end() ) {//trouvé
            faute = false;
            correction = false;
        }
        else{//Si le mot n'est pas dans le dictionnaire

            //Parcourir les éléments du vecteur
            for(iter=dictionnaire.begin(); iter!=dictionnaire.end(); iter++){
                // L'itérateur est un pointeur sur un élément du vecteur. Pour avoir son contenu il faut donc mettre la petite étoile devant.
                int lev = Intellisence::Distance_Levenshtein(mot, (*iter));
                if(lev <= 1){
                    faute = true;
                    correction = true;
                    tableau_De_Chaines.push_back((*iter));
                }
            }
            if (!faute) { //S'il n'existe aucun mot à correspondance exacte et si le mot est trop éloigné du dico
                faute = true;
                correction = false;
            }
        }
        if (faute && correction){ //Si le mot est faux et il éxiste des corrections
            Anagramme anagr;
            cout << "\n  >> Anagrammes Possibles"<< endl;
            anagr.search ( anagr.parse_args ( anagr.analyse (mot,dico)+2 ), anagr.analyse (mot,dico) + 2 );//recherche d'éventuel(s) anagramme(s)
            cout << "\n  >> Propositions"<< endl;
            return tableau_De_Chaines;
        }
        else if (faute && !correction){//Le mot est faut est il n'existe aucune correction possible
            tableau_De_Chaines.push_back("Acune correction");
            Anagramme anagr;
            cout << "\n  >> Anagrammes Possibles"<< endl;
            anagr.search ( anagr.parse_args ( anagr.analyse (mot,dico)+2 ), anagr.analyse (mot,dico) + 2 );
            cout << "\n  >> Propositions" << endl;
            return tableau_De_Chaines;
        }
        else{
            tableau_De_Chaines.push_back("Le mot saisi est correct \n");
            Traduction traduc;
            tableau_De_Chaines.push_back("    Traduction en seconde langue: \n");
            //Choix du dictionnaire de traduction correspondant;
            if( strcmp(dico,"dictionnaires/fr.txt")==0  )
                if( !traduc.afficher_resultat((char*)"fr", (char*)mot.c_str() ).empty() )
                    tableau_De_Chaines.push_back( traduc.afficher_resultat((char*)"fr", (char*)mot.c_str() ) );
                else
                    tableau_De_Chaines.push_back( "Non disponible!" );
            else
                if( !traduc.afficher_resultat((char*)"an", (char*)mot.c_str() ).empty() )
                    tableau_De_Chaines.push_back( traduc.afficher_resultat((char*)"an", (char*)mot.c_str() ) );
                else
                    tableau_De_Chaines.push_back( "Non disponible!" );
            return tableau_De_Chaines;
        }
    }
    else{
        tableau_De_Chaines.push_back("Mot Invalide");
        return tableau_De_Chaines;
    }

}

void Intellisence::correct_text(string texte){
  /*string str = "Bonjour le monde ...";
	vector<string> VecStr;
	int nbTabl = Split(VecStr, str, "jj");
	cout << "Nb de parties : " << nbTabl << "\n" << endl;*/

}

int Intellisence::Distance_Levenshtein(string source, string cible) {
  //Etape 1
  int n = source.size();//taille de la chaine source
  int m = cible.size(); //taille de la chaine cible
  if (n == 0)
        return m;
  if (m == 0)
        return n;
  //Déclaration de type Tmatrix
  typedef std::vector< std::vector<int> > Tmatrix;
  Tmatrix matrix(n+1);

  for (int i = 0; i <= n; i++)
    matrix[i].resize(m+1);

  //Etape 2
  for (int i = 0; i <= n; i++)
    matrix[i][0]=i;

  for (int j = 0; j <= m; j++)
    matrix[0][j]=j;

  //Etape 3
  for (int i = 1; i <= n; i++) {
    const char s_i = source[i-1];
    //Etape 4
    for (int j = 1; j <= m; j++) {
      const char t_j = cible[j-1];
      //Etape 5
      int cost;
      if (s_i == t_j)
        cost = 0;

      else
        cost = 1;

      //Etape 6
      const int au_dessus = matrix[i-1][j];
      const int gauche = matrix[i][j-1];
      const int diag = matrix[i-1][j-1];
      int cell = min( au_dessus + 1, min(gauche + 1, diag + cost));

      if (i>2 && j>2) {
        int trans=matrix[i-2][j-2]+1;
        if (source[i-2]!=t_j) trans++;
        if (s_i!=cible[j-2]) trans++;
        if (cell>trans) cell=trans;
      }

      matrix[i][j]=cell;
    }
  }
  //Etape 7
  return matrix[n][m];
}

vector<string> Intellisence::charge_dicos(char dico[]){
    string ligne;
    ifstream Fichier(dico);
    while(getline(Fichier, ligne)){ //Au lieu de !Fichier.eof(); car on peut rencontrer une erreur avant la fin du fichier
        getline(Fichier,ligne); //On lit une ligne complète
        dictionnaire.push_back(ligne);//Ajout de la ligne courante dans le tableau de mots
    }
    Fichier.close();
    return dictionnaire;
}

bool Intellisence::is_correct(string mot){
    //On parcourt la chaîne pour la convertir en minuscule
    for(int i(0); i<(int)mot.size(); ++i)
         mot[i] = tolower(mot[i]);

    mot = Intellisence::no_accents(mot);

    //Vérifie sii la 1ère lettre est comprise de a à z ou est un tiret(-), de même pour la deuxième lettre
    //et le 1er caractère est strictement différent de inférieur(<)
    if (( ( (int)mot[0]>=97 && (int)mot[0]<=122) || (int)mot[0]==45) && (((int)mot[1]>=97 && (int)mot[1]<=122) || (int)mot[1]==45) && (int)mot[0]!=60)
        return true;
    else
        return false;
}

string Intellisence::no_accents(string mot){
    // tableau accents
    int pattern_accent[8]={-118,-126,-120,-119,-117,-105,-121, -109}; //tableau des codes ASCII corespondants aux caractères: è,é,ê,ë,ï,ù,ç et ô

    // tableau sans accents"
    string pattern_replace_accent("eeeeiuco");

    //pour chaque caractere si accentué le remplacer par un non accentué
    for(int i=0; i<(int)mot.size(); i++){
        for(int j=0; j<8; j++){
            if( (int)mot[i]== (int)pattern_accent[j])// si on trouve un accent dans la chaine on le remplace par le caractere non accentué correspondant
                mot[i] = pattern_replace_accent[j];
        }
    }
    return mot;
}


    //Recherche d'éventuel(s) Anagramme(s
void  Anagramme::do_search ( char *es, char *sw ){//**********************************************************141 lignes
  int cl;
  int co = 0;
  int cp;
  int i;
  int j;
  long na = 0;
  int rs;
  int rs2;
  int slev;
  dent *sp[50];
  int tn[26];

  find_valid_words();

  for ( i = 0; i < 50; i++ ){
    stack[i] = 0;
    sp[i] = NULL;
  }

  cl = longueur_Mot;
  slev = 0;
  stack[0] = 1;
  sp[0] = gptr[1];

  for ( ; ; ){
    for ( i = 0, j = 0, rs = longueur_Mot, rs2 = trl; i < rl && j <= slev; ){
      if ( reql[i] <= stack[j] ) {
        rs -= stack[j];
        if ( stack[j] == reql[i] ){
          rs2 -= stack[j];
          i++;
        }
        if ( rs < rs2 ){
          j = -1;
          break;
        }
        j++;
      }
      else{
        j = -1;
        break;
      }
    }

    if ( j == -1 )
      sp[slev] = NULL;

    while ( sp[slev] != NULL ){
      for ( i = 0; i < 26; i++ )
        tn[i] = tab_carac[i];

      for ( i = 0; i < stack[slev]; i++ ){
        if ( --tn[( (int) sp[slev]->word[i] ) - 'a'] < 0 )
          break;
      }

      if ( i == stack[slev] )
        break;

      else
        sp[slev] = sp[slev]->gl;
    }

    if ( sp[slev] == NULL ){
      if ( slev != 0 ){
        if ( stack[slev] < stack[slev - 1] && stack[slev] < cl ){
          stack[slev]++;
          if ( stack[slev] != stack[slev - 1] )
            sp[slev] = gptr[stack[slev]];

          else
            sp[slev] = sp[slev - 1];
        }
        else{
          stack[slev] = 0;
          sp[slev] = NULL;
          slev--;
          for ( i = 0; i < stack[slev]; i++ )
            tab_carac[( (int) sp[slev]->word[i] ) - 'a']++;

          sp[slev] = sp[slev]->gl;
          cl = cl + stack[slev];
        }
      }
      else{
        if ( stack[0] < longueur_Mot ){
          stack[0]++;
          sp[0] = gptr[stack[0]];
        }
        else
          break;

      }
    }
    else{
      if ( stack[slev] < cl ){
        for ( i = 0; i < 26; i++ )
          tab_carac[i] = tn[i];

        cl -= stack[slev];
        slev++;
        stack[slev] = 1;
        sp[slev] = gptr[1];
      }
      else{
        cp = longueur_Mot + slev + 3;

        if ( sw != NULL )
          cp = cp + 1 + strlen( sw );

        if ( 79 < co + cp ){
          co = cp;
        }
        else
          co = co + cp;

        for ( i = 0; i <= slev; i++ ){
            if( (int)strlen(sp[i]->word)== longueur_Mot && strcmp( sp[i]->word ,tab_mot[1]) ) { //selectionne uniquement les mots de longueur égale
                // à celle du mot entré et en ecarte le mot lui même...
                cout << endl <<" ->"<< sp[i]->word ;
            }
        }

        na++;
        sp[slev] = sp[slev]->gl;
      }
    }
  }
  cout << endl;
  return ;
}

void Anagramme::find_valid_words ( ){//**********************************************************************54
//
//  Purpose:
//
//    FIND_VALID_WORDS ??
//
  int c;
  long checked;
  dent *dptr;
  long found;
  int i;
  int j;
  int k;
  int tn[26];
  dent *tp[40];

  checked = 0;
  found = 0;

  for ( i = 0; i < 40; i++ )
    gptr[i] = NULL;

  for ( i = 0; i < 26; i++ ){
    for ( j = 1; j < 40; j++ ){
      dptr = sptr[i][j];
      while ( dptr != NULL ){
        for ( k = 0; k < 26; k++ )
          tn[k] = tab_carac[k];

        for ( k = 0; k < j; k++ ){
          c = dptr->word[k] - 'a';
          if ( --tn[c] < 0 )
            break;
        }

        if ( k == j ){
          if ( gptr[j] == NULL ){
            gptr[j] = dptr;
            tp[j] = dptr;
          }
          else{
            tp[j]->gl = dptr;
            tp[j] = dptr;
          }
          dptr->gl = NULL;
          found++;
        }
        dptr = dptr->len;
        checked++;
      }
    }
  }
  return;
}

int Anagramme::analyse (string mot, char dico[] ){//**********************************************************************31
//****************************************************************************80
//  analyse:
  initialize ( );
  read_dict (dico);

  int i = 0;
  int j = 0;
  int k = 0;
  char inp;

  do{
    inp = mot[k];
    if ( ' ' < inp ){//Vérifie si l'on a un caractère utlisatble dans un dictionnaire
      if ( j == 0 )
        i++;

      if ( 'A' <= inp && inp <= 'Z' )//Si le caractère courant est une majuscule
        inp = inp + 'a' - 'A';// correspondance du caractère en miniscule

      tab_mot[i][j++] = inp;
    }
    else{//si le caractère courant n'est pas utilisable
      if ( 0 < i )
        tab_mot[i][j] = '\0';
      j = 0;
    }
    k++;
  } while ( k != (int)mot.size()+1 );
  return ( i - 1 );//retourne la longueur du mot
}

void Anagramme::initialize ( ){//****************************************************************************29
//****************************************************************************80
//
//  Purpose:
//
//    INITIALIZE initializes certain data.
//
  int i;
  int j;
  bool VERBOSE = false;

  if ( VERBOSE )
  {
    cout << "\n";
    cout << "INITIALIZE:\n";
    cout << "  Initializing data...\n";
  }

  for ( i = 0; i < 26; i++ )
  {
    aptr[i] = NULL;
    for ( j = 1; j < 40; j++ )
    {
      sptr[i][j] = NULL;
      cptr[i][j] = NULL;
    }
  }
  return;
}

int Anagramme::parse_args ( int nargs ){//*******************************************************************62
//Purpose:
//  PARSE_ARGS parses the command line arguments.
//Parameters:
//    Output, int parse_args, the number of command line arguments.

  int c;
  int i;
  int j;
  int t;
//mise à zéro du tableau des caractères non valides d'un mot
  for ( i = 0; i < 26; i++ )
    tab_carac[i] = 0;

  longueur_Mot = strlen ( tab_mot[1] );

  for ( i = 0; i < longueur_Mot; i++ ){
    c = tab_mot[1][i];
    if ( c < 'a' || 'z' < c ) //si "c" n'est pas un miniscule
      break;

    tab_carac[c - 'a']++;
  }

  if ( i != longueur_Mot ){//si l'on est sortir un prématurément de la boucle précédente
    cout << "\n";
    cout << "ERREUR !!!"<<endl;
    cout << "  Ce mot comporte des caracteres non alphabetique."<<endl;
    exit ( 1 );
  }

  i = 2;
  rl = 0;
//
//  The optional numbers field forces the program to use words of specific lengths.
//
  while ( i < nargs ){
    if ( tab_mot[i][0] < '1' || '9' < tab_mot[i][0] )
      break;

    reql[rl++] = atoi( tab_mot[i++] );
  }
//
//  Descending sort the numbers list.
//
  for ( j = rl - 1; 0 < j; j-- ){
    for ( c = 0; c < j; c++ ){
      if ( reql[c] < reql[c + 1] ){
        t = reql[c];
        reql[c] = reql[c + 1];
        reql[c + 1] = t;
      }
    }
  }
//
//  Sum the numbers.
//
  for ( j = 0, trl = 0; j < rl; j++ )
    trl = trl + reql[j];

  return ( i );
}

void Anagramme::read_dict (char dico[] ){//*****************************************************************************103
//****************************************************************************80
//  Purpose:
//    READ_DICT reads the word list from the dictionary file.
//  Modified:
//    23 March 2013

  int ai;
  ifstream file_in;
  char *file_in_name = dico;
  int i;
  int long_Mot_Dico;
  dent *tdent;
  char *tword;
  char word[40];
  dent *wptr;
  int wi;
//
//  Open the dictionary file.
//
  file_in.open ( file_in_name );

  if ( !file_in ){
    cout << "\n";
    cout << "  Impossible de trouver le dictionnaire:\n";
    cout << file_in_name << "\n";
    exit ( 1 );
  }

  wptr = NULL;
  ai = 0;

  while ( 1 ){
    file_in.getline ( word, sizeof ( word ) );
    if ( file_in.eof ( ) ) //si l'on est arrivé à la fin du fichier
      break;

    long_Mot_Dico = strlen ( word );//longueur d'un mot du dictionnaire

    //Transformation des caractère majuscule en miniscule.
    for ( i = 0; i < long_Mot_Dico; i++ ){
      if ( 'A' <= word[i] && word[i] <= 'Z' )
        word[i] = word[i] + ( 'a' - 'A' );
      if ( word[i] < 'a' || 'z' < word[i] )
        break;
    }

    if ( i != long_Mot_Dico )
      continue;

    tdent = ( dent * ) malloc( DE_SIZE );
    tword = ( char * ) malloc( long_Mot_Dico + 1 );

    if ( tdent == NULL || tword == NULL ){
      cout << "\n";
      cout << "READ_DICT - Fatal error!\n";
      cout << "  malloc() failed\n";
      exit ( 1 );
    }

    wi = word[0] - 'a';
    strcpy( tword, word );
    tdent->word = tword;
    tdent->long_Mot_Dico = long_Mot_Dico;
    tdent->alph = NULL;
    tdent->len = NULL;

    if ( wptr == NULL )
      aptr[ai++] = tdent;

    else{
      if ( wptr->word[0] != word[0] )
        aptr[ai++] = tdent;

      wptr->alph = tdent;
    }

    if ( cptr[wi][long_Mot_Dico] == NULL ) {
      sptr[wi][long_Mot_Dico] = tdent;
      cptr[wi][long_Mot_Dico] = tdent;
    }
    else{
      cptr[wi][long_Mot_Dico]->len = tdent;
      cptr[wi][long_Mot_Dico] = tdent;
    }

    wptr = tdent;
  }
  file_in.close ( );
  return;
}

void Anagramme::search ( int suggp, int sugg ){//************************************************************46
//****************************************************************************80
//  Purpose:
//    SEARCH ??
  int c;
  int i;
  int j;
  int tn[26];

  if ( suggp == sugg ){
    do_search ( (char*)"", NULL );
    return;
  }

  for ( i = suggp; i < sugg; i++ ){
    cout << "Target letters: " << tab_mot[i] << "\n";

    for ( j = 0; j < 26; j++ )
      tn[j] = tab_carac[j];

    for ( j = 0; j < (int)strlen( tab_mot[i] ); j++ ){
      c = tab_mot[i][j];
      if ( c < 'a' || 'z' < c )
        break;
      if ( --tn[c - 'a'] < 0 )
        break;
    }

    if ( j == (int)strlen ( tab_mot[i] ) ){
      for ( j = 0; j < 26; j++ )
        tab_carac[j] = tn[j];

      longueur_Mot -= strlen( tab_mot[i] );
      do_search( (char*)"*** ", tab_mot[i] );

      for ( j = 0; j < (int)strlen( tab_mot[i] ); j++ )
        tab_carac[( (int) tab_mot[i][j] ) - 'a']++;

      longueur_Mot = longueur_Mot + strlen( tab_mot[i] );
    }
  }
  return;
}


// La fonction afficher_resultat a pour but d'appeler la fonction 'recherche' qui
// lui renvoi le nombre de mots trouvés, ensuite la fonction retoune le resultat trouvé
string Traduction::afficher_resultat(char *langue, char* mot){
  int lan; // la langue

  // on affecte la variable correspondante , selon la langue choisie par l'utilisateur
  if (strcmp(langue, "an") == 0 ){
    lan = 2; //Pour l'anglais
  }
  else{
    lan = 1; //Pour le français
  }
  return recherche(lan, mot); // on récupère la traduction trouvé
}

// La fonction recherche lance la recherche et appelle les sous fonctions recherche_ligne.
string Traduction::recherche(int langue, char *mot){
  char ligne[100]; // tableau qui recevra la ligne lue dans le fichier d'archive
  FILE *fichier;
  string nom_fichier; //contient le nom du fichier à ouvrir

  if(langue == 2 ) //Dico Anglais
    nom_fichier= "dictionnaires/en_fr.txt";
  else
    nom_fichier= "dictionnaires/fr_en.txt";

  //On ouvre le fichier d'archive
  fichier = fopen( nom_fichier.c_str() ,"r+");
  if(fichier == NULL){
    cout <<"impossible d'ouvrir "<< nom_fichier <<endl;
    return resultat;
  }
  else{
    if(langue == 2 )
      fseek(fichier, pointeur[mot[0] - 'a'][2], SEEK_SET); // on se place au bon endroit dans le fichier grâce aux resultats de la fonction recherche_pointeur
    else
      fseek(fichier, pointeur[mot[0] - 'a'][1], SEEK_SET); // idem

    while ( fgets(ligne, 100, fichier) != NULL){ // on prend la ligne jusqu'à la fin du fichier
        if(recherche_ligne(ligne, mot) != 0)
            resultat= mot_trouve ; // on copie le mot trouvé par la fonction recherhche_ligne dans la resultat
    }
    fclose(fichier);
    return resultat;
  }
}

// La fonction recherche_ligne prend en argument le mot donné par l'utilisateur et la ligne lue par la fonction recherche,
// si recherche_ligne trouve le mot donné elle place sa traduction dans la variable mot_trouve
int Traduction::recherche_ligne(char *ligne, char *mot){
  char *egal;
  char *avant;
  char *apres;

  if ( strstr(ligne, mot) != NULL ){        //On vérifie si ce mot est contenu dans la ligne prit en paramètre
      egal = strchr(ligne, '=');                         //on regarde la position du séparateur
      apres = egal+1;                                    //pointeur vers la chaine apres le =
      apres[strlen( apres )-1] = '\0';                   //on enleve le \n
      avant = ligne;                                     //on prend la ligne avant d'enlever tout ce qu'il y a apres le =
      avant[ strlen(ligne) - (strlen(apres)+1)] = '\0';  //on enleve tout ce qu'il y a après le '='

      if(strcmp( avant , mot ) == 0){
          strcpy( mot_trouve , apres); //on copie dans le tableau apres le mot apres le '='
          return 1;
	  }
      else
        return 0;
   }
   return 0;
}

// La fonction recherche_pointeur lit les fichiers d'archive, et repère toutes les expressions de la forme *a*
// afin de noter leur emplacement pour optimiser la l'execution du programme.
void Traduction::recherche_pointeur(){
  char lettre[4];
  FILE *fichier;
  char ligne[100];
  int j, res;

/* les pointeurs du fichier anglais.txt */

  fichier = fopen("dictionnaires/en_fr.txt","r+");

  if(fichier == NULL)
    printf("impossible d'ouvrir dictionnaires/en.txt \n");
  else{
    while ( fgets(ligne, 100, fichier) != NULL){
        for(j=0;j < 26;j++){
            lettre[0]='*';
            lettre[1]='a'+j;
            lettre[2]='*';
            lettre[3]='\0';

            if ( strstr(ligne, lettre) != NULL ){
                res = ftell(fichier);
                pointeur[j][2] = res + strlen(lettre) - 3 ; //on place la position courrante de l'expression trouvée dans un tableau pointeur
                break;
            }
         }
    }
    fclose(fichier);
  }

  // les pointeurs du fichier fr_en.txt
  fichier = fopen("dictionnaires/fr_en.txt","r+");
  if(fichier == NULL)
    cout<<"impossible d'ouvrir dictionnaires/fr_en.txt"<<endl;
  else{
    while ( fgets(ligne, 100, fichier) != NULL){
        for(j=0;j < 26;j++){
            lettre[0]='*';
            lettre[1]='a'+j;
            lettre[2]='*';
            lettre[3]='\0';
            if ( strstr(ligne, lettre) != NULL ){
                res = ftell(fichier);
                pointeur[j][1] = res + strlen(lettre) - 2 ; //idem
                break;
            }
        }
    }
    fclose(fichier);
  }
}
