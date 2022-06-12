
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <iterator>
#include <climits>
#include<bits/stdc++.h>
#include<cmath>
using namespace std;
typedef pair<int,double> dest_weight;
typedef pair<int,int> vertice;
#define inf 9999999.9999999
class Graph
{
public:
    int num_of_vertices;
    int num_of_edges;
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
    vector<int>bfs_parent;
    vector<int>dfs_parent;
    vector<double>bfs_src_distances;
    vector<int>bfs_visit_count;
    int dfs_time=0;
    vector<int>dfs_disc_t;
    vector<int>dfs_end_t;
    stack<int> reverse_topological;
    queue<int> bfs_queue;
    vector<bool>und_cycle_visit;
    vector<colour>bipart;
    vector<colour>visit_status;
    vector<bool>visit_status_dir;
    vector<bool>visit_status_rev_dir;
    //MST Elements
    vector<int> sources;
    vector<pair<vertice,double>> List_of_Edges;//Storing the edges
    vector<pair<vertice,double>> Kruskal_MST_Edges;//Storing the mst
    double cost_k;
    double cost_p;
    vector<double>key;//Vector of Weights
    vector<int> parent;//Prim's MST parent List
    vector<bool>mst_status;//Prim's node in MST checker

    double **Adj_Matt;
    double **Result_Mat;

    Graph(int num_v,int num_e)
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
        for(int i=0; i<num_of_vertices; i++)
        {
            sources[i]=i;
        }
        List_of_Edges.clear();
        Kruskal_MST_Edges.clear();
        key.resize(num_of_vertices,inf);
        parent.resize(num_of_vertices,-1);
        mst_status.resize(num_of_vertices,false);

        Adj_Matt=new double*[num_of_vertices];
        for(int i=0; i<num_of_vertices; i++)
        {
            Adj_Matt[i]=new double[num_of_vertices];
        }
        for(int i=0;i<num_of_vertices;i++)
        {
            for(int j=0;j<num_of_vertices;j++)
            {
                if(i==j)
                {
                    Adj_Matt[i][j]=0;
                }
                else{
                    Adj_Matt[i][j]=INT_MAX;
                }
            }
        }


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
        for(int i=0; i<num_of_vertices; i++)
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
    vector<int> get_parent()
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
    void Add_Edge_Directed(int startv,int endv,double weight=0)
    {
        dir_graph[startv].push_back(make_pair(endv,weight));
        rev_dir_graph[endv].push_back(make_pair(startv,weight));
        List_of_Edges.push_back(make_pair(make_pair(startv,endv),weight));
        Adj_Matt[startv][endv]=weight;

    }
    void Add_Edge_Undirected(int startv,int endv,double weight=0)
    {
        dir_graph[startv].push_back(make_pair(endv,weight));
        dir_graph[endv].push_back(make_pair(startv,weight));
        List_of_Edges.push_back(make_pair(make_pair(startv,endv),weight));
        List_of_Edges.push_back(make_pair(make_pair(endv,startv),weight));
        Adj_Matt[startv][endv]=weight;
        Adj_Matt[endv][startv]=weight;
    }
    void Remove_Edge_Directed(int startv,int endv,double weight=0)
    {

        for(int i=0; i<List_of_Edges.size(); i++)
        {
            if(List_of_Edges[i].first.first==startv&&List_of_Edges[i].first.second==endv&&List_of_Edges[i].second==weight)
            {
                List_of_Edges.erase(List_of_Edges.begin()+i);
            }
        }
        for(int j=0; j<dir_graph[startv].size(); j++)
        {
            if(dir_graph[startv][j].first==endv&&dir_graph[startv][j].second==weight)
            {
                dir_graph[startv].erase(dir_graph[startv].begin()+j);
            }
        }
        for(int j=0; j<rev_dir_graph[endv].size(); j++)
        {
            if(rev_dir_graph[endv][j].first==startv&&rev_dir_graph[endv][j].second==weight)
            {
                rev_dir_graph[endv].erase(rev_dir_graph[endv].begin()+j);
            }
        }
        Adj_Matt[startv][endv]=INT_MAX;

    }
    void Remove_Edge_UnDirected(int startv,int endv,int weight=0)
    {

        for(int i=0; i<List_of_Edges.size(); i++)
        {
            if(List_of_Edges[i].first.first==startv&&List_of_Edges[i].first.second==endv&&List_of_Edges[i].second==weight)
            {
                List_of_Edges.erase(List_of_Edges.begin()+i);
            }
            if(List_of_Edges[i].first.first==startv&&List_of_Edges[i].first.second==endv&&List_of_Edges[i].second==weight)
            {
                List_of_Edges.erase(List_of_Edges.begin()+i);
            }
        }
        for(int j=0; j<dir_graph[startv].size(); j++)
        {
            if(dir_graph[startv][j].first==endv&&dir_graph[startv][j].second==weight)
            {
                dir_graph[startv].erase(dir_graph[startv].begin()+j);
            }
        }
        for(int j=0; j<dir_graph[endv].size(); j++)
        {
            if(dir_graph[endv][j].first==startv&&dir_graph[endv][j].second==weight)
            {
                dir_graph[endv].erase(dir_graph[endv].begin()+j);
            }
        }
        Adj_Matt[startv][endv]=INT_MAX;
        Adj_Matt[endv][startv]=INT_MAX;

    }
    double find_cost(int u,int v)
    {
        for(int i=0; i<List_of_Edges.size(); i++)
        {
            if(List_of_Edges[i].first.first==u&&List_of_Edges[i].first.second==v)
            {
                return List_of_Edges[i].second;
            }
        }
        return inf;

    }
    bool check_edge_existence(int u,int v,double w)
    {
        for(int i=0; i<List_of_Edges.size(); i++)
        {
            if(List_of_Edges[i].first.first==u&&List_of_Edges[i].first.second==v&&List_of_Edges[i].second==w)
            {
                return true;
            }
        }
        return false;
    }

    void print_AdjL()
    {
        cout<<"Adjacency List:"<<endl;
        for(int i=0; i<num_of_vertices; i++)
        {
            cout<<i<<"->"<<" ";
            for(vector<dest_weight>::iterator it=dir_graph[i].begin(); it!=dir_graph[i].end(); it++)
            {
                cout<<(*it).first<<"(w= "<<(*it).second<<" )";
            }
            cout<<endl;
        }
    }
    void print_AdjM()
    {
        cout<<"Adjacency Matrix"<<endl;
        for(int i=0;i<num_of_vertices;i++)
        {
            for(int j=0;j<num_of_vertices;j++)
            {
               if(Adj_Matt[i][j]==INT_MAX)
                {
                    cout<<"INF  ";
                }
                else
                {
                cout<<Adj_Matt[i][j]<<"  ";
                }
            }
            cout<<endl;
        }
    }
    void BFS_Unweighted(int root)
    {
        visit_status[root]=GRAY;
        bfs_src_distances[root]=0;
        bfs_parent[root]=0;

        bfs_queue.push(root);
        while(!bfs_queue.empty())
        {
            int temp=bfs_queue.front();
            bfs_queue.pop();
            cout<<temp<<" ";

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
    void DFS_Unweighted(int root)
    {
        dfs_time++;
        dfs_disc_t[root]=dfs_time;
        visit_status[root]=GRAY;
        cout<<root<<" ";
        for(vector<dest_weight>::iterator it=dir_graph[root].begin(); it!=dir_graph[root].end(); it++)
        {
            if(visit_status[(*it).first]==WHITE)
            {

                dfs_parent[(*it).first]=root;
                DFS_Unweighted((*it).first);
            }
        }
        visit_status[root]=BLACK;

        dfs_time++;
        dfs_end_t[root]=dfs_time;

    }
    void Traverse_Choice(char c)
    {
        reset();
        for(int i=0; i<num_of_vertices; i++)
        {
            if(visit_status[i]==WHITE)
            {
                if(c=='B')
                {
                    BFS_Unweighted(i);
                    cout<<endl;
                }
                else if(c=='D')
                {
                    DFS_Unweighted(i);
                    cout<<endl;
                }
                else
                {
                    cout<<"Invalid Choice"<<endl;
                }
            }
        }
    }
    bool pre_cycle_check_Dir(int vert)
    {
        visit_status[vert]=GRAY;
        for(vector<dest_weight>::iterator it=dir_graph[vert].begin(); it!=dir_graph[vert].end(); it++)
        {
            if(visit_status[(*it).first]==GRAY)
            {
                return true;
            }
            if((visit_status[(*it).first]==WHITE)&&(pre_cycle_check_Dir((*it).first)))
            {
                return true;
            }
        }
        visit_status[vert]=BLACK;
        return false;
    }
    bool pre_cycle_check_Undir(int vert,int parent)
    {
        und_cycle_visit[vert]=true;
        for(vector<dest_weight>::iterator it=dir_graph[vert].begin(); it!=dir_graph[vert].end(); it++)
        {
            if(!(und_cycle_visit[(*it).first]))
            {
                if(pre_cycle_check_Undir((*it).first,vert))
                {
                    return true;
                }
            }
            else if((*it).first!=parent)
            {
                return true;
            }

        }
        return false;
    }
    bool dir_cycle_check()
    {
        reset();
        for(int i=0; i<num_of_vertices; i++)
        {
            if(visit_status[i]==WHITE&&pre_cycle_check_Dir(i))
            {
                return true;
            }
        }
        return false;

    }
    bool undir_cycle_check()
    {
        reset();
        for(int i=0; i<num_of_vertices; i++)
        {
            if(!und_cycle_visit[i])
            {
                if(pre_cycle_check_Undir(i,-1))
                {
                    return true;
                }
            }
        }
        return false;
    }
    void pre_topological_sort(int vert)
    {
        visit_status[vert]=BLACK;
        for(vector<dest_weight>::iterator it=dir_graph[vert].begin(); it!=dir_graph[vert].end(); it++)
        {
            if(visit_status[(*it).first]!=BLACK)
            {
                pre_topological_sort((*it).first);
            }
        }
        reverse_topological.push(vert);

    }
    void topological_sort()
    {
        reset();
        for(int i=0; i<num_of_vertices; i++)
        {
            if(visit_status[i]!=BLACK)
            {
                pre_topological_sort(i);
            }
        }
        while(!reverse_topological.empty())
        {
            cout<<reverse_topological.top()<<"  ";
            reverse_topological.pop();
        }
    }
    void dir_dfs(int root)
    {
        visit_status_dir[root]=true;
        for(vector<dest_weight>::iterator it=dir_graph[root].begin(); it!=dir_graph[root].end(); it++)
        {
            if(!visit_status_dir[(*it).first])
            {
                dir_dfs((*it).first);
            }
        }
    }
    void rev_dir_dfs(int root)
    {
        visit_status_rev_dir[root]=true;
        for(vector<dest_weight>::iterator it=rev_dir_graph[root].begin(); it!=rev_dir_graph[root].end(); it++)
        {
            if(!visit_status_rev_dir[(*it).first])
            {
                rev_dir_dfs((*it).first);
            }
        }
    }
    bool undir_connected()
    {
        reset();
        dir_dfs(0);
        rev_dir_dfs(0);
        for(int i=1; i<=num_of_vertices; i++)
        {
            if(visit_status_dir[i]==false && visit_status_rev_dir[i]==false)
            {
                return false;
            }
        }
        return true;
    }
    void undir_connected_components()
    {
        Traverse_Choice('D');
    }
    bool bipartite_check()
    {
        reset();
        queue<int> temp;
        for(int i=0; i<num_of_vertices&&bipart[i]==WHITE; i++)
        {
            temp.push(i);
            bipart[i]=RED;
            while(!temp.empty())
            {
                int root=temp.front();
                temp.pop();
                for(vector<dest_weight>::iterator it=dir_graph[root].begin(); it!=dir_graph[root].end(); it++)
                {
                    if(bipart[(*it).first]==bipart[root])
                    {
                        return false;
                    }
                    if(bipart[(*it).first]==WHITE)
                    {
                        if(bipart[root]==RED)
                        {
                            bipart[(*it).first]=BLUE;
                        }
                        else
                        {
                            bipart[(*it).first]=RED;
                        }
                        temp.push((*it).first);
                    }
                }
            }
            return true;
        }
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
                if(List_of_Edges[i].second>=List_of_Edges[j].second)
                {
                    pair<vertice,double> temp;
                    temp=List_of_Edges[i];
                    List_of_Edges[i]=List_of_Edges[j];
                    List_of_Edges[j]=temp;
                }
            }
        }
    }
    void sort_by_edges_r()//Order of E(square)
    {
        for(int i=0; i<num_of_edges; i++)
        {
            for(int j=i+1; j<num_of_edges; j++)
            {
                if(List_of_Edges[i].second<=List_of_Edges[j].second)
                {
                    pair<vertice,double> temp;
                    temp=List_of_Edges[i];
                    List_of_Edges[i]=List_of_Edges[j];
                    List_of_Edges[j]=temp;
                }
            }
        }
    }
    void mst_kruskal()//Order of Elog(E) if sort stl used.
    //As my manual sort takes E(square) so the algo becomes of order E(square)
    {
        int i,temp_u,temp_v;
        sort_by_edges();
        for(int i=0; i<List_of_Edges.size(); i++)
        {
            temp_u=find_set(List_of_Edges[i].first.first);
            temp_v=find_set(List_of_Edges[i].first.second);
            if(temp_u!=temp_v)
            {
                Kruskal_MST_Edges.push_back(List_of_Edges[i]);
                union_set(temp_u,temp_v);
                cost_k+=List_of_Edges[i].second;
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

            for(vector<dest_weight>::iterator it=dir_graph[temp_u].begin(); it!=dir_graph[temp_u].end(); it++)
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
    void Shortest_path_Djakstra(int source)//Complexity Elog(V)
    {
        reset();
        priority_queue <pair<double,int>,vector<pair<double,int>>,greater<pair<double,int>>> pq;
        pq.push(make_pair(0,source));
        bfs_src_distances[source]=0;
        while(!pq.empty())
        {
            int temp=pq.top().second;
            pq.pop();
            if(visit_status_dir[temp]==false)
            {
                visit_status_dir[temp]=true;
                for(int i=0; i<dir_graph[temp].size(); i++)
                {
                    int temp_dest=dir_graph[temp][i].first;
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
    void Shortest_path_Least_Edges(int source)//Complexity Elog(V)
    {
        reset();
        priority_queue <pair<double,int>,vector<pair<double,int>>,greater<pair<double,int>>> pq;
        pq.push(make_pair(0,source));
        for(int i=0;i<num_of_vertices;i++)
        {
            bfs_visit_count[i]=0;
        }
        bfs_src_distances[source]=0;
        while(!pq.empty())
        {
            int temp=pq.top().second;
            pq.pop();
            if(visit_status_dir[temp]==false)
            {
                visit_status_dir[temp]=true;
                for(int i=0; i<dir_graph[temp].size(); i++)
                {
                    int temp_dest=dir_graph[temp][i].first;
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
    void Shortest_path_Generalized(int source,vector<double> vert_cost)
    {
        reset();
        priority_queue <pair<double,int>,vector<pair<double,int>>,greater<pair<double,int>>> pq;
        bfs_src_distances[source]=vert_cost[source];
        pq.push(make_pair(bfs_src_distances[source],source));
        while(!pq.empty())
        {
            int temp=pq.top().second;
            pq.pop();
            if(visit_status_dir[temp]==false)
            {
                visit_status_dir[temp]=true;
                for(int i=0; i<dir_graph[temp].size(); i++)
                {
                    int temp_dest=dir_graph[temp][i].first;
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
    bool Shortest_path_BellmanFord(int source)//Complexity VE
    {
        reset();
        for(int i=0; i<num_of_vertices; i++)
        {
            bfs_src_distances[i]=inf;
        }
        bfs_src_distances[source]=0;
        for(int i=0; i<num_of_vertices-1; i++)
        {
            for(int j=0; j<num_of_edges; j++)
            {
                int u=List_of_Edges[j].first.first;
                int v=List_of_Edges[j].first.second;
                double w=List_of_Edges[j].second;
                if(bfs_src_distances[u]+w<bfs_src_distances[v])
                {
                    bfs_src_distances[v]=bfs_src_distances[u]+w;
                    bfs_parent[v]=u;
                }
            }
        }
        for(int j=0; j<num_of_edges; j++)
        {
            int u=List_of_Edges[j].first.first;
            int v=List_of_Edges[j].first.second;
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
    bool Currency_Trading_BellmanFord(int source)//Complexity VE
    {
        reset();
        for(int i=0; i<num_of_vertices; i++)
        {
            bfs_src_distances[i]=inf;
        }
        bfs_src_distances[source]=0;
        for(int i=0; i<num_of_vertices-1; i++)
        {
            for(int j=0; j<num_of_edges; j++)
            {
                int u=List_of_Edges[j].first.first;
                int v=List_of_Edges[j].first.second;
                double w=List_of_Edges[j].second;
                if(bfs_src_distances[u]+w<bfs_src_distances[v])
                {
                    bfs_src_distances[v]=bfs_src_distances[u]+w;
                    bfs_parent[v]=u;
                }
            }
        }
        for(int j=0; j<num_of_edges; j++)
        {
            int u=List_of_Edges[j].first.first;
            int v=List_of_Edges[j].first.second;
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
    bool Longest_path_BellmanFord(int source)//Complexity VE
    {
        reset();
        bfs_src_distances[source]=0;
        for(int i=0; i<num_of_vertices-1; i++)
        {
            for(int j=0; j<num_of_edges; j++)
            {
                int u=List_of_Edges[j].first.first;
                int v=List_of_Edges[j].first.second;
                double w=-List_of_Edges[j].second;
                if(bfs_src_distances[u]!=INT_MAX && bfs_src_distances[u]+w<bfs_src_distances[v])
                {
                    bfs_src_distances[v]=bfs_src_distances[u]+w;
                    bfs_parent[v]=u;
                }
            }
        }
        for(int j=0; j<num_of_edges; j++)
        {
            int u=List_of_Edges[j].first.first;
            int v=List_of_Edges[j].first.second;
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

    void trace_path_rec(int dest)
    {
        if(bfs_parent[dest]==-1)
        {
            cout<<dest;
            return;
        }
        trace_path_rec(bfs_parent[dest]);
        cout<<"->"<<dest;
    }
    void print_path(int source,int dest)
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
        for(int i=0;i<num_of_vertices;i++)
        {
            Result_Mat[i]=new double[num_of_vertices];
        }
        for(int i=0;i<num_of_vertices;i++)
        {
            for(int j=0;j<num_of_vertices;j++)
            {
                Result_Mat[i][j]=Adj_Matt[i][j];
            }
        }

        for(int k=0;k<num_of_vertices;k++)
        {
            for(int i=0;i<num_of_vertices;i++)
            {
                for(int j=0;j<num_of_vertices;j++)
                {
                    if(Result_Mat[i][k]+Result_Mat[k][j]<Result_Mat[i][j])
                    {
                        Result_Mat[i][j]=Result_Mat[i][k]+Result_Mat[k][j];
                    }
                }
            }
        }
    }
    double ** Shortest_Path_Extention(double **mat_A,double **mat_B)
    {
        double **Temp_Mat= new double*[num_of_vertices];
        for(int i=0;i<num_of_vertices;i++)
        {
            Temp_Mat[i]=new double[num_of_vertices];
        }
        for(int i=0;i<num_of_vertices;i++)
        {
            for(int j=0;j<num_of_vertices;j++)
            {
                Temp_Mat[i][j]=INT_MAX;
                for(int k=0;k<num_of_vertices;k++)
                {
                    if(mat_A[i][k]+mat_B[k][j]<Temp_Mat[i][j])
                    {
                        Temp_Mat[i][j]=mat_A[i][k]+mat_B[k][j];
                    }
                }
            }

        }
        return Temp_Mat;
    }
    void All_Pair_Shortest_Path_Matrix_Multiplication()
    {
         Result_Mat= new double*[num_of_vertices];
        for(int i=0;i<num_of_vertices;i++)
        {
            Result_Mat[i]=new double[num_of_vertices];
        }
        for(int i=0;i<num_of_vertices;i++)
        {
            for(int j=0;j<num_of_vertices;j++)
            {
                Result_Mat[i][j]=Adj_Matt[i][j];
            }
        }
        for(int i=1;i<num_of_vertices;i++)
        {
            Result_Mat=Shortest_Path_Extention(Result_Mat,Adj_Matt);
        }
    }
    ~Graph()
    {
        for(int i=0;i<num_of_vertices;i++)
        {
            delete[] Result_Mat[i];
            delete[] Adj_Matt[i];
        }
        delete[]Result_Mat;
        delete[]Adj_Matt;
    }
};
void Print_Edges(vector<pair<vertice,double>> test)//Order of V
{

    cout<<"Edges"<<"   "<<"Weight"<<endl;
    for(int i=0; i<test.size(); i++)
    {
        cout<<test[i].first.first<<" -- "<<test[i].first.second<<"  "<<test[i].second<<endl;
    }
}
void Print_Edges(vector<int> arr,vector<double> key,int num_of_vertices)//Order of V
{

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
    {
        if(check==false)
        {
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
            for(int i=1; i<num_of_vertices; i++)
            {
                bool present=false;
                for(int j=0; j<dest.size(); j++)
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


void print_edge_Set(vector<pair<vertice,double>> test)//Order of V
{

    cout<<"List of Edges Selected By Kruskal's:";
    cout<<"{";
    for(int i=0; i<test.size(); i++)
    {
        if(i!=0)
        {
            cout<<",";
        }
        cout<<"("<<test[i].first.first<<","<<test[i].first.second<<")";
    }
    cout<<"}"<<endl;
}
void print_APSP(double **mat,int num_of_vertices)
{
    cout<<"Shortest Distance Mattrix:"<<endl;
     for(int i=0;i<num_of_vertices;i++)
        {
            for(int j=0;j<num_of_vertices;j++)
            {
                if(mat[i][j]==INT_MAX)
                {
                    cout<<"INF  ";
                }
                else
                {
                cout<<mat[i][j]<<"  ";
                }
            }
            cout<<endl;
        }

}
int main()
{
/*
    int num_v,num_edge;
    cin>>num_v>>num_edge;
    Graph X(num_v,num_edge);
   for(int i=0;i<num_edge;i++)
   {   int u,v;
       cin>>u>>v;
       X.Add_Edge_Directed(u,v);
   }
   X.print();
   if(X.dir_cycle_check()==1)
   {
       cout<<"Not Possible";
   }
   else
   {
       cout<<"Possible"<<endl;
       X.topological_sort();
   }
*/
 /*   int num_of_vertices,num_of_edges,u,v;
    double weight;
    cin>>num_of_vertices>>num_of_edges;
    Graph X(num_of_vertices,num_of_edges);
    for(int i=0;i<num_of_edges;i++)
    {
        cin>>u>>v>>weight;
        X.Add_Edge_Undirected(u,v,weight);
    }
    X.mst_kruskal();
    X.mst_prims();
    cout<<"Cost of minimum spanning tree:"<<X.get_cost('p')<<endl;
    //Print_Edges(X.get_mst());
    //Print_Edges(X.get_parent(),X.get_weight(),num_of_vertices);
    print_edge_Set(X.get_parent(),num_of_vertices);
    print_edge_Set(X.get_mst());
    */

    /*int num_of_vertices,num_of_edges,u,v;
    double weight;
    cin>>num_of_vertices>>num_of_edges;
    Graph X(num_of_vertices,num_of_edges);
    /*
    vector<double> vert_cost;
    vert_cost.resize(num_of_vertices,0);
    for(int i=0;i<num_of_vertices;i++)
    {   int vert;
        double cst;
        cin>>vert>>cst;
        vert_cost[vert]=cst;
    }
*//*
    for(int i=0;i<num_of_edges;i++)
    {
        cin>>u>>v>>weight;
        weight=-log(weight);
        X.Add_Edge_Directed(u,v,weight);
    }
    int src,dest;
    cin>>src>>dest;
//    X.Shortest_path_Generalized(src,vert_cost);
   // X.print_path(src,dest);
    //X.print();
    //X.Shortest_path_Djakstra(src);

   if(X.Currency_Trading_BellmanFord(src))
    {
    X.print_path(src,dest);
    }


    /*
if(X.Longest_path_BellmanFord(src))
    {
    X.print_path(src,dest);
    }
    */
    /*
    int num_of_vertices,num_of_edges,u,v;
    double weight;
    cin>>num_of_vertices>>num_of_edges;
    Graph X(num_of_vertices,num_of_edges);

    for(int i=0; i<num_of_edges; i++)
    {
        cin>>u>>v>>weight;
        X.Add_Edge_Undirected(u,v,weight);
    }
    int src;
    cin>>src;
    X.Shortest_path_Least_Edges(src);
    for(int i=0;i<num_of_vertices;i++)
    {
        X.print_path(src,i);
    }
    */
    int num_of_vertices,num_of_edges,u,v;
    double weight;
    cin>>num_of_vertices>>num_of_edges;
    Graph X(num_of_vertices,num_of_edges);
   for(int i=0; i<num_of_edges; i++)
    {
        cin>>u>>v>>weight;
        X.Add_Edge_Directed(u-1,v-1,weight);
    }
    X.print_AdjM();
    X.All_Pair_Shortest_Path_Matrix_Multiplication();
    print_APSP(X.get_APSP(),num_of_vertices);
    return 0;
}
