// Sokoban (partie 3)
// Donnons vie à notre gardien d'entrepot
// Au programme: 
//           - ajout du joueur
//           - gestion des mouvements
//           - gestion des caisses mobiles
//

#include <Gamebuino-Meta.h>
#include "Level.h"
#include "Player.h"
#include "Graphics.h"

char NIVEAU_COURRANT;

void setup() {
  gb.begin();
  NIVEAU_COURRANT = 1;
  init_level();
}

void init_level() {
  // initialisation du niveau
  Joueur.En_mouvement = STOP;
  Joueur.Direction = BAS;
  Joueur.Anime = 0;
  Joueur.Pousse  = 0;
    currentLevelData = levels[NIVEAU_COURRANT] + 2;
  NB_LIGNES_NIVEAUX = levels[NIVEAU_COURRANT][1];
  NB_COLONNES_NIVEAUX = levels[NIVEAU_COURRANT][0];
  trouve_position_perso();
}

void loop() {
  while (!gb.update());
  gb.display.clear();

  // On actualise l'état du joueur
  MAJ_Joueur();

  // On actualise la position de la zone du tableau à afficher
  Position_camera(Joueur.X, Joueur.Y);

  // On affiche le contenu de chacune des cases
  DessineNiveau();
  DessinePerso();
}
