#include "interface.h"
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>

#define MINPRESSURE 1
#define MAXPRESSURE 1000

Interface::Interface()
{
  //constructeur
}

void Interface::creerOnglet(char* titre)
{
    Serial.println("NouvelOnglet");
    nombreOnglets += 1;
    Onglets[nombreOnglets-1] = titre;
    changement = true;
}

//A appeler apres la création des onglets pour placer les zones tactiles au dessus des onglets
void Interface::rendreOngletTactile()
{
  for (int i = 0; i < nombreOnglets; i += 1)
  {
    //positionsBoutons[i] = {int((240/nombreOnglets)*i), 0, int((240/nombreOnglets)*(i+1)), 20};
    positionsOnglets[i][0] = int((240/nombreOnglets)*i);
    positionsOnglets[i][1] = 0;
    positionsOnglets[i][2] = int((240/nombreOnglets)*(i+1));
    positionsOnglets[i][3] = 20;
    //Fonction[i] = &Interface::ongletActif;
  }
}

void Interface::modifierTexte(int identifiant, char* texte)
{
  Textes[identifiant - 1] = texte;
  changement = true;
}


int Interface::afficherTexte(char* texte, int taille, int onglet, int positionx, int positiony)
{
  nombreTextes += 1;
  Textes[nombreTextes - 1] = texte;
  positionsTextes[nombreTextes - 1][0] = onglet;
  positionsTextes[nombreTextes - 1][1] = positionx;
  positionsTextes[nombreTextes - 1][2] = positiony;
  positionsTextes[nombreTextes - 1][3] = taille;
  changement = true;
  return(nombreTextes);
}

//Permet de modifier l'onglet actif
void Interface::ongletActif(int onglet, MCUFRIEND_kbv ecran)
{
  numOngletActif = onglet;
  changement = true;
}

void Interface::creerBouton(int debX, int debY, int finX, int finY, int onglet, char* nom, void *fonction, int param)
{
  nombreBoutons += 1;
  positionsBoutons[nombreBoutons-1][0] = debX;
  positionsBoutons[nombreBoutons-1][1] = debY;
  positionsBoutons[nombreBoutons-1][2] = finX;
  positionsBoutons[nombreBoutons-1][3] = finY;
  positionsBoutons[nombreBoutons-1][4] = onglet;
  Boutons[nombreBoutons-1] = nom;
  Fonction[nombreBoutons -1] = fonction;
  paramsBoutons[nombreBoutons -1] = param;
  changement = true;
}

//Conveerti les valeurs de resistances renvoyers par l'écran par des valeur x y cohérnte avec la resolution
void Interface::convertTactile(TSPoint p)
{
  // int16_t xtemp = p.x;
  // int16_t ytemp = p.y;//ordre bizarre car ecran utilise à l'envers
  // xtemp = xtemp - Xmax;
  // ytemp = ytemp - Ymax;
  // xtemp = (xtemp/float(Xmin - Xmax))*240;
  // ytemp = (ytemp/float(Ymin - Ymax))*320;
  // x = xtemp;
  // y = ytemp;
  //Serial.println(p.x);
  //Serial.println(p.y);
  x = map(p.x, Xmin, Xmax, tft.width(), 0);
  y = map(p.y, Ymin, Ymax, tft.height(), 0);
}

//appele en cas d'evenement tactile
void Interface::evenement(TSPoint p, MCUFRIEND_kbv ecran)
{
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
  {
    this->convertTactile(p);
    // Serial.println("Coord");
   //Serial.println(x);
   //Serial.println(y);
    //Check onglet
    for (int i = 0; i < nombreOnglets; i += 1)
    {
      if (positionsOnglets[i][0] < x && positionsOnglets[i][2] > x && positionsOnglets[i][1] < y && positionsOnglets[i][3] > y)
      {
        this->ongletActif(i,ecran);
        //(this->*Fonction[i])(i, ecran);
      }
    }
    for (int i = 0; i < nombreBoutons; i += 1)
    {
      if (positionsBoutons[i][0] < x && positionsBoutons[i][0] + positionsBoutons[i][2] > x && positionsBoutons[i][1] < y && positionsBoutons[i][1]+positionsBoutons[i][3] > y)
      {
        if (positionsBoutons[i][4] == numOngletActif)
        {
          Fonction[i](paramsBoutons[i]);
        }
      }
    }
  }
}

//fct vide pour boutons non associes ?
void Interface::tranquille(int position, MCUFRIEND_kbv ecran)
{
  //pass
}

//mise a jour de l'interface graphique
void Interface::maj(MCUFRIEND_kbv ecran)
{
  if (changement == true)
  {
    Serial.println("Interface maj");
    //Dessine onglets
    if (nombreOnglets != 0)
    {
      for (int i = 0; i < nombreOnglets; i += 1)
      {
        ecran.fillRect(int((240/nombreOnglets)*i), 0, int((240/nombreOnglets)*(i+1)), 20, Couleurs[i]);
        ecran.setCursor(int((240/nombreOnglets)*i), 0);
        ecran.setTextSize(2);
        ecran.print(Onglets[i]);
      }
    }
    //dessine fond de fenetre
    ecran.fillRect(0, 20, ecran.width(), ecran.height(), Couleurs[numOngletActif]);

    //dessine texte
    if (nombreTextes !=0)
    {
      for (int i = 0; i < nombreTextes; i += 1)
      {
        if (positionsTextes[i][0] == numOngletActif)
        {
          ecran.setCursor(positionsTextes[i][1], positionsTextes[i][2]);
          ecran.setTextSize(positionsTextes[i][3]);
          ecran.print(Textes[i]);
        }
      }
    }

    //dessine boutons
    if (nombreBoutons !=0)
    {
      for (int i = 0; i < nombreBoutons; i += 1)
      {
        if (positionsBoutons[i][4] == numOngletActif)
        {
          ecran.setCursor(positionsBoutons[i][0], positionsBoutons[i][1]);
          ecran.setTextSize(2);
          ecran.print(Boutons[i]);
          ecran.drawRoundRect(positionsBoutons[i][0],positionsBoutons[i][1], positionsBoutons[i][2],positionsBoutons[i][3], 1, BLACK);
        }
      }
    }



    changement = false;
  }
}
