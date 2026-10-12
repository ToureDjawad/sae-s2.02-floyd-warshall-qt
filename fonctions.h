#ifndef FONCTIONS_H
#define FONCTIONS_H
#include <QVector>
#include <QString>
#include "ville.h"

class Fonctions {
private:
    // Stocke les objets Ville chargés depuis le fichier ville.csv
    QVector<Ville> listeVilles;

    // Matrice pour stocker les temps de trajet entre chaque paire de villes
    double matriceDeTemps[100][100];

public:
    // Constructeur
    Fonctions();
    // Destructeur
    ~Fonctions();
    // Retourne le nombre total de villes
    inline int nbVilles() const { return listeVilles.size(); }
    // Retourne la ville cocorrespondant à l'indice i donné
    Ville getVille(const int i) const;
    // Retourne le temps de trajet stocké dans la matrice entre la ville i et la ville j
    double getTemps(const int i, const int j) const;
    // Lit le fichier CSV des villes et remplit le vecteur listeVilles
    void chargerVilles(QString nomFichier);
    // Lit le fichier de temps
    void chargerTemps(QString nomFichier);
    //l'algorithme de Floyd-Warshall pour calculer les plus courts chemins entre toutes les villes
    void plusCourtsChemins();
    // Calcule l'ordre de visite optimal à partir d'une liste d'indices de villes sélectionnées
    QVector<int> meilleurOrdre(const QVector<int> indicesVilles);
    // Identifie l'id de la ville centrale  minimisant la somme des trajets vers les villes choisies
    int meilleurDepot(QVector<int> villesChoisies);
};

#endif // FONCTIONS_H