#include "Grille.h"
#include <iostream>
#include <utility>

// Libère la mémoire de toutes les cellules allouées
void Grille::libererCellules() {
    for (int x = 0; x < largeur; ++x) {
        for (int y = 0; y < hauteur; ++y) {
            delete cellules[x][y];
            cellules[x][y] = nullptr;
        }
    }
}

// Constructeur principal
Grille::Grille(int largeur, int hauteur, bool torique)
    : largeur(largeur), hauteur(hauteur), torique(torique),
      cellules(largeur, std::vector<Cellule*>(hauteur, nullptr)),
      etatsSuivants(largeur, std::vector<bool>(hauteur, false)) {
    
    // Initialise toutes les cellules avec des cellules mortes par défaut
    for (int x = 0; x < largeur; ++x) {
        for (int y = 0; y < hauteur; ++y) {
            cellules[x][y] = new CelluleMorte();
        }
    }
}

// Destructeur
Grille::~Grille() {
    libererCellules();
}

// Constructeur de copie (copie profonde via cloner())
Grille::Grille(const Grille& autre)
    : largeur(autre.largeur), hauteur(autre.hauteur), torique(autre.torique),
      cellules(autre.largeur, std::vector<Cellule*>(autre.hauteur, nullptr)),
      etatsSuivants(autre.etatsSuivants) {
    for (int x = 0; x < largeur; ++x) {
        for (int y = 0; y < hauteur; ++y) {
            if (autre.cellules[x][y]) {
                cellules[x][y] = autre.cellules[x][y]->cloner();
            }
        }
    }
}

// Opérateur d'affectation par copie
Grille& Grille::operator=(const Grille& autre) {
    if (this != &autre) {
        libererCellules();

        largeur = autre.largeur;
        hauteur = autre.hauteur;
        torique = autre.torique;
        etatsSuivants = autre.etatsSuivants;
        cellules.assign(largeur, std::vector<Cellule*>(hauteur, nullptr));

        for (int x = 0; x < largeur; ++x) {
            for (int y = 0; y < hauteur; ++y) {
                if (autre.cellules[x][y]) {
                    cellules[x][y] = autre.cellules[x][y]->cloner();
                }
            }
        }
    }
    return *this;
}

// Constructeur de déplacement
Grille::Grille(Grille&& autre) noexcept
    : largeur(autre.largeur), hauteur(autre.hauteur), torique(autre.torique),
      cellules(std::move(autre.cellules)),
      etatsSuivants(std::move(autre.etatsSuivants)) {
    autre.largeur = 0;
    autre.hauteur = 0;
}

// Opérateur d'affectation par déplacement
Grille& Grille::operator=(Grille&& autre) noexcept {
    if (this != &autre) {
        libererCellules();

        largeur = autre.largeur;
        hauteur = autre.hauteur;
        torique = autre.torique;
        cellules = std::move(autre.cellules);
        etatsSuivants = std::move(autre.etatsSuivants);

        autre.largeur = 0;
        autre.hauteur = 0;
    }
    return *this;
}

// Ajuste les coordonnées pour la grille torique
int Grille::coordonneeTorique(int coord, int max) const {
    return (coord % max + max) % max;
}

// Compte le nombre de voisines vivantes d'une cellule
int Grille::compterVoisinesVivantes(int x, int y) const {
    int count = 0;

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;

            int nx = x + dx;
            int ny = y + dy;

            if (torique) {
                nx = coordonneeTorique(nx, largeur);
                ny = coordonneeTorique(ny, hauteur);
            } else if (nx < 0 || ny < 0 || nx >= largeur || ny >= hauteur) {
                continue;
            }

            if (cellules[nx][ny] && cellules[nx][ny]->getEtatActuel()) {
                ++count;
            }
        }
    }

    return count;
}

// Compte le nombre total de cellules vivantes dans la grille
int Grille::compterCellulesVivantes() const {
    int count = 0;
    for (int x = 0; x < largeur; ++x) {
        for (int y = 0; y < hauteur; ++y) {
            if (cellules[x][y] && cellules[x][y]->getEtatActuel()) {
                ++count;
            }
        }
    }
    return count;
}

// Calcule l'état suivant d'une cellule selon les règles de Conway
void Grille::calculerEtatSuivant(int x, int y) {
    int nbVoisinesVivantes = compterVoisinesVivantes(x, y);
    bool estVivante = cellules[x][y] && cellules[x][y]->getEtatActuel();

    if (!estVivante) {
        // Une cellule morte avec exactement 3 voisines devient vivante (naissance)
        etatsSuivants[x][y] = (nbVoisinesVivantes == 3);
    } else {
        // Une cellule vivante avec 2 ou 3 voisines reste vivante (survie), sinon meurt
        etatsSuivants[x][y] = (nbVoisinesVivantes == 2 || nbVoisinesVivantes == 3);
    }
}

// Fait évoluer la grille à l'étape suivante (Transition synchrone)
void Grille::next() {
    // 1. Calculer les états futurs pour toute la grille
    for (int x = 0; x < largeur; ++x) {
        for (int y = 0; y < hauteur; ++y) {
            calculerEtatSuivant(x, y);
        }
    }

    // 2. Mettre à jour les cellules polymorphiquement
    for (int x = 0; x < largeur; ++x) {
        for (int y = 0; y < hauteur; ++y) {
            bool futurEtat = etatsSuivants[x][y];
            bool actuelEtat = cellules[x][y] && cellules[x][y]->getEtatActuel();

            // Remplacement uniquement en cas de changement d'état
            if (futurEtat != actuelEtat) {
                delete cellules[x][y];
                if (futurEtat) {
                    cellules[x][y] = new CelluleVivante();
                } else {
                    cellules[x][y] = new CelluleMorte();
                }
            }
        }
    }
}

// Affiche la grille dans la console
void Grille::afficherConsole() const {
    for (int y = 0; y < hauteur; ++y) {
        for (int x = 0; x < largeur; ++x) {
            bool vivante = cellules[x][y] && cellules[x][y]->getEtatActuel();
            std::cout << (vivante ? '1' : '0') << " ";
        }
        std::cout << '\n';
    }
}

// Définit une cellule à une position (x, y)
void Grille::setCellule(int x, int y, Cellule* cellule) {
    if (torique) {
        x = coordonneeTorique(x, largeur);
        y = coordonneeTorique(y, hauteur);
    }

    if (x >= 0 && x < largeur && y >= 0 && y < hauteur) {
        delete cellules[x][y];
        cellules[x][y] = cellule;
    } else {
        delete cellule; // Évite les fuites mémoire si les coordonnées sont hors limites
    }
}

// Bascule l'état d'une cellule (pratique pour l'édition interactive)
void Grille::basculerCellule(int x, int y) {
    Cellule* c = getCellule(x, y);
    if (c) {
        if (c->getEtatActuel()) {
            setCellule(x, y, new CelluleMorte());
        } else {
            setCellule(x, y, new CelluleVivante());
        }
    }
}

// Obtient une cellule à une position donnée
Cellule* Grille::getCellule(int x, int y) const {
    if (torique) {
        x = coordonneeTorique(x, largeur);
        y = coordonneeTorique(y, hauteur);
    } else if (x < 0 || x >= largeur || y < 0 || y >= hauteur) {
        return nullptr;
    }

    return cellules[x][y];
}
