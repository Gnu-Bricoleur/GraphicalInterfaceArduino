#ifndef DEF_INTERFACE
#define DEF_INTERFACE

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include "TouchScreen.h"
#include "couleurs.h"


/**
 * Ordre des parametres :
 *  
 * 
 * 
 */


class Interface
{
public:
  Interface();
  void creerOnglet(char* titre);
  void creerBouton(int debX, int debY, int finX, int finY,int onglet,  char* nom, void *fonction, int param);
  void maj(MCUFRIEND_kbv ecran);
  void evenement(TSPoint p, MCUFRIEND_kbv ecran);
  void tranquille(int position, MCUFRIEND_kbv ecran);
  void convertTactile(TSPoint p);
  void rendreOngletTactile(void);
  void ongletActif(int onglet, MCUFRIEND_kbv ecran);
  int afficherTexte(char* texte, int taille, int onglet, int positionx, int positiony);
  void modifierTexte(int identifiant, char* texte);

private:
  bool changement = true;
  int nombreOnglets = 0;
  int nombreTextes = 0;
  int nombreBoutons = 0;
  char* Textes[50] = {"XXXXXXXXXX", "XXXXXXXXXX", "XXXXXXXXXX"};
  char* Onglets[10] = {"XXXXXXXXXX", "XXXXXXXXXX", "XXXXXXXXXX", "XXXXXXXXXX", "XXXXXXXXXX", "XXXXXXXXXX", "XXXXXXXXXX"};
  char* Couleurs[10] = {AQUA, FUCHSIA, YELLOW, BLUE, RED, GREEN};
  int positionsTextes[50][4] = {{1,2,3,4}, {1, 2,3,4}};//Onglet PositionX PositionY taillePolice
  int positionsBoutons[50][5] = {{1,2,3,4,5},{1,2,3,4,5}};//debutX,debutY,largeurX, longueruY, onglet
  int positionsOnglets[50][4] = {};//debutX, debutY, finX, finY
  int paramsBoutons[50] = {};
  char* Boutons[50] = {"XXXXXXXXXX", "XXXXXXXXXX"};
  void (*Fonction[50])(int param) = {};
  
  int Xmax = 885;
  int Xmin = 175;
  int Ymax = 951;
  int Ymin = 166;
  int16_t x = 1000;
  int16_t y = 1000;
  int numOngletActif = 0;
 
};
#endif
