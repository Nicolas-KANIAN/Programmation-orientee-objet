#ifndef CELLULEMORTE_H
#define CELLULEMORTE_H

#include "Cellule.h"

// Classe représentant une cellule morte, dérivée de la classe Cellule
class CelluleMorte : public Cellule {
public:
    CelluleMorte();

    bool getEtatActuel() const override;
    void setEtatActuel(bool etat) override;
    Cellule* cloner() const override;
};

#endif // CELLULEMORTE_H
