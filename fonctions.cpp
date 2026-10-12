#include "fonctions.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include <QHash>


//constructeur qui initialise la matrice avec des valeurs par défaut
Fonctions::Fonctions() {
    double infini = std::numeric_limits<double>::infinity();

    // On parcourt toute la matrice 100x100 pour l'initialiser
    for(int i = 0; i < 100; ++i) {
        for(int j = 0; j < 100; ++j) {
            if(i == j) {
                matriceDeTemps[i][j] = 0;
            } else {
                matriceDeTemps[i][j] = infini;
            }
        }
    }
}
// Destructeur
Fonctions::~Fonctions() {}

// La fonction getVille retourne la ville situé à l'indice i dans le vecteur
Ville Fonctions::getVille(int i) const {
    return listeVilles[i];
}

// La getTemps retourne le temps de trajet stocké entre deux villes i et j
double Fonctions::getTemps(int i, int j) const {
    if (i < 100 && j < 100){
        return matriceDeTemps[i][j];
    }
    return std::numeric_limits<double>::infinity();
}


//La chargerVilles lit le fichier CSV des villes et crée les objets Ville qu'il push dans la liste des lives
void Fonctions::chargerVilles(QString nomFichier) {
    QFile fichier(nomFichier);
    if (fichier.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream flux(&fichier);
        flux.readLine(); // Saute la ligne d'en-tete du CSV
        while (!flux.atEnd()) {
            QString ligne = flux.readLine();
            QStringList colonnes = ligne.split(","); // Découpe la ligne à chaque virgule
            if (colonnes.size() >= 5) {
                QString nom = colonnes[0];
                int id      = colonnes[1].toInt();
                double lat  = colonnes[3].toDouble();
                double lon  = colonnes[4].toDouble();
                // Ajoute la nouvelle ville au vecteur listeVilles
                listeVilles.push_back(Ville(id, nom, lat, lon));
            }
        }
        fichier.close();
        qDebug() << "Succès : " << listeVilles.size() << " villes chargées.";
    }
}

//La fontion chargerTemps lit les liaisons entre les villes et remplit la matrice de temps
void Fonctions::chargerTemps(QString nomFichier) {
    // temps.csv référence les villes par leur city_id, alors que la matrice et
    // l'interface utilisent la position de la ville dans listeVilles
    QHash<int, int> indiceParId;
    for (int i = 0; i < listeVilles.size(); ++i) {
        indiceParId.insert(listeVilles[i].getId(), i);
    }

    QFile fichier(nomFichier);
    if (fichier.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream flux(&fichier);
        while (!flux.atEnd()) {
            QString ligne = flux.readLine();
            QStringList colonnes = ligne.split(",");
            if (colonnes.size() >= 3) {
                int idA = colonnes[0].toInt();
                int idB = colonnes[1].toInt();
                double t = colonnes[2].toDouble();
                if (indiceParId.contains(idA) && indiceParId.contains(idB)) {
                    int a = indiceParId.value(idA);
                    int b = indiceParId.value(idB);
                    matriceDeTemps[a][b] = t;
                    matriceDeTemps[b][a] = t;
                }
            }
        }
        fichier.close();
    }
}

//La fontion plusCourtsChemins calcule les plus courts chemins entre tous les couples de villes possibles
void Fonctions::plusCourtsChemins() {
    int n = 100;
    for (int k = 0; k < n; ++k) { // Ville intermédiaire
        for (int u = 0; u < n; ++u) { // Ville de départ
            for (int v = 0; v < n; ++v) { // Ville d'arrivée
                // Si passer par k est plus court que le trajet direct u->v on met à jour la matrice de temps
                matriceDeTemps[u][v] = std::min(matriceDeTemps[u][v], matriceDeTemps[u][k] + matriceDeTemps[k][v]);
            }
        }
    }
    qDebug() << "Calcul de tous les plus courts chemins terminé ";
}

//La fonction meilleurOrdre détermine un ordre de passage vers la ville la plus proche
QVector<int> Fonctions::meilleurOrdre(QVector<int> villesAChoisir) {
    QVector<int> ordreFinal;
    double infini = std::numeric_limits<double>::infinity();

    if (villesAChoisir.isEmpty()) return ordreFinal;
    // Fixe la première ville de la sélection comme point de départ
    int villeActuelle = villesAChoisir[0];
    ordreFinal.push_back(villeActuelle);
    villesAChoisir.removeFirst();

    // Boucle de recherche du Plus Proche Voisin
    while (!villesAChoisir.isEmpty()) {
        int plusProcheIndice = 0;
        double distanceMin = infini;

        // Cherche quelle ville dans la liste est la plus proche de la position actuelle
        for (int i = 0; i < villesAChoisir.size(); ++i) {
            double d = matriceDeTemps[villeActuelle][villesAChoisir[i]];
            if (d < distanceMin) {
                distanceMin = d;
                plusProcheIndice = i;
            }
        }
        // Se déplace vers ville la plus proche et met à jour les listes
        villeActuelle = villesAChoisir[plusProcheIndice];
        ordreFinal.push_back(villeActuelle);
        villesAChoisir.removeAt(plusProcheIndice);
    }

    return ordreFinal;
}

//la fonction meilleurDepot() Trouve la ville qui est la plus centrale par rapport aux villes choisis
int Fonctions::meilleurDepot(QVector<int> villesChoisies) {
    double infini = std::numeric_limits<double>::infinity();
    int meilleurId = -1;
    double scoreMinimal = infini;
    // On teste les villes qui ont été cochées par l'utilisateur
    for (int i : villesChoisies) {
        double sommeDistances = 0;
        bool accessible = true;
        for (int j : villesChoisies) {
            double d = matriceDeTemps[i][j];
            if (d >= infini) {
                accessible = false;
                break;
            }
            sommeDistances += d;
        }
        if (accessible && sommeDistances < scoreMinimal) {
            scoreMinimal = sommeDistances;
            meilleurId = i;
        }
    }
    return meilleurId;
}