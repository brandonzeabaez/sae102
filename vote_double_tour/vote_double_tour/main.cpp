/**
 *  @date : 22 octobre 2024
 *  @author : Alain Casali
 *  @Brief : une prmière réponse à la SAE 1.02 de 24/25
**/
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

/**
 * @brief litUneString
 * @return la chaine lue sauf si :
 * (1) on une boulette sur l'entrée
 * (2) on trouve un commentaire sur l'entrée
 * le commentaire est matérialisé par la chaine "//"
 */

string litUneString (){
    string uneChaine;
    while (true){
        getline (cin, uneChaine);
        if ((!cin) || (uneChaine.substr(0,2) != "//")) break;
    }
    return uneChaine;
}

int litUnEntier (){
    string uneChaine;
    while (true){
        getline (cin, uneChaine);
        if ((!cin) || (uneChaine.substr(0,2) != "//")) break;
    }
    return stoi(uneChaine);
}

struct participant {
    string nom;
    string prenom;
    int vote;
};

bool compare2part (const participant & p1, const participant & p2){
    return p1.prenom < p2.prenom;
}

void affichVectString (const vector<string> & v){
    for (const string & val : v)
        cout << val << '\t';
    cout << endl;
}

void affichVectParticipants (const vector<participant> & vPart){
    for (const participant & part : vPart){
        cout << part.nom << endl;
        cout << part.prenom << endl;
        cout << part.vote << endl;
    }
}

int main()
{
    /* on va detecter les votes des votants
    tout en virant les commentaires*/

    /* on vote sur 4 glaces */
    vector <string> vVote;
    for (unsigned i (0); i < 4; ++i)
        vVote.push_back(litUneString());
    map <string, int> votes;
    //On initialise le nombre de voies
    votes [vVote[0]] = 0;
    votes [vVote[1]] = 0;
    votes [vVote[2]] = 0;
    votes [vVote[3]] = 0;

    //On lit les datas du clavier, et on les stocke
    vector<participant> vParticipant;
    for (unsigned i (0); i < 10; ++i){
        string nom (litUneString());
        string prenom  (litUneString());
        int numVote (litUnEntier());
        vParticipant.push_back(participant{nom, prenom, numVote});
    }
    /* debug */
    affichVectString (vVote);
/*
    affichVectParticipants(vParticipant);
    cout << string (15, '-') << endl;
    sort (vParticipant.begin(), vParticipant.end(), compare2part);
    affichVectParticipants(vParticipant);
*/
    cout << "c'est la glace " << vVote[0] << " qui a gagne" << endl;
    return 0;
}
