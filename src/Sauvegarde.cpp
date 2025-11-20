#include "Sauvegarde.h"
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <memory>
#include <map>
#include <iostream>

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
    return "Carriere";
}


void saveHex(std::ostream& os, const Hexagone* hex)
{
    os << "Hexagone {\n";
    os << "  x=" << hex->getX() << "\n";
    os << "  y=" << hex->getY() << "\n";
    os << "  z=" << hex->getZ() << "\n";
    os << "  type=" << typeToString(hex->getType()) << "\n";
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
    const auto& tuiles = plateau.getTuiles();
    os << "Plateau {\n";
    os << "  nb_tuiles=" << tuiles.size() << "\n";
    for (size_t i = 0; i < tuiles.size(); ++i)
    {
    os << "  tuile_" << i << "=\n";
        saveTuile(os, tuiles[i]);
    }
    os << "}\n";
}


void sauvegarderPartie(const Partie& p)
{
    std::string date = getCurrentDateTime();
    std::string nom = "saves/save_" + date + ".ratatata";
    std::ofstream f(nom);
    if (!f)
    {
        std::cerr << "Impossible d’ouvrir le fichier de sauvegarde.\n";
        return;
    }

    f << "=== SAUVEGARDE AKROPOLIS ===\n";
    f << "[PARTIE]\n";
    f << "nb_joueurs=" << p.getNbrJoueurs() << "\n";
    f << "tour=" << p.getNbrTours() << "\n";
    f << "maitre_archi=" << p.getMaitreArchitecte() << "\n";
    f << "joueur_courant=" << p.getMainJoueur() << "\n\n";

    f << "[CHANTIER]\n";
    const auto &tuilesChantier = p.getChantier().getTuiles();
    f << "nb_tuiles=" << tuilesChantier.size() << "\n";
    for (size_t i = 0; i < tuilesChantier.size(); ++i)
    {
        f << "Tuile chantier " << i << ":\n";
        saveTuile(f, tuilesChantier[i]);
    }
    f << "\n";

    f << "[PILES]\n";
    const auto &piles = p.getPile();
    f << "nb_piles=" << piles.size() << "\n";
    for (size_t i = 0; i < piles.size(); ++i)
    {
        f << "Pile " << i << " (taille=" << piles[i].size() << ")\n";
        for (size_t j = 0; j < piles[i].size(); ++j)
        {
            f << "  Tuile pile " << i << "-" << j << ":\n";
            saveTuile(f, piles[i][j]);
        }
    }
    f << "\n";

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

    std::cout << "Sauvegarde réalisée : " << nom << "\n";
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




bool lireHexagone(std::ifstream& f, std::unique_ptr<Hexagone>& hex, std::vector<std::string>& erreurs)
{
    std::string ligne;
    int x = 0, 
    y = 0, 
    z = 0;
    bool recouvert = false;
    std::string typeStr;
    bool finBloc = false;

    while (std::getline(f, ligne))
    {
        if (ligne.find('}') != std::string::npos) { 
            finBloc = true; 
            break; 
        }
        auto pos = ligne.find('=');
        if (pos == std::string::npos) continue;
        auto cle = ligne.substr(0, pos);
        auto val = ligne.substr(pos + 1);
        if (cle.find("x") != std::string::npos) x = std::stoi(val);
        else if (cle.find("y") != std::string::npos) y = std::stoi(val);
        else if (cle.find("z") != std::string::npos) z = std::stoi(val);
        else if (cle.find("type") != std::string::npos) typeStr = val;
        else if (cle.find("recouvert") != std::string::npos) recouvert = std::stoi(val) != 0;
    }
    if (!finBloc)
    {
        erreurs.push_back("Hexagone incomplet");
        return false;
    }
    auto type = stringToType(typeStr);
    hex = std::make_unique<Hexagone>(x, y, z, type);
    hex->setEstRecouvert(recouvert);
    return true;
}


bool lireTuile(std::ifstream& f, Tuile& tuile, std::vector<std::string>& erreurs)
{
    std::string ligne;
    bool inside = false;
    std::vector<std::unique_ptr<Hexagone>> hexs;

    while (std::getline(f, ligne))
    {
        if (!inside)
        {
            if (ligne.find("Tuile") != std::string::npos && ligne.find('{') != std::string::npos)
                inside = true;
            continue;
        }

        if (ligne.find("Hexagone {") != std::string::npos)
        {
            std::unique_ptr<Hexagone> h;
            if (!lireHexagone(f, h, erreurs)) return false;
            hexs.push_back(std::move(h));
        }
        else if (ligne.find('}') != std::string::npos)
        {
            break; 
        }
    }

    if (hexs.size() != 3 && hexs.size() != 4)
    {
        erreurs.push_back("Tuile avec nombre d'hexagones invalide: " + std::to_string(hexs.size()));
        return false;
    }

    if (hexs.size() == 3)
        tuile = Tuile(hexs[0].release(), hexs[1].release(), hexs[2].release());
    else
        tuile = Tuile(hexs[0].release(), hexs[1].release(), hexs[2].release(), hexs[3].release());
    return true;
}

bool lirePlateau(std::ifstream& f, std::vector<std::string>& erreurs)
{
    std::string ligne;
    if (!std::getline(f, ligne) || ligne.find('{') == std::string::npos)
    {
        erreurs.push_back("Plateau manquant après plateau=");
        return false;
    }

    int nbTuiles = -1;
    while (std::getline(f, ligne))
    {
        if (ligne.find("nb_tuiles=") != std::string::npos)
        {
            nbTuiles = std::stoi(ligne.substr(ligne.find('=') + 1));
        }
        else if (ligne.find("tuile_") != std::string::npos && ligne.find('=') != std::string::npos)
        {
            Tuile t;
            if (!lireTuile(f, t, erreurs)) return false;
        }
        else if (ligne.find('}') != std::string::npos)
        {
            break;
        }
    }
    if (nbTuiles < 0) erreurs.push_back("nb_tuiles manquant dans Plateau");
    return erreurs.empty();
}

bool lirePartie(std::ifstream& f, PartieSauvegarde& p, std::vector<std::string>& erreurs) {
    p.nbrJoueurs = p.maitreArchitecte = p.mainJoueur = p.nbrTours = -1;

    std::string ligne;
    while (std::getline(f, ligne)) {
        if (ligne.empty()) break;
        auto pos = ligne.find('=');
        if (pos == std::string::npos) continue;

        auto cle = ligne.substr(0, pos);
        auto val = ligne.substr(pos + 1);

        if (cle == "nb_joueurs") p.nbrJoueurs = std::stoi(val);
        else if (cle == "tour") p.nbrTours = std::stoi(val);
        else if (cle == "maitre_archi") p.maitreArchitecte = std::stoi(val);
        else if (cle == "joueur_courant") p.mainJoueur = std::stoi(val);
    }

    if (p.nbrJoueurs <= 0) erreurs.push_back("nb_joueurs manquant ou <= 0");
    if (p.nbrTours < 0) erreurs.push_back("tour manquant ou < 0");
    if (p.maitreArchitecte < 0 || p.maitreArchitecte >= p.nbrJoueurs) erreurs.push_back("maitre_archi hors bornes");
    if (p.mainJoueur < 0 || p.mainJoueur >= p.nbrJoueurs) erreurs.push_back("joueur_courant hors bornes");

    return erreurs.empty();
}

bool lireChantier(std::ifstream& f, PartieSauvegarde& p, std::vector<std::string>& erreurs)
{
    std::string ligne;
    int nbTuiles = -1;
    if (std::getline(f, ligne))
    {
        auto pos = ligne.find('=');
        if (pos != std::string::npos)
        {
            nbTuiles = std::stoi(ligne.substr(pos + 1));
        }
    }
    if (nbTuiles < 0) erreurs.push_back("nb_tuiles manquant ou < 0 dans [CHANTIER]");

    for (int i = 0; i < nbTuiles; ++i)
    {
        if (!std::getline(f, ligne)) { 
            erreurs.push_back("Tuile chantier manquante"); 
            break; }
        Tuile t;
        if (!lireTuile(f, t, erreurs)) break;
    }
    return erreurs.empty();
}

bool lirePiles(std::ifstream& f, PartieSauvegarde& p, std::vector<std::string>& erreurs)
{
    std::string ligne;
    int nbPiles = -1;
    if (std::getline(f, ligne))
    {
        auto pos = ligne.find('=');
        if (pos != std::string::npos)
        {
            nbPiles = std::stoi(ligne.substr(pos + 1));
        }
    }
    if (nbPiles < 0) erreurs.push_back("nombre de pile manquant ou < 0");

    for (int i = 0; i < nbPiles; ++i)
    {
        if (!std::getline(f, ligne)) { 
            erreurs.push_back("En-tête de pile manquant"); 
            break; 
        }
        auto posTaille = ligne.find("taille=");
        int taillePile = 0;
        if (posTaille != std::string::npos)
        {
            taillePile = std::stoi(ligne.substr(posTaille + 7));
        }
        for (int j = 0; j < taillePile; ++j)
        {
        if (!std::getline(f, ligne)) { 
            erreurs.push_back("Tuile pile manquante"); 
            break; }
            Tuile t;
            if (!lireTuile(f, t, erreurs)) break;
        }
    }
    return erreurs.empty();
}

bool lireJoueurs(std::ifstream& f, PartieSauvegarde& p, std::vector<std::string>& erreurs)
{
    std::string ligne;
    while (std::getline(f, ligne))
    {
        if (ligne.empty()) continue;
        if (ligne.find("Joueur") == std::string::npos) continue;

        std::string nom; int pierres=-1; int points=-1;
        bool plateauLu=false, tuileMainLu=false;

        while (std::getline(f, ligne))
        {
            if (ligne.find("nom=") != std::string::npos)
                nom = ligne.substr(ligne.find('=') + 1);
            else if (ligne.find("pierres=") != std::string::npos)
            {
                pierres = std::stoi(ligne.substr(ligne.find('=') + 1));
            }
            else if (ligne.find("points=") != std::string::npos)
            {
                points = std::stoi(ligne.substr(ligne.find('=') + 1));
            }
            else if (ligne.find("plateau=") != std::string::npos)
            {
                plateauLu = lirePlateau(f, erreurs);
            }
            else if (ligne.find("tuile_en_main=") != std::string::npos)
            {
                Tuile t;
                if (!lireTuile(f, t, erreurs)) break;
                tuileMainLu = true;
            }
            else if (ligne.find('}') != std::string::npos)
            {
                break;
            }
        }
        if (nom.empty()) erreurs.push_back("nom de joueur manquant");
        if (pierres < 0) erreurs.push_back("pierres manquant/invalides pour un joueur");
        if (points < 0) erreurs.push_back("points manquant/invalides pour un joueur");
        if (!plateauLu) erreurs.push_back("plateau manquant pour un joueur");
        if (!tuileMainLu) erreurs.push_back("tuile_en_main manquante pour un joueur");
    }
    return erreurs.empty();
}



bool chargerPartie(const std::string& nomSauvegarde){
    PartieSauvegarde p;
    std::vector<std::string> erreurs;
    std::ifstream f(nomSauvegarde);
    if (!f) {
        std::cerr << "Impossible d’ouvrir la sauvegarde : " << nomSauvegarde << "\n";
        return false;
    }
    std::string ligne;
    while (std::getline(f, ligne))
    {
       if (ligne=="[PARTIE]"){
            if (!lirePartie(f, p, erreurs)) break;
       } else if (ligne=="[CHANTIER]"){
            if (!lireChantier(f, p, erreurs)) break;
       } else if (ligne=="[PILES]") {
            if (!lirePiles(f, p, erreurs)) break;
       } else if (ligne=="[JOUEURS]") {
            if (!lireJoueurs(f, p, erreurs)) break;
       }
    }
    if (!erreurs.empty()) {
        std::cerr << "Erreurs lors du chargement de la partie :\n";
        for (const auto& err : erreurs) std::cerr << "  - " << err << "\n";
        return false;
    }
    return true;
}
