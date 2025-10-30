#include "Hexagone.h"

void Hexagone::afficher(bool court) const
{
    if (court)
    {
        std::cout << "<";
    }
    else
    {
        std::cout << "Hexagone(" << x << "," << y << "," << z << ")"
                  << (est_recouvert ? " [recouvert]" : "") << "\n  ";
    }

    // Affichage du type
    if (this->isQuartier())
    {
        std::cout << "Quartier : ";
    }
    else if (this->isPlace())
    {
        std::cout << "Place : ";
    }

    switch (this->getType())
    {
    case TypeHex::Carriere:
        texte_couleur(BLEU);
        std::cout << "Carrière";
        break;
    case TypeHex::Habitation:
    case TypeHex::PHabitation:
        texte_couleur(CYAN);
        std::cout << "Habitation";
        break;
    case TypeHex::Marche:
    case TypeHex::PMarche:
        texte_couleur(JAUNE);
        std::cout << "Marché";
        break;
    case TypeHex::Temple:
    case TypeHex::PTemple:
        texte_couleur(MAGENTA);
        std::cout << "Temple";
        break;
    case TypeHex::Caserne:
    case TypeHex::PCaserne:
        texte_couleur(ROUGE);
        std::cout << "Caserne";
        break;
    case TypeHex::Jardin:
    case TypeHex::PJardin:
        texte_couleur(VERT);
        std::cout << "Jardin";
        break;
    }
    texte_reset();
    if (court)
    {
        std::cout << ">";
    }
    else
    {
        std::cout << std::endl;
    }
}