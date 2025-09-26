#include <iostream>
#include "Partie.h" // <-- ton header qui contient la classe Partie

using namespace std;

int main()
{

    // Test avec 2 joueurs
    Partie partie2(2);
    partie2.preparerPartie();
    std::cout << "✅ Partie à 2 joueurs préparée avec succès." << std::endl;

    
    return 0;
}
