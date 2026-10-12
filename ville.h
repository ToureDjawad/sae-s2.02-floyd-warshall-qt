#ifndef VILLE_H
#define VILLE_H

#include <QString>

class Ville {
private:
    int id;              // L'identifiant numérique (0 à 99)
    QString nom;         // Le nom de la ville
    double latitude;     // Pour affichage sur carte
    double longitude;    // Pour  affichage sur carte

public:
    // Constructeur avec paramètres
    Ville(const int &idVille,const QString &nomVille,const double &lat,const double &lon);
    // Getters  pour lire les données
    inline int getId() const { return id; }
    inline QString getNom() const { return nom; }
    inline double getLat() const { return latitude; }
    inline double getLon() const { return longitude; }
};

#endif // VILLE_H
