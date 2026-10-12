#include "ville.h"
// Constructeur utilisé lors de la lecture du fichier CSV
Ville::Ville(const int &idVille,const  QString &nomVille,const double &lat,const double &lon) : id(idVille) , nom(nomVille) , latitude(lat), longitude(lon){}
