#include "Hexagone.h"

void Hexagone::afficher() const
{
    std::cout << "Hexagone(" << x << "," << y << "," << z << ")"
                << (est_recouvert ? " [recouvert]" : "") << "\n";
    // Vérifie si c'est une Carrière, Quartier ou Place
    if (auto quartier = dynamic_cast<const Quartier *>(this))
    {
        std::cout << "  Type Quartier: ";
        switch (quartier->getTypeQuartier())
        {
            case TypeQuartier::Habitation:
                std::cout << " Habitation\n";
                break;
            case TypeQuartier::Marche:
                std::cout << " Marché\n";
                break;
            case TypeQuartier::Temple:
                std::cout << " Temple\n";
                break;
            case TypeQuartier::Caserne:
                std::cout << " Caserne\n";
                break;
            case TypeQuartier::Jardin:
                std::cout << " Jardin\n";
                break;
        }
    }
    else if (auto place = dynamic_cast<const Place *>(this))
    {
        std::cout << "  Type Place: ";
        switch (place->getTypePlace())
        {
            case TypePlace::Habitation:
                std::cout << " Habitation\n";
                break;
            case TypePlace::Marche:
                std::cout << " Marché\n";
                break;
            case TypePlace::Temple:
                std::cout << " Temple\n";
                break;
            case TypePlace::Caserne:
                std::cout << " Caserne\n";
                break;
            case TypePlace::Jardin:
                std::cout << " Jardin\n";
                break;
        }
    }
}