#include "CelluleMorte.h"

// Constructeur de la classe CelluleMorte
CelluleMorte::CelluleMorte() {
    etatActuel = false;
}

// Retourne l'état actuel de la cellule
bool CelluleMorte::getEtatActuel() const {
    return false;
}

// Modifie l'état actuel de la cellule
void CelluleMorte::setEtatActuel(bool etat) {
    etatActuel = etat;
}

// Clone polymorphique de la cellule
Cellule* CelluleMorte::cloner() const {
    return new CelluleMorte(*this);
}
