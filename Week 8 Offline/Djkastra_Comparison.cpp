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
#include<unordered_map>
#include<cstdio>
#include<ctime>
#include<chrono>
using namespace std;
using namespace std::chrono;
typedef pair<int,double> dest_weight;
typedef pair<int,int> vertice;
#define inf 9999999.9999999
template<class T>struct Node
{
    bool mark;	      // Whether the node has lost a child since
    // it was made child of another node.
    int index;	      // Index of the vertex
    T value;      	      // Value of the node
    int deg;           // Degree of the node
    Node<T> *parent;         // Parent Pointer
    Node<T> *left, *right;   // Siblings
    Node<T> *child;	      // Child Pointer
};

template<class T>Node<T>* createnode(T key,int index)
{
    Node<T> *newnode = new Node<T>;
    newnode->index = index;
    newnode->value = key;
    newnode->parent = newnode->child = NULL;
    newnode->left = newnode->right = newnode;
    newnode->deg = 0;
    newnode->mark = false;
    return newnode;
}
template<class T>class Fib_heap
{
    Node<T> *head;
    int num_of_nodes;
    unordered_map<int,Node<T>*>found;
public:
    Fib_heap()
    {
        head = NULL;
        num_of_nodes = 0;
    }
    // Search for a node
    Node<T>* fib_heap_search(T key)
    {
        if(found.find(key)==found.end())
            return NULL;
        return found[key];
    }
    // Insertion
    void fib_heap_insert(T key,int index)
    {
        Node<T> *x = createnode<T>(key,index);
        found[index] = x;
        if(head==NULL)
            head = x;
        else
        {
            // Insert new node to root list
            Node<T> *l1 = head->left;
            Node<T> *l2 = x->left;
            l1->right = x;
            x->left = l1;
            l2->right = head;
            head->left = l2;
            // Head Update
            if(head->value > x->value)
                head = x;
        }
        num_of_nodes += 1;
    }
    //Create heap function
    void create_heap(int vertices,int src)
    {
        int index = 0;
        while(index<vertices)
        {
            double key;
            if(index==src)
                key = 0;
            else key = INT_MAX;
            fib_heap_insert(key,index);
            index++;
        }
    }
    // Extract Min
    Node<T>* fib_heap_extract_min()
    {
        if(head==NULL)
        {
            cout << "Error: Empty fibonacci heap\n";
            return NULL;
        }
        Node<T> *z = head;
        found.erase(head->index);
        if(z!=NULL)
        {
            // Adding its children to root list
            Node<T> *ch = z->child;
            Node<T> *ch_copy = ch;
            do
            {
                if(ch==NULL)
                    break;
                Node<T> *l1 = head->left;
                l1->right = ch;
                ch->left = l1;
                Node<T> *chc = ch->right;
                ch->right = head;
                head->left = ch;
                ch->parent = NULL;
                ch = chc;
            }
            while(ch!=ch_copy);
            // Remove z from the root list
            Node<T> *l = z->left, *r = z->right;
            l->right = r;
            r->left = l;
            if(z==z->right)
                head = NULL;
            else
            {
                head = z->right;
                consolidate();
            }
            num_of_nodes--;
        }
        if(z!=NULL)
        {
            z->child = NULL;
            z->left = z->right = z;
        }
        return z;
    }
    // Consolidation of root list
    void consolidate()
    {
        int max_deg = floor(log2(num_of_nodes));
        Node<T>* A[max_deg+1];
        for(int i=0; i<=max_deg; i++)
            A[i] = NULL;
        Node<T> *w = head;
        unordered_map<Node<T>*,bool>mp;
        do
        {
            Node<T> *x = w;
            Node<T> *r = w->right;
            mp[w] = 1;
            int d = x->deg;
            while(A[d]!=NULL and d<=max_deg)
            {
                Node<T> *y = A[d];
                if(x->value>y->value)
                    swap(x,y);
                fib_heap_link(y,x);
                A[d] = NULL;
                d = d+1;
            }
            A[d] = x;
            A[d]->left = A[d]->right = A[d];
            w = r;
        }
        while(mp.find(w)==mp.end());
        head = NULL;
        for(int i=0; i<=max_deg; i++)
        {
            if(A[i]!=NULL)
            {
                if(head==NULL)
                    head = A[i];
                else
                {
                    Node<T> *l1 = head->left;
                    l1->right = A[i];
                    A[i]->left = l1;
                    A[i]->right = head;
                    head->left = A[i];
                    // Head Update
                    if(head->value > A[i]->value)
                        head = A[i];
                }
            }
        }
    }
    // Linking
    void fib_heap_link(Node<T> *y,Node<T> *x)
    {
        Node<T> *l = y->left, *r = y->right;
        l->right = r;
        r->left = l;
        x->deg++;
        if(x->child==NULL)
        {
            x->child = y;
            y->left = y->right = y;
        }
        else
        {
            Node<T> *r = x->child->right;
            x->child->right = y;
            y->left = x->child;
            y->right = r;
            r->left = y;
        }
        y->parent = x;
        if(y->mark==true)
        {
            y->mark = false;
        }
    }
    // Helper Function: Cut
    void cut(Node<T> *x,Node<T> *y)
    {
        // Remove node from the child list
        Node<T> *child = y->child;
        if(child->left==child and child->right==child)
            y->child = NULL;
        else
        {
            Node<T> *l = x->left;
            Node<T> *r = x->right;
            l->right = r;
            r->left  = l;
            if(y->child==x)
                y->child = l;
        }
        y->deg--;
        // Adding x to the root list
        Node<T> *l = head->left;
        l->right = x;
        x->left  = l;
        x->right = head;
        head->left = x;
        x->parent = NULL;
        x->mark = false;
    }
    // Helper Function: Cascading-Cut
    void cascading_cut(Node<T> *y)
    {
        Node<T> *z = y->parent;
        if(z!=NULL)
        {
            if(y->mark==false)
                y->mark = true;
            else
            {
                cut(y,z);
                cascading_cut(z);
            }
        }
    }
    // Decrease Key
    void fib_heap_decrease_key(T x_key,T k)
    {
        Node<T> *x = fib_heap_search(x_key);
        Node<T> *y = x->parent;
        x->value = k;
        if(x->parent!=NULL and x->value < y->value)
        {
            cut(x,y);
            cascading_cut(y);
        }
        if(x->value<head->value)
            head = x;
    }
    bool is_empty()
    {
        if(num_of_nodes==0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};
template<class T>class Bin_heap
{
private:
    pair<int,T> * holder;
    int max_size;
    int curr_size;
    //left child
     pair<int,T> left(int index)
    {
        return holder[2*index+1];
    }
    //right child
    pair<int,T> right(int index)
    {
        return holder[2*index+2];
    }
    //parent
    pair<int,T> parent(int index)
    {
        return holder[(index-1)/2];
    }
    //shifting up for insert
    void tickle_up(int index)
    {
            pair<int,T> bottom=holder[index];
            while((index>0)&&(parent(index).second>=bottom.second))
            {
                holder[index]=parent(index);
                index=(index-1)/2;
            }
            holder[index]=bottom;
    }
    //shifting down for delete key
    void tickle_down(int index)
    {
        int larger;
        pair<int,T> temp=holder[index];
        while(index<curr_size/2)
        {
            larger=(right(index).second>=left(index).second)?2*index+2:2*index+1;
            if(temp.second<=holder[larger].second)
                break;
            holder[index]=holder[larger];
            index=larger;
        }
        holder[index]=temp;
    }
public :
    //Constructors
    Bin_heap()
    {
        holder= new pair<int,T> [10000];
        max_size=10000;
        curr_size=0;
    }
    //Destructor
    ~Bin_heap()
    {
        delete [] holder;
    }
     //Returns size
    int size()
    {
        return curr_size;
    }
    //Inserting node
    void insert(pair<int,T> x)
    {
        if(curr_size==max_size)
        {
            cout<<"Heap Full"<<'\n';
        }
        else
        {   //Inserting at the next available position and shifting to suitable position up
            holder[curr_size]=x;
            tickle_up(curr_size++);
        }
    }
    //Returns root of max heap
    pair<int,T> getMin()
    {
        return holder[0];
    }
    //Delete root and shifting swapped root to its suitable position
    pair<int,T> deleteKey()
    {
        if(curr_size==0)
        {
            cout<<"Empty Heap"<<'\n';
        }
        else
        {
            pair<int,T> temp=holder[0];
            holder[0]=holder[--curr_size];
            tickle_down(0);
            return temp;
        }

    }
    bool empty()
    {
        if(curr_size==0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    /*
    void print()
    {
        for(int i=0;i<curr_size;i++)
        {
            std::cout<<holder[i]<<" ";
        }
    }
    */
};
class Graph
{
public:
    int num_of_vertices;
    int num_of_edges;
    vector<dest_weight> *dir_graph;
    vector<dest_weight> *rev_dir_graph;
    vector<double>bfs_src_distances;
    vector<int>bfs_visit_count;
    vector<bool>visit_status_dir;
    Graph(int num_v,int num_e)
    {
        num_of_vertices=num_v;
        num_of_edges=num_e;
        dir_graph= new vector<dest_weight>[num_of_vertices];
        rev_dir_graph= new vector<dest_weight>[num_of_vertices];
        bfs_src_distances.resize(num_of_vertices,INT_MAX);
        bfs_visit_count.resize(num_of_vertices,INT_MAX);
        visit_status_dir.resize(num_of_vertices,false);
    }
    void reset()
    {

        bfs_src_distances.clear();
        bfs_visit_count.clear();
        visit_status_dir.clear();
        bfs_src_distances.resize(num_of_vertices,INT_MAX);
        visit_status_dir.resize(num_of_vertices,false);
    }
    void Add_Edge_Directed(int startv,int endv,double weight=0)
    {
        dir_graph[startv].push_back(make_pair(endv,weight));
        rev_dir_graph[endv].push_back(make_pair(startv,weight));


    }
    void Add_Edge_Undirected(int startv,int endv,double weight=0)
    {
        dir_graph[startv].push_back(make_pair(endv,weight));
        dir_graph[endv].push_back(make_pair(startv,weight));

    }
    void Remove_Edge_Directed(int startv,int endv,double weight=0)
    {

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

    }
    void Remove_Edge_UnDirected(int startv,int endv,int weight=0)
    {


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
    void Shortest_path_Bin(int source)
    {
        reset();
        Bin_heap <double>pq;
        pq.insert(make_pair(source,0));
        for(int i=0; i<num_of_vertices; i++)
        {
            bfs_visit_count[i]=0;
        }
        bfs_src_distances[source]=0;
        while(!pq.empty())
        {
            int temp=pq.deleteKey().first;

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
                            pq.insert(make_pair(temp_dest,dist_1));
                            bfs_src_distances[temp_dest]=dist_1;
                            bfs_visit_count[temp_dest]=bfs_visit_count[temp]+1;
                        }
                    }


                }
            }
        }

    }

    vector<Node<double>*> Shortest_path_Fib(int source)
    {
        reset();
        vector<Node<double>*> result;
        Fib_heap<double> pq;
        pq.create_heap(num_of_vertices,source);
        for(int i=0; i<num_of_vertices; i++)
        {
            bfs_visit_count[i]=0;
        }
        while(!pq.is_empty())
        {
            Node<double>* z = pq.fib_heap_extract_min();
            //  cout<<"Vertex: "<<z->index<<"  Shortest Path cost  "<<z->value<<endl;
            // cout<<"Path Length  "<<bfs_visit_count[z->index]<<endl;
            result.push_back(z);
            int temp=z->index;
            for(int i=0; i<dir_graph[temp].size(); i++)
            {
                int temp_dest=dir_graph[temp][i].first;
                double temp_weight=dir_graph[temp][i].second;
                Node<double> *x=pq.fib_heap_search(temp_dest);
                if(x!=NULL)
                {
                    if(x->value>z->value+temp_weight)
                        pq.fib_heap_decrease_key(temp_dest,z->value+temp_weight);
                    bfs_visit_count[temp_dest]=bfs_visit_count[temp]+1;
                }
            }
        }
        return result;


    }
    void Shortest_path_Calculation(vector<int> &source,vector<int> &dest)
    {   freopen("opt.txt","w",stdout);
        cout<<"Path Length"<<"  "<<"Path Cost"<<"  "<<"Bin_Heap Runtime"<<"  "<<"Fib_Heap Runtime"<<endl;
        for(int i=0; i<source.size(); i++)
        {
            //clock_t start_bin = clock();
            auto start_bin=high_resolution_clock::now();
            Shortest_path_Bin(source[i]);
            //double result=double(clock() - start_bin) / CLOCKS_PER_SEC;
            auto end_bin=high_resolution_clock::now();
            auto dur_bin=duration_cast<microseconds>(end_bin-start_bin);
           // cout<<"Vertex: "<<dest[i]<<"  Shortest Path cost  "<<bfs_src_distances[dest[i]]<<endl;
            //cout<<"Path Length  "<<bfs_visit_count[dest[i]]<<endl;
            //cout<<"Time: "<<result*10000000000000<<endl;
            cout<<bfs_visit_count[dest[i]]<<"                 "<<bfs_src_distances[dest[i]]<<"                 "<<dur_bin.count()<<"                ";

           // clock_t start_fib = clock();
            auto start_fib=high_resolution_clock::now();
            vector<Node<double>*> res = Shortest_path_Fib(source[i]);
            //result=double(clock() - start_fib) / CLOCKS_PER_SEC;
            auto end_fib=high_resolution_clock::now();
            auto dur_fib=duration_cast<microseconds>(end_bin-start_bin);
            cout<<dur_fib.count()<<endl;
/*
            for(int j=0; j<res.size(); j++)
            {
                Node<double>* z=res[j];
                if(z->index==dest[i])
                {
                   cout<<"Fibonacci Heap"<<endl;
                    cout<<"Vertex: "<<z->index<<"  Shortest Path cost  "<<z->value<<endl;
                    cout<<"Path Length  "<<bfs_visit_count[z->index]<<endl;
                    cout<<"Time: "<<result*10000000000000<<endl;

                }


            }
            */


        }
         fclose(stdout);

    }
    ~Graph()
    {

    }
};
int main()
{
    freopen("inp1.txt","r",stdin);
    int num_of_vertices,num_of_edges,u,v;
    double weight;
    cin>>num_of_vertices>>num_of_edges;
    Graph X(num_of_vertices,num_of_edges);
    for(int i=0; i<num_of_edges; i++)
    {
        cin>>u>>v>>weight;
        X.Add_Edge_Directed(u,v,weight);
    }
    fclose(stdin);
    freopen("inp2.txt","r",stdin);
    int src,dest,test;
    cin>>test;
    vector<int> srcs;
    vector<int> dests;
    for(int i=0; i<test; i++)
    {   int x,y;
        cin>>x;
        cin>>y;
        //cout<<x<<" "<<y<<endl;
        srcs.push_back(x);
        dests.push_back(y);
    }
    fclose(stdin);

    X.Shortest_path_Calculation(srcs,dests);


    return 0;
}

