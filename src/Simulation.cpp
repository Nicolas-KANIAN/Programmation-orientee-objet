#include "Simulation.h"
#include <iostream>

// Constructeur de la classe Simulation
Simulation::Simulation(int largeur, int hauteur, bool torique, int nombreIterations)
    : grille(new Grille(largeur, hauteur, torique)),
      nombreIterations(nombreIterations),
      iterationActuelle(0) {}

// Destructeur
Simulation::~Simulation() {
    delete grille;
}

// Remplacement de la grille
void Simulation::setGrille(Grille* nouvelleGrille) {
    if (grille != nouvelleGrille) {
        delete grille;
        grille = nouvelleGrille;
    }
}

// Fait avancer la simulation d'une génération
void Simulation::step() {
    if (grille) {
        grille->next();
        iterationActuelle++;
    }
}

// Méthode de démarrage (un pas d'évolution)
void Simulation::demarrer() {
    step();
}

// Charge un état initial depuis un fichier
bool Simulation::chargerEtatInitial(const std::string& cheminFichier) {
    if (!grille) return false;
    try {
        fichierTexte.charger(cheminFichier, *grille);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors du chargement : " << e.what() << std::endl;
        return false;
    }
}

// Sauvegarde l'état courant dans un fichier
void Simulation::sauvegarderEtatCourant(const std::string& cheminFichier) {
    if (grille) {
        fichierTexte.sauvegarder(cheminFichier, *grille);
    }
}
