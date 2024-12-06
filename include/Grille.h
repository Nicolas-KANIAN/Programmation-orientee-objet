#ifndef GRILLE_H
#define GRILLE_H

#include "Cellule.h"
#include "CelluleMorte.h"
#include "CelluleVivante.h"
#include <vector>

// Classe représentant une grille de cellules pour l'automate cellulaire
class Grille {
private:
    int largeur;                                    // Largeur de la grille (colonnes)
    int hauteur;                                    // Hauteur de la grille (lignes)
    bool torique;                                   // Indique si la grille est torique (bords connectés)
    std::vector<std::vector<Cellule*>> cellules;    // Matrice de cellules polymorphiques
    std::vector<std::vector<bool>> etatsSuivants;   // Mémoire tampon pour l'état à l'itération t+1

    // Utilitaires internes
    int coordonneeTorique(int coord, int max) const;
    void calculerEtatSuivant(int x, int y);
    void libererCellules();

public:
    // Constructeur principal
    Grille(int largeur, int hauteur, bool torique);

    // Destructeur
    ~Grille();

    // Règle des 5 (Copie et Déplacement)
    Grille(const Grille& autre);
    Grille& operator=(const Grille& autre);
    Grille(Grille&& autre) noexcept;
    Grille& operator=(Grille&& autre) noexcept;

    // Accesseurs
    int getLargeur() const { return largeur; }
    int getHauteur() const { return hauteur; }
    bool estTorique() const { return torique; }
    void setTorique(bool t) { torique = t; }

    // Évolution et calcul
    void next();
    int compterVoisinesVivantes(int x, int y) const;
    int compterCellulesVivantes() const;

    // Affichage
    void afficherConsole() const;

    // Manipulation des cellules
    void setCellule(int x, int y, Cellule* cellule);
    Cellule* getCellule(int x, int y) const;
    void basculerCellule(int x, int y);
};

#endif // GRILLE_H
