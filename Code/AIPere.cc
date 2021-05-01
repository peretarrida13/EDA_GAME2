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
	Dir getDirection(Pos initial, Pos final, map<Pos, Pos> tracker){
		//Given the initial position and the final one get the next direction for the tracker
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
		//BUSCAR LES ENEMIGUES I MATAR (
		//primer en desenvolupar		
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
							Dir d = getDirection(p, next, tracker);
							return make_pair(1, d);
						}
						
					}
				}
			}
		}
		return make_pair(-1, Dir(0));
	}

	pair<int,Dir> BFSTreballadorsBonus(Pos p) {
		//BUSCAR MENJAR PORTAR A LA REINA I TORNAR A BUSCAR MENJAR MÉS LLUNY.
		//STORE ALL POSSITIONS THAT MOVE FOR NO REPETITION
		//STORE THE LET GO POSITION OF FOOD FOR NO REPETITION
		// Segon en desenvolupar

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

	pair<int,Dir> BFSReina(Pos p) {
		//MENJAR I CREAR
		//fer que es quedi per la zona de bonus i els agafi
		//ultim en desenvolupar
		//BUSCAR MENJAR PORTAR A LA REINA I TORNAR A BUSCAR MENJAR MÉS LLUNY.
		//STORE ALL POSSITIONS THAT MOVE FOR NO REPETITION
		//STORE THE LET GO POSITION OF FOOD FOR NO REPETITION
		// Segon en desenvolupar

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

	void moveQueen(Ant a) {
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

	bool antArround(Pos p){
		for(int i = 0; i < 4; ++i){
			Cell c = cell(p + Dir(i));

			if(c.id != -1){
				return true;
			}
		}
		return false;
	}

	/**
	 * Play method, invoked once per each round.
	 */
	virtual void play(){
		vector<int> r = queens(me());
		vector<int> wrks = workers(me());
		vector<int> sold = soldiers(me());


		for(int i = 0; i < r.size(); ++i){
			Ant a = ant(r[i]);
			cerr << a.reserve.size() << endl;
			moveQueen(a);

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
				bool arround = antArround(a.pos);
				if(not arround){
					pair<int, Dir> d = BFSReina(a.pos);
					if(d.first != -1){
						if(pos_ok(a.pos + d.second) and cell(a.pos + d.second).id == -1){
							move(a.id, d.second);
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
									if(pos_ok(wrk.pos + Left)) move(wrk.id, Left);
									else{
										if(pos_ok(wrk.pos + Up)) move(wrk.id, Up);
										else move(wrk.id, Down);
									}
								} else{
									if(pos_ok(wrk.pos + Right)) move(wrk.id, Right);
									else{
										if(pos_ok(wrk.pos + Up)) move(wrk.id, Up);
										else move(wrk.id, Down);
									}
								}
							} 
							if(wrk.pos.j == a.pos.j){
								if(wrk.pos.i < a.pos.i){
									if(pos_ok(wrk.pos + Up)) move(wrk.id, Up);
									else{
										if(pos_ok(wrk.pos+ Left)) move(wrk.id, Left);
										else move(wrk.id, Right);
									}
								} else{
									if(pos_ok(wrk.pos + Down)) move(wrk.id, Down);
									else{
										if(pos_ok(wrk.pos + Left)) move(wrk.id, Left);
										else move(wrk.id, Right);
									}
								}
							}
						}
						/*
						Pos aux = wrk.pos + Up;
						if(pos_ok(aux)){
							move(wrk.id, Up);
						} else{
							move(wrk.id, Down);
						} */
					}else{
						pair<int,Dir> d = BFSTreballadorsBonus(wrk.pos);
				
						if(d.first != -1){
							Cell c = cell(wrk.pos + d.second);
							if(pos_ok(wrk.pos + d.second) and c.id == -1) {
								move(wrk.id, d.second);
							}
						}
					}
				}
			} else{
				if(wrk.bonus != None) move2queen(wrk.pos, wrk.id);
				else{
					if(c.bonus != None) take(wrk.id);
					else{
						pair<int,Dir> d = BFSTreballadorsBonus(wrk.pos);
				
						if(d.first != -1){
							Cell c = cell(wrk.pos + d.second);
							if(pos_ok(wrk.pos + d.second) and c.id == -1) {
								if(not next2queen(wrk.pos + d.second)){
									move(wrk.id, d.second);
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
					Cell c = cell(s.pos + d.second);
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
						move(s.id, d.second);
					}
				}
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