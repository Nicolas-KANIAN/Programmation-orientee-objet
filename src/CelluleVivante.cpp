#include "CelluleVivante.h"

// Constructeur de la classe CelluleVivante
CelluleVivante::CelluleVivante() {
    etatActuel = true;
}

// Retourne l'état actuel de la cellule
bool CelluleVivante::getEtatActuel() const {
    return true;
}

// Modifie l'état actuel de la cellule
void CelluleVivante::setEtatActuel(bool etat) {
    etatActuel = etat;
}

// Clone polymorphique de la cellule
Cellule* CelluleVivante::cloner() const {
    return new CelluleVivante(*this);
}
