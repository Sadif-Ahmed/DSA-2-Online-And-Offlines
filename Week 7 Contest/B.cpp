
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

    void Shortest_path_Djakstra(long long source)//Complexity Elog(V)
    {
        reset();
        priority_queue <pair<double,long long>,vector<pair<double,long long>>,greater<pair<double,long long>>> pq;
        pq.push(make_pair(0,source));
        bfs_src_distances[source]=0;
        while(!pq.empty())
        {
            long long temp=pq.top().second;
            pq.pop();
            if(visit_status_dir[temp]==false)
            {
                visit_status_dir[temp]=true;
                for(long long i=0; i<dir_graph[temp].size(); i++)
                {
                    long long temp_dest=dir_graph[temp][i].first;
                    if(visit_status_dir[temp_dest]==false)
                    {
                        double dist_1=bfs_src_distances[temp]+dir_graph[temp][i].second;
                        double dist_2=bfs_src_distances[temp_dest];
                        if(dist_2>dist_1)
                        {
                            pq.push(make_pair(dist_1,temp_dest));
                            bfs_src_distances[temp_dest]=dist_1;
                            bfs_parent[temp_dest]=temp;
                        }
                    }


                }
            }
        }


    }
    void Shortest_path_City(long long source)//Complexity Elog(V)
    {
        reset();
        double temp;
        priority_queue <pair<double,long long>,vector<pair<double,long long>>,greater<pair<double,long long>>> pq;
        pq.push(make_pair(0,source));
        bfs_src_distances[source]=0;
        while(!pq.empty())
        {
            long long temp=pq.top().second;
            pq.pop();
            if(visit_status_dir[temp]==false)
            {
                visit_status_dir[temp]=true;
                for(long long i=0; i<dir_graph[temp].size(); i++)
                {
                    long long temp_dest=dir_graph[temp][i].first;
                    if(visit_status_dir[temp_dest]==false)
                    {
                         double dist_1;
                        if(bfs_src_distances[temp]>dir_graph[temp][i].second)
                        {
                            dist_1=bfs_src_distances[temp];
                        }
                        else
                        {
                            dist_1=dir_graph[temp][i].second;
                        }
                        double dist_2=bfs_src_distances[temp_dest];
                        if(dist_2>dist_1)
                        {
                            pq.push(make_pair(dist_1,temp_dest));
                            bfs_src_distances[temp_dest]=dist_1;
                            bfs_parent[temp_dest]=temp;
                        }
                    }


                }
            }
        }


    }
    void Shortest_path_Least_Edges(long long source)//Complexity Elog(V)
    {
        reset();
        priority_queue <pair<double,long long>,vector<pair<double,long long>>,greater<pair<double,long long>>> pq;
        pq.push(make_pair(0,source));
        for(long long i=0; i<num_of_vertices; i++)
        {
            bfs_visit_count[i]=0;
        }
        bfs_src_distances[source]=0;
        while(!pq.empty())
        {
            long long temp=pq.top().second;
            pq.pop();
            if(visit_status_dir[temp]==false)
            {
                visit_status_dir[temp]=true;
                for(long long i=0; i<dir_graph[temp].size(); i++)
                {
                    long long temp_dest=dir_graph[temp][i].first;
                    if(visit_status_dir[temp_dest]==false)
                    {
                        double dist_1=bfs_src_distances[temp]+dir_graph[temp][i].second;
                        double dist_2=bfs_src_distances[temp_dest];
                        if(dist_2>dist_1)
                        {
                            pq.push(make_pair(dist_1,temp_dest));
                            bfs_src_distances[temp_dest]=dist_1;
                            bfs_parent[temp_dest]=temp;
                            bfs_visit_count[temp_dest]=bfs_visit_count[temp]+1;
                        }
                        if((dist_2==dist_1)&&(bfs_visit_count[temp]+1<bfs_visit_count[temp_dest]))
                        {
                            bfs_parent[temp_dest]=temp;
                            bfs_visit_count[temp_dest]=bfs_visit_count[temp]+1;
                        }
                    }


                }
            }
        }


    }
    void Shortest_path_Generalized(long long source,vector<double> vert_cost)
    {
        reset();
        priority_queue <pair<double,long long>,vector<pair<double,long long>>,greater<pair<double,long long>>> pq;
        bfs_src_distances[source]=vert_cost[source];
        pq.push(make_pair(bfs_src_distances[source],source));
        while(!pq.empty())
        {
            long long temp=pq.top().second;
            pq.pop();
            if(visit_status_dir[temp]==false)
            {
                visit_status_dir[temp]=true;
                for(long long i=0; i<dir_graph[temp].size(); i++)
                {
                    long long temp_dest=dir_graph[temp][i].first;
                    if(visit_status_dir[temp_dest]==false)
                    {
                        double dist_1=bfs_src_distances[temp]+dir_graph[temp][i].second+vert_cost[temp_dest];
                        double dist_2=bfs_src_distances[temp_dest];
                        if(dist_2>dist_1)
                        {
                            pq.push(make_pair(dist_1,temp_dest));
                            bfs_src_distances[temp_dest]=dist_1;
                            bfs_parent[temp_dest]=temp;
                        }
                    }


                }
            }
        }


    }
    bool Shortest_path_BellmanFord(long long source)//Complexity VE
    {
        reset();
        for(long long i=0; i<num_of_vertices; i++)
        {
            bfs_src_distances[i]=inf;
        }
        bfs_src_distances[source]=0;
        for(long long i=0; i<num_of_vertices-1; i++)
        {
            for(long long j=0; j<num_of_edges; j++)
            {
                long long u=List_of_Edges[j].first.first;
                long long v=List_of_Edges[j].first.second;
                double w=List_of_Edges[j].second;
                if(bfs_src_distances[u]+w<bfs_src_distances[v])
                {
                    bfs_src_distances[v]=bfs_src_distances[u]+w;
                    bfs_parent[v]=u;
                }
            }
        }
        for(long long j=0; j<num_of_edges; j++)
        {
            long long u=List_of_Edges[j].first.first;
            long long v=List_of_Edges[j].first.second;
            double w=List_of_Edges[j].second;
            if(bfs_src_distances[u]+w<bfs_src_distances[v])
            {
                cout<<"The graph contains a negative cycle";
                return false;
            }
        }
        cout<<"The graph does not contain a negative cycle";
        return true;
    }
    bool Currency_Trading_BellmanFord(long long source)//Complexity VE
    {
        reset();
        for(long long i=0; i<num_of_vertices; i++)
        {
            bfs_src_distances[i]=inf;
        }
        bfs_src_distances[source]=0;
        for(long long i=0; i<num_of_vertices-1; i++)
        {
            for(long long j=0; j<num_of_edges; j++)
            {
                long long u=List_of_Edges[j].first.first;
                long long v=List_of_Edges[j].first.second;
                double w=List_of_Edges[j].second;
                if(bfs_src_distances[u]+w<bfs_src_distances[v])
                {
                    bfs_src_distances[v]=bfs_src_distances[u]+w;
                    bfs_parent[v]=u;
                }
            }
        }
        for(long long j=0; j<num_of_edges; j++)
        {
            long long u=List_of_Edges[j].first.first;
            long long v=List_of_Edges[j].first.second;
            double w=List_of_Edges[j].second;
            if(bfs_src_distances[u]+w<bfs_src_distances[v])
            {
                cout<<"The graph contains a negative cycle";
                return false;
            }
        }
        cout<<"The graph does not contain a negative cycle";
        return true;
    }
    bool Longest_path_BellmanFord(long long source)//Complexity VE
    {
        reset();
        bfs_src_distances[source]=0;
        for(long long i=0; i<num_of_vertices-1; i++)
        {
            for(long long j=0; j<num_of_edges; j++)
            {
                long long u=List_of_Edges[j].first.first;
                long long v=List_of_Edges[j].first.second;
                double w=-List_of_Edges[j].second;
                if(bfs_src_distances[u]!=INT_MAX && bfs_src_distances[u]+w<bfs_src_distances[v])
                {
                    bfs_src_distances[v]=bfs_src_distances[u]+w;
                    bfs_parent[v]=u;
                }
            }
        }
        for(long long j=0; j<num_of_edges; j++)
        {
            long long u=List_of_Edges[j].first.first;
            long long v=List_of_Edges[j].first.second;
            double w=-List_of_Edges[j].second;
            if(bfs_src_distances[u]!=INT_MAX && bfs_src_distances[u]+w<bfs_src_distances[v])
            {
                cout<<"The graph contains a negative cycle";
                return false;
            }
        }
        cout<<"The graph does not contain a negative cycle";
        return true;
    }

    void trace_path_rec(long long dest)
    {
        if(bfs_parent[dest]==-1)
        {
            cout<<dest;
            return;
        }
        trace_path_rec(bfs_parent[dest]);
        cout<<"->"<<dest;
    }
    void print_path(long long source,long long dest)
    {
        cout<<"Shortest Path Cost:"<<bfs_src_distances[dest]<<endl;
        cout<<"Number of Edges: "<<bfs_visit_count[dest]<<endl;
        cout<<"Path: ";
        trace_path_rec(dest);
        cout<<endl;

    }
    void All_Pair_Shortest_Path_Floyd_Warshall()
    {
        Result_Mat= new double*[num_of_vertices];
        for(long long i=0; i<num_of_vertices; i++)
        {
            Result_Mat[i]=new double[num_of_vertices];
        }
        for(long long i=0; i<num_of_vertices; i++)
        {
            for(long long j=0; j<num_of_vertices; j++)
            {
                Result_Mat[i][j]=Adj_Matt[i][j];
            }
        }

        for(long long k=0; k<num_of_vertices; k++)
        {
            for(long long i=0; i<num_of_vertices; i++)
            {
                for(long long j=0; j<num_of_vertices; j++)
                {
                    if(Result_Mat[i][k]+Result_Mat[k][j]<Result_Mat[i][j])
                    {
                        Result_Mat[i][j]=Result_Mat[i][k]+Result_Mat[k][j];
                        Predecessor[i][j]=Predecessor[i][k];
                    }
                }
            }
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

long long t,u,v,w,s;
cin>>t;
for(long long i=0;i<t;i++)
{   long long vertices,edges;
    cin>>vertices>>edges;
    Graph X(vertices,edges);
   for(long long j=0;j<edges;j++)
   {
    cin>>u>>v>>w;
    X.Add_Edge_Undirected(u,v,w);
   }
   long long s;
   cin>>s;
   X.Shortest_path_City(s);
   cout<<"Case "<<i+1<<":"<<endl;
   for(int i=0;i<vertices;i++)
   {
       if(X.bfs_src_distances[i]==INT_MAX)
       {
           cout<<"Impossible"<<endl;
       }
       else
       {
           cout<<X.bfs_src_distances[i]<<endl;
       }
   }
}
    return 0;
}


