#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>
#include <QListWidget>
#include <QLabel>
#include "fonctions.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void calculerTrajet();    // Calcul direct entre une ville de départ et une ville d'arrivée
    void afficherTableau();    // Remplit le tableau TableWidget avec les distances de la matrice
    void optimiserTrajet();   // Lance l'optimisation des trajets
    void rechercherDepot();   // Lance la recherche du meilleur dépôt

private:
    // Instance de la classe Fonctions
    Fonctions fonctions;
    // Menu pour choisir les deux villes du trajet
    QComboBox *comboDepart;
    QComboBox *comboArrivee;
    // Bouton de déclenchement et zone de texte pour afficher le résultat du trajet
    QPushButton *boutonCalculer;
    QLabel *labelResultatTrajet;
    // Liste interactive ou l'utilisateur coche les villes à visiter
    QListWidget *listeSelectionVilles;
    // Boutons pour lancer les différentes fonctionnalités du projet
    QPushButton *boutonafficherTableau;
    QPushButton *boutonOptimiser;
    QPushButton *boutonDepot;
    // Affiche le parcours final calculé
    QLabel *labelResultatOptimisation;
    // Affiche le nom de la ville choisie comme dépôt idéal
    QLabel *labelResultatDepot;
    // Tableau affichant la distance entre les villes
    QTableWidget *tableauDistances;
};

#endif // MAINWINDOW_H