#include "InterfaceGraphique.h"
#include "Grille.h"
#include <iostream>
#include <string>

// Constructeur de la classe InterfaceGraphique
InterfaceGraphique::InterfaceGraphique(int width, int height, int cellSize, Simulation* sim)
    : window(sf::VideoMode(width * cellSize, height * cellSize), "Jeu de la Vie - Initialisation"),
      cellSize(cellSize),
      simulation(sim),
      enPause(true),
      intervalleMs(100.0f) {
    window.setFramerateLimit(60);
    mettreAJourTitre();
}

// Met à jour le titre de la fenêtre avec les informations de statut
void InterfaceGraphique::mettreAJourTitre() {
    if (!simulation || !simulation->getGrille()) return;

    std::string statut = enPause ? "[PAUSE]" : "[EN COURS]";
    std::string titre = "Jeu de la Vie " + statut +
                        " | Gen: " + std::to_string(simulation->getIterationActuelle()) +
                        " | Vivantes: " + std::to_string(simulation->getGrille()->compterCellulesVivantes()) +
                        " | [ESPACE]: Play/Pause | [->]: Pas | [Clic]: Editer";
    window.setTitle(titre);
}

// Affiche la grille et les cellules dans la fenêtre
void InterfaceGraphique::afficherGrille() {
    if (!simulation || !simulation->getGrille()) return;

    Grille* grille = simulation->getGrille();
    window.clear(sf::Color(15, 17, 22)); // Fond sombre

    sf::RectangleShape cellShape(sf::Vector2f(cellSize - 1.0f, cellSize - 1.0f));

    for (int x = 0; x < grille->getLargeur(); ++x) {
        for (int y = 0; y < grille->getHauteur(); ++y) {
            cellShape.setPosition(static_cast<float>(x * cellSize), static_cast<float>(y * cellSize));
            Cellule* c = grille->getCellule(x, y);
            bool vivante = c && c->getEtatActuel();

            if (vivante) {
                cellShape.setFillColor(sf::Color(46, 204, 113)); // Vert émeraude
            } else {
                cellShape.setFillColor(sf::Color(28, 32, 40));   // Gris sombre pour cellules mortes
            }
            window.draw(cellShape);
        }
    }

    window.display();
}

// Met à jour l'affichage
void InterfaceGraphique::mettreAJourAffichage() {
    afficherGrille();
}

// Gère les événements clavier et souris
void InterfaceGraphique::gererInteractionsUtilisateur() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        } else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Space) {
                enPause = !enPause; // Bascule Play / Pause
                mettreAJourTitre();
            } else if (event.key.code == sf::Keyboard::Right) {
                simulation->step(); // Avance d'un pas
                mettreAJourTitre();
            } else if (event.key.code == sf::Keyboard::Up) {
                if (intervalleMs > 20.0f) intervalleMs -= 20.0f; // Accélère
            } else if (event.key.code == sf::Keyboard::Down) {
                intervalleMs += 20.0f; // Ralentit
            }
        } else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                int cx = event.mouseButton.x / cellSize;
                int cy = event.mouseButton.y / cellSize;
                if (simulation && simulation->getGrille()) {
                    simulation->getGrille()->basculerCellule(cx, cy);
                    mettreAJourTitre();
                }
            }
        }
    }
}

// Boucle principale interactive
void InterfaceGraphique::executer() {
    sf::Clock horloge;
    afficherGrille();

    while (window.isOpen() && simulation->getIterationActuelle() < simulation->getNombreIterations()) {
        gererInteractionsUtilisateur();

        if (!enPause) {
            if (horloge.getElapsedTime().asMilliseconds() >= intervalleMs) {
                simulation->step();
                mettreAJourTitre();
                horloge.restart();
            }
        }

        afficherGrille();
    }
}

// Démarrage initial
void InterfaceGraphique::demarrer() {
    afficherGrille();
}

// Arrêt de la fenêtre
void InterfaceGraphique::arreter() {
    window.close();
}

// Vérifie si la fenêtre est active
bool InterfaceGraphique::estOuverte() const {
    return window.isOpen();
}
