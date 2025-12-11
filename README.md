# LO21

Projet de LO21 sur le jeu **Akropolis** réalisé en C++ / Qt6.  
Le dépôt contient à la fois une interface graphique et un mode console permettant de jouer et de tester les règles.

## Prérequis

- CMake ≥ 3.15
- Un compilateur C++17 (g++, clang++, MSVC…)
- Qt 6 Core/Gui/Widgets

## Compilation rapide

```bash
cmake -S . -B build
cmake --build build
```

Les ressources sont copiées automatiquement dans `build/bin/img` à la fin de la compilation, et l’exécutable se trouve dans `build/bin/lo21`.

## Paramètres de lancement

L’exécutable accepte un unique argument déterminant le mode de lancement :

- `-g` ou `-G` : lance l’interface graphique Qt (comportement par défaut si aucun argument n’est fourni).
- `-c` ou `-C` : lance le contrôleur console.
- `-h`, `-H` ou `--help` : affiche ce fichier README directement dans le terminal.

Si aucun argument n'est fournis le jeu se lance en interface Graphique.

Exemples :

```bash
./build/bin/lo21           # interface graphique
./build/bin/lo21 -c        # mode console
./build/bin/lo21 --help    # afficher l’aide
```

## Structure du projet

- `src/` : logique métier (plateau, règles, modèles…)
- `srcView/` et `IncludeView/` : widgets Qt, contrôleur console, widgets utilitaires.
- `img/` : ressources graphiques copiées aux côtés de l’exécutable.
- `saves/` : fichiers de sauvegarde générés au runtime.

## Crédit

Thibault de Boutray , Léo Laborie , Dimitri Périgois et Jules Chiron

