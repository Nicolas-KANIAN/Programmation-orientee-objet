#ifndef FICHIER_TEXTE_H
#define FICHIER_TEXTE_H

#include <string>
#include "Grille.h"

// Classe utilitaire pour la persistance des grilles dans des fichiers texte
class FichierTexte {
public:
    FichierTexte() = default;

    // Charge une grille depuis un fichier texte
    void charger(const std::string& chemin, Grille& grille);

    // Sauvegarde une grille dans un fichier texte
    void sauvegarder(const std::string& chemin, const Grille& grille);

    // Génère un fichier avec une grille aléatoire
    void genererGrilleAleatoire(const std::string& chemin, int largeur, int hauteur);

private:
    std::string lireFichierTexte(const std::string& chemin);
    void ecrireFichierTexte(const std::string& chemin, const std::string& contenu);
};

#endif // FICHIER_TEXTE_H
