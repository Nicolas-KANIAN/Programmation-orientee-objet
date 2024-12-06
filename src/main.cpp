#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <thread>
#include <chrono>

#include "Simulation.h"
#include "FichierTexte.h"
#include "InterfaceGraphique.h"

int main() {
    // Paramètres par défaut de la simulation
    const int largeur = 50;
    const int hauteur = 50;
    const int nombreIterationsMax = 10000;
    const bool torique = false;

    // Création de l'objet Simulation
    Simulation simulation(largeur, hauteur, torique, nombreIterationsMax);
    FichierTexte fichierTexte;

    const std::string cheminInitial = "data/GrilleInitiale.txt";
    const std::string cheminFinal = "data/GrilleFinale.txt";

    // Vérification de la présence du fichier initial
    std::ifstream testFichier(cheminInitial);
    if (!testFichier.is_open()) {
        std::cout << "[INFO] " << cheminInitial << " introuvable. Génération d'une grille aléatoire...\n";
        try {
            fichierTexte.genererGrilleAleatoire(cheminInitial, largeur, hauteur);
        } catch (const std::exception& e) {
            std::cerr << "[ERREUR] Impossible de générer la grille : " << e.what() << '\n';
            return 1;
        }
    } else {
        testFichier.close();
    }

    // Chargement de l'état initial via la classe Simulation
    if (!simulation.chargerEtatInitial(cheminInitial)) {
        std::cerr << "[ERREUR] Échec du chargement de " << cheminInitial << '\n';
        return 1;
    }
    std::cout << "[OK] Grille initiale chargée depuis " << cheminInitial << " (" << largeur << "x" << hauteur << ")\n";

    // Choix du mode d'affichage
    std::cout << "\n============================================\n";
    std::cout << "      JEU DE LA VIE DE CONWAY (POO C++)     \n";
    std::cout << "============================================\n";
    std::cout << "Choisissez le mode de simulation :\n";
    std::cout << "  [G] Interface Graphique (SFML interactive)\n";
    std::cout << "  [C] Mode Console\n";
    std::cout << "Votre choix (G/C) : ";

    char choix = 'G';
    std::cin >> choix;

    // Nettoyage complet du flux d'entrée pour éviter de sauter le premier tour en console
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choix == 'G' || choix == 'g') {
        std::cout << "\n[LANCEMENT] Démarrage de l'interface graphique...\n";
        std::cout << "Contrôles :\n";
        std::cout << "  - [ESPACE]       : Lancer / Mettre en pause la simulation\n";
        std::cout << "  - [FLECHE DROITE]: Avancer pas-à-pas d'une génération\n";
        std::cout << "  - [HAUT / BAS]   : Modifier la vitesse de simulation\n";
        std::cout << "  - [CLIC GAUCHE]  : Activer ou désactiver une cellule\n\n";

        // Délégation complète à la classe InterfaceGraphique
        const int tailleCellulePx = 16;
        InterfaceGraphique gui(largeur, hauteur, tailleCellulePx, &simulation);
        gui.executer();

    } else {
        std::cout << "\n[CONSOLE] Mode console sélectionné.\n";
        std::cout << "Commandes :\n";
        std::cout << "  - Appuyez sur [Entrée] pour l'étape suivante\n";
        std::cout << "  - Entrez 'a' puis [Entrée] pour le défilement automatique\n";
        std::cout << "  - Entrez 'q' puis [Entrée] pour quitter et sauvegarder\n\n";

        bool autoPlay = false;

        while (simulation.getIterationActuelle() < simulation.getNombreIterations()) {
            std::cout << "\n--- Génération " << simulation.getIterationActuelle()
                      << " (Vivantes : " << simulation.getGrille()->compterCellulesVivantes() << ") ---\n";
            simulation.getGrille()->afficherConsole();

            if (!autoPlay) {
                std::cout << "\n[Entrée: Suivant | a: Auto | q: Quitter] > ";
                std::string commande;
                std::getline(std::cin, commande);

                if (commande == "q" || commande == "Q") {
                    std::cout << "Arrêt demandé par l'utilisateur.\n";
                    break;
                } else if (commande == "a" || commande == "A") {
                    autoPlay = true;
                }
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(150));
            }

            simulation.step();
        }
    }

    // Sauvegarde de l'état final via la classe Simulation
    simulation.sauvegarderEtatCourant(cheminFinal);
    std::cout << "\n[SAUVEGARDE] État final sauvegardé dans " << cheminFinal << " (Itération finale : "
              << simulation.getIterationActuelle() << ")\n";

    return 0;
}
