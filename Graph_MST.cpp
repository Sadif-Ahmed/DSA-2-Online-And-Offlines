#include<iostream>
#include<vector>
#include <fstream>
#include <string>
#include<list>
#include<bits/stdc++.h>

using namespace std;
#define inf 9999999
class Graph
{
public:
    int num_of_vertices;
    int num_of_edges;
    vector<int> sources;
    vector<pair<pair<int,int>,double>> grph;
    vector<pair<pair<int,int>,double>> mst;
    double cost_k;
    double cost_p;
    list <pair<int,double>> *adjl;
    vector<double>key;
    vector<int> parent;
    vector<bool>mst_status;
    Graph()
    {
        cost_k=0;
        cost_p=0;
    }
    Graph(int vertices,int edges)
    {
        num_of_vertices=vertices;
        num_of_edges=edges;
        cost_k=0;
        sources.resize(num_of_vertices,0);
        for(int i=0; i<num_of_vertices; i++)
        {
            sources[i]=i;
        }
        grph.clear();
        mst.clear();
        adjl=new list<pair<int,double>>[num_of_vertices];
        key.resize(num_of_vertices,inf);
        parent.resize(num_of_vertices,-1);
        mst_status.resize(num_of_vertices,false);

    }
    void Set_graph(int vertices,int edges)
    {
        num_of_vertices=vertices;
        num_of_edges=edges;
        cost_k=0;
        cost_p=0;
        sources.resize(num_of_vertices,0);
        for(int i=0; i<num_of_vertices; i++)
        {
            sources[i]=i;
        }
        grph.clear();
        mst.clear();
        adjl=new list<pair<int,double>>[num_of_vertices];
        key.resize(num_of_vertices,inf);
        parent.resize(num_of_vertices,-1);
        mst_status.resize(num_of_vertices,false);

    }
    vector<pair<pair<int,int>,double>> get_mst()
    {
        return mst;
    }
    vector<pair<pair<int,int>,double>> get_graph()
    {
        return grph;
    }
    double get_cost(char algo)
    {
        if(algo=='k')
            return cost_k;
        else
            return cost_p;
    }
    vector<int> get_parent()
    {
        return parent;
    }
    void add_w_edge(int u,int v,double w)
    {
        grph.push_back(make_pair(make_pair(u,v),w));
        adjl[u].push_back(make_pair(v,w));
        adjl[v].push_back(make_pair(u,w));
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
                cost_k+=grph[i].second;
            }
        }
    }
    void mst_prims()
    {
        priority_queue <pair<double,int>,vector<pair<double,int>>,greater<pair<double,int>>> min_heap;
        int prim_src=0;
        min_heap.push(make_pair(0,prim_src));
        key[prim_src]=0;
        while(!min_heap.empty())
        {
            int temp_u=min_heap.top().second;
            min_heap.pop();
            if(mst_status[temp_u]==true)
            {
                continue;
            }
            mst_status[temp_u]=true;

            for(list<pair<int,double>>::iterator it=adjl[temp_u].begin(); it!=adjl[temp_u].end(); it++)
            {
                int temp_v=(*it).first;
                double temp_w=(*it).second;
                if(mst_status[temp_v]==false && key[temp_v]>temp_w)
                {
                    key[temp_v]=temp_w;
                    min_heap.push(make_pair(key[temp_v],temp_v));
                    parent[temp_v]=temp_u;
                }

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
void Print_Edges(vector<int> parent,int num_of_vertices)
{
    for(int i=1; i<num_of_vertices; i++)
        {
            cout<<parent[i]<<"---"<<i<<endl;
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
    // X.mst_kruskal();
    //Print_Edges(X.get_graph());
    // cout<<"The Cost is:"<<X.get_cost('k')<<endl;
    // Print_Edges(X.get_mst());
    X.mst_prims();
    Print_Edges(X.get_parent(),num_of_vertices);
}
