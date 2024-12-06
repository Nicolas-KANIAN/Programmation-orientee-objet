#ifndef SIMULATION_H
#define SIMULATION_H

#include "Grille.h"
#include "FichierTexte.h"
#include <string>

// Classe gérant la logique globale d'une simulation d'automate cellulaire
class Simulation {
private:
    Grille* grille;             // Grille de la simulation
    int nombreIterations;       // Nombre total d'itérations prévues
    int iterationActuelle;      // Numéro de l'itération en cours
    FichierTexte fichierTexte;  // Gestionnaire de persistance fichier

public:
    // Constructeur et Destructeur
    Simulation(int largeur, int hauteur, bool torique, int nombreIterations);
    ~Simulation();

    // Protection contre les copies accidentelles (évite les double-free sur grille)
    Simulation(const Simulation&) = delete;
    Simulation& operator=(const Simulation&) = delete;

    // Évolution de la simulation
    void step();        // Avance d'une seule génération
    void demarrer();    // Compatibilité : avance d'une étape sans affichage console forcé

    // Gestion des fichiers
    bool chargerEtatInitial(const std::string& cheminFichier);
    void sauvegarderEtatCourant(const std::string& cheminFichier);

    // Accesseurs et Mutateurs
    Grille* getGrille() const { return grille; }
    void setGrille(Grille* nouvelleGrille);

    int getNombreIterations() const { return nombreIterations; }
    int getIterationActuelle() const { return iterationActuelle; }
    void setIterationActuelle(int it) { iterationActuelle = it; }
};

#endif // SIMULATION_H
