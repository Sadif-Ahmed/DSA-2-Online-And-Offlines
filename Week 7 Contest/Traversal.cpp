
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <iterator>
#include <climits>
#include<bits/stdc++.h>
#include<cmath>
#include<limits.h>
#include <fstream>
#include <sstream>
using namespace std;
typedef pair<long long,double> dest_weight;
typedef pair<long long,long long> vertice;
#define inf 9999999.9999999
class Graph
{
public:
    long long num_of_vertices;
    long long num_of_edges;
    vector<dest_weight> *dir_graph;
    vector<dest_weight> *rev_dir_graph;
    enum colour
    {
        WHITE,
        RED,
        BLUE,
        GRAY,
        BLACK
    };
    vector<long long>bfs_parent;
    vector<long long>dfs_parent;
    vector<long long>bfs_src_distances;
    vector<long long>bfs_visit_count;
    long long dfs_time=0;
    vector<long long>dfs_disc_t;
    vector<long long>dfs_end_t;
    stack<long long> reverse_topological;
    queue<long long> bfs_queue;
    vector<bool>und_cycle_visit;
    vector<colour>bipart;
    vector<colour>visit_status;
    vector<bool>visit_status_dir;
    vector<bool>visit_status_rev_dir;
    //MST Elements
    vector<long long> sources;
    vector<pair<vertice,double>> List_of_Edges;//Storing the edges
    vector<pair<vertice,double>> Kruskal_MST_Edges;//Storing the mst
    double cost_k;
    double cost_p;
    vector<double>key;//Vector of Weights
    vector<long long> parent;//Prim's MST parent List
    vector<bool>mst_status;//Prim's node in MST checker

    double **Adj_Matt;
    double **Result_Mat;
    long long **Predecessor;

    Graph(long long num_v=10000,long long num_e=10000)
    {
        num_of_vertices=num_v;
        num_of_edges=num_e;
        dir_graph= new vector<dest_weight>[num_of_vertices];
        rev_dir_graph= new vector<dest_weight>[num_of_vertices];
        bfs_parent.resize(num_of_vertices,-1);
        dfs_parent.resize(num_of_vertices,-1);
        bfs_src_distances.resize(num_of_vertices,INT_MAX);
        bfs_visit_count.resize(num_of_vertices,INT_MAX);
        dfs_disc_t.resize(num_of_vertices,0);
        dfs_end_t.resize(num_of_vertices,0);
        und_cycle_visit.resize(num_of_vertices,false);
        bipart.resize(num_of_vertices,WHITE);
        visit_status.resize(num_of_vertices,WHITE);
        visit_status_dir.resize(num_of_vertices,false);
        visit_status_rev_dir.resize(num_of_vertices,false);

        cost_k=0;
        cost_p=0;
        sources.resize(num_of_vertices,0);
        for(long long i=0; i<num_of_vertices; i++)
        {
            sources[i]=i;
        }
        List_of_Edges.clear();
        Kruskal_MST_Edges.clear();
        key.resize(num_of_vertices,inf);
        parent.resize(num_of_vertices,-1);
        mst_status.resize(num_of_vertices,false);
/*
        Adj_Matt=new double*[num_of_vertices];
        for(long long i=0; i<num_of_vertices; i++)
        {
            Adj_Matt[i]=new double[num_of_vertices];
        }
        for(long long i=0; i<num_of_vertices; i++)
        {
            for(long long j=0; j<num_of_vertices; j++)
            {
                if(i==j)
                {
                    Adj_Matt[i][j]=0;
                }
                else
                {
                    Adj_Matt[i][j]=INT_MAX;
                }
            }
        }
        Predecessor=new long long*[num_of_vertices];
        for(long long i=0; i<num_of_vertices; i++)
        {
            Predecessor[i]=new long long[num_of_vertices];
        }
        for(long long i=0; i<num_of_vertices; i++)
        {
            for(long long j=0; j<num_of_vertices; j++)
            {
                Predecessor[i][j]=-1;
            }
        }
*/
    }
    void reset_graph(long long num_v,long long num_e)
    {
         bfs_parent.clear();
        dfs_parent.clear();
        bfs_src_distances.clear();
        bfs_visit_count.clear();
        dfs_disc_t.clear();
        dfs_end_t.clear();
        und_cycle_visit.clear();
        bipart.clear();
        visit_status.clear();
        visit_status_dir.clear();
        visit_status_rev_dir.clear();
        num_of_vertices=num_v;
        num_of_edges=num_e;
        dir_graph= new vector<dest_weight>[num_of_vertices];
        rev_dir_graph= new vector<dest_weight>[num_of_vertices];
        bfs_parent.resize(num_of_vertices,-1);
        dfs_parent.resize(num_of_vertices,-1);
        bfs_src_distances.resize(num_of_vertices,INT_MAX);
        bfs_visit_count.resize(num_of_vertices,INT_MAX);
        dfs_disc_t.resize(num_of_vertices,0);
        dfs_end_t.resize(num_of_vertices,0);
        und_cycle_visit.resize(num_of_vertices,false);
        bipart.resize(num_of_vertices,WHITE);
        visit_status.resize(num_of_vertices,WHITE);
        visit_status_dir.resize(num_of_vertices,false);
        visit_status_rev_dir.resize(num_of_vertices,false);

        cost_k=0;
        cost_p=0;
        sources.resize(num_of_vertices,0);
        for(long long i=0; i<num_of_vertices; i++)
        {
            sources[i]=i;
        }
        List_of_Edges.clear();
        Kruskal_MST_Edges.clear();
        key.resize(num_of_vertices,inf);
        parent.resize(num_of_vertices,-1);
        mst_status.resize(num_of_vertices,false);
/*
        Adj_Matt=new double*[num_of_vertices];
        for(long long i=0; i<num_of_vertices; i++)
        {
            Adj_Matt[i]=new double[num_of_vertices];
        }
        for(long long i=0; i<num_of_vertices; i++)
        {
            for(long long j=0; j<num_of_vertices; j++)
            {
                if(i==j)
                {
                    Adj_Matt[i][j]=0;
                }
                else
                {
                    Adj_Matt[i][j]=INT_MAX;
                }
            }
        }
        Predecessor=new long long*[num_of_vertices];
        for(long long i=0; i<num_of_vertices; i++)
        {
            Predecessor[i]=new long long[num_of_vertices];
        }
        for(long long i=0; i<num_of_vertices; i++)
        {
            for(long long j=0; j<num_of_vertices; j++)
            {
                Predecessor[i][j]=-1;
            }
        }
*/
    }
    void reset()
    {
        bfs_parent.clear();
        dfs_parent.clear();
        bfs_src_distances.clear();
        bfs_visit_count.clear();
        dfs_disc_t.clear();
        dfs_end_t.clear();
        und_cycle_visit.clear();
        bipart.clear();
        visit_status.clear();
        visit_status_dir.clear();
        visit_status_rev_dir.clear();
        bfs_parent.resize(num_of_vertices,-1);
        dfs_parent.resize(num_of_vertices,-1);
        bfs_src_distances.resize(num_of_vertices,INT_MAX);
        bfs_visit_count.resize(num_of_vertices,INT_MAX);
        dfs_disc_t.resize(num_of_vertices,0);
        dfs_end_t.resize(num_of_vertices,0);
        und_cycle_visit.resize(num_of_vertices,false);
        bipart.resize(num_of_vertices,WHITE);
        visit_status.resize(num_of_vertices,WHITE);
        visit_status_dir.resize(num_of_vertices,false);
        visit_status_rev_dir.resize(num_of_vertices,false);

        cost_k=0;
        cost_p=0;
        sources.resize(num_of_vertices,0);
        for(long long i=0; i<num_of_vertices; i++)
        {
            sources[i]=i;
        }
        Kruskal_MST_Edges.clear();
        key.resize(num_of_vertices,inf);
        parent.resize(num_of_vertices,-1);
        mst_status.resize(num_of_vertices,false);
    }
    vector<pair<vertice,double>> get_mst()
    {
        return Kruskal_MST_Edges;
    }
    vector<pair<vertice,double>> get_graph_edges()
    {
        return List_of_Edges;
    }
    double get_cost(char algo)
    {
        if(algo=='k')
            return cost_k;
        else
            return cost_p;
    }
    vector<long long> get_parent()
    {
        return parent;
    }
    vector<double> get_weight()
    {
        return key;
    }
    double ** get_APSP()
    {
        return Result_Mat;
    }
    long long ** get_Predecessor()
    {
        return Predecessor;
    }
    double ** get_mat()
    {
        return Adj_Matt;
    }
    void Add_Edge_Directed(long long startv,long long endv,double weight=0)
    {
        dir_graph[startv].push_back(make_pair(endv,weight));
        rev_dir_graph[endv].push_back(make_pair(startv,weight));
        List_of_Edges.push_back(make_pair(make_pair(startv,endv),weight));
      //  Adj_Matt[startv][endv]=weight;
        //Predecessor[startv][endv]=endv;

    }
    void Add_Edge_Undirected(long long startv,long long endv,double weight=0)
    {
        dir_graph[startv].push_back(make_pair(endv,weight));
        dir_graph[endv].push_back(make_pair(startv,weight));
        List_of_Edges.push_back(make_pair(make_pair(startv,endv),weight));
        List_of_Edges.push_back(make_pair(make_pair(endv,startv),weight));
        /*Adj_Matt[startv][endv]=weight;
        Adj_Matt[endv][startv]=weight;
        Predecessor[startv][endv]=endv;
        Predecessor[endv][startv]=startv;
    */
    }

    void BFS_Unweighted(long long root)
    {
        visit_status[root]=GRAY;
        bfs_src_distances[root]=0;
        bfs_parent[root]=0;

        bfs_queue.push(root);
        while(!bfs_queue.empty())
        {
            long long temp=bfs_queue.front();
            bfs_queue.pop();
          //  cout<<temp<<" ";

            for(vector<dest_weight>::iterator it=dir_graph[temp].begin(); it!=dir_graph[temp].end(); it++)
            {
                if(visit_status[(*it).first]==WHITE)
                {
                    visit_status[(*it).first]=GRAY;
                    bfs_src_distances[(*it).first]=bfs_src_distances[root]+1;
                    bfs_parent[(*it).first]=temp;
                    bfs_queue.push((*it).first);
                }
            }
            visit_status[temp]=BLACK;
        }

    }

/*
    ~Graph()
    {
        for(long long i=0; i<num_of_vertices; i++)
        {
            delete[] Result_Mat[i];
            delete[] Adj_Matt[i];
        }
        delete[]Result_Mat;
        delete[]Adj_Matt;
    }
    */
};

int main()
{

long long t;
cin>>t;
long long x,u,v;
Graph X;

for(long long i=0;i<t;i++)
{   X.reset_graph(20005,40010);
    cin>>x;
   for(long long j=0;j<x;j++)
   {
    cin>>u>>v;
    X.Add_Edge_Undirected(u-1,v-1);
   }
   for(int i=0;i<20005;i++)
   {   if(X.dir_graph[i].size()!=0)
   {
       X.BFS_Unweighted(i);
   }
   }
   long long blk=0,gr=0;
   for(int i=0;i<20005;i++)
   {
       if(X.visit_status[i]==X.BLACK)
       {
           blk++;
       }
       if(X.visit_status[i]==X.GRAY)
       {
           gr++;
       }
   }
   cout<<"Case "<<i+1<<": "<<max(blk,gr)-1<<endl;
}
    return 0;
}


