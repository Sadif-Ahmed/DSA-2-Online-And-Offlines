#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <iterator>
#include <climits>
using namespace std;
class Graph{
    public:
    int num_of_vertices;
    vector<int> *dir_graph;
    vector<int> *rev_dir_graph;
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
    vector<int>bfs_src_distances;
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



    Graph(int num)
    {
        num_of_vertices=num;
        dir_graph= new vector<int>[num_of_vertices+1];
        rev_dir_graph= new vector<int>[num_of_vertices+1];
        bfs_parent.resize(num_of_vertices+1,0);
        dfs_parent.resize(num_of_vertices+1,0);
        bfs_src_distances.resize(num_of_vertices+1,INT_MAX);
        dfs_disc_t.resize(num_of_vertices+1,0);
        dfs_end_t.resize(num_of_vertices+1,0);
        und_cycle_visit.resize(num_of_vertices+1,false);
        bipart.resize(num_of_vertices+1,WHITE);
        visit_status.resize(num_of_vertices+1,WHITE);
        visit_status_dir.resize(num_of_vertices+1,false);
        visit_status_rev_dir.resize(num_of_vertices+1,false);

    }
    void reset()
    {
        bfs_parent.clear();
        dfs_parent.clear();
        bfs_src_distances.clear();
        dfs_disc_t.clear();
        dfs_end_t.clear();
        und_cycle_visit.clear();
        bipart.clear();
        visit_status.clear();
        visit_status_dir.clear();
        visit_status_rev_dir.clear();
        bfs_parent.resize(num_of_vertices+1,0);
        dfs_parent.resize(num_of_vertices+1,0);
        bfs_src_distances.resize(num_of_vertices+1,INT_MAX);
        dfs_disc_t.resize(num_of_vertices+1,0);
        dfs_end_t.resize(num_of_vertices+1,0);
        und_cycle_visit.resize(num_of_vertices+1,false);
        bipart.resize(num_of_vertices+1,WHITE);
        visit_status.resize(num_of_vertices+1,WHITE);
        visit_status_dir.resize(num_of_vertices+1,false);
        visit_status_rev_dir.resize(num_of_vertices+1,false);
    }
    void Add_Edge_Directed(int startv,int endv)
    {
        dir_graph[startv].push_back(endv);
        rev_dir_graph[endv].push_back(startv);
    }
    void Add_Edge_Undirected(int startv,int endv)
    {
        dir_graph[startv].push_back(endv);
        dir_graph[endv].push_back(startv);
    }

    void print()
    {    cout<<"Adjacency List:"<<endl;
        for(int i=1;i<=num_of_vertices;i++)
        {
            cout<<i<<"->"<<" ";
            for(vector<int>::iterator it=dir_graph[i].begin();it!=dir_graph[i].end();it++)
            {
                cout<<*it<<" ";
            }
            cout<<endl;
        }
    }
    void print_BFS(int root)
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

            for(vector<int>::iterator it=dir_graph[temp].begin();it!=dir_graph[temp].end();it++)
            {
                if(visit_status[*it]==WHITE)
                {
                    visit_status[*it]=GRAY;
                    bfs_src_distances[*it]=bfs_src_distances[root]+1;
                    bfs_parent[*it]=temp;
                    bfs_queue.push(*it);
                }
            }
            visit_status[temp]=BLACK;
        }

    }
    void print_DFS(int root)
    {
        dfs_time++;
        dfs_disc_t[root]=dfs_time;
        visit_status[root]=GRAY;
        cout<<root<<" ";
        for(vector<int>::iterator it=dir_graph[root].begin();it!=dir_graph[root].end();it++)
            {
                if(visit_status[*it]==WHITE)
                {

                    dfs_parent[*it]=root;
                    print_DFS(*it);
                }
            }
             visit_status[root]=BLACK;

             dfs_time++;
             dfs_end_t[root]=dfs_time;

    }
    void Traverse_Choice(char c)
    {
        reset();
        for(int i=1;i<=num_of_vertices;i++)
        {
            if(visit_status[i]==WHITE)
            {
                if(c=='B')
                {
                    print_BFS(i);
                    cout<<endl;
                }
                else if(c=='D')
                {
                    print_DFS(i);
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
        for(vector<int>::iterator it=dir_graph[vert].begin();it!=dir_graph[vert].end();it++)
        {
            if(visit_status[*it]==GRAY)
            {
                return true;
            }
            if((visit_status[*it]==WHITE)&&(pre_cycle_check_Dir(*it)))
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
        for(vector<int>::iterator it=dir_graph[vert].begin();it!=dir_graph[vert].end();it++)
        {
            if(!(und_cycle_visit[*it]))
            {
                 if(pre_cycle_check_Undir(*it,vert))
                {
                return true;
                }
            }
            else if(*it!=parent)
            {
                return true;
            }

            }
            return false;
        }
    bool dir_cycle_check()
    {
        reset();
        for(int i=1;i<=num_of_vertices;i++)
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
        for(int i=1;i<=num_of_vertices;i++)
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
        for(vector<int>::iterator it=dir_graph[vert].begin();it!=dir_graph[vert].end();it++)
        {
            if(visit_status[*it]!=BLACK)
            {
                pre_topological_sort(*it);
            }
        }
        reverse_topological.push(vert);

    }
    void topological_sort()
    {
        reset();
        for(int i=1;i<=num_of_vertices;i++)
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
        for(vector<int>::iterator it=dir_graph[root].begin();it!=dir_graph[root].end();it++)
        {
            if(!visit_status_dir[*it])
            {
                dir_dfs(*it);
            }
        }
    }
    void rev_dir_dfs(int root)
    {
        visit_status_rev_dir[root]=true;
        for(vector<int>::iterator it=rev_dir_graph[root].begin();it!=rev_dir_graph[root].end();it++)
        {
            if(!visit_status_rev_dir[*it])
            {
                rev_dir_dfs(*it);
            }
        }
    }
    bool undir_connected()
    {
        reset();
        dir_dfs(1);
        rev_dir_dfs(1);
        for(int i=1;i<=num_of_vertices;i++)
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
       for(int i=1;i<=num_of_vertices&&bipart[i]==WHITE;i++)
       {
           temp.push(i);
           bipart[i]=RED;
           while(!temp.empty())
           {
               int root=temp.front();
               temp.pop();
               for(vector<int>::iterator it=dir_graph[root].begin();it!=dir_graph[root].end();it++)
               {
                   if(bipart[*it]==bipart[root])
                   {
                       return false;
                   }
                   if(bipart[*it]==WHITE)
                   {
                       if(bipart[root]==RED)
                       {
                           bipart[*it]=BLUE;
                       }
                       else
                       {
                           bipart[*it]=RED;
                       }
                       temp.push(*it);
                   }
               }
           }
           return true;
           }
    }
};
int main()
{
    int num_v,num_edge;
    cin>>num_v>>num_edge;
    Graph X(num_v);
   for(int i=1;i<=num_edge;i++)
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

}

