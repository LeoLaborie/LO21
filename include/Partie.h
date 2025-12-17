#ifndef PARTIE_H
#define PARTIE_H

#include <math.h>
#include <map>
#include <memory>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

#include "Chantier.h"
#include "Joueur.h"
#include "Tuile.h"
#include "Hexagone.h"
#include "Sauvegarde.h"

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

    // std::unique_ptr<IllustreArchitecte> fauxJoueur;
    bool fauxJoueurP = false;

    Chantier chantier;
    std::vector<Joueur *> joueurs;
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
           std::vector<Joueur *> joueurs,
           bool fauxJoueurP);

    /**
     * @brief Tire une carte du stock de cartes disponibles
     * @param stock Référence vers la map du stock de cartes
     * @param marcheDejaPresent Indique si un marché est déjà présent
     * @return TypeHex : type de la carte tirée
     */
    TypeHex tirerCarte(std::map<TypeHex, int> &stock, bool marcheDejaPresent);

    /**
     * @brief Génère les tuiles pour la partie
     * @param fullTuiles Indique si la variante Full Tuile est activée, par défaut false
     */
    void genererTuilesParties(bool fullTuiles = false);

public:
    /**
     * @brief Constructeur de copie supprimé pour éviter la copie de la partie
     */
    Partie(const Partie &) = delete;

    /**
     * @brief Opérateur d'affectation de copie supprimé pour éviter la copie de la partie
     */
    Partie &operator=(const Partie &) = delete;

    /**
     * @brief Constructeur de déplacement par défaut
     */
    Partie(Partie &&) noexcept = default;

    /**
     * @brief Opérateur d'affectation de déplacement par défaut
     */
    Partie &operator=(Partie &&) noexcept = default;

    /**
     * @brief Constructeur de Partie
     * @param nbJouer Nombre de joueurs
     * @param pseudo Vecteur des pseudos des joueurs
     * @param variantesScore Tableau des variantes de score
     * @param varianteFullTuile Indique si la variante Full Tuile est activée
     * @param difficulte Niveau de difficulté (optionnel, par défaut 0)
     */
    Partie(int nbJouer, std::vector<std::string> &pseudo, const bool variantesScore[5], bool varianteFullTuile, int difficulte = 0);

    /**
     * @brief constructeur de base de partie utiliser pour créer la partie dans le main
     */
    Partie()
        : nbrJoueurs(0), maitreArchitecte(0), mainJoueur(0), nbrTours(0), taillepaquet(0), fauxJoueurP(false), chantier(), joueurs(), piles() {}

    /**
     * @brief Destructeur de Partie
     */
    ~Partie()
    {
        for (auto j : joueurs)
        {
            delete j;
        }
    };

    // Getters

    /**
     * @brief Retourne le nombre de joueurs dans la partie
     * @return int : nombre de joueurs
     */
    int getNbrJoueurs() const
    {
        return nbrJoueurs;
    }

    /**
     * @brief Retourne l'indice du maître architecte
     * @return int : indice du maître architecte
     */
    int getMaitreArchitecte() const
    {
        return maitreArchitecte;
    }

    /**
     * @brief Retourne le joueur dont c'est le tour
     * @return Joueur& : référence vers le joueur dont c'est le tour
     */
    Joueur &getJoueurMain()
    {
        return *joueurs[static_cast<size_t>(mainJoueur)];
    }

    /**
     * @brief Retourne l'indice du joueur dont c'est le tour
     * @return int : indice du joueur dont c'est le tour
     */
    int getMainJoueur() const
    {
        return mainJoueur;
    }

    /**
     * @brief Retourne la taille du paquet de tuiles
     * @return int : taille du paquet de tuiles
     */
    int getTaillePaquet() const
    {
        return taillepaquet;
    }

    /**
     * @brief Retourne le chantier de la partie
     * @return Chantier& : référence vers le chantier
     */
    Chantier &getChantier()
    {
        return chantier;
    }

    /**
     * @brief Retourne le chantier constant de la partie
     * @return const Chantier& : référence constante vers le chantier
     */
    const Chantier &getChantier() const
    {
        return chantier;
    }

    /**
     * @brief Retourne les joueurs de la partie
     * @return const std::vector<Joueur>& : référence constante vers le vecteur des joueurs
     */
    const std::vector<Joueur *> &getJoueurs() const
    {
        return joueurs;
    }

    /**
     * @brief Retourne les piles de tuiles de la partie
     * @return const std::vector<std::vector<Tuile>>& : référence constante vers le vecteur des piles de tuiles
     */
    const std::vector<std::vector<Tuile>> &getPile() const
    {
        return piles;
    };

    /**
     * @brief Retourne le nombre de tours effectués dans la partie
     * @return int : nombre de tours
     */
    int getNbrTours() const
    {
        return nbrTours;
    }

    /**
     * @brief Retourne le nombre de piles restantes dans la partie
     * @return size_t : nombre de piles
     */
    size_t getNbrPiles() const
    {
        return piles.size();
    }

    // Setters

    /**
     * @brief Définit l'indice du maître architecte
     * @param i Indice du maître architecte
     */
    void setMaitreArchitecte(int i)
    {
        maitreArchitecte = i % (nbrJoueurs > 0 ? nbrJoueurs : 1);
    }

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
    bool pilesRestantes() const
    {
        return !piles.empty();
    }

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
    bool fauxJoueurPresent() const
    {
        return fauxJoueurP;
    }

    /**
     * @brief Crée un faux joueur (Illustre Architecte)
     * @param difficulte Difficulté de l'IA (1 à 3)
     */
    void creerFauxJoueur(int difficulte);

    /**
     * @brief Retourne le faux joueur (Illustre Architecte)
     * @return IllustreArchitecte* : pointeur vers le faux joueur
     */
    IllustreArchitecte *getFauxJoueur() const
    {
        if(fauxJoueurP){
            return dynamic_cast<IllustreArchitecte *>(joueurs[static_cast<size_t>(nbrJoueurs) - 1]);
        }else{
            return nullptr;
        }

    }

    /**
     * @brief Charge une partie sauvegardée depuis un fichier.
     * @param nomFichier Chemin du fichier de sauvegarde.
     * @return Partie reconstruite à partir du fichier.
     * @throws std::runtime_error si le fichier est introuvable ou invalide.
     */
    static Partie FromSave(const std::string &nomFichier);
};

#endif