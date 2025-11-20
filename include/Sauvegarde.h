#ifndef SAUVEGARDE_H
#define SAUVEGARDE_H

#include <iosfwd>
#include <string>
#include "Partie.h"

/**
 * @brief Convertit un TypeHex en chaîne lisible.
 * @param t Type d'hexagone à convertir.
 * @return Nom du type sous forme de chaîne.
 */
std::string typeToString(TypeHex t);

/**
 * @brief Convertit une chaîne en TypeHex.
 * @param s Chaîne représentant un type d'hexagone.
 * @return Valeur TypeHex correspondante.
 * @throws std::invalid_argument si la chaîne est inconnue.
 */
TypeHex stringToType(const std::string& s);

/**
 * @brief Sauvegarde l'état complet d'une partie sur disque.
 * @param p Partie à sauvegarder.
 */
void sauvegarderPartie(const Partie& p);

/**
 * @brief Sérialise un hexagone vers un flux de sortie.
 * @param os Flux cible.
 * @param h Hexagone à écrire.
 * @return Le flux pour chaînage.
 */
std::ostream& operator<<=(std::ostream& os, const Hexagone& h);

/**
 * @brief Désérialise un hexagone depuis un flux d'entrée.
 * @param is Flux source.
 * @param h Hexagone à remplir.
 * @return Le flux pour chaînage.
 */
std::istream& operator>>=(std::istream& is, Hexagone& h);

/**
 * @brief Sérialise une tuile vers un flux.
 * @param os Flux cible.
 * @param t Tuile à écrire.
 * @return Le flux pour chaînage.
 */
std::ostream& operator<<=(std::ostream& os, const Tuile& t);

/**
 * @brief Désérialise une tuile depuis un flux.
 * @param is Flux source.
 * @param t Tuile à remplir.
 * @return Le flux pour chaînage.
 */
std::istream& operator>>=(std::istream& is, Tuile& t);

/**
 * @brief Charge une tuile depuis un fichier texte au format attendu.
 * @param nomFichier Chemin du fichier à lire.
 * @param t Tuile résultat si le chargement réussit.
 * @return true en cas de succès, false sinon.
 */
bool chargerTuileDepuisFichier(const std::string& nomFichier, Tuile& t);

#endif 
