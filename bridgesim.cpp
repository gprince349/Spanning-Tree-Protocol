#include "bridgesim.h"
using namespace std;


int which_LAN(string h,vector<LAN> &L){
    for(LAN x : L){
        if(x.is_host_present(h)){
            return x.id;
        }
    }
    return -1;

}

class tr_save{
	public:
		int time;
		string typ;
		int B;
		Msg M;
		tr_save(){

		}
		tr_save(int t,string ty,int b, Msg m){
			time = t;
			typ = ty;
			B = b;
			M = m;
		}
		bool operator <  (tr_save obj){
				if(time != obj.time){
					return time < obj.time;
				}
				else if(B != obj.B){
					return B < obj.B;
				}
				else{
					return true;
				}
			
		}
};

class tr_save2{
	public:
		int time;
		string typ;
		int B;
		Packet P;
		tr_save2(){

		}
		tr_save2(int t,string ty,int b, Packet p){
			time = t;
			typ = ty;
			B = b;
			P = p;
		}
		bool operator <  (tr_save2 obj){
				if(time != obj.time){
					return time < obj.time;
				}
				else if(B != obj.B){
					return B < obj.B;
				}
				else{
					return true;
				}
			
		}
};

void tr_func(int time,int trace,string typ,int B,Msg M){
		if(trace == 1){
			cout<<time<<" "<<typ<<" "<<"B"<<B+1<<" "<<"("<<"B"<<M.root_id+1<<" "<<M.distance<<" "<<"B"<<M.id+1<<")"<<endl;
		}

}

void tr_func2(int time,int trace,string typ,int B,Packet P){
		if(trace == 1){
			cout<<time<<" "<<typ<<" "<<"B"<<B+1<<" "<<P.source<<" --> "<<P.dest<<endl;
		}

}

vector<int> Mysort1(vector<int> S){
	vector<int> S1 = S;
	sort(S1.begin(),S1.end());
	return S1;
}



void Spanning_tree(int trace,int n,vector<Bridge> &G1,vector<LAN> &L){
		// vector<Bridge> G1 = G; //sorted order bridges inside vector

		

		vector<vector<int>> adj_LAN(26); // 0 == A --- 25 == Z and inside vector are ids-1 of bridges

		for (int i = 0; i < n; ++i)
		{
			for(int x : G1[i].Lans){
				adj_LAN[x].push_back(i);
			}
		}
		for (int i = 0; i < 26; i++)
		{
			adj_LAN[i] =  Mysort1(adj_LAN[i]);
		}
		

		bool go = true;
		int time = 0;

		vector<tr_save> tr_print;

		while(go == true){
			go = false;

			

			for(Bridge B : G1){
				int trigger = 0;

				for(int Lid : B.Lans){
					if(L[Lid].Bid_Ptype[B.id] == 1){
					L[Lid].M_list.push_back(B.Nconf);  //need to remove also

						//tracing
						trigger = 1;
						

					}
				}

				if(trigger == 1){
					// tr_func(time,trace,"s",B.id,B.Nconf); //send =1 ,recceive = 0
					tr_print.push_back(tr_save(time,"s",B.id,B.Nconf));
				}

			}
		// time
				time++;

			for (int i = 0; i < 26; ++i)
			{
				for(int Bid : adj_LAN[i]){
						for(Msg M: L[i].M_list){
							if(M.id != Bid && L[i].Bid_Ptype[Bid] != 0)  //L[i].Bid_Ptype[Bid] != 0
							{ // lan to bridge msg 

									// M and B.M comparison
								Msg M2 = G1[Bid].Nconf;
								// if(M.root_id != M2.root_id){
								// 	go = true;
								// }

								tr_print.push_back(tr_save(time,"r",Bid,M));

								if(M.root_id < M2.root_id || M.root_id == M2.root_id && M.distance+1 < M2.distance || M.root_id == M2.root_id && M.distance +1 == M2.distance && M.id < G1[Bid].LRec.id){
									Msg Temp(M.root_id,M.distance+1,M2.id);
									G1[Bid].Nconf = Temp;
									G1[Bid].LRec = M;

									for(int j : G1[Bid].Lans){
										if(L[j].Bid_Ptype[Bid] == 2){
											L[j].Bid_Ptype[Bid] =1;	
										}
									}

									L[i].Bid_Ptype[Bid] = 2;
									go = true;

								}
								else if(M.root_id == G1[Bid].Nconf.root_id && M.distance +1 == G1[Bid].Nconf.distance  || M.root_id == G1[Bid].Nconf.root_id && M.distance == G1[Bid].Nconf.distance && Bid > M.id ){
									//DP
									if(L[i].Bid_Ptype[Bid] == 1){
										L[i].Bid_Ptype[Bid] = 0;
										go = true;
									}
									

								}
								else {}

							}

						}
				}

			}

		
			//empty msges
			for (int i = 0; i < 26; ++i)
			{
				L[i].M_list.clear();
			}

		
		}

			sort(tr_print.begin(),tr_print.end());

			for(auto x : tr_print){

					tr_func(x.time,trace,x.typ,x.B,x.M);

			}

		// return G;
}

void learningAlgo(int trace,int n,vector<Bridge> &G,vector<LAN> &L,Packet P){

	vector<vector<int>> adj_LAN(26); // 0 == A --- 25 == Z and inside vector are ids-1 of bridges

		for (int i = 0; i < n; ++i)
		{
			for(int x : G[i].Lans){
				adj_LAN[x].push_back(i);
			}
		
		}
		for (int i = 0; i < 26; i++)
		{
			adj_LAN[i] = Mysort1(adj_LAN[i]);
		}
		
	
	int source_id = which_LAN(P.source,L);
	int dest_id = which_LAN(P.dest,L);

	L[source_id].P = P;
	
	// int curr_lanid = source_id;
	// int curr_bridgeid;

	bool change = true;
	int time = 0;
	vector<tr_save2> tr_print2;

	while(change){
		change = false;

			

			//LAN to bridge
			for (int i = 0; i < 26; i++)
			{
				if(!L[i].P.is_empty()){

					for(auto j : adj_LAN[i]){
						if(L[i].port_type(j) != 0 && j != L[i].last){

							G[j].P = L[i].P;
							G[j].H_destp[P.source] = i;
							G[j].last = i;
							change = true;

							//tracing

							tr_print2.push_back(tr_save2(time,"r",j,L[i].P));

						}

					}

					L[i].P.empty();	

				}
			}

			time++;
			////////
			//Bridge to LAN
			for (int i = 0; i < n; i++)
			{
				if(!G[i].P.is_empty()){

					tr_print2.push_back(tr_save2(time,"s",i,G[i].P));

					//check for destination port
					string d_temp = G[i].P.dest;
					if(G[i].H_destp.find(d_temp) != G[i].H_destp.end()){
						int l = G[i].H_destp[d_temp]; // l is the lan id to which bridge is gonna transfer 
						if(l != G[i].last){	
							L[l].P = G[i].P;
							L[l].last = i;
							change = true;
							}
					}

					else{
						for(int j : G[i].Lans){
							if(L[j].port_type(i) != 0 && j != G[i].last){
								L[j].P = G[i].P;
								L[j].last = i;
								change = true;
							}
						}
					}

					G[i].P.empty();

				}

			}
			
			

	}

	sort(tr_print2.begin(),tr_print2.end());

			for(auto x : tr_print2){

					tr_func2(x.time,trace,x.typ,x.B,x.P);

			}

	//empty all packets in bridge and lans
	for (int i = 0; i < 26; i++)
	{
		L[i].P.empty();
		L[i].last = -1;
	}
	for (int i = 0; i < n; i++)
	{
		G[i].P.empty();
		G[i].last = -1;
	}
	
	


}
