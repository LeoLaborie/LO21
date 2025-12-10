#include "Sauvegarde.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <filesystem>
#include <QCoreApplication>
#include <filesystem>
#include <vector>
#include <string>

std::vector<std::string> getSauvegardes()
{
    //on récuperes le dossiers saves
    const auto cheminCourrant   = std::filesystem::current_path();
    const auto cheminComplet = cheminCourrant / "saves";
    std::vector<std::string> fichiersSave;
    const std::string extension = ".ratatata";
    std::error_code e;
    //si le dossier n'existe pas encore on le crée
    if (!std::filesystem::exists(cheminComplet, e)) {
        if (!std::filesystem::create_directories(cheminComplet, e)) {
            return fichiersSave;
        }
        return fichiersSave;
    }
    //on définit l'itérator pour le parcours des fichiers
    std::filesystem::directory_iterator it(cheminComplet, e);
    if (e) {
        return fichiersSave;
    }
    for (const auto& entry : it) {
      //on vérifie si c'est un fichier et que l'extension est bien celle des fichiers sauvegardes
        if (entry.is_regular_file() && entry.path().extension() == extension) {
            fichiersSave.push_back(entry.path().filename().string()); //on ajoutes dans le vecteur le nom du fichier
        }
    }
    return fichiersSave;
}

//récupere la dateCourante
std::string getCurrentDate() {
  std::time_t t = std::time(nullptr);
  char buf[32];
  std::strftime(buf, sizeof(buf), "%d-%m-%Y_%H-%M", std::localtime(&t));
  return buf;
}


//convertir type en string et inversement
std::string typeToString(TypeHex t) {
  switch (t) {
  case TypeHex::Habitation:
    return "Habitation";
  case TypeHex::Marche:
    return "Marche";
  case TypeHex::Temple:
    return "Temple";
  case TypeHex::Caserne:
    return "Caserne";
  case TypeHex::Jardin:
    return "Jardin";
  case TypeHex::PHabitation:
    return "PHabitation";
  case TypeHex::PMarche:
    return "PMarche";
  case TypeHex::PTemple:
    return "PTemple";
  case TypeHex::PCaserne:
    return "PCaserne";
  case TypeHex::PJardin:
    return "PJardin";
  case TypeHex::Carriere:
    return "Carriere";
  }
  return "Carriere";
}

TypeHex stringToType(const std::string &s) {
  if (s == "Habitation")
    return TypeHex::Habitation;
  if (s == "Marche")
    return TypeHex::Marche;
  if (s == "Temple")
    return TypeHex::Temple;
  if (s == "Caserne")
    return TypeHex::Caserne;
  if (s == "Jardin")
    return TypeHex::Jardin;
  if (s == "PHabitation")
    return TypeHex::PHabitation;
  if (s == "PMarche")
    return TypeHex::PMarche;
  if (s == "PTemple")
    return TypeHex::PTemple;
  if (s == "PCaserne")
    return TypeHex::PCaserne;
  if (s == "PJardin")
    return TypeHex::PJardin;
  if (s == "Carriere")
    return TypeHex::Carriere;

  throw std::invalid_argument("TypeHex inconnu: " + s);
}

//surchage opérator pour sauvegarder un hexagone => mettre les infos importantes dans le flux ostream
std::ostream &operator<<=(std::ostream &os, const Hexagone &h) {
  os << "HEX " << h.getX() << ' ' << h.getY() << ' ' << h.getZ() << ' '
     << typeToString(h.getType()) << ' ' << (h.getEstRecouvert() ? 1 : 0)
     << '\n';

  return os;
}

//surchage opérator pour charger un hexagone
std::istream &operator>>=(std::istream &is, Hexagone &h) {
  //principe : on récupere une ligne courantes où il y a normalement le début de la saugarde d'un hex
  std::string ligne;
  if (!(is >> ligne))
    return is;

  //si c'est pas un hex on modifie le bit d'erreur d'extraction pour signifier que ce n'est pas un hex
  if (ligne != "HEX") {
    is.setstate(std::ios::failbit);
    return is;
  }

  //on déclare les variables pour créer l'hexagones
  int x = 0, y = 0, z = 0;
  std::string typeStr;
  int rec = 0;

  //on utilise les operator de base de ostream pour affecter la valeur à chaque variable si il y a une erreur 
  // on le signal dans le bit d'erreur d'extraction
  if (!(is >> x >> y >> z >> typeStr >> rec)) {
    is.setstate(std::ios::failbit);
    return is;
  }
  //on affecte à l'hexagones passer en référence les valeurs de la sauvegarde
  h = Hexagone(x, y, z, stringToType(typeStr));
  h.setEstRecouvert(rec != 0);

  return is;
}

/*meme chose pour sauvegarde la tuile, mais cette fois on utilise notre opérator créer pour les hexagones 
pour sauvegarder les hexagones de la tuile
*/
std::ostream &operator<<=(std::ostream &os, const Tuile &t) {
  const auto &hexs = t.getHexagones();
  os << "TUILE " << hexs.size() << '\n';

  for (auto h : hexs)
    os <<= *h;

  return os;
}
// de même pour le chargement on utilise l'opérator surchager précédemment
std::istream &operator>>=(std::istream &is, Tuile &tuile) {
  std::string ligne;
  if (!(is >> ligne))
    return is;

  if (ligne != "TUILE") {
    is.setstate(std::ios::failbit);
    return is;
  }

  int nbHex = 0;
  if (!(is >> nbHex)) {
    is.setstate(std::ios::failbit);
    return is;
  }

  if (nbHex == 0) {
    tuile = Tuile();
    return is;
  }

  if (nbHex != 3 && nbHex != 4) {
    is.setstate(std::ios::failbit);
    return is;
  }

  //on utilise les des pointeurs temporaires pour créer les hexagones de la tuile vue qu'ils sont stocker par pointeurs
  std::vector<std::unique_ptr<Hexagone>> hexs;
  hexs.reserve(nbHex);
  for (int i = 0; i < nbHex; ++i) {
    Hexagone tmp(0, 0, 0, TypeHex::Carriere);
    if (!(is >>= tmp))
      return is;
    hexs.push_back(std::make_unique<Hexagone>(tmp));
  }

  //on change le propriétaire des pointeurs  des hexagones pour la tuile
  if (nbHex == 3)
    tuile = Tuile(hexs[0].release(), hexs[1].release(), hexs[2].release());
  else
    tuile = Tuile(hexs[0].release(), hexs[1].release(), hexs[2].release(),hexs[3].release());

  return is;
}
//on va utiliser tt les opérators de base et se surchager pour suavegarder dans les fichiers les différents objets de la partie

bool sauvegarderPartie(const Partie &p) {
  std::string date = getCurrentDate();
  std::string nom = "saves/save_" + date + ".ratatata";
  std::ofstream f(nom);
  if (!f) {
    std::cerr << "Erreur : impossible de créer la sauvegarde.\n";
    return false;
  }

  f << "PARTIE " << p.getNbrJoueurs() << ' ' << p.getNbrTours() << ' '
    << p.getMaitreArchitecte() << ' ' << p.getMainJoueur() << '\n';
  const auto &chantier = p.getChantier().getTuiles();
  f << "CHANTIER " << chantier.size() << '\n';
  for (const auto &t : chantier) {
    f <<= t;
  }
  const auto &piles = p.getPile();
  f << "PILES " << piles.size() << '\n';

  for (size_t i = 0; i < piles.size(); ++i) {
    f << "PILE " << i << " " << piles[i].size() << '\n';
    for (const auto &t : piles[i])
      f <<= t;
  }
  const auto &joueurs = p.getJoueurs();
  f << "JOUEURS " << '\n';

  for (const auto &j : joueurs) {
    f << "NOM " << j.getNom() << '\n';
    f << "PIERRES " << j.getNbrPierres() << '\n';
    f << "POINTS " << j.getNbrPoints() << '\n';
    f << "TUILE_MAIN\n";
    f <<= j.getTuileEnMain();
    const auto &tuilesPlateau = j.getPlateau().getTuiles();
    f << "PLATEAU " << tuilesPlateau.size() << '\n';
    for (const auto &t : tuilesPlateau) {
      f <<= t;
    }
    f << "VARIANTES ";
    for (unsigned int i = 0; i < 5; ++i) {
      f << j.getPlateau().getVarianteScores()[i] << ' ';
    }
    f << '\n';
  }
  f << "FAUX_JOUEUR ";
  if (p.fauxJoueurPresent()) {
    f << "1"
      << "\n";
    f << "DIFFICULTE " << p.getFauxJoueur()->getdifficulte() << "\n";
    f << "PIERRES " << p.getFauxJoueur()->getNbrPierres() << '\n';
    f << "POINTS " << p.getFauxJoueur()->getNbrPoints() << '\n';
    f << "PLATEAU " << p.getFauxJoueur()->getPlateau().getTuiles().size()
      << "\n";
    for (auto tuile : p.getFauxJoueur()->getPlateau().getTuiles()) {
      f <<= tuile;
    }
  } else
    f << '0' << '\n';
  return true;
}
//meme chose pour le chargement, on traite les erreurs aussi
Partie Partie::FromSave(const std::string &nomFichier) {
  std::ifstream f(nomFichier);
  if (!f)
    throw std::runtime_error("Impossible d'ouvrir le fichier " + nomFichier);

  auto expectLigne = [&](const std::string &attendu) {
    std::string ligne;
    if (!(f >> ligne) || ligne != attendu)
      throw std::runtime_error("Format invalide : attendu \"" + attendu + "\"");
  };

  bool variantesScore[5] = {};
  int present = 0;
  int difficulteFaux = 0, pierresFaux = 0, pointsFaux = 0;
  std::vector<Tuile> plateauFaux;

  // gestion Partie
  expectLigne("PARTIE");

  int nbJoueurs = 0;
  int nbTours = 0;
  int maitreArchitecte = 0;
  int mainJoueur = 0;

  if (!(f >> nbJoueurs >> nbTours >> maitreArchitecte >> mainJoueur))
    throw std::runtime_error(
        "Format invalide : informations de partie manquantes");

  // gestion Chantier
  expectLigne("CHANTIER");
  size_t nbChantier = 0;
  if (!(f >> nbChantier))
    throw std::runtime_error("Format invalide : taille du chantier");

  std::vector<Tuile> tuilesChantier;
  tuilesChantier.reserve(nbChantier);

  for (size_t i = 0; i < nbChantier; ++i) {
    Tuile t;
    if (!(f >>= t))
      throw std::runtime_error("Format invalide : tuile chantier");
    tuilesChantier.push_back(t);
  }

  // gestion Piles
  expectLigne("PILES");
  size_t nbPiles = 0;

  if (!(f >> nbPiles))
    throw std::runtime_error("Format invalide : nb piles");

  std::vector<std::vector<Tuile>> piles(nbPiles);

  for (size_t id = 0; id < nbPiles; ++id) {
    expectLigne("PILE");
    size_t pileId = 0, taille = 0;

    if (!(f >> pileId >> taille))
      throw std::runtime_error("Format invalide : PILE");

    if (pileId != id)
      throw std::runtime_error("Ordre des piles incorrect");

    for (size_t j = 0; j < taille; ++j) {
      Tuile t;
      if (!(f >>= t))
        throw std::runtime_error("Format invalide : tuile pile");
      piles[id].push_back(t);
    }
  }

  // gestion Joueurs
  expectLigne("JOUEURS");

  struct JoueurCharge {
    std::string nom;
    int pierres = 0;
    int points = 0;
    Tuile tuileMain;
    std::vector<Tuile> plateau;
  };

  std::vector<JoueurCharge> joueurs;
  joueurs.reserve(nbJoueurs);

  for (size_t idx = 0; idx < static_cast<size_t>(nbJoueurs); ++idx) {
    JoueurCharge data;

    expectLigne("NOM");
    if (!(f >> data.nom))
      throw std::runtime_error("Format invalide : nom joueur");

    expectLigne("PIERRES");
    if (!(f >> data.pierres))
      throw std::runtime_error("Format invalide : pierres joueur");

    expectLigne("POINTS");
    if (!(f >> data.points))
      throw std::runtime_error("Format invalide : points joueur");

    expectLigne("TUILE_MAIN");
    if (!(f >>= data.tuileMain))
      throw std::runtime_error("Format invalide : tuile main joueur");

    expectLigne("PLATEAU");
    size_t nbTP = 0;
    if (!(f >> nbTP))
      throw std::runtime_error("Format invalide : nb tuiles plateau joueur");

    data.plateau.reserve(nbTP);

    for (size_t k = 0; k < nbTP; ++k) {
      Tuile t;
      if (!(f >>= t))
        throw std::runtime_error("Format invalide : tuile du plateau joueur");
      data.plateau.push_back(t);
    }

    expectLigne("VARIANTES");
    for (int i = 0; i < 5; ++i) {
      if (!(f >> variantesScore[i]))
        throw std::runtime_error("Format invalide : variantesScore");
    }

    joueurs.push_back(std::move(data));
  }

  // gestion Faux Joueur
  expectLigne("FAUX_JOUEUR");
  if (!(f >> present))
    throw std::runtime_error("Format invalide : FAUX_JOUEUR");

  if (present == 1) {
    expectLigne("DIFFICULTE");
    if (!(f >> difficulteFaux))
      throw std::runtime_error("Format invalide : difficulté IA");

    expectLigne("PIERRES");
    if (!(f >> pierresFaux))
      throw std::runtime_error("Format invalide : pierres IA");

    expectLigne("POINTS");
    if (!(f >> pointsFaux))
      throw std::runtime_error("Format invalide : points IA");

    expectLigne("PLATEAU");
    size_t nbIA = 0;
    if (!(f >> nbIA))
      throw std::runtime_error("Format invalide : taille plateau IA");

    plateauFaux.reserve(nbIA);

    for (size_t k = 0; k < nbIA; ++k) {
      Tuile t;
      if (!(f >>= t))
        throw std::runtime_error("Format invalide : tuile IA");
      plateauFaux.push_back(t);
    }
  }

  // construction des objets
  std::vector<Joueur> joueursConstruits;
  joueursConstruits.reserve(joueurs.size());

  for (auto &j : joueurs) {
    joueursConstruits.push_back(
        Joueur::fromSave(variantesScore, std::move(j.nom), j.pierres, j.points,
                         std::move(j.tuileMain), std::move(j.plateau)));
  }

  Chantier chantierConstruits = Chantier::fromSave(std::move(tuilesChantier));

  Partie partie(nbJoueurs, nbTours, maitreArchitecte, mainJoueur,
                std::move(chantierConstruits), std::move(piles),
                std::move(joueursConstruits));

  if (present == 1) {
    partie.fauxJoueur =
        IllustreArchitecte::fromSave(difficulteFaux, pierresFaux, pointsFaux,
                                     variantesScore, std::move(plateauFaux));
  }

  return partie;
}
