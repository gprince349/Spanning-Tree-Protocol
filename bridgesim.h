#ifndef BRIDGESIM_H
#define BRIDGESIM_H
#include "bridge.h"

#include<iostream>
#include<bits/stdc++.h>
#include<map>
#include<string>

using namespace std;


void Spanning_tree(int trace,int n,vector<Bridge> &G1,vector<LAN> &L);
void learningAlgo(int trace,int n,vector<Bridge> &G,vector<LAN> &L,Packet P);


#endif