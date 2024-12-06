#ifndef INTERFACE_GRAPHIQUE_H
#define INTERFACE_GRAPHIQUE_H

#include <SFML/Graphics.hpp>
#include "Simulation.h"

// Classe gérant l'affichage graphique interactif avec SFML
class InterfaceGraphique {
private:
    sf::RenderWindow window;
    int cellSize;
    Simulation* simulation;
    bool enPause;
    float intervalleMs;

    void mettreAJourTitre();

public:
    InterfaceGraphique(int width, int height, int cellSize, Simulation* sim);
    ~InterfaceGraphique() = default;

    // Boucle d'exécution principale
    void executer();

    // Rendu et gestion des événements
    void afficherGrille();
    void gererInteractionsUtilisateur();
    void mettreAJourAffichage();

    // Contrôles de cycle de vie
    void demarrer();
    void arreter();
    bool estOuverte() const;
};

#endif // INTERFACE_GRAPHIQUE_H
