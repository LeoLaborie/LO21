#pragma once

#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "Partie.h"
#include "Position.h"
#include "Sauvegarde.h"
#include "couleurs_console.h"

Partie CreerNouvellePartie();
void controleConsole();
void lancerPartie(Partie& partie);