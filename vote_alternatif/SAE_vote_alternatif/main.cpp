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

vector<int> litUnChoix (){ // code fourni par le professeur modifié
    string chaine;
    while (true){
        getline (cin, chaine);
        if ((!cin) || (chaine.substr(0,2) != "//")) break;
    }

    string choix = "";
    vector<int> choix_liste;
    for (unsigned int i=0; i<chaine.size(); ++i) {
        if (chaine[i] >='0' && chaine[i] <= '9') {
            choix += chaine[i];
        } else if (choix != "") {
            choix_liste.push_back(stoi(choix));
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

void affichVectString (const vector<string> & v){
    for (const string & val : v)
        cout << val << '\t';
    cout << endl;
}

void affichVectInt (const vector<int> & v){
    for (const int & val : v)
        cout << to_string(val) << '\t';
    cout << endl;
}

void affichVectParticipants (const vector<participant> & vPart){
    for (const participant & part : vPart){
        cout << part.nom << endl;
        cout << part.prenom << endl;
        affichVectInt(part.choix);
    }
}

int supprimerElem (vector<int> & tab, const size_t & pos){
    int temp = tab[pos];
    for (size_t i=pos; i<tab.size() -1; ++i) {
        tab[i] = tab[i+1];
    }
    tab.resize(tab.size()-1);
    return temp;
}

bool choixInvalide(const vector<candidat> & tabCand, const int & choix) {
    return tabCand[choix -1].estHorsCourse || choix < 1 || (choix > int(tabCand.size()));
}


int main() {
    /* On commence par mettre dans une liste les candidats*/
    vector <candidat> vCandidats;
    string ligne;

    while (true) {
        ligne = litUneString();
        if (ligne == string (10, '-')) break; // On considère une ligne de 10 '-' comme la fin de la section de definition des candidats
        vector<participant> tmp;
        vCandidats.push_back(candidat{ligne, tmp, false}); // on met une liste vide pour les electeurs
    }

    //On lit les datas du clavier, et on les stocke
    vector<participant> vParticipant;

    for (unsigned i (0); i < 100; ++i){ // on considère qu'on aura toujours 110 participants
        string nom (litUneString());
        string prenom  (litUneString());
        vector<int> choixDuParticipant (litUnChoix()); // choix par ordre de préférence décroissante
        vParticipant.push_back(participant{nom, prenom, choixDuParticipant});
    }

    candidat gagnant;
    bool gagnantTrouve = false;
    while (!gagnantTrouve) {
        /*
        On va implémenter ici notre algorithme de code pour le système de vote alternatif
        On a détaillé dans le dossier déjà le fonctionnement de ce système de vote,
        on commence donc par lire les choix des participants et ainsi comptabiliser
        pour chaque candidats ses voix.
        */
        for (unsigned int i=0; i<vParticipant.size(); ++i) {
            // on commence par determiner le choix du participant sur lequel on itère.
            // pour que le choix soit valide, il doit correspondre au choix préféré du participant
            // parmi les candidats valides
            bool choixExploitable = vParticipant[i].choix.size() > 0;
            while (choixExploitable && choixInvalide(vCandidats, vParticipant[i].choix[0])) {
                // on purge au fur et à mesure les candidats hors course des votes des participants
                if (choixInvalide(vCandidats, vParticipant[i].choix[0])) {
                    supprimerElem(vParticipant[i].choix, 0);
                }
                // on vérifie à chaque fois qu'il y a encore de quoi determiner le vote du participant
                choixExploitable = vParticipant[i].choix.size() > 0;
            }

            // on ajoute aux electeurs du candidat le participant qui lui accorde son vote
            // les numeros de choix des particpants correspondont à leur indice dans la liste
            // vCandidats auquel on additionne 1. On utilise donc -1 pour obtenir l'indice.
            vCandidats[vParticipant[i].choix[0] -1].electeurs.push_back(vParticipant[i]);
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
                gagnantTrouve = true;
            }
        }

        /*
        On détermine à présent le candidat qui a le moins de voix et on le désigne
        comme étant hors course
        */
        vector<unsigned int> indicesMin; // on gère les candidats en dernière positions avec une liste pour gérer les ex aequo
        indicesMin.push_back(0);
        for (unsigned int i=1; i<vCandidats.size(); ++i) { // on commence à 1 car on n'a pas besoin de tester pour i=0
            if (vCandidats[i].electeurs.size() == vCandidats[indicesMin[0]].electeurs.size()) {
                indicesMin.push_back(i);
            }
            else if (vCandidats[i].electeurs.size() < vCandidats[indicesMin[0]].electeurs.size()) {
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
        if (indicesMin.size() == nbEnLice && !gagnantTrouve) {
            cout << "Les candidats \"";
            for (unsigned int i=0; i<vCandidats.size(); ++i) {
                cout << vCandidats[i].nom << ", ";
            }
            cout << "ont gagné: félicitations !" << endl;
            return 0;
        }
        for (unsigned int i=0; i<indicesMin.size(); ++i) {
            vCandidats[indicesMin[i]].estHorsCourse = true;
        }
        // on réinitialise ensuite les electeurs de chaque candidats pour préparer le prochain tour
        for (unsigned int i=0; i<vCandidats.size(); ++i) {
            vCandidats[i].electeurs.resize(0);
        }
    }
    cout << "Le candidat \"" << gagnant.nom << "\" a gagné, félicitations !" << endl;
    return 0;
}
