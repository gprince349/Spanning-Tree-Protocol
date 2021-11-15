#include "bridgesim.h"
#include "bridge.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////

vector<string> split(string s){
	vector<string> temp;

	 string word = ""; 
	   for (auto x : s) 
	   { 
	       if (x == ' ') 
	       { 
	           temp.push_back(word); 
	           word = ""; 
	       } 
	       else
	       { 
	           word = word + x; 
	       } 
	   }  
    temp.push_back(word); 
    return temp;
} 

vector<int> Mysort(vector<int> S){
	vector<int> S1 = S;
	sort(S1.begin(),S1.end());
	return S1;
}


void print(vector<Bridge> &G1,vector<LAN> &L){

	for(Bridge B : G1){
	cout<<B.name<<" ";
		for(int Lid : Mysort(B.Lans) ){
			if(L[Lid].Bid_Ptype[B.id] == 1){
				cout<<L[Lid].name<<"-"<<"DP"<<" ";
			}
			if(L[Lid].Bid_Ptype[B.id] == 2){
				cout<<L[Lid].name<<"-"<<"RP"<<" ";
			}
			if(L[Lid].Bid_Ptype[B.id] == 0){
				cout<<L[Lid].name<<"-"<<"NP"<<" ";
			}

		}
		cout<<endl;
	}
}

void print_after_learn(vector<Bridge> &G){
	for(auto x : G){
		cout<<x.name<<endl;
		cout<<"HOST ID | FORWARDING PORT"<<endl;
		map<string,int>::iterator it;
		for(it = x.H_destp.begin(); it!=x.H_destp.end(); it++){
			
			cout<<it->first<<" | "<<char(it->second + 65)<<endl;
		}	

	}
}

bool check_lan(char A){
    if(int(A) >64 && int(A) < 91){
        return true;
    }
    else{
        return false;
    }
}


// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////


int main(){

	vector<Bridge> G;

	vector<LAN> L(26);
	for (int i = 0; i < 26; ++i)
	{
		L[i].name = (char)(65+i);
		L[i].id = i;
	}

	int trace,n;
	cin>>trace>>n;

	string g;
	getline(cin,g);

	for (int i = 0; i < n; ++i)
	{
		Bridge B;
		string s;
		getline(cin,s);

		trim(s);
		
		int it = 1;

		for(string x : split(s)){
			//Bridge

			if(it == 1){
				B.name = x;

				it = 0;
			}
			else{
				int code = int(x[0]) - int('A');
				
				B.Lans.push_back(code);
				L[code].Bid_Ptype[i] = 1;
			}
		}

		B.id = i;
		B.LRec = B.Nconf = Msg(i,0,i);

		G.push_back(B);
	}



Spanning_tree(trace,n,G,L);

	print(G,L);


////////////////////////////////////////////////////////////////////
int No_of_query;


while(true){
	string s1;
    getline(cin,s1);
	trim(s1);
    if( !check_lan(s1[0]) ){
        No_of_query = stoi(s1);
        break;
    }  
    else{
        vector<string> temp = split(s1);
        int code;
        int it= 1;
        for(string x : temp){
            if(it ==1){
                 code = int(x[0]) - int('A');
                 it = 0;
            }
            else{

                L[code].Host.push_back(x);
            }
        }

    } 
}

for (int i = 0; i < No_of_query ; i++)
{

    string s2;
    getline(cin,s2);
	trim(s2);

	Packet P;
	P.source = split(s2)[0];
	P.dest = split(s2)[1];
    learningAlgo(trace,n,G,L,P);
	print_after_learn(G);
	cout<<endl;

}



}