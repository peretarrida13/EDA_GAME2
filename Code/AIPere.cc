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
	/**
	 * Given the initial position and the final position
	 * We use the tracker to get the direction where it
	 * should move next
	 */ 
	Dir getDirection(Pos initial, Pos final, map<Pos, Pos> tracker){
		//
		Pos active = final;
		Pos res; 

		while(active != initial){
			map<Pos, Pos>::iterator it = tracker.find(active);

			active = it -> second;
			res = it ->first;
		}
		if(res.i == initial.i){
      		if(res.j < initial.j) return Left;
      		else return Right;
    	}else{
      		if(res.i < initial.i) return Up;
      		else return Down;
    	}
	}

	pair<int, Dir> BFSGuerrer(Pos p) {	
		queue<Pos> q;
		q.push(p);

		map<Pos, bool> visited;
		map<Pos, Pos> tracker;

		while(not q.empty()){
			Pos active = q.front();
			q.pop(); 

			for(int i = 0; i < 4; ++i) {
				Pos next = active + Dir(i);
				Cell c = cell(next);

				if(pos_ok(next) and c.type == Soil) {
					map<Pos, bool>::iterator it  = visited.find(next);

					if (it == visited.end()){
						q.push(next);
						visited.insert(make_pair(next, true));
						tracker.insert(make_pair(next, active));
					}
					if(c.id != -1){
						bool mine = false;

						vector<int> w = workers(me());
						for(int j = 0; j < w.size() and not mine; ++j){
							if(c.id == w[j]) mine = true;
						}

						vector<int> s = soldiers(me());
						for(int j = 0; j < s.size() and not mine; ++j){
							if(c.id == s[j]) mine = true;
						}

						vector<int> q = queens(me());
						for(int j = 0; j < q.size() and not mine; ++j){
							if(c.id == q[j]) mine = true;
						}

						if(not mine){
							Ant aux = ant(c.id);
							if(aux.type == Worker){
								Dir d = getDirection(p, next, tracker);
								return make_pair(1, d);
							}
						}
					}
				}
			}
		}
		return make_pair(-1, Dir(0));
	}

	pair<int,Dir> BFSTreballadorsBonus(Pos p) {
		queue<Pos> q;
		q.push(p);

		map<Pos, bool> visited;
		map<Pos, Pos> tracker;

		while(not q.empty()){
			Pos active = q.front();
			q.pop(); 

			for(int i = 0; i < 4; ++i) {
				Pos next = active + Dir(i);
				Cell c = cell(next);

				if(pos_ok(next) and c.type == Soil) {
					map<Pos, bool>::iterator it  = visited.find(next);

					if (it == visited.end()){
						q.push(next);
						visited.insert(make_pair(next, true));
						tracker.insert(make_pair(next, active));
					}
					if(c.bonus != None){
						if(c.id == -1){
							Dir d = getDirection(p, next, tracker);
							return make_pair(1, d);
						}
					}
				}
			}
		}
		return make_pair(-1, Dir(0));
	}

	pair<int,Dir> BFSReina(Pos p) {
		queue<Pos> q;
		q.push(p);

		map<Pos, bool> visited;
		map<Pos, Pos> tracker;

		while(not q.empty()){
			Pos active = q.front();
			q.pop(); 

			for(int i = 0; i < 4; ++i) {
				Pos next = active + Dir(i);
				Cell c = cell(next);

				if(pos_ok(next) and c.type == Soil) {
					map<Pos, bool>::iterator it  = visited.find(next);

					if (it == visited.end()){
						q.push(next);
						visited.insert(make_pair(next, true));
						tracker.insert(make_pair(next, active));
					}
					if(c.bonus != None){
						Dir d = getDirection(p, next, tracker);
						return make_pair(1, d);
					}
				}
			}
		}
		return make_pair(-1, Dir(0));

	}

	bool moveQueen(Ant a) {
		int i = 0;
		bool moved = false;
		
		while(i < 4 and not moved) {
			Dir d = Dir(i);
			Pos p = a.pos + d;
			Cell c = cell(p);

			if(c.bonus != None){
				move(a.id, d);
				moved = true;
			} 
			++i;
		}

		return moved;
	}

	bool next2queen(Pos p){
		vector<int> reina = queens(me());

		for(int i = 0; i < reina.size(); ++i){
			Ant a = ant(reina[i]);
			for(int j = 0; j < 4; ++j){
				Pos aux = a.pos + Dir(j);
				if(aux == p) return true;
			}
		}
		return false;
	}

	void move2queen(Pos p, int id){
		Pos final;
		queue<Pos> q;
		q.push(p);

		map<Pos, Pos> tracker;
		map<Pos, bool> visited;
		bool pathFound = false;

		while(not q.empty() and not pathFound){
			Pos active = q.front();
			q.pop(); 

			for(int i = 0; i < 4; ++i) {
				Pos next = active + Dir(i);
				Cell c = cell(next);

				if(pos_ok(next) and c.type == Soil) {
					map<Pos, bool>::iterator it  = visited.find(next);

					if (it == visited.end()){
						q.push(next);
						visited.insert(make_pair(next, true));
						tracker.insert(make_pair(next, active));
					}

					vector<int> r = queens(me());
					for(int j = 0 ; j < r.size(); ++j){
						Ant a = ant(r[j]);
						if(next == a.pos){
							final = next;
							pathFound = true;
						} 
					}
				}
			}
		}

		Dir d = getDirection(p, final, tracker);
		Cell c = cell(p + d);
		if(pos_ok(p + d) and c.id == -1) {
			move(id, d);
		}

	}

	bool createAnt(Ant a){
		vector<int> r = a.reserve;
		for(int i = 0; i < r.size(); ++i){
			if(r[i] == 0) return false;
		}
		return true;
	}

	bool antAround(Pos p){
		for(int i = 0; i < 4; ++i){
			Cell c = cell(p + Dir(i));

			if(c.id != -1){
				return true;
			}
		}
		return false;
	}

	int identQuadrant(Pos p){
		int c = board_cols();
		int r = board_rows();

		if(p.i <= r/2 and p.j <= c/2) return 1;
		else if(p.i <= r/2 and p.j > c/2) return 2;
		else if(p.i > r/2 and p.j <= c/2) return 3;
		else return 4;
	}

	bool outOfQuadrant(Pos p, int i){
		int q = identQuadrant(p);
		if(i != q) return true;
		else return false;
	}

	/**
	 * Play method, invoked once per each round.
	*/
	map<int, Dir> checkSoldiers(map<int, Dir> s, map<int, Dir>w) {
		map<int, Dir> res;
	
		for(map<int, Dir>::iterator it = s.begin(); it != s.end(); ++it) {
			bool add = true;
			Ant a1 = ant(it -> first);

			for(map<int, Dir>::iterator it1 = s.begin(); it1 != s.end() and add; ++it1) {
				Ant a2 = ant(it1 -> first);
				if(it ->first != it1 -> first){
					Pos p1 = a1.pos + it -> second;
					Pos p2 = a2.pos + it1 -> second;
					if(p1 == p2) add = false;
				}
			}

			for(map<int, Dir>::iterator it1 = w.begin(); it1 != w.end() and add; ++it1) {
				Ant a2 = ant(it1 -> first);
				Pos p1 = a1.pos + it -> second;
				Pos p2 = a2.pos + it1 -> second;
				if(p1 == p2) add = false;
			}

			if(add) res.insert(make_pair(it -> first, it -> second));
		}

		return res;	
	}

	map<int, Dir> checkWorkers(map<int, Dir> tracker) {
		map<int, Dir> res;
		bool afegir = true;

		for(map<int, Dir>::iterator it = tracker.begin(); it != tracker.end(); ++it) {
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
			afegir = true;
		}
		return res;
	}

	Dir pos2dir(pair<Pos, int> p){
		Pos final = p.first;
		Ant a = ant(p.second);
		Pos inici = a.pos;

		if(inici.i == final.i){
			if(inici.j < final.j){
				if(pos_ok(inici + Right)) return Right;
			}else{
				if(pos_ok(inici + Left)) return Left;
			}
		} else if(inici.i < final.i){
			if(pos_ok(inici + Up)) return Up;
		} else{
			if(pos_ok(inici + Down)) return Down;
		}
	}

	virtual void play(){
		vector<int> r = queens(me());	
		vector<int> wrks = workers(me());
		vector<int> sold = soldiers(me());

		vector<Pos> finals;
		map<Pos, int> moves;
		map<int, Dir> wTracker;
		map<int, Dir> sTracker;	
		for(int i = 0; i < r.size(); ++i){
			Ant a = ant(r[i]);
			if(not moveQueen(a)){
				if(createAnt(a)){
					if(wrks.size() <= sold.size()){
						for(int j = 0; j < 4; ++j){
							lay(a.id, Dir(i), Worker);
						}
					}  
					else{
						for(int j = 0; j < 4; ++j){
							lay(a.id, Dir(i), Soldier);
						}
					}  
				} else{
					bool around = antAround(a.pos);
					if(not around){
						pair<int, Dir> d = BFSReina(a.pos);
						if(d.first != -1){
							if(pos_ok(a.pos + d.second) and cell(a.pos + d.second).id == -1){
								int quadrant = identQuadrant(a.pos);
								bool out = outOfQuadrant(a.pos+d.second, quadrant);
								if(not out) move(a.id, d.second);
							}
						}
					}
				}
			}
		}
			
		for(int i = 0; i < wrks.size(); ++i){
			Ant wrk  = ant(wrks[i]);
			Cell c = cell(wrk.pos);

			if(next2queen(wrk.pos)){
				if(wrk.bonus != None) leave(wrk.id);
				else{
					if(c.bonus != None){
						vector<int> q = queens(me());
						for(int j = 0; j < q.size(); ++j){
							Ant a = ant(q[j]);
							
							if(wrk.pos.i == a.pos.i){
								if(wrk.pos.i < a.pos.i){
									if(pos_ok(wrk.pos + Left) and cell(wrk.pos + Left).id == -1) wTracker.insert(make_pair(wrk.id, Left));
									else{
										if(pos_ok(wrk.pos + Up) and cell(wrk.pos + Up).id == -1) wTracker.insert(make_pair(wrk.id, Up));
										else{
											if(cell(wrk.pos + Down).id == -1) wTracker.insert(make_pair(wrk.id, Down));
										} 
									}
								} else{
									if(pos_ok(wrk.pos + Right) and cell(wrk.pos + Right).id == -1) wTracker.insert(make_pair(wrk.id, Right));
									else{
										if(pos_ok(wrk.pos + Up) and cell(wrk.pos + Up).id == -1) wTracker.insert(make_pair(wrk.id, Up));
										else{
											if(cell(wrk.pos + Down).id == -1) wTracker.insert(make_pair(wrk.id, Down));
										} 
									}
								}
							} 
							if(wrk.pos.j == a.pos.j){
								if(wrk.pos.i < a.pos.i){
									if(pos_ok(wrk.pos + Up) and cell(wrk.pos + Up).id == -1) wTracker.insert(make_pair(wrk.id, Up));
									else{
										if(pos_ok(wrk.pos+ Left) and cell(wrk.pos + Left).id == -1) wTracker.insert(make_pair(wrk.id, Left));
										else{
											if(cell(wrk.pos + Right).id == -1) wTracker.insert(make_pair(wrk.id, Right));
										}
									}
								} else{
									if(pos_ok(wrk.pos + Down)and cell(wrk.pos + Down).id == -1) wTracker.insert(make_pair(wrk.id, Down));
									else{
										if(pos_ok(wrk.pos + Left) and cell(wrk.pos + Left).id == -1) wTracker.insert(make_pair(wrk.id, Left));
										else{
											if(cell(wrk.pos + Right).id == -1) wTracker.insert(make_pair(wrk.id, Right));
										} 
									}
								}
							}
						}
					}else{
						pair<int,Dir> d = BFSTreballadorsBonus(wrk.pos);
						if(d.first != -1){
							if(pos_ok(wrk.pos + d.second)) {
								Cell c = cell(wrk.pos + d.second);
								if(cell(wrk.pos + d.second).id == -1) wTracker.insert(make_pair(wrk.id, d.second));
							}
						}
					}
				}
			} else{
				if(wrk.bonus != None){
					move2queen(wrk.pos, wrk.id);
				} else{
					if(c.bonus != None) take(wrk.id);
					else{
						pair<int,Dir> d = BFSTreballadorsBonus(wrk.pos);
				
						if(d.first != -1){
							if(pos_ok(wrk.pos + d.second)){
								Cell c = cell(wrk.pos + d.second);
								if(not next2queen(wrk.pos + d.second)){
									if(cell(wrk.pos + d.second).id == -1) wTracker.insert(make_pair(wrk.id, d.second));
								}
							}
						}
					}
				}
			}
		}

		for(int i = 0; i < sold.size(); ++i){
			Ant s  = ant(sold[i]);
			pair<int,Dir> d = BFSGuerrer(s.pos);
				
			if(d.first != -1){
				if(pos_ok(s.pos + d.second)) {
					sTracker.insert(make_pair(s.id, d.second));
				}
			}
		}
		map<int, Dir> newWorkers = checkWorkers(wTracker);
		map<int, Dir> newSoldiers = checkSoldiers(sTracker, newWorkers);
		

		for(map<int, Dir>::iterator it = newWorkers.begin(); it != newWorkers.end(); ++it){
			cerr << 'w' << endl;
			Ant a = ant(it -> first);
			move(a.id, it -> second);
		}

		for(map<int, Dir>::iterator it = newSoldiers.begin(); it != newSoldiers.end(); ++it){
			cerr << 's' << endl;
			Ant a = ant(it -> first);
			Cell c = cell(a.pos + it ->second);
			bool mine = false;

			vector<int> w = workers(me());
			for(int j = 0; j < w.size() and not mine; ++j){
				if(c.id == w[j]) mine = true;
			}
			vector<int> so = soldiers(me());
			for(int j = 0; j < so.size() and not mine; ++j){
				if(c.id == so[j]) mine = true;
			}
			vector<int> q = queens(me());
			for(int j = 0; j < q.size() and not mine; ++j){
				if(c.id == q[j]) mine = true;
			}

			if(not mine){
				move(it -> first, it -> second);
			}
			
		}
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