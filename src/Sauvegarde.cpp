#include "Sauvegarde.h"
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <sys/stat.h>

std::string getCurrentDateTime()
{
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    std::tm* tm = std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(tm, "%d-%m-%Y_%H-%M-%S");
    return oss.str();
}


std::string typeToString(TypeHex type)
{
    switch (type)
    {
    case TypeHex::Habitation:  return "Habitation";
    case TypeHex::Marche:      return "Marche";
    case TypeHex::Temple:      return "Temple";
    case TypeHex::Caserne:     return "Caserne";
    case TypeHex::Jardin:      return "Jardin";
    case TypeHex::PHabitation: return "PHabitation";
    case TypeHex::PMarche:     return "PMarche";
    case TypeHex::PTemple:     return "PTemple";
    case TypeHex::PCaserne:    return "PCaserne";
    case TypeHex::PJardin:     return "PJardin";
    case TypeHex::Carriere:    return "Carriere";
    }
    return "Inconnu";
}


void saveHex(std::ostream& os, const Hexagone* hex)
{
    os << "Hexagone {\n";
    os << "  x=" << hex->getX() << "\n";
    os << "  y=" << hex->getY() << "\n";
    os << "  z=" << hex->getZ() << "\n";
    os << "  type=" << typeToString(hex->getType()) << "\n";
    os << "  mult=" << hex->getMultiplicateur() << "\n";
    os << "  recouvert=" << hex->getEstRecouvert() << "\n";
    os << "}\n";
}

void saveTuile(std::ostream& os, const Tuile& t)
{
    os << "Tuile {\n";

    const auto& hexs = t.getHexagones();
    for (size_t i = 0; i < hexs.size(); ++i)
    {
        os << "  hex_" << i << ":\n";
        saveHex(os, hexs[i]);
    }

    const auto& offs = t.getOffsets();
    for (size_t i = 0; i < offs.size(); ++i)
        os << "  offset_" << i << ": (" << offs[i].q << "," << offs[i].r << ")\n";

    os << "}\n";
}


void savePlateau(std::ostream& os, const Plateau& plateau)
{
    os << "Plateau {\n";
    os << "}\n";
}


void sauvegarderPartie(const Partie& p)
{
    std::string date = getCurrentDateTime();
    std::string nom="saves/save_" + date + ".ratatata";
    std::fstream f(nom, std::ios::out);
    chmod(nom.c_str(), 0444);
    if (!f)
    {
        std::cerr << "Impossible d’ouvrir le fichier de sauvegarde.\n";
        return;
    }

    f << "=== SAUVEGARDE AKROPOLIS ===\n";
    f << "date=" << date << "\n\n";

    f << "[PARTIE]\n";
    f << "nb_joueurs=" << p.getNbrJoueurs() << "\n";
    f << "tour=" << p.getNbrTours() << "\n";
    f << "maitre_archi=" << p.getMaitreArchitecte() << "\n";
    f << "joueur_courant=" << p.getMainJoueur() << "\n\n";

    f << "[JOUEURS]\n";
    const auto& joueurs = p.getJoueurs();
    for (size_t i = 0; i < joueurs.size(); ++i)
    {
        const auto& j = joueurs[i];
        f << "Joueur " << i << " {\n";
        f << "  nom=" << j.getNom() << "\n";
        f << "  pierres=" << j.getNbrPierres() << "\n";
        f << "  points=" << j.getNbrPoints() << "\n";

        f << "  plateau=\n";
        savePlateau(f, j.getPlateau());

        f << "  tuile_en_main=\n";
        saveTuile(f, j.getTuileEnMain());

        f << "}\n\n";
    }

    std::cout << "Sauvegarde réalisée : saves/save_" << date << ".txt\n";
}
bool charger(const std::string& nomSauvegarde){
    std::ifstream f(nomSauvegarde);
    if (!f) {
        std::cerr << "Impossible d’ouvrir la sauvegarde : " << nomSauvegarde << "\n";
        return false;
    }
    std::string ligne;
    std::map<std::string, std::string> data;
    while (std::getline(f, ligne))
    {
        size_t pos = ligne.find('=');
        if (pos == std::string::npos) continue;

        std::string cle = ligne.substr(0, pos);
        std::string valeur = ligne.substr(pos + 1);

        data[cle] = valeur;
    }
}