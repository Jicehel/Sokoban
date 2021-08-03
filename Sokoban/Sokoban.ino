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
  gameover
};

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
