#include "Player.hh"

/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Pere

struct PLAYER_NAME : public Player {

	/**
	 * Factory: returns a new instance of this class.
	 * Do not modify this function.
	 */
	static Player *factory() {
		return new PLAYER_NAME;
	}

	/**
	 * Types and attributes for your player can be defined here.
	 */

	void BFSTreballadores() {
		//BUSCAR MENJAR PORTAR A LA REINA I TORNAR A BUSCAR MENJAR MÉS LLUNY.
		//STORE ALL POSSITIONS THAT MOVE FOR NO REPETITION
		//STORE THE LET GO POSITION OF FOOD FOR NO REPETITION
		// Segon en desenvolupar
	}

	void BFSGuerrer() {
		//BUSCAR LES ENEMIGUES I MATAR (
		//primer en desenvolupar
	}

	void BFSReina() {
		//MENJAR I CREAR
		//fer que es quedi per la zona de bonus i els agafi
		//ultim en desenvolupar
	}

	Pos getNextPos() {}
	/**
	 * Play method, invoked once per each round.
	 */
	virtual void play(){
		
	}
};

/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);


/* TACTICA:
		Com deixo al costat de la formiga madre sense que torni a mirar el element del voltant.
		Put cpu time to check
*/