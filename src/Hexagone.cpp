#include "Hexagone.h"

void Hexagone::afficher() const
{
    std::cout << "Hexagone(" << x << "," << y << "," << z << ")"
                << (est_recouvert ? " [recouvert]" : "") << "\n";
    // Vérifie si c'est une Carrière, Quartier ou Place
    if (type_ == TypeHex::Carriere)
    {
        texte_couleur(BLEU);
        std::cout << "  Carrière\n";
    }
    else if (this->isQuartier())
    {
        std::cout << "  Quartier : ";
        if (type_ == TypeHex::Habitation){
            texte_couleur(CYAN);
            std::cout << "Habitation\n";
        }
        if (type_ == TypeHex::Marche){
            texte_couleur(JAUNE);
            std::cout << "Marché\n";
        }
        if (type_ == TypeHex::Temple){
            texte_couleur(MAGENTA);
            std::cout << "Temple\n";
        }
        if (type_ == TypeHex::Caserne){
            texte_couleur(ROUGE);
            std::cout << "Caserne\n";
        }
        if (type_ ==  TypeHex::Jardin){
            texte_couleur(VERT);
            std::cout << "Jardin\n";
        }
    }
    else if (this->isPlace())
    {
        std::cout << "  Place : ";
        if (type_ == TypeHex::PHabitation){
                texte_couleur(CYAN);
                std::cout << "Habitation\n";
            }
            if (type_ == TypeHex::PMarche){
                texte_couleur(JAUNE);
                std::cout << "Marché\n";
            }
            if (type_ == TypeHex::PTemple){
                texte_couleur(MAGENTA);
                std::cout << "Temple\n";
            }
            if (type_ == TypeHex::PCaserne){
                texte_couleur(ROUGE);
                std::cout << "Caserne\n";
            }
            if (type_ ==  TypeHex::PJardin){
                texte_couleur(VERT);
                std::cout << "Jardin\n";
            }
    }
    texte_reset();
}