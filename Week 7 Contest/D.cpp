
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
        /*
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
        /*
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
        Adj_Matt[startv][endv]=weight;
        //Predecessor[startv][endv]=endv;

    }
    void Add_Edge_Undirected(long long startv,long long endv,double weight=0)
    {
        dir_graph[startv].push_back(make_pair(endv,weight));
        dir_graph[endv].push_back(make_pair(startv,weight));
        List_of_Edges.push_back(make_pair(make_pair(startv,endv),weight));
        List_of_Edges.push_back(make_pair(make_pair(endv,startv),weight));
        Adj_Matt[startv][endv]=weight;
        Adj_Matt[endv][startv]=weight;
        /*
        Predecessor[startv][endv]=endv;
        Predecessor[endv][startv]=startv;
    */
    }
    void Remove_Edge_Directed(long long startv,long long endv,double weight=0)
    {

        for(long long i=0; i<List_of_Edges.size(); i++)
        {
            if(List_of_Edges[i].first.first==startv&&List_of_Edges[i].first.second==endv&&List_of_Edges[i].second==weight)
            {
                List_of_Edges.erase(List_of_Edges.begin()+i);
            }
        }
        for(long long j=0; j<dir_graph[startv].size(); j++)
        {
            if(dir_graph[startv][j].first==endv&&dir_graph[startv][j].second==weight)
            {
                dir_graph[startv].erase(dir_graph[startv].begin()+j);
            }
        }
        for(long long j=0; j<rev_dir_graph[endv].size(); j++)
        {
            if(rev_dir_graph[endv][j].first==startv&&rev_dir_graph[endv][j].second==weight)
            {
                rev_dir_graph[endv].erase(rev_dir_graph[endv].begin()+j);
            }
        }
     Adj_Matt[startv][endv]=INT_MAX;
       // Predecessor[startv][endv]=-1;

    }
    void Remove_Edge_UnDirected(long long startv,long long endv,long long weight=0)
    {

        for(long long i=0; i<List_of_Edges.size(); i++)
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
        for(long long j=0; j<dir_graph[startv].size(); j++)
        {
            if(dir_graph[startv][j].first==endv&&dir_graph[startv][j].second==weight)
            {
                dir_graph[startv].erase(dir_graph[startv].begin()+j);
            }
        }
        for(long long j=0; j<dir_graph[endv].size(); j++)
        {
            if(dir_graph[endv][j].first==startv&&dir_graph[endv][j].second==weight)
            {
                dir_graph[endv].erase(dir_graph[endv].begin()+j);
            }
        }

        Adj_Matt[startv][endv]=INT_MAX;
        Adj_Matt[endv][startv]=INT_MAX;
       /* Predecessor[startv][endv]=-1;
        Predecessor[endv][startv]=-1;
*/
    }
    double find_cost(long long u,long long v)
    {
        for(long long i=0; i<List_of_Edges.size(); i++)
        {
            if(List_of_Edges[i].first.first==u&&List_of_Edges[i].first.second==v)
            {
                return List_of_Edges[i].second;
            }
        }
        return inf;

    }
    bool check_edge_existence(long long u,long long v,double w)
    {
        for(long long i=0; i<List_of_Edges.size(); i++)
        {
            if(List_of_Edges[i].first.first==u&&List_of_Edges[i].first.second==v&&List_of_Edges[i].second==w)
            {
                return true;
            }
        }
        return false;
    }


    long long bfs(double ** rGraph, long long s, long long t, long long parent[])
{
    // Create a visited array and mark all vertices as not visited
    bool visited[num_of_vertices];
    memset(visited, 0, sizeof(visited));

    // Create a queue, enqueue source vertex and mark source vertex
    // as visited
    queue <long long> q;
    q.push(s);
    visited[s] = true;
    parent[s] = -1;

    // Standard BFS Loop
    while (!q.empty())
    {
        long long u = q.front();
        q.pop();

        for (long long v=0; v<num_of_vertices; v++)
        {
            if (visited[v]==false && rGraph[u][v] > 0)
            {
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }

    // If we reached sink in BFS starting from source, then return
    // true, else false
    return (visited[t] == true);
}

// A DFS based function to find all reachable vertices from s.  The function
// marks visited[i] as true if i is reachable from s.  The initial values in
// visited[] must be false. We can also use BFS to find reachable vertices
void dfs(double ** rGraph, long long s, bool visited[])
{
    visited[s] = true;
    for (long long i = 0; i < num_of_vertices; i++)
       if (rGraph[s][i] && !visited[i])
           dfs(rGraph, i, visited);
}

double max_flow(long long s, long long t)
{
    long long u, v;

    // Create a residual graph and fill the residual graph with
    // given capacities in the original graph as residual capacities
    // in residual graph
    double ** rGraph;// rGraph[i][j] indicates residual capacity of edge i-j
    rGraph=new double*[num_of_vertices];
        for(long long i=0; i<num_of_vertices; i++)
        {
            rGraph[i]=new double[num_of_vertices];
        }
    for (u = 0; u < num_of_vertices; u++)
    {
        for (v = 0; v < num_of_vertices; v++)
             {
                 if(Adj_Matt[u][v]==INT_MAX)
                 {
                     rGraph[u][v]=0;
                 }
                 else
                 {
                     rGraph[u][v]=Adj_Matt[u][v];
                 }
             }
    }

    long long parent[num_of_vertices];  // This array is filled by BFS and to store path
    double max_flow=0;
    // Augment the flow while there is a path from source to sink
    while (bfs(rGraph, s, t, parent))
    {
        // Find minimum residual capacity of the edges along the
        // path filled by BFS. Or we can say find the maximum flow
        // through the path found.
        double path_flow = INT_MAX;
        for (v=t; v!=s; v=parent[v])
        {
            u = parent[v];
            //path_flow = min(path_flow, rGraph[u][v]);
            if(path_flow>rGraph[u][v])
            {
                path_flow=rGraph[u][v];
            }
        }

        // update residual capacities of the edges and reverse edges
        // along the path
        for (v=t; v != s; v=parent[v])
        {
            u = parent[v];
            rGraph[u][v] -= path_flow;
            rGraph[v][u] += path_flow;
        }
        max_flow+=path_flow;
    }
    for(long long i=0; i<num_of_vertices; i++)
        {
            delete[] rGraph[i];
        }
        delete[]rGraph;
    return max_flow;

}
// prints the minimum s-t cut
void minCut(long long s, long long t)
{
    long long u, v;

    // Create a residual graph and fill the residual graph with
    // given capacities in the original graph as residual capacities
    // in residual graph
      double ** rGraph;// rGraph[i][j] indicates residual capacity of edge i-j
    rGraph=new double*[num_of_vertices];
        for(long long i=0; i<num_of_vertices; i++)
        {
            rGraph[i]=new double[num_of_vertices];
        }
    for (u = 0; u < num_of_vertices; u++)
    {
        for (v = 0; v < num_of_vertices; v++)
             {
                 if(Adj_Matt[u][v]==INT_MAX)
                 {
                     rGraph[u][v]=0;
                 }
                 else
                 {
                     rGraph[u][v]=Adj_Matt[u][v];
                 }
             }
    }

    long long parent[num_of_vertices];  // This array is filled by BFS and to store path

    // Augment the flow while there is a path from source to sink
    while (bfs(rGraph, s, t, parent))
    {
        // Find minimum residual capacity of the edges along the
        // path filled by BFS. Or we can say find the maximum flow
        // through the path found.
        long long path_flow = INT_MAX;
        for (v=t; v!=s; v=parent[v])
        {
            u = parent[v];
            if(path_flow>rGraph[u][v])
            {
                path_flow=rGraph[u][v];
            }
            //path_flow = min(path_flow, rGraph[u][v]);
        }

        // update residual capacities of the edges and reverse edges
        // along the path
        for (v=t; v != s; v=parent[v])
        {
            u = parent[v];
            rGraph[u][v] -= path_flow;
            rGraph[v][u] += path_flow;
        }
    }

    // Flow is maximum now, find vertices reachable from s
    bool visited[num_of_vertices];
    memset(visited, false, sizeof(visited));
    dfs(rGraph, s, visited);

    // print all edges that are from a reachable vertex to
    // non-reachable vertex in the original graph
    for (long long i = 0; i < num_of_vertices; i++)
      for (long long j = 0; j < num_of_vertices; j++)
         if (visited[i] && !visited[j] && Adj_Matt[i][j])
              cout << i << " - " << j << endl;
    for(long long i=0; i<num_of_vertices; i++)
        {
            delete[] rGraph[i];
        }
        delete[]rGraph;
       return;
}

bool bpm(bool **bpGraph,long long M,long long N, long long u,
         bool seen[], long long matchR[])
{
    // Try every job one by one
    for (long long v = 0; v < N; v++)
    {
        // If applicant u is long longerested in
        // job v and v is not visited
        if (bpGraph[u][v] && !seen[v])
        {
            // Mark v as visited
            seen[v] = true;

            // If job 'v' is not assigned to an
            // applicant OR previously assigned
            // applicant for job v (which is matchR[v])
            // has an alternate job available.
            // Since v is marked as visited in
            // the above line, matchR[v] in the following
            // recursive call will not get job 'v' again
            if (matchR[v] < 0 || bpm(bpGraph,M,N, matchR[v],
                                     seen, matchR))
            {
                matchR[v] = u;
                return true;
            }
        }
    }
    return false;
}

// Returns maximum number
// of matching from M to N
long long maxBPM(bool **bpGraph,long long M,long long N)
{
    // An array to keep track of the
    // applicants assigned to jobs.
    // The value of matchR[i] is the
    // applicant number assigned to job i,
    // the value -1 indicates nobody is
    // assigned.
    long long matchR[N];

    // Initially all jobs are available
    memset(matchR, -1, sizeof(matchR));

    // Count of jobs assigned to applicants
    long long result = 0;
    for (long long u = 0; u < M; u++)
    {
        // Mark all jobs as not seen
        // for next applicant.
        bool seen[N];
        memset(seen, 0, sizeof(seen));

        // Find if the applicant 'u' can get a job
        if (bpm(bpGraph,M,N, u, seen, matchR))
            result++;
    }
    return result;
}

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

};

int main()
{
    long long t,u,v,w,s;

  long long vertices,edges;
    cin>>vertices>>edges;
    Graph X(vertices,edges);
   for(long long j=0;j<edges;j++)
   {
    cin>>u>>v>>w;
    X.Add_Edge_Directed(u-1,v-1,w);
   }
   cout<<X.max_flow(0,vertices-1);
   return 0;
}


