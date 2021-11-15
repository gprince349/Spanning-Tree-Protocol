#ifndef BRIDGE_H
#define BRIDGE_H

#include<bits/stdc++.h>
#include<map>
#include<string>

using namespace std;

struct Packet{
    string source;
    string dest;
	Packet(){
		source = "none";
		dest = "none";
	}
	void empty(){
		source = "none";
		dest = "none";
	}
	bool is_empty(){
		if(source == "none" && dest == "none"){
			return true;
		}
		else{
			return false;
		}
	}
};

struct Msg{
	int root_id; 
	int distance;
	int id;  //sender bridge id
	Msg(){}
	Msg(int a,int b,int c){
		root_id = a;
		distance = b;
		id = c;
	}
};

struct LAN{
	int id; //name character of LAN 0 to 25
	// int type=1; // root port = 2, DP = 1 , NP = 0 initially all DP
	string name;
	map<int,int> Bid_Ptype; 
	vector<Msg> M_list;
	//
    Packet P;
	int last = -1;
    vector<string> Host;
    bool is_host_present(string H){
        for(string x : Host){
            if (x == H){
                return true;
            } 
        }
        return false;
    }
	int port_type(int Bid){

		return Bid_Ptype[Bid];
	}
	
};

struct Bridge{
	int id; //bridge id  start from 0
	vector<int> Lans; //lans to which bridge is connected  sorted order 0 to 25
	string name;
	int last = -1;
	// Msg OldConf; // Old configuration to send within one iteration
	Msg LRec;	// msg received
	Msg Nconf;  //msg received new configuration

	//maps of host and their ports ie list of learning bridges
    map<string,int> H_destp;
    Packet P;
};



#endif

