#include "Sauvegarde.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <chrono>


std::string getCurrentDate()
{
    std::time_t t = std::time(nullptr);
    char buf[32];
    std::strftime(buf, sizeof(buf), "%d-%m-%Y_%H-%M", std::localtime(&t));
    return buf;
}


std::string typeToString(TypeHex t)
{
    switch (t)
    {
        case TypeHex::Habitation: return "Habitation";
        case TypeHex::Marche: return "Marche";
        case TypeHex::Temple: return "Temple";
        case TypeHex::Caserne: return "Caserne";
        case TypeHex::Jardin: return "Jardin";
        case TypeHex::PHabitation: return "PHabitation";
        case TypeHex::PMarche: return "PMarche";
        case TypeHex::PTemple: return "PTemple";
        case TypeHex::PCaserne: return "PCaserne";
        case TypeHex::PJardin: return "PJardin";
        case TypeHex::Carriere: return "Carriere";
    }
    return "Carriere";
}

TypeHex stringToType(const std::string& s)
{
    if (s == "Habitation") return TypeHex::Habitation;
    if (s == "Marche") return TypeHex::Marche;
    if (s == "Temple") return TypeHex::Temple;
    if (s == "Caserne") return TypeHex::Caserne;
    if (s == "Jardin") return TypeHex::Jardin;
    if (s == "PHabitation") return TypeHex::PHabitation;
    if (s == "PMarche") return TypeHex::PMarche;
    if (s == "PTemple") return TypeHex::PTemple;
    if (s == "PCaserne") return TypeHex::PCaserne;
    if (s == "PJardin") return TypeHex::PJardin;
    if (s == "Carriere") return TypeHex::Carriere;

    throw std::invalid_argument("TypeHex inconnu: " + s);
}


std::ostream& operator<<=(std::ostream& os, const Hexagone& h)
{
    os << "HEX "
       << h.getX() << ' '
       << h.getY() << ' '
       << h.getZ() << ' '
       << typeToString(h.getType()) << ' '
       << (h.getEstRecouvert() ? 1 : 0) << '\n';

    return os;
}

std::istream& operator>>=(std::istream& is, Hexagone& h)
{
    std::string ligne;
    if (!(is >> ligne)) return is;

    if (ligne != "HEX") {
        is.setstate(std::ios::failbit);
        return is;
    }

    int x = 0, y = 0, z = 0;
    std::string typeStr;
    int rec = 0;

    if (!(is >> x >> y >> z >> typeStr >> rec)) {
        is.setstate(std::ios::failbit);
        return is;
    }

    h = Hexagone(x, y, z, stringToType(typeStr));
    h.setEstRecouvert(rec != 0);

    return is;
}

std::ostream& operator<<=(std::ostream& os, const Tuile& t)
{
    const auto& hexs = t.getHexagones();
    os << "TUILE " << hexs.size() << '\n';

    for (auto h : hexs)
        os <<= *h;

    return os;
}

std::istream& operator>>=(std::istream& is, Tuile& tuile)
{
    std::string ligne;
    if (!(is >> ligne)) return is;

    if (ligne != "TUILE") {
        is.setstate(std::ios::failbit);
        return is;
    }

    int nbHex = 0;
    if (!(is >> nbHex)) {
        is.setstate(std::ios::failbit);
        return is;
    }

    if (nbHex != 3 && nbHex != 4) {
        is.setstate(std::ios::failbit);
        return is;
    }

    std::vector<std::unique_ptr<Hexagone>> hexs;
    hexs.reserve(nbHex);

    for (int i = 0; i < nbHex; ++i)
    {
        Hexagone tmp(0, 0, 0, TypeHex::Carriere);
        if (!(is >>= tmp)) return is;
        hexs.push_back(std::make_unique<Hexagone>(tmp));
    }

    if (nbHex == 3)
        tuile = Tuile(hexs[0].release(), hexs[1].release(), hexs[2].release());
    else
        tuile = Tuile(hexs[0].release(), hexs[1].release(), hexs[2].release(), hexs[3].release());

    return is;
}
void sauvegarderPartie(const Partie& p)
{
    std::string date = getCurrentDate();
    std::string nom = "saves/save_" + date + ".ratatata";
    std::ofstream f(nom);
    if (!f) {
        std::cerr << "Erreur : impossible de créer la sauvegarde.\n";
        return;
    }

    f << "PARTIE "
      << p.getNbrJoueurs() << ' '
      << p.getNbrTours() << ' '
      << p.getMaitreArchitecte() << ' '
      << p.getMainJoueur() << '\n';
    const auto& chantier = p.getChantier().getTuiles();
    f << "CHANTIER " << chantier.size() << '\n';
    for (const auto& t : chantier) {
        f <<= t;
    }
    const auto& piles = p.getPile();
    f << "PILES " << piles.size() << '\n';

    for (size_t i = 0; i < piles.size(); ++i)
    {
        f << "PILE " << i << " " << piles[i].size() << '\n';
        for (const auto& t : piles[i])
            f <<= t;
    }
    const auto& joueurs = p.getJoueurs();
    f << "JOUEURS " << joueurs.size() << '\n';

    for (const auto& j : joueurs)
    {
        f << "NOM " << j.getNom() << '\n';
        f << "PIERRES " << j.getNbrPierres() << '\n';
        f << "POINTS " << j.getNbrPoints() << '\n';
        f << "TUILE_MAIN\n";
        f <<= j.getTuileEnMain();
        const auto& tuilesPlateau = j.getPlateau().getTuiles();
        f << "PLATEAU " << tuilesPlateau.size() << '\n';
        for (const auto& t : tuilesPlateau)
            f <<= t;
    }

    std::cout << "Sauvegarde terminée.\n";
}

bool chargerTuileDepuisFichier(const std::string& nomFichier, Tuile& t)
{
    std::ifstream f(nomFichier);
    if (!f) {
        std::cerr << "Impossible d'ouvrir le fichier : " << nomFichier << '\n';
        return false;
    }

    if (!(f >>= t)) {
        std::cerr << "Erreur de format dans le fichier : " << nomFichier << '\n';
        return false;
    }

    return true;
}

