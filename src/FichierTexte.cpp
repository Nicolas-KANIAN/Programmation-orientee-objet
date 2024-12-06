#include "FichierTexte.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <random>
#include <vector>

// Lire le contenu d'un fichier texte et le retourner sous forme de chaîne
std::string FichierTexte::lireFichierTexte(const std::string& chemin) {
    std::ifstream fichier(chemin);
    if (!fichier.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier : " + chemin);
    }

    std::ostringstream contenu;
    contenu << fichier.rdbuf();
    return contenu.str();
}

// Écrire une chaîne dans un fichier texte
void FichierTexte::ecrireFichierTexte(const std::string& chemin, const std::string& contenu) {
    std::ofstream fichier(chemin);
    if (!fichier.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier pour écriture : " + chemin);
    }

    fichier << contenu;
}

// Charger une grille depuis un fichier texte
void FichierTexte::charger(const std::string& chemin, Grille& grille) {
    std::string contenu = lireFichierTexte(chemin);
    std::istringstream flux(contenu);

    int largeur = grille.getLargeur();
    int hauteur = grille.getHauteur();

    int y = 0;
    std::string ligne;
    while (std::getline(flux, ligne) && y < hauteur) {
        // Nettoie les retours chariots Windows éventuels (\r)
        if (!ligne.empty() && ligne.back() == '\r') {
            ligne.pop_back();
        }

        if (ligne.empty()) continue;

        // Supporte les grilles avec chiffres contigus ('0101') ou séparés par des espaces ('0 1 0 1')
        std::vector<int> valeurs;
        if (ligne.find(' ') != std::string::npos) {
            std::istringstream iss(ligne);
            int v;
            while (iss >> v) {
                valeurs.push_back(v);
            }
        } else {
            for (char c : ligne) {
                if (c == '0' || c == '1') {
                    valeurs.push_back(c - '0');
                }
            }
        }

        for (int x = 0; x < largeur && x < static_cast<int>(valeurs.size()); ++x) {
            if (valeurs[x] == 1) {
                grille.setCellule(x, y, new CelluleVivante());
            } else {
                grille.setCellule(x, y, new CelluleMorte());
            }
        }
        ++y;
    }
}

// Sauvegarder une grille dans un fichier texte
void FichierTexte::sauvegarder(const std::string& chemin, const Grille& grille) {
    int largeur = grille.getLargeur();
    int hauteur = grille.getHauteur();

    std::ostringstream contenu;

    for (int y = 0; y < hauteur; ++y) {
        for (int x = 0; x < largeur; ++x) {
            Cellule* c = grille.getCellule(x, y);
            contenu << (c && c->getEtatActuel() ? '1' : '0');
        }
        contenu << '\n';
    }

    ecrireFichierTexte(chemin, contenu.str());
}

// Créer une grille aléatoire et la sauvegarder dans un fichier texte
void FichierTexte::genererGrilleAleatoire(const std::string& chemin, int largeur, int hauteur) {
    std::ofstream fichier(chemin);
    if (!fichier.is_open()) {
        throw std::runtime_error("Impossible de créer le fichier : " + chemin);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 1);

    for (int y = 0; y < hauteur; ++y) {
        for (int x = 0; x < largeur; ++x) {
            fichier << distrib(gen);
        }
        fichier << '\n';
    }

    std::cout << "Grille aléatoire générée et sauvegardée dans : " << chemin << std::endl;
}
