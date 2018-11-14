#include "edge_evolution.h"
#include "pattern.h"
#include "declaration.h"

extern vector<graph *> graph_database;
extern map<string,pattern*> edge_code;

string edgeToHashVal(int uLabel,int vLabel,int edgeLabel){
	string hashVal = "";
	hashVal += to_string(uLabel);
	hashVal += to_string(edgeLabel);
	hashVal += to_string(vLabel);
	hashVal += "0";
	return hashVal;
}

pattern* convertEdgeToPattern(int u,int v,int uLabel,int vLabel,int edgeLabel){
    pattern* p = new pattern;
	p->momentum = INITIAL_M;
    p->code.push_back(uLabel);
    p->code.push_back(edgeLabel);
    p->code.push_back(vLabel);
    p->size = 2;
    p->edge_size = 1;
    p->matrix = new int*[2];
    for (int i = 0; i < 2; i++)
        p->matrix[i] = new int[2];
    
    p->dead_node.push_back(false);
    p->dead_node.push_back(false);
    p->matrix[0][0] = uLabel;
    p->matrix[1][1] = vLabel;
    p->matrix[0][1] = edgeLabel;
    p->matrix[1][0] = edgeLabel;
    return p;
}

void updatePatternGids(pattern *pat,int currentGid){
	bool isPositiveGraph = currentGid < positive_graph_count;
	update_gids(isPositiveGraph,currentGid,pat->pgids,pat->ngids);
}

void updatePatternEmbeddings(pattern *pat,int gid,int u,int v)
{
    vector<pair<int, vector<occ*>* > >& emb = pat->embeddings;
    occ *new_occurence=new occ;
    new_occurence->pb(u);
    new_occurence->pb(v);
    int x=emb.size();

    if(!x || emb[x-1].x!=gid)
    {
    	vector<occ*>* new_vec=new vector<occ*>;
    	new_vec->pb(new_occurence);
    	emb.pb({gid,new_vec});
    }
    else
    	emb[x-1].y->pb(new_occurence);
}

void edge_evolution(){
	for (int gid = 0; gid < graph_database.size(); gid++){
		for (int nodeId = 0; nodeId < graph_database[gid]->labels.size(); nodeId++){
			for (int edgeId = 0; edgeId < graph_database[gid]->adjList[nodeId].size(); edgeId++){
				int u = nodeId;
				int v = graph_database[gid]->adjList[nodeId][edgeId].x;
				int uLabel = graph_database[gid]->labels[u];
				int vLabel = graph_database[gid]->labels[v];
				if (uLabel > vLabel){
					swap(u,v);
					swap(uLabel,vLabel);
				}
				int edgeLabel =  graph_database[gid]->adjList[nodeId][edgeId].y;
				string edgeHash = edgeToHashVal(uLabel, vLabel,edgeLabel);

				auto itr = edge_code.find(edgeHash);
				bool isNewEdge = (itr == edge_code.end());
				pattern* pat;
				if (isNewEdge){
					if(gid>=positive_graph_count)
						continue;
					pat = convertEdgeToPattern(u,v,uLabel,vLabel,edgeLabel);
					// insert pattern into the database
					edge_code[edgeHash] = pat; 
					itr = edge_code.find(edgeHash);
				}
				else {
					pat = itr->second;
				}

				updatePatternGids(pat, gid);
				updatePatternEmbeddings(pat,gid,u,v);

				if(uLabel==vLabel)
				{
				    occ *new_occurence=new occ;
				    new_occurence->pb(u);
				    new_occurence->pb(v);
				    int x=pat->embeddings.size();
					pat->embeddings[x - 1].y->pb(new_occurence);
				}
			}
		}
	}
	cout<<"Edge Database"<<edge_code.size()<<endl;
}