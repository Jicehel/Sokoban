// Sokoban (partie 4)
// Donnons vie à notre gardien d'entrepot
// Au programme: 
//           - ajout du joueur
//           - gestion des mouvements
//           - gestion des caisses mobiles
//           - gestion des niveaux
//
// Source des niveaux: http://www.sourcecode.se/sokoban/levels
//


#include <Gamebuino-Meta.h>
#include "Level.h"
#include "Player.h"
#include "Graphics.h"

enum class State : uint8_t {
  initialize,
  running,
  endlevel,
  map,
  reinit,
  gameover
};

float decalageH;
float decalageV;

struct Game {
  State state;
  uint8_t NIVEAU_COURRANT;
};

Game game;

void setup() {
  gb.begin();
  game.state = State::initialize;
}

void init_level() {
  // initialisation du niveau
  Joueur.En_mouvement = STOP;
  Joueur.Direction = BAS;
  Joueur.Anime = 0;
  Joueur.Pousse  = 0;
  currentLevelData = levels[game.NIVEAU_COURRANT] + 2;
  NB_LIGNES_NIVEAUX = levels[game.NIVEAU_COURRANT][1];
  NB_COLONNES_NIVEAUX = levels[game.NIVEAU_COURRANT][0];
  for (int ligne = 0; ligne < NB_LIGNES_NIVEAUX; ligne++) {
    for (int colonne = 0; colonne < NB_COLONNES_NIVEAUX; colonne++) {
      saveLevel[ligne * NB_COLONNES_NIVEAUX + colonne] = currentLevelData[ligne * NB_COLONNES_NIVEAUX + colonne];
    }
  }
  count_Boxes();
  trouve_position_perso();
  game.state = State::running;
  
}

void loop() {
  gb.waitForUpdate();
  gb.display.clear();

  switch (game.state) {

    case State::initialize:
      game.NIVEAU_COURRANT = 0;
      init_level();
      break;

    case State::reinit:

      gb.display.setColor(WHITE);
      gb.display.setCursor(0, 5);
      gb.display.print("Restart level ?");
      gb.display.setCursor(0, 35);
      gb.display.print("<A> Yes, restart");
      gb.display.setCursor(0, 50);
      gb.display.print("<B> Return to game");

      if (gb.buttons.pressed(BUTTON_A)) {
          for (int ligne = 0; ligne < NB_LIGNES_NIVEAUX; ligne++) {
            for (int colonne = 0; colonne < NB_COLONNES_NIVEAUX; colonne++) {
              currentLevelData[ligne * NB_COLONNES_NIVEAUX + colonne] = saveLevel[ligne * NB_COLONNES_NIVEAUX + colonne];
            }
          }
          init_level();
      }
      if (gb.buttons.pressed(BUTTON_B)) game.state = State::running;
      break;

    case State::map:
      decalageH = (20 - NB_COLONNES_NIVEAUX)*2;
      decalageV = (16 - NB_LIGNES_NIVEAUX)*2;
      
      for (int ligne = 0; ligne < NB_LIGNES_NIVEAUX; ligne++) {
        for (int colonne = 0; colonne < NB_COLONNES_NIVEAUX; colonne++) {
            switch (currentLevelData[(ligne)*NB_COLONNES_NIVEAUX + colonne ]) {
              case ' ':
                 gb.display.setColor(BLACK);
              break;
              case '#':
                  gb.display.setColor(RED);
              break;
              case '$':
                  gb.display.setColor(YELLOW);
              break;
              case '*':
                  gb.display.setColor(GREEN);
              break;
              case '@':
                  gb.display.setColor(WHITE);
              break;              
              case '.':
                  gb.display.setColor(BLUE);
            } 
            gb.display.fillRect(decalageH + colonne * 4, decalageV + ligne * 4, 4, 4);
        }
      }
      gb.display.setColor(WHITE);
      gb.display.printf(8, 0, "Map of level %03u", game.NIVEAU_COURRANT + 1);
      if (gb.buttons.pressed(BUTTON_B)) game.state = State::running;
      break;

    case State::running:
      // On actualise l'état du joueur
      MAJ_Joueur();

      // On actualise la position de la zone du tableau à afficher
      Position_camera(Joueur.X, Joueur.Y);

      // On affiche le contenu de chacune des cases
      DessineNiveau();
      DessinePerso();

      // On compte les caisses bien positionnées
      count_Boxes_On_Good_Place();

      if (PlacedBoxes == Boxes) game.state = State::endlevel;

      if (gb.buttons.pressed(BUTTON_A)) game.state = State::map;
      if (gb.buttons.pressed(BUTTON_B)) game.state = State::reinit;
      break;

      case State::endlevel:
      if (game.NIVEAU_COURRANT < NB_NIVEAUX) {
        ++game.NIVEAU_COURRANT;
        PlacedBoxes = 0;
        init_level();
        game.state = State::running;
      } else {
        game.state = State::gameover;
      }
      break;      
  }

}
