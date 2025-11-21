#ifndef PARTIE_H
#define PARTIE_H

#include <vector>
#include <math.h>
#include <stdexcept>
#include <random>
#include <map>
#include <string>
#include "Tuile.h"
#include "Chantier.h"
#include "Joueur.h"

/**
 * @class Partie
 * @brief Représente une partie du jeu, gérant les joueurs, le chantier, et les piles de tuiles.
 */
class Partie
{
private:
    int nbrJoueurs = 0;
    int maitreArchitecte = 0; // indice du joueur maître architecte
    int mainJoueur = 0;       // indice du joueur dont c’est le tour
    int nbrTours = 0;
    int taillepaquet = 0;

    IllustreArchitecte *fauxJoueur;

    Chantier chantier;
    std::vector<Joueur> joueurs;
    std::vector<std::vector<Tuile>> piles;
    /**
     * @brief Constructeur privé utilisé uniquement pour reconstruire une partie depuis une sauvegarde.
     */
    Partie(int nbJoueurs,
           int nbTours,
           int maitreArchitecte,
           int mainJoueur,
           Chantier chantier,
            std::vector<std::vector<Tuile>> piles,
            std::vector<Joueur> joueurs);
           
public:
    /**
     * * @brief Constructeur de Partie
     * @param nbJouer Nombre de joueurs
     * @param pseudo Vecteur des pseudos des joueurs
     * @param variantesScore Tableau des variantes de score
     * @param varianteFullTuile Indique si la variante Full Tuile est activée
     */
    Partie(int nbJouer, std::vector<std::string> &pseudo, const bool variantesScore[5], bool varianteFullTuile);

    /**
     * @brief Destructeur de Partie
     */
    ~Partie() = default;

    // Getters

    /**
     * @brief Retourne le nombre de joueurs dans la partie
     * @return int : nombre de joueurs
     */
    int getNbrJoueurs() const { return nbrJoueurs; }

    /**
     * @brief Retourne l'indice du maître architecte
     * @return int : indice du maître architecte
     */
    int getMaitreArchitecte() const { return maitreArchitecte; }

    /**
     * @brief Retourne le joueur dont c'est le tour
     * @return Joueur& : référence vers le joueur dont c'est le tour
     */
    Joueur &getJoueurMain() { return joueurs.at(mainJoueur); }

    /**
     * @brief Retourne l'indice du joueur dont c'est le tour
     * @return int : indice du joueur dont c'est le tour
     */
    int getMainJoueur() const { return mainJoueur; }

    /**
     * @brief Retourne la taille du paquet de tuiles
     * @return int : taille du paquet de tuiles
     */
    int getTaillePaquet() const { return taillepaquet; }

    /**
     * @brief Retourne le chantier de la partie
     * @return Chantier& : référence vers le chantier
     */
    Chantier &getChantier() { return chantier; }

    /**
     * @brief Retourne le chantier constant de la partie
     * @return const Chantier& : référence constante vers le chantier
     */
    const Chantier &getChantier() const { return chantier; }

    /**
     * @brief Retourne les joueurs de la partie
     * @return const std::vector<Joueur>& : référence constante vers le vecteur des joueurs
     */
    const std::vector<Joueur> &getJoueurs() const { return joueurs; }

    /**
     * @brief Retourne les piles de tuiles de la partie
     * @return const std::vector<std::vector<Tuile>>& : référence constante vers le vecteur des piles de tuiles
     */
    const std::vector<std::vector<Tuile>> &getPile() const { return piles; };

    /**
     * @brief Retourne le nombre de tours effectués dans la partie
     * @return int : nombre de tours
     */
    int getNbrTours() const { return nbrTours; }

    /**
     * @brief Retourne le nombre de piles restantes dans la partie
     * @return int : nombre de piles
     */
    int getNbrPiles() const { return piles.size(); }

    // Setters

    /**
     * @brief Définit l'indice du maître architecte
     * @param i Indice du maître architecte
     */
    void setMaitreArchitecte(int i) { maitreArchitecte = i % (nbrJoueurs > 0 ? nbrJoueurs : 1); }

    /**
     * @brief Définit le prochain joueur dont c'est le tour
     */
    void setProchainJoueur()
    {
        if (nbrJoueurs)
            mainJoueur = (mainJoueur + 1) % nbrJoueurs;
    }

    // Autres méthodes

    /**
     * @brief Retire et retourne la dernière pile de tuiles
     * @return std::vector<Tuile> : la pile de tuiles retirée
     */
    std::vector<Tuile> retirerPaquet();

    /**
     * @brief Indique s'il reste des piles de tuiles
     * @return bool : true s'il reste des piles, false sinon
     */
    bool pilesRestantes() const { return !piles.empty(); }

    /**
     * @brief Génère les tuiles pour la partie
     * @param fullTuiles Indique si la variante Full Tuile est activée, par défaut false
     */
    void genererTuilesParties(bool fullTuiles = false);

    /**
     * @brief Ajoute des tuiles du paquet au chantier
     */
    void addTuileInChantierFromPiles();

    /**
     * @brief Termine le tour en cours et passe au suivant
     */
    void tourTermine();

    /**
     * @brief Indique si un faux joueur (Illustre Architecte) est présent
     * @return bool : true si un faux joueur est présent, false sinon
     */
    bool fauxJoueurPresent() const { return fauxJoueur != nullptr; }

    /**
     * @brief Crée un faux joueur (Illustre Architecte)
     */
    void creerFauxJoueur();

    /**
     * @brief Retourne le faux joueur (Illustre Architecte)
     * @return IllustreArchitecte* : pointeur vers le faux joueur
     */
    IllustreArchitecte *getFauxJoueur() const { return fauxJoueur; }

    /**
     * @brief Charge une partie sauvegardée depuis un fichier.
     * @param nomFichier Chemin du fichier de sauvegarde.
     * @return Partie reconstruite à partir du fichier.
     * @throws std::runtime_error si le fichier est introuvable ou invalide.
     */
    static Partie FromSave(const std::string& nomFichier);
};

#endif
