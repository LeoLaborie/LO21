#include "Hexagone.h"

void Hexagone::afficher() const
{
    std::cout << "Hexagone(" << x << "," << y << "," << z << ")"
                << (est_recouvert ? " [recouvert]" : "") << "\n";
    // Vérifie si c'est une Carrière, Quartier ou Place
    if (dynamic_cast<const Carriere *>(this))
    {
        texte_couleur(BLEU);
        std::cout << "  Carrière\n";
    }
    else if (auto quartier = dynamic_cast<const Quartier *>(this))
    {
        std::cout << "  Quartier : ";
        switch (quartier->getTypeQuartier())
        {
            case TypeQuartier::Habitation:
                texte_couleur(CYAN);
                std::cout << "Habitation\n";
                break;
            case TypeQuartier::Marche:
                texte_couleur(JAUNE);
                std::cout << "Marché\n";
                break;
            case TypeQuartier::Temple:
                texte_couleur(MAGENTA);
                std::cout << "Temple\n";
                break;
            case TypeQuartier::Caserne:
                texte_couleur(ROUGE);
                std::cout << "Caserne\n";
                break;
            case TypeQuartier::Jardin:
                texte_couleur(VERT);
                std::cout << "Jardin\n";
                break;
        }
    }
    else if (auto place = dynamic_cast<const Place *>(this))
    {
        std::cout << "  Place : ";
        switch (place->getTypePlace())
        {
            case TypePlace::Habitation:
                texte_couleur(CYAN);
                std::cout << "Habitation\n";
                break;
            case TypePlace::Marche:
                texte_couleur(JAUNE);
                std::cout << "Marché\n";
                break;
            case TypePlace::Temple:
                texte_couleur(MAGENTA);
                std::cout << "Temple\n";
                break;
            case TypePlace::Caserne:
                texte_couleur(ROUGE);
                std::cout << "Caserne\n";
                break;
            case TypePlace::Jardin:
                texte_couleur(VERT);
                std::cout << "Jardin\n";
                break;
        }
    }
    texte_reset();
}