#ifndef CELLULEVIVANTE_H
#define CELLULEVIVANTE_H

#include "Cellule.h"

// Classe représentant une cellule vivante, dérivée de la classe Cellule
class CelluleVivante : public Cellule {
public:
    CelluleVivante();

    bool getEtatActuel() const override;
    void setEtatActuel(bool etat) override;
    Cellule* cloner() const override;
};

#endif // CELLULEVIVANTE_H
