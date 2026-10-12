#include "mainwindow.h"
#include <QVBoxLayout>// Layout vertical
#include <QHBoxLayout>// Layout horizontal
#include <QHeaderView>// Permet de configurer les en-têtes du QTableWidget
#include <QMessageBox>// Pour afficher des fenêtres pop-up d'avertissement



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //Chargement des fichiers
    fonctions.chargerVilles("data/villes.csv");
    fonctions.chargerTemps("data/temps.csv");

    //Widget central qui contiendra tout
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    setWindowTitle("SAE 2.02 Gestion des temps de route");  // Titre affiché dans la barre

    // Layout principal
    QVBoxLayout *layoutPrincipal = new QVBoxLayout(centralWidget);

    //Calcul de trajet entre deux villes
    layoutPrincipal->addWidget(new QLabel(" Calcul de trajet entre deux villes "));  // Titre
    // Sous-layout horizontal pour mettre les éléments sur une seule ligne
    QHBoxLayout *layoutSimple = new QHBoxLayout();
    comboDepart = new QComboBox();// Menu déroulant départ
    comboArrivee = new QComboBox();// Menu déroulant arrivée
    boutonCalculer = new QPushButton("Calculer le temps");// Bouton de calcul

    // Ajout des éléments dans le layout horizontal
    layoutSimple->addWidget(new QLabel("Ville de départ :"));
    layoutSimple->addWidget(comboDepart);
    layoutSimple->addWidget(new QLabel(" Ville d'arriver :"));
    layoutSimple->addWidget(comboArrivee);
    layoutSimple->addWidget(boutonCalculer);

    // Ajout du layout horizontal dans le layout vertical principal
    layoutPrincipal->addLayout(layoutSimple);

    // Label qui affichera le résultat du trajet
    labelResultatTrajet = new QLabel("En attente de calcul...");
    layoutPrincipal->addWidget(labelResultatTrajet);

    layoutPrincipal->addSpacing(15);  // Espace vertical de 15 pixels pour aérer

    //Liste des villes à cocher
    layoutPrincipal->addWidget(new QLabel("Sélectionner les villes pour les analyses suivantes : "));
    listeSelectionVilles = new QListWidget();
    listeSelectionVilles->setMaximumHeight(120);
    layoutPrincipal->addWidget(listeSelectionVilles);

    //Bouton et label pour l'optimisation
    boutonOptimiser = new QPushButton("Optimiser l'ordre de visite");
    layoutPrincipal->addWidget(boutonOptimiser);
    labelResultatOptimisation = new QLabel("Le parcours s'affichera ici.");
    layoutPrincipal->addWidget(labelResultatOptimisation);

    layoutPrincipal->addSpacing(10);

    //Bouton et label pour le dépot
    boutonDepot = new QPushButton("Trouver le meilleur emplacement de dépôt");
    layoutPrincipal->addWidget(boutonDepot);
    labelResultatDepot = new QLabel("La ville idéale pour le dépôt s'affichera ici.");
    layoutPrincipal->addWidget(labelResultatDepot);

    layoutPrincipal->addSpacing(15);

    //Bouton et tableau des distances
    boutonafficherTableau = new QPushButton("Afficher le tableau complet des distances");
    layoutPrincipal->addWidget(boutonafficherTableau);

    tableauDistances = new QTableWidget();
    tableauDistances->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layoutPrincipal->addWidget(tableauDistances);

    //Parcourt des 99 villes pour remplir les 2 menus déroulants et la liste à cocher
    for(int i = 0; i < fonctions.nbVilles(); ++i) {
        QString nom = fonctions.getVille(i).getNom(); // Nom de la ville d'indice i

        comboDepart->addItem(nom);// Ajoute au menu départ
        comboArrivee->addItem(nom);// Ajoute au menu arrivée
        QListWidgetItem *item = new QListWidgetItem(nom, listeSelectionVilles);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }

    connect(boutonCalculer,&QPushButton::clicked, this, &MainWindow::calculerTrajet);
    connect(boutonafficherTableau,&QPushButton::clicked, this, &MainWindow::afficherTableau);
    connect(boutonOptimiser,&QPushButton::clicked, this, &MainWindow::optimiserTrajet);
    connect(boutonDepot,&QPushButton::clicked, this, &MainWindow::rechercherDepot);
    fonctions.plusCourtsChemins();
}

void MainWindow::calculerTrajet() {
    int id1 = comboDepart->currentIndex();
    int id2 = comboArrivee->currentIndex();
    double temps = fonctions.getTemps(id1, id2);
    double infini = std::numeric_limits<double>::infinity();

    if(temps >= infini) {
        labelResultatTrajet->setText("Aucun chemin trouvé.");
    } else {
        // Conversion des minutes en heures + minutes
        int h = static_cast<int>(temps) / 60;
        int m = static_cast<int>(temps) % 60;
        labelResultatTrajet->setText(QString("Temps direct : %1h%2 min ").arg(h).arg(m));
    }
}

void MainWindow::optimiserTrajet() {
    QVector<int> selection;  // Vecteur qui contiendra les ids des villes cochées

    // On parcourt tous les items de la liste pour récupérer ceux qui sont cochés
    for(int i = 0; i < listeSelectionVilles->count(); ++i) {
        if(listeSelectionVilles->item(i)->checkState() == Qt::Checked) {
            selection.push_back(i);
        }
    }
    if (selection.size() < 2) {
        QMessageBox::warning(this, "Attention!", "Sélectionnez au moins 2 villes.");
        return;
    }

    QVector<int> resultat = fonctions.meilleurOrdre(selection);
    // Construction du mesage qui affiche le parcours sous forme Ville1 -> Ville2 -> Ville3
    QString msg = "Parcours :";
    for (int i = 0; i < resultat.size(); ++i) {
        msg += fonctions.getVille(resultat[i]).getNom();
        if (i < resultat.size() - 1){
            msg += "->";
        }
    }
    // Affichage du parcours dans le label
    labelResultatOptimisation->setText(msg);
}


void MainWindow::rechercherDepot() {
    QVector<int> selection;

    // Récupération des villes cochées
    for(int i = 0; i < listeSelectionVilles->count(); ++i) {
        if(listeSelectionVilles->item(i)->checkState() == Qt::Checked) {
            selection.push_back(i);
        }
    }
    if(selection.isEmpty()) {
        QMessageBox::warning(this, "Attention!", "Cochez des villes à desservir.");
        return;
    }

    int idDepot = fonctions.meilleurDepot(selection);
    if (idDepot != -1) {
        QString nom = fonctions.getVille(idDepot).getNom();
        labelResultatDepot->setText("Dépôt recommandé : " + nom);
    } else {
        labelResultatDepot->setText("Impossible de trouver un dépôt.");
    }
}

void MainWindow::afficherTableau() {
    QVector<int> selection;
    double infini = std::numeric_limits<double>::infinity();
    // Récupération des villes cochées
    for(int i = 0; i < listeSelectionVilles->count(); ++i) {
        if(listeSelectionVilles->item(i)->checkState() == Qt::Checked) {
            selection.push_back(i);
        }
    }
    //si au qu'une n'a été ville cochée
    if(selection.isEmpty()) {
        QMessageBox::warning(this, "Tableau", "Cochez des villes pour remplir le tableau.");
        return;
    }

    int n = selection.size();// Nombre de villes sélectionnées

    //INitialisation de la taille du tableau
    tableauDistances->setRowCount(n);
    tableauDistances->setColumnCount(n);

    // Double boucle pour remplir chaque case
    for(int i = 0; i < n; i++) {
        int idI = selection[i];// Id de la ville en ligne i

        // Définit les en-tetes des lignes et colonnes
        tableauDistances->setHorizontalHeaderItem(i,new QTableWidgetItem(fonctions.getVille(idI).getNom()));
        tableauDistances->setVerticalHeaderItem(i,new QTableWidgetItem(fonctions.getVille(idI).getNom()));

        for(int j = 0; j < n; j++) {
            if(i == j) {
                tableauDistances->setItem(i, j, new QTableWidgetItem("-"));
            } else {
                int idJ = selection[j]; // Id de la ville en colonne j
                double t = fonctions.getTemps(idI, idJ);  // Temps entre idI et idJ

                if(t >= infini) {
                    tableauDistances->setItem(i, j, new QTableWidgetItem("-"));
                } else {
                    int h = static_cast<int>(t) / 60;
                    int m = static_cast<int>(t) % 60;
                    QString texte = QString("%1h%2 min").arg(h).arg(m);
                    // Création de la cellule et centrage du texte
                    QTableWidgetItem *item = new QTableWidgetItem(texte);
                    item->setTextAlignment(Qt::AlignCenter);
                    tableauDistances->setItem(i, j, item);
                }
            }
        }
    }
}

MainWindow::~MainWindow() {}