#include <iostream>
#include <vector>

using namespace std;

string litUneString (){ // code fourni par le professeur
    string uneChaine;
    while (true){
        getline (cin, uneChaine);
        if ((!cin) || (uneChaine.substr(0,2) != "//")) break;
    }
    return uneChaine;
}

vector<int> litUnChoix (){
    string uneChaine;
    while (true){
        getline (cin, uneChaine);
        if ((!cin) || (uneChaine.substr(0,2) != "//")) break;
    }

    string chaine = stoi(uneChaine);
    string choix;
    vector<int> choix_liste;
    for (unsigned int i=0; i<chaine.size(); ++i) {
        if (chaine[i] >='0' && chaine[i] <= '9') {
            choix += chaine[i];
        } else if (choix == "") {
            choix_liste.push_back(int(choix));
            choix = "";
        }
    }
    return choix_liste;
}

struct participant {
    string nom;
    string prenom;
    vector<int> choix;
};

struct candidat {
    string nom;
    vector<participant> electeurs;
    bool estHorsCourse;
};

/*
bool compare2part (const participant & p1, const participant & p2){
    return p1.choix <= p2.choix;
    error
}
*/

void affichVectString (const vector<string> & v){
    for (const string & val : v)
        cout << val << '\t';
    cout << endl;
}

void affichVectParticipants (const vector<participant> & vPart){
    for (const participant & part : vPart){
        cout << part.nom << endl;
        cout << part.prenom << endl;
        cout << part.choix << endl;
    }
}

int supprimerElem (vector<int> & tab, const size_t & pos){
    int temp = tab[pos];
    for (size_t i=pos; i<tab.size() -1; i++) {
        tab[i] = tab[i+1];
    }
    tab.resize(tab.size()-1);
    return temp;
}

bool choixInvalide(tabCand, choix) {
    return vCand[choix].estHorsCourse || choix < 1 || choix >= tab.size();
}


int main()
{
    /*
    cout << "Hello World!" << endl;
    return 0;
    */
    /* On commence par mettre dans une liste les candidats*/
    vector <string> vCandidats;
    string ligne;
    while (true) {
        ligne = litUneString();
        if (ligne == string (10, '-')) break; // On considère une ligne de 10 '-' comme la fin de la section de definition des candidats
        vCandidats.push_back(candidat{ligne, vector<participant>, false}); // on met une liste vide pour les electeurs
    }

    /* debug */
    affichVectString (vCandidats);

    //On lit les datas du clavier, et on les stocke
    vector<participant> vParticipant;

    for (unsigned i (0); i < 10; ++i){
        string nom (litUneString());
        string prenom  (litUneString());
        vector<int> choixDuParticipant (litUnChoix()); // choix par ordre de préférence décroissante

        //cout << nom << endl << prenom << endl << choixDuParticipant << endl;

        vParticipant.push_back(participant{nom, prenom, choixDuParticipant});
    }

    /* Tri des participants par ordre de choix // Pas obligé ce sera compliqué
    affichVectParticipants(vParticipant); // liste des participants
    cout << string (15, '-') << endl;
    sort (vParticipant.begin(), vParticipant.end(), compare2part); // même liste mais triée en place cette fois
    affichVectParticipants(vParticipant);
    */

    candidat gagnant;
    while (!gagnant) {
        /*
        On va implémenter ici notre algorithme de code pour le système de vote alternatif
        On a détaillé dans le dossier déjà le fonctionnement de ce système de vote,
        on commence donc par lire les choix des participants et ainsi comptabiliser
        pour chaque candidats ses voix.
        */
        for (unsigned int i=0; i<vParticipant.size(); ++1) {
            // on commence par determiner le choix du participant sur lequel on itère.
            // pour que le choix soit valide, il doit correspondre au choix préféré du participant
            // parmi les candidats valides
            bool choixExploitable = vParticipants[i].choix.size() > 0;
            while (choixExploitable && choixInvalide(vCandidats, choix)) {
                // on purge au fur et à mesure les candidats hors course des votes des participants
                supprimerElem(vParticipants[i].choix, 0);
                // on vérifie à chaque fois qu'il y a encore de quoi determiner le vote du participant
                choixExploitable = vParticipants[i].choix.size() > 0;
            }

            // on ajoute aux electeurs du candidat le participant qui lui accorde son vote
            // les numeros de choix des particpants correspondont à leur indice dans la liste
            // vCandidats auquel on additionne 1. On utilise donc -1 pour obtenir l'indice.
            vCandidats[vParticipants[i].choix[0] -1].electeur.push_pack(vParticipants[i]);
        }

        // on détermine si un des candidats a la majorité absolue
        unsigned int seuilDeMajorite = (int(vParticipant.size()) / 2) + 1;
        for (unsigned int i=0; i<vCandidats.size(); ++i) {
            if (vCandidats[i].electeurs.size() >= seuilDeMajorite) {
                // On désigne tout les candidats comme hors course
                for (unsigned int j=0; j<vCandidats.size(); ++j) { // tous sauf le gagnant
                    if (j != i) {
                        vCandidats[j].estHorsCourse = true;
                    }
                }
                gagnant = vCandidats[i];
            }
        }

        /*
        On détermine à présent le candidat qui a le moins de voix et on le désigne
        comme étant hors course
        */
        vector<unsigned int> indicesMin; // on gère les candidats en dernière positions avec une liste pour gérer les ex aequo
        indicesMin.push_back(0);
        for (unsigned int i=1; i<vCandidats.size(); ++1) { // on commence à 1 car on n'a pas besoin de tester pour i=0
            if (vCandidats[i] == vCandidats[indicesMin[0]]) {
                indicesMin.push_back(i);
            }
            else if (vCandidats[i] < vCandidats[indicesMin[0]]) {
                vector<unsigned int> temp;
                temp.push_back(i);
                indicesMin = temp; // on remplace la liste avec une nouvelle contenant le nouveau minimum
            }
        }

        // à présent on le mets hors course
        // sauf si les derniers candidats qu'on peut mettre hors course représentent les
        // derniers participants
        // cela arrive lorsqu'on a une égalité
        unsigned int nbEnLice = 0;
        for (unsigned int i=0; i<vCandidats.size(); ++i) {
            if (!vCandidats[i].estHorsCourse) {
                nbEnLice += 1;
            }
        }
        if (indicesMin.size() == nbEnLice) {
            cout << "Le candidat \"";
            for (unsigned int i=0; i<vCandidats.size(); ++i) {
                cout << vCandidats[i] << ", ";
            }
            cout << "ont gagné, félicitation !";
            return 0;
        }
        for (unsigned int i=0; i<indiceMin.size(); ++i) {
            vCandidats[indiceMin[i]].estHorsCourse = true;
        }
        // on réinitialise ensuite les electeurs de chaque candidats pour préparer le prochain tour
        for (unsigned int i=0; i<vCandidats.size(); ++i) {
            vCandidats[i].electeur.resize(0);
        }
    }
    cout << "Le candidat \"" << vCandidats[0] << "\" a gagné, félicitations !" << endl;
    return 0;
}
