#ifndef SAUVEGARDE_H
#define SAUVEGARDE_H
#include <string>
#include <vector>
#include <fstream>
#include "Partie.h"

/**
 * @brief Donnees extraites d'une sauvegarde pour reconstruire la partie.
 */
typedef struct PartieSauvegarde{
    int nbrJoueurs;                         ///< Nombre total de joueurs.
    int maitreArchitecte;                   ///< Indice du maitre architecte.
    int mainJoueur;                         ///< Indice du joueur actif au moment de la sauvegarde.
    int nbrTours;                           ///< Nombre de tours deja joues.
    int taillepaquet;                       ///< Taille restante du paquet de tuiles.
    IllustreArchitecte *fauxJoueur;         ///< Pointeur vers un faux joueur eventuel.
    Chantier chantier;                      ///< Etat du chantier au moment de la sauvegarde.
    std::vector<Joueur> joueurs;            ///< Liste des joueurs et leur etat.
    std::vector<std::vector<Tuile>> piles;  ///< Piles de tuiles restantes.
}PartieSauvegarde;


/**
 * @brief Sauvegarde l'etat complet d'une partie dans un fichier timestamp.
 * @param partie Partie a serialiser sur disque.
 */
void sauvegarderPartie(const Partie& partie);

/**
 * @brief Charge une partie a partir d'un fichier de sauvegarde et valide son contenu.
 * @param nomSauvegarde Chemin du fichier a ouvrir.
 * @return true si le fichier est bien formate et charge, false sinon.
 */
bool chargerPartie(const std::string& nomSauvegarde);

/**
 * @brief lecture de la section [PARTIE] et remplit les champs de base.
 * @param f Flux deja positionne apres [PARTIE].
 * @param p Objet PartieSauvegarde qui contient les élements du fichier.
 * @param erreurs Liste des erreurs detectees .
 * @return true si aucune erreur n'est remontee.
 */
bool lirePartie(std::ifstream& f, PartieSauvegarde& p, std::vector<std::string>& erreurs);

/**
 * @brief lecture de la section [CHANTIER] et avance le flux du fichier en vérifiant le format des tuiles.
 * @param f Flux deja positionne apres [CHANTIER].
 * @param p Objet PartieSauvegarde qui contient les élements du fichier.
 * @param erreurs Liste des erreurs detectees.
 * @return true si aucune erreur n'est remontee.
 */
bool lireChantier(std::ifstream& f, PartieSauvegarde& p, std::vector<std::string>& erreurs);

/**
 * @brief lecture de la la section [PILES] et valide les compteurs/structures des tuiles.
 * @param f Flux deja positionne apres [PILES].
 * @param p Objet PartieSauvegarde qui contient les élements du fichier.
 * @param erreurs Liste des erreurs detectees.
 * @return true si aucune erreur n'est remontee.
 */
bool lirePiles(std::ifstream& f, PartieSauvegarde& p, std::vector<std::string>& erreurs);

/**
 * @brief lecture de la la section [JOUEURS] et valide nom, scores, plateau et tuile en main.
 * @param f Flux deja positionne apres [JOUEURS].
 * @param p Objet PartieSauvegarde qui contient les élements du fichier.
 * @param erreurs Liste des erreurs detectees.
 * @return true si aucune erreur n'est remontee.
 */
bool lireJoueurs(std::ifstream& f, PartieSauvegarde& p, std::vector<std::string>& erreurs);



#endif
