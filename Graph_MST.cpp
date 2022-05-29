#include<iostream>
#include<vector>
#include <fstream>
#include <string>
using namespace std;

class Graph
{
public:
    int num_of_vertices;
    int num_of_edges;
    vector<int> sources;
    vector<pair<pair<int,int>,double>> grph;
    vector<pair<pair<int,int>,double>> mst;
    double cost;
    Graph()
    {
        cost=0;
    }
    Graph(int vertices,int edges)
    {
        num_of_vertices=vertices;
        num_of_edges=edges;
        cost=0;
        sources.resize(num_of_vertices,0);
        for(int i=0; i<num_of_vertices; i++)
        {
            sources[i]=i;
        }
        grph.clear();
        mst.clear();

    }
    void Set_graph(int vertices,int edges)
    {
        num_of_vertices=vertices;
        num_of_edges=edges;
        cost=0;
        sources.resize(num_of_vertices,0);
        for(int i=0; i<num_of_vertices; i++)
        {
            sources[i]=i;
        }
        grph.clear();
        mst.clear();

    }
    vector<pair<pair<int,int>,double>> get_mst()
    {
        return mst;
    }
    vector<pair<pair<int,int>,double>> get_graph()
    {
        return grph;
    }
    double get_cost()
    {
        return cost;
    }
    void add_w_edge(int u,int v,double w)
    {
        grph.push_back(make_pair(make_pair(u,v),w));
    }
    int find_set(int v)
    {
        if(v==sources[v])
        {
            return v;
        }
        else
        {
            return find_set(sources[v]);
        }
    }
    void union_set(int u,int v)
    {
        sources[u]=sources[v];
    }
    void sort_by_edges()
    {
        for(int i=0; i<num_of_edges; i++)
        {
            for(int j=i+1; j<num_of_edges; j++)
            {
                if(grph[i].second>=grph[j].second)
                {
                    pair<pair<int,int>,double> temp;
                    temp=grph[i];
                    grph[i]=grph[j];
                    grph[j]=temp;
                }
            }
        }
    }
    void mst_kruskal()
    {
        int i,temp_u,temp_v;
        sort_by_edges();
        for(int i=0; i<grph.size(); i++)
        {
            temp_u=find_set(grph[i].first.first);
            temp_v=find_set(grph[i].first.second);
            if(temp_u!=temp_v)
            {
                mst.push_back(grph[i]);
                union_set(temp_u,temp_v);
                cost+=grph[i].second;
            }
        }

    }

};
void Print_Edges(vector<pair<pair<int,int>,double>> test)
{
    cout<<"Edges"<<"   "<<"Weight"<<endl;
    for(int i=0; i<test.size(); i++)
    {
        cout<<test[i].first.first<<" -- "<<test[i].first.second<<"   "<<test[i].second<<endl;
    }
}
int main()
{
    ifstream file;
    bool check=true;
     int num_of_edges,num_of_vertices;
    file.open("mst.in.txt");
    string line;
    Graph X;
    if ( file.is_open() )
    {
        while (file && num_of_edges!=0 )   // equivalent to myfile.good()
        {
            std::getline (file, line);
            if(check)
            {
                num_of_vertices=line[0]-'0';
                num_of_edges=line[2]-'0';
                X.Set_graph(num_of_vertices,num_of_edges);
                check=false;
            }
            else
            {
                X.add_w_edge(line[0]-'0',line[2]-'0',line[4]-'0');
                num_of_edges--;
            }

        }
    }
    else
    {
        std::cout << "Couldn't open file\n";
    }
    X.mst_kruskal();
    Print_Edges(X.get_graph());
    cout<<"The Cost is:"<<X.get_cost()<<endl;
    Print_Edges(X.get_mst());
}
