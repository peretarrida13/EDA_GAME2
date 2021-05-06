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
    	}else if(res.i < initial.i) return Up;
      	else return Down;
	}

	pair<int, Dir> BFSGuerrer(Pos p, vector<Pos>& finals) {	
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
				
				bool found = false;

				for(int j = 0; j < finals.size() and not found; ++j){
					if(finals[j] == next) found = true;
				}
				
				if(pos_ok(next) and c.type == Soil and not found) {
					map<Pos, bool>::iterator it  = visited.find(next);
					if (it == visited.end()){
						q.push(next);
						visited.insert(make_pair(next, true));
						tracker.insert(make_pair(next, active));
					}
					if(c.id != -1){
						Ant aux = ant(c.id);
						if(aux.type == Worker){
							bool mine = false;
							vector<int> w = workers(me());
							vector<int> q = queens(me());
							vector<int> s = soldiers(me());

							for(int j = 0; j < w.size(); ++j){
								if(w[j] == c.id) mine = true;
							}
							
							for(int j = 0; j < q.size(); ++j){
								if(q[j] == c.id) mine = true;
							}
							
							for(int j = 0; j < s.size(); ++j){
								if(s[j] == c.id) mine = true;
							}

							if(not mine){
								Dir d = getDirection(p,next, tracker);
								finals.push_back(next);
								return make_pair(1, d);
							}
						}
					}
				}
			}
		}
		return make_pair(-1, Up);
	}

	pair<int,Dir> BFSTreballadorsBonus(Pos p, vector<Pos>& finals) {
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
				
				if(pos_ok(next) and c.type == Soil and c.id == -1) {
					map<Pos, bool>::iterator it  = visited.find(next);
					if (it == visited.end()){
						q.push(next);
						visited.insert(make_pair(next, true));
						tracker.insert(make_pair(next, active));
					}
					if(c.bonus != None){
						if(c.id == -1){
							Dir d = getDirection(p, next, tracker);
							finals.push_back(next);
							return make_pair(1, d);
						}
					}
				}
			}
		}
		return make_pair(-1, Dir(0));
	}

	pair<int,Dir> BFSReina(Pos p, vector<Pos>& finals) {
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

				if(pos_ok(next) and c.type == Soil and c.id == -1) {
					map<Pos, bool>::iterator it  = visited.find(next);

					if (it == visited.end()){
						q.push(next);
						visited.insert(make_pair(next, true));
						tracker.insert(make_pair(next, active));
					}
					if(c.bonus != None){
						Dir d = getDirection(p, next, tracker);
						finals.push_back(next);
						return make_pair(1, d);
					}
				}
			}
		}
		return make_pair(-1, Dir(0));

	}

	bool moveQueen(Ant a, map<Pos, int>& moves) {
		int i = 0;
		bool moved = false;
		
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

		return moved;
	}

	bool next2queen(Pos p){
		vector<int> reina = queens(me());
		int r_size = reina.size();
		for(int i = 0; i < r_size; ++i){
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

				if(pos_ok(next) and c.type == Soil) {
					map<Pos, bool>::iterator it  = visited.find(next);

					if (it == visited.end()){
						q.push(next);
						visited.insert(make_pair(next, true));
						tracker.insert(make_pair(next, active));
					}

					vector<int> r = queens(me());
					int r_size = r.size();
					for(int j = 0 ; j < r_size; ++j){
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
		int r_size = r.size();
		for(int i = 0; i < r_size; ++i){
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
				return make_pair(true, Right);
			} else{
				return make_pair(true, Left);
			}
		} else if(inici.i < final.i){
			return make_pair(true, Down);
		} else{
			return make_pair(true, Up);
		}

		return make_pair(false, Dir(random(0, 3)));
	}

	virtual void play(){
		vector<int> r = queens(me());
		int r_size = r.size();
		vector<int> wrks = workers(me());
		int w_size = wrks.size();
		vector<int> sold = soldiers(me());
		int s_size = sold.size();

		vector<Pos> finals;
		map<Pos, int> moves;

		for(int i = 0; i < s_size; ++i){
			Ant s  = ant(sold[i]);
			pair<int,Dir> d = BFSGuerrer(s.pos, finals);
				
			if(d.first != -1){
				if(pos_ok(s.pos + d.second)) {
					bool mine = false;
					Cell c = cell(s.pos + d.second);
					for(int j = 0; j < w_size; ++j){
						if(wrks[j] == c.id) mine = true;
					}
						
					for(int j = 0; j < r_size; ++j){
						if(r[j] == c.id) mine = true;
					}
						
					for(int j = 0; j < s_size; ++j){
						if(sold[j] == c.id) mine = true;
					}
					if(not mine) moves.insert(make_pair((s.pos + d.second), s.id));	
				}
			}
		}
			
		for(int i = 0; i < w_size; ++i){
			Ant wrk  = ant(wrks[i]);
			Cell c = cell(wrk.pos);

			if(next2queen(wrk.pos)){
				if(wrk.bonus != None) leave(wrk.id);
				else{
					if(c.bonus != None){
						for(int j = 0; j < r_size; ++j){
							Ant a = ant(r[j]);
							
							if(wrk.pos.i == a.pos.i){
								if(wrk.pos.i < a.pos.i){
									if(pos_ok(wrk.pos + Left) and cell(wrk.pos + Left).id == -1) moves.insert(make_pair((wrk.pos + Left), wrk.id));
									else{
										if(pos_ok(wrk.pos + Up) and cell(wrk.pos + Up).id == -1) moves.insert(make_pair((wrk.pos + Up), wrk.id));
										else{
											if(cell(wrk.pos + Down).id == -1) moves.insert(make_pair((wrk.pos + Down), wrk.id));
										} 
									}
								} else{
									if(pos_ok(wrk.pos + Right) and cell(wrk.pos + Right).id == -1) moves.insert(make_pair((wrk.pos + Right), wrk.id));
									else{
										if(pos_ok(wrk.pos + Up) and cell(wrk.pos + Up).id == -1) moves.insert(make_pair((wrk.pos + Up), wrk.id));
										else{
											if(cell(wrk.pos + Down).id == -1) moves.insert(make_pair((wrk.pos + Down), wrk.id));
										} 
									}
								}
							} 
							if(wrk.pos.j == a.pos.j){
								if(wrk.pos.i < a.pos.i){
									if(pos_ok(wrk.pos + Up) and cell(wrk.pos + Up).id == -1) moves.insert(make_pair((wrk.pos + Up), wrk.id));
									else{
										if(pos_ok(wrk.pos+ Left) and cell(wrk.pos + Left).id == -1) moves.insert(make_pair((wrk.pos + Left), wrk.id));
										else{
											if(cell(wrk.pos + Right).id == -1) moves.insert(make_pair((wrk.pos + Right), wrk.id));
										}
									}
								} else{
									if(pos_ok(wrk.pos + Down)and cell(wrk.pos + Down).id == -1) moves.insert(make_pair((wrk.pos + Down), wrk.id));
									else{
										if(pos_ok(wrk.pos + Left) and cell(wrk.pos + Left).id == -1) moves.insert(make_pair((wrk.pos + Left), wrk.id));
										else{
											if(cell(wrk.pos + Right).id == -1) moves.insert(make_pair((wrk.pos + Right), wrk.id));
										} 
									}
								}
							}
						}
					}else{
						pair<int,Dir> d = BFSTreballadorsBonus(wrk.pos, finals);
						if(d.first != -1){
							Cell c1 = cell(wrk.pos + d.second);
							if(pos_ok(wrk.pos + d.second) and c1.id == -1) {
								if(cell(wrk.pos + d.second).id == -1) moves.insert(make_pair((wrk.pos + d.second), wrk.id));
								else{
									int j = 0;
									bool found = false;
									Pos auxiliar;
									while(j < 4 and not found){
										auxiliar = wrk.pos + Dir(j);
										if(pos_ok(auxiliar) and cell(auxiliar).id == -1){
											found = true;
										}
										++j;
									}
									if(found) moves.insert(make_pair(auxiliar, wrk.id));
								}
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
						pair<int,Dir> d = BFSTreballadorsBonus(wrk.pos, finals);
				
						if(d.first != -1){
							if(pos_ok(wrk.pos + d.second)){
								if(not next2queen(wrk.pos + d.second)){
									if(cell(wrk.pos + d.second).id == -1) moves.insert(make_pair((wrk.pos + d.second), wrk.id));
								}
							}
						}
					}
				}
			}
		}

		for(int i = 0; i < r_size; ++i){
			Ant a = ant(r[i]);
			if(not moveQueen(a, moves)){
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
						pair<int, Dir> d = BFSReina(a.pos, finals);
						if(d.first != -1){
							if(pos_ok(a.pos + d.second) and cell(a.pos + d.second).id == -1){
								int quadrant = identQuadrant(a.pos);
								bool out = outOfQuadrant(a.pos+d.second, quadrant);
								if(not out) moves.insert(make_pair(a.pos +d.second,a.id));
							}
						}
						
					}
				}
			}
		}

		for(map<Pos, int>::iterator it = moves.begin(); it != moves.end(); ++it){
			pair<Pos, int> res = make_pair(it -> first, it -> second);
			pair<bool, Dir> finalDirection = pos2dir(res);
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