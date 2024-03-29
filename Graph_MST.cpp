#include<iostream>
#include<vector>
#include <fstream>
#include <string>
#include<list>
#include<bits/stdc++.h>
#include<stdio.h>

using namespace std;
#define inf 9999999
class Graph
{
private:
    int num_of_vertices;
    int num_of_edges;
    vector<int> sources;
    vector<pair<pair<int,int>,double>> grph;//Storing the edges
    vector<pair<pair<int,int>,double>> mst;//Storing the mst
    double cost_k;
    double cost_p;
    list <pair<int,double>> *adjl;//Adjacency List of Graph
    vector<double>key;//Vector of Weights
    vector<int> parent;//Prim's MST parent List
    vector<bool>mst_status;//Prim's node in MST checker
public:
    Graph()
    {
        cost_k=0;
        cost_p=0;
    }
    Graph(int vertices,int edges)//Order of V
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
    void Reset_graph(int vertices,int edges)//Order of V
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
    vector<double> get_weight()
    {
        return key;
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
    void sort_by_edges()//Order of E(square)
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
    void mst_kruskal()//Order of Elog(E) if sort stl used.
                      //As my manual sort takes E(square) so the algo becomes of order E(square)
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
    void mst_prims()//Order of Elog(V)
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
        for(int i=0; i<num_of_vertices; i++)
        {
            cost_p+=key[i];
        }

    }
};
void Print_Edges(vector<pair<pair<int,int>,double>> test)//Order of V
{
    cout<<"Kruskal Selected Edges"<<endl;
    cout<<"Edges"<<"   "<<"Weight"<<endl;
    for(int i=0; i<test.size(); i++)
    {
        cout<<test[i].first.first<<" -- "<<test[i].first.second<<"  "<<test[i].second<<endl;
    }
}
void Print_Edges(vector<int> arr,vector<double> key,int num_of_vertices)//Order of V
{
    cout<<"Prims Selected Edges"<<endl;
    cout<<"Edges"<<"   "<<"Weight"<<endl;
    for(int i=1; i<num_of_vertices; i++)
    {
        cout<<arr[i]<<" -- "<<i<<"  "<<key[i]<<endl;

    }
}

void print_edge_Set(vector<int> arr,int num_of_vertices)//Order of V(square)
{
    int temp=0;
    vector<int> dest;
    int cnt=1;
    bool check=false;
    cout<<"List of Edges Selected By Prim's:";
    cout<<"{";
    while(1)
    {   if(check==false){
        for(int i=1; i<num_of_vertices; i++)
        {
            if(arr[i]==temp)
            {
                temp=i;
                if(cnt!=1)
                {
                 cout<<",";
                }
                cout<<"("<<arr[i]<<","<<i<<")";
                dest.push_back(i);
                cnt++;
            }
        }
        check=true;
    }
        if(cnt<num_of_vertices-1)
        {
            for(int i=1;i<num_of_vertices;i++)
            { bool present=false;
              for(int j=0;j<dest.size();j++)
              {
                  if(i==dest[j])
                  {
                    present=true;
                  break;
                  }
              }
              if(present==false)
              {
                  if(cnt!=1)
                {
                 cout<<",";
                }
                  cout<<"("<<arr[i]<<","<<i<<")";
                  cnt++;
              }
            }
        }
            else
            {
                break;
            }

        }
    cout<<"}"<<endl;
}


void print_edge_Set(vector<pair<pair<int,int>,double>> test)//Order of V
{

    cout<<"List of Edges Selected By Kruskal's:";
    cout<<"{";
    for(int i=0; i<test.size(); i++)
    {   if(i!=0)
                {
                 cout<<",";
                }
        cout<<"("<<test[i].first.first<<","<<test[i].first.second<<")";
    }
    cout<<"}"<<endl;
}
int main()
{
    freopen("mst.in.txt","r",stdin);
    int num_of_vertices,num_of_edges,u,v;
    double weight;
    cin>>num_of_vertices>>num_of_edges;
    Graph X(num_of_vertices,num_of_edges);
    for(int i=0;i<num_of_edges;i++)
    {
        cin>>u>>v>>weight;
        X.add_w_edge(u,v,weight);
    }
    X.mst_kruskal();
    X.mst_prims();
    cout<<"Cost of minimum spanning tree:"<<X.get_cost('p')<<endl;
    //Print_Edges(X.get_mst());
    //Print_Edges(X.get_parent(),X.get_weight(),num_of_vertices);
    print_edge_Set(X.get_parent(),num_of_vertices);
    print_edge_Set(X.get_mst());
    return 0;


}
