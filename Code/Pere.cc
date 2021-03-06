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

	void moveQueen(Ant a, map<Pos, int> moves, bool moved) {
		int i = 0;
		moved = false;
		
		while(i < 4 and not moved) {
			Dir d = Dir(i);
			Pos p = a.pos + d;
			Cell c = cell(p);

			if(c.bonus != None){
				moves.insert(make_pair(p, a.id));
				moved = true;
			} 
			++i;
		}
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

	pair<int, Dir> move2queen(Pos p, int id){
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

				if(pos_ok(next) and c.type == Soil and c.bonus == None) {
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
			return make_pair(1, d);
		} else{
			return make_pair(-1, d);
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
				Ant a = ant(c.id);
				if(a.bonus != None) return true;
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

	pair<bool, Dir> pos2dir(pair<Pos, int> p){
		Pos final = p.first;
		Ant a = ant(p.second);
		Pos inici = a.pos;

		if(inici.i == final.i){
			if(inici.j < final.j){
				if(pos_ok(inici + Right)) return make_pair(true, Right);
			} else{
				if(pos_ok(inici + Left)) return make_pair(true, Left);
			}
		} else if(inici.i < final.i){
			if(pos_ok(inici + Up)) return make_pair(true, Down);
		} else{
			if(pos_ok(inici + Down)) return make_pair(true, Up);
		}

		return make_pair(false, Dir(random(0, 3)));
	}

	virtual void play(){
		vector<int> r = queens(me());	
		vector<int> wrks = workers(me());
		vector<int> sold = soldiers(me());

		vector<Pos> finals;
		map<Pos, int> moves;

		for(int i = 0; i < sold.size(); ++i){
			Ant s  = ant(sold[i]);
			pair<int,Dir> d = BFSGuerrer(s.pos);
				
			if(d.first != -1){
				if(pos_ok(s.pos + d.second)) {
					bool mine = false;
					for(int j = 0; j < r.size() and not mine; ++j){
						if(r[j] == cell(s.pos + d.second).id) mine = true;
					}

					for(int j = 0; j < wrks.size() and not mine; ++j){
						if(wrks[j] == cell(s.pos + d.second).id) mine = true;
					}

					for(int j = 0; j < sold.size() and not mine; ++j){
						if(sold[j] == cell(s.pos + d.second).id) mine = true;
					}

					if(not mine) moves.insert(make_pair((s.pos + d.second), s.id));
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
						pair<int,Dir> d = BFSTreballadorsBonus(wrk.pos);
						if(d.first != -1){
							if(pos_ok(wrk.pos + d.second)) {
								Cell c = cell(wrk.pos + d.second);
								if(cell(wrk.pos + d.second).id == -1) moves.insert(make_pair((wrk.pos + d.second), wrk.id));
							}
						}						
					}else{
						pair<int,Dir> d = BFSTreballadorsBonus(wrk.pos);
						if(d.first != -1){
							if(pos_ok(wrk.pos + d.second)) {
								Cell c = cell(wrk.pos + d.second);
								if(cell(wrk.pos + d.second).id == -1) moves.insert(make_pair((wrk.pos + d.second), wrk.id));
							}
						}
					}
				}
			} else{
				if(wrk.bonus != None){
					pair<int, Dir> par = move2queen(wrk.pos, wrk.id);
					if(par.first == 1){
						moves.insert(make_pair((wrk.pos + par.second), wrk.id));
					}
				} else{
					if(c.bonus != None) take(wrk.id);
					else{
						pair<int,Dir> d = BFSTreballadorsBonus(wrk.pos);
				
						if(d.first != -1){
							if(pos_ok(wrk.pos + d.second)){
								Cell c = cell(wrk.pos + d.second);
								if(not next2queen(wrk.pos + d.second)){
									if(cell(wrk.pos + d.second).id == -1) moves.insert(make_pair((wrk.pos + d.second), wrk.id));
								}
							}
						}
					}
				}
			}
		}

		for(int i = 0; i < r.size(); ++i){
			Ant a = ant(r[i]);
			bool moved = false;
			moveQueen(a, moves, moved);
			if(not moved){
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
		

		for(map<Pos, int>::iterator it = moves.begin(); it != moves.end(); ++it){
			pair<Pos, int> res = make_pair(it -> first, it -> second);
			pair<bool, Dir> finalDirection = pos2dir(res);
			cerr << 't' << endl;
			if(finalDirection.first) move(it -> second, finalDirection.second);
		}
	}
};

/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);


/* TACTICA
	Put cpu time to check
*/