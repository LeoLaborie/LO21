#ifndef SAUVEGARDE_H
#define SAUVEGARDE_H
#include <string>
#include "Partie.h"

void sauvegarderPartie(const Partie& partie);
bool charger(const std::string& nomSauvegarde);
#endif