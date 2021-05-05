#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Null


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */

  /**
   * Play method, invoked once per each round.
   */
	map<int, Dir> check(map<int, Dir> tracker) {
		map<int, Dir> res;
		bool afegir = true;

		for(map<int, Dir>::iterator it = tracker.begin(); it != tracker.end(); ++it) {
			afegir = true;
			Ant a = ant(it -> first);
			for(map<int, Dir>::iterator it1 = tracker.begin(); it1 != tracker.end() and afegir; ++it1) {
				Ant a1 = ant(it1 -> first);
				if(it ->first != it1 -> first){
					Pos p1 = a.pos + it -> second;
					Pos p2 = a1.pos + it1 -> second;
					if(p1 == p2) afegir = false;
				}
			}
			if(afegir) res.insert(make_pair(it -> first, it -> second));
		}
		return res;
	}

  	virtual void play () {
		vector<int> w = workers(me());
		map<int, Dir> tracker;

		for(int i = 0; i < w.size(); ++i){
			Dir d = Dir(random(0,3));
			Ant a = ant(w[i]);
			if(pos_ok(a.pos + d) and cell(a.pos + d).type != Water and cell(a.pos +d).id == -1) tracker.insert(make_pair(a.id, d));
		}

		map<int, Dir> result = check(tracker);

		for(map<int, Dir>::iterator it = result.begin(); it != result.end(); ++it){
			Ant a = ant(it -> first);
			move(a.id, it -> second);
		}

  	}

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
