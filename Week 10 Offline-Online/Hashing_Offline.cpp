#include<iostream>
#include<vector>
#include<unordered_map>
#include<unordered_set>
#include <bits/stdc++.h>
#include<chrono>
#include<cstdlib>
#include<fstream>

using namespace std;
using namespace std::chrono;

const long long c1 = 39;
const long long c2 = 41;

struct key_value_pair
{
    string key;
    long long value;
};
struct node
{
    int flag;
    //Needed of open addressing
    //no data=0
    //has data=1
    //deleted data=2
    key_value_pair * data;
    node * next;
};
long long h1(string key)
{
    long long h = 3332189845;
    for (long long ki : key)
    {
        h ^= ki;
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }
    return h;
}
long long h2(string key)
{
    long long h = 1;

    for (long long ki : key)
        h = ((h << 8) + h) + ki;

    return h;
}
long long index_calculator(long long h1_val,long long h2_val,long long i,long long n,long long hash_type)
{
    long long idx;
    if(hash_type==2)
    {
        idx=(h1_val+i)%n;
    }
    else if(hash_type==3)
    {
        idx=(h1_val+i*c1+i*i*c2)%n;
    }
    else
    {
        idx=(h1_val+i*h2_val)%n;
    }
    return idx;
}
vector<string> wordgenerator(long long n)
{
    unordered_set<string> dump;
    string alphabets = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    long long len = 7;
    while (dump.size() < 2*n)
    {
        string temp = "";
        for (long long i = 0; i < len; i++)
            temp += alphabets[rand() % alphabets.length()];
        dump.insert(temp);
    }
    vector<string> reserve=vector<string>(dump.begin(), dump.end());
    sort(reserve.begin(), reserve.end());
    vector<string>::iterator it;
    it = unique(reserve.begin(),reserve.end());
    reserve.resize(distance(reserve.begin(),it));
    return reserve;
}
void count_unique_hash(long long n)
{
    vector<string>dump=wordgenerator(100);
    vector<long long>store1;
    vector<long long>store2;
    for(long long i=0; i<100; i++)
    {
        store1.push_back(h1(dump[i])%n);
        store2.push_back(h2(dump[i])%n);
    }
    sort(store1.begin(), store1.end());
    vector<long long>::iterator it;
    it = unique(store1.begin(), store1.end());
    store1.resize(distance(store1.begin(),it));

    sort(store2.begin(), store2.end());
    it = unique(store2.begin(), store2.end());
    store2.resize(distance(store2.begin(),it));
    cout<<store1.size()<<endl;
    cout<<store2.size();
}
class Hash_table
{
    vector<node *>table;
    long long count;
    long long size;
    long long collision_choice;
    long long no_of_probes;
public:
    void reset()
    {
        no_of_probes=0;
    }
    Hash_table(long long n,long long c_c)
    {
        this->table = vector<node *>(n);
        for(int i=0; i<n; i++)
        {
            table[i] = new node;
            table[i]->flag = 0, table[i]->data = nullptr, table[i]->next = nullptr;
        }
        this->count=0;
        this->size=n;
        this->collision_choice=c_c;
        reset();
    }
    ~Hash_table()
    {
        this->table.clear();
    }
    bool is_full()
    {
        if(count>=size)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    void insert(string key,long long value)
    {
        if(is_full())
        {
            cout<<"The table is full";
            return;
        }

        long long h1_val=h1(key)%this->size;
        long long h2_val=h2(key)%this->size;
        long long idx;

        key_value_pair * temp = new key_value_pair;
        temp->key= key;
        temp->value=value;

        if(collision_choice==1)
        {
            idx=h1_val;
            if(table[idx]->flag != 1)
            {
                table[idx]->flag = 1, table[idx]->data = temp, table[idx]->next = nullptr;
            }
            else
            {
                node * head= table[idx];
                while(head->next)
                {
                    head=head->next;
                }
                node * pos=new node;
                pos->flag = 1;
                pos->data = temp;
                pos ->next = nullptr;

                head->next=pos;
            }
            count++;

        }
        else
        {
            long long i=0;
            while(table[idx=index_calculator(h1_val,h2_val,i,this->size,collision_choice)]->flag==1)
            {
                if(i && idx==h1_val)
                {
                    return;
                }
                i++;
            }
            table[idx]->flag=1;
            table[idx]->data=temp;
            table[idx]->next=nullptr;
            count++;
        }
    }
    long long search(string key)
    {
        long long h1_val=h1(key)%this->size;
        long long h2_val=h2(key)%this->size;
        long long idx;
        if(collision_choice==1)
        {
            idx= h1_val;

            if(table[idx]->flag!=1)
            {
                return -INT_MAX;
            }
            else
            {

                node * head = table[idx];
                while(head)
                {

                    if(head->data->key==key)
                    {
                        return head->data->value;
                    }
                    head=head->next;
                }
                return -INT_MAX;
            }
        }
        else
        {
            long long i=0;
            while(table[idx=index_calculator(h1_val,h2_val,i,this->size,collision_choice)]->flag!=0)
            {
                if(i && idx==h1_val)
                    break;
                if(table[idx]->flag == 1 && table[idx]->data->key == key)
                {
                    return table[idx]->data->value;
                }
                i++;
                this->no_of_probes++;
            }
            return -INT_MAX;

        }
    }
    void remove(string key)
    {
        if(count!=0 || search(key)!=-INT_MAX)
        {
            return;
        }
        long long h1_val=h1(key)%this->size;
        long long h2_val=h2(key)%this->size;
        long long idx;
        if(collision_choice==1)
        {
            idx=h1_val;
            if(table[idx]->flag!=1)
            {
                return;
            }
            else
            {
                node * head= table[idx];
                node * prev= nullptr;
                while(head)
                {
                    if(head->data->key==key)
                    {
                        break;
                    }
                    prev=head;
                    head=head->next;
                }
                if(prev)
                {
                    prev->next=head->next;
                }
                else
                {
                    table[idx]= head->next;
                    if(!table[idx])
                    {
                        node * x= new node;
                        x->flag=0;
                        x->data=nullptr;
                        x->next= nullptr;
                        table[idx]=x;
                    }
                }
                delete head;
                count--;
            }
        }
        else
        {
            long long i=0;
            while(table[idx=index_calculator(h1_val,h2_val,i,this->size,collision_choice)]->flag!=0)
            {
                if(i && idx==h1_val)
                    break;
                if(table[idx]->flag == 1 && table[idx]->data->key == key)
                {
                    table[idx]->flag=2;
                    delete table[idx]->data;
                    count--;
                    return;
                }
                i++;

            }
        }
    }
    long long get_probes()
    {
        return no_of_probes;
    }
    void print()
    {

        for (int i = 0; i < size; i++)
        {
            if (table[i]->flag == 1)
            {
                cout << i + 1 << " ";
                node * head = table[i];
                while (head)
                {
                    cout << "->" << " " << "(" << head->data->key << ", " << head->data->value << ")" << " ";
                    head = head->next;
                }
                cout << endl;
            }
        }
    }
};
void result_Calculator(vector<double>&results,vector<string>&dump,long long N,long long collision_choice,double load_factor)
{
    long long l;
    vector<string>tempr;
    auto start_time=high_resolution_clock::now();
    auto end_time=high_resolution_clock::now();
    auto dur_time=duration_cast<nanoseconds>(end_time-start_time);
    Hash_table t1 = Hash_table(N,collision_choice);
    l=N*load_factor;
    cout<<"Load Factor:   "<<load_factor<<endl<<endl;
    cout<<"Collision Resolution:   ";
    switch(collision_choice)
    {
    case 1:
        cout<<"Separate Chaining"<<endl<<endl;
        break;
    case 2:
        cout<<"Linear Probing"<<endl<<endl;
        break;
    case 3:
        cout<<"Quadratic Probing"<<endl<<endl;
        break;
    case 4:
        cout<<"Double Hashing"<<endl<<endl;
        break;
    };
    for(long long i=0; i<l; i++)
    {
        t1.insert(dump[i],i+1);

    }
    for(long long i=0; i<l*0.1; i++)
    {
        tempr.push_back(dump[rand()%l]);
    }
    start_time=high_resolution_clock::now();
    for(long long i=0; i<tempr.size(); i++)
    {
        t1.search(tempr[i]);
    }
    end_time=high_resolution_clock::now();
    dur_time=duration_cast<nanoseconds>(end_time-start_time);
    cout<<"Before Deletion Average Search Time:  "<<(double)dur_time.count()/tempr.size()<<endl;
    cout<<"Average Probes:  "<<(double)t1.get_probes()/tempr.size()<<endl;
    results.push_back((double)dur_time.count()/tempr.size());
    results.push_back((double)t1.get_probes()/tempr.size());
    for(long long i=0; i<tempr.size()/2; i++)
    {
        t1.remove(tempr[rand()%tempr.size()]);
    }
    start_time=high_resolution_clock::now();
    for(long long i=0; i<tempr.size(); i++)
    {
        t1.search(tempr[i]);
    }
    end_time=high_resolution_clock::now();
    dur_time=duration_cast<nanoseconds>(end_time-start_time);
    cout<<"After Deletion Average Search Time:  "<<(double)dur_time.count()/tempr.size()<<endl;
    cout<<"Average Probes:  "<<(double)t1.get_probes()/tempr.size()<<endl<<endl;
    results.push_back((double)dur_time.count()/tempr.size());
    results.push_back((double)t1.get_probes()/tempr.size());
}
int main()
{
    long long N;
    vector<double> results;
    cin>>N;
    vector<string>dump=wordgenerator(N);
    double alpha;
//    count_unique_hash(N);
for(int i=1;i<=4;i++)
{   alpha=0.4;
    for(int j=0;j<6;j++)
    {
        result_Calculator(results,dump,N,i,alpha);
        alpha+=0.1;
    }
}

cout<<"Report Generation in csv:"<<endl;
cout<<"Performance of separate chaining under load condition: Table 1"<<endl;
cout<<"Performance of linear Probing under load condition:Table 2"<<endl;
cout<<"Performance of Quadratic Probing under load condition:Table 3"<<endl;
cout<<"Performance of Double Hashing under load condition:Table 4"<<endl;
cout<<"Performance of Multiple methods under at 0.4 load factor:Table 5 "<<endl;
cout<<"Performance of Multiple methods under at 0.5 load factor:Table 6 "<<endl;
cout<<"Performance of Multiple methods under at 0.6 load factor:Table 7 "<<endl;
cout<<"Performance of Multiple methods under at 0.7 load factor:Table 8 "<<endl;
cout<<"Performance of Multiple methods under at 0.8 load factor:Table 9 "<<endl;
cout<<"Performance of Multiple methods under at 0.9 load factor:Table 10 "<<endl;
/*
cout<<"Performance of separate chaining under load condition: Table 1"<<endl;
cout<<"              "<<"Before Deletion"<<"     "<<"After Deletion"<<endl;
cout<<"Load Factor   "<<"Avg Search Time"<<"     "<<"Avg Search Time"<<endl;
alpha=0.4;
for(int i=0;i<24;i+=4)
{
    cout<<alpha<<"             "<<results[i]<<"              "<<results[i+2]<<endl;
    alpha+=0.1;
}
cout<<"Performance of linear Probing under load condition:Table 2"<<endl;
cout<<"              "<<"Before Deletion"<<"                            "<<"After Deletion"<<endl;
cout<<"Load Factor   "<<"Avg Search Time"<<"     Avg Probing"<<"     "<<"Avg Search Time         "<<"Avg Probing"<<endl;
alpha=0.4;
for(int i=24;i<48;i+=4)
{
    cout<<alpha<<"             "<<results[i]<<"                 "<<results[i+1]<<"    "<<results[i+2]<<"         "<<results[i+3]<<endl;
    alpha+=0.1;
}
cout<<"Performance of Quadratic Probing under load condition: "<<endl;
cout<<"              "<<"Before Deletion"<<"                            "<<"After Deletion"<<endl;
cout<<"Load Factor   "<<"Avg Search Time"<<"     Avg Probing"<<"     "<<"Avg Search Time         "<<"Avg Probing"<<endl;
alpha=0.4;
for(int i=48;i<72;i+=4)
{
    cout<<alpha<<"             "<<results[i]<<"                 "<<results[i+1]<<"    "<<results[i+2]<<"         "<<results[i+3]<<endl;
    alpha+=0.1;
}

cout<<"Performance of Double Hashing under load condition: "<<endl;
cout<<"              "<<"Before Deletion"<<"                            "<<"After Deletion"<<endl;
cout<<"Load Factor   "<<"Avg Search Time"<<"     Avg Probing"<<"     "<<"Avg Search Time         "<<"Avg Probing"<<endl;
alpha=0.4;
for(int i=72;i<96;i+=4)
{
    cout<<alpha<<"             "<<results[i]<<"                 "<<results[i+1]<<"    "<<results[i+2]<<"         "<<results[i+3]<<endl;
    alpha+=0.1;
}


cout<<"Performance of Multiple methods under at 0.4 load factor: "<<endl;
cout<<"              "<<"Before Deletion"<<"                            "<<"After Deletion"<<endl;
cout<<"Load Factor   "<<"Avg Search Time"<<"     Avg Probing"<<"     "<<"Avg Search Time         "<<"Avg Probing"<<endl;
for(int i=0,j=1;i<96;i+=24,j++)
{
    cout<<"Method"<<j<<"             "<<results[i]<<"                 "<<results[i+1]<<"    "<<results[i+2]<<"         "<<results[i+3]<<endl;
}
cout<<"Performance of Multiple methods under at 0.5 load factor: "<<endl;
cout<<"              "<<"Before Deletion"<<"                            "<<"After Deletion"<<endl;
cout<<"Load Factor   "<<"Avg Search Time"<<"     Avg Probing"<<"     "<<"Avg Search Time         "<<"Avg Probing"<<endl;
for(int i=4,j=1;i<96;i+=24,j++)
{
    cout<<"Method"<<j<<"             "<<results[i]<<"                 "<<results[i+1]<<"    "<<results[i+2]<<"         "<<results[i+3]<<endl;
}
cout<<"Performance of Multiple methods under at 0.6 load factor: "<<endl;
cout<<"              "<<"Before Deletion"<<"                            "<<"After Deletion"<<endl;
cout<<"Load Factor   "<<"Avg Search Time"<<"     Avg Probing"<<"     "<<"Avg Search Time         "<<"Avg Probing"<<endl;
for(int i=8,j=1;i<96;i+=24,j++)
{
    cout<<"Method"<<j<<"             "<<results[i]<<"                 "<<results[i+1]<<"    "<<results[i+2]<<"         "<<results[i+3]<<endl;
}
cout<<"Performance of Multiple methods under at 0.7 load factor: "<<endl;
cout<<"              "<<"Before Deletion"<<"                            "<<"After Deletion"<<endl;
cout<<"Load Factor   "<<"Avg Search Time"<<"     Avg Probing"<<"     "<<"Avg Search Time         "<<"Avg Probing"<<endl;
for(int i=12,j=1;i<96;i+=24,j++)
{
    cout<<"Method"<<j<<"             "<<results[i]<<"                 "<<results[i+1]<<"    "<<results[i+2]<<"         "<<results[i+3]<<endl;
}
cout<<"Performance of Multiple methods under at 0.8 load factor: "<<endl;
cout<<"              "<<"Before Deletion"<<"                            "<<"After Deletion"<<endl;
cout<<"Load Factor   "<<"Avg Search Time"<<"     Avg Probing"<<"     "<<"Avg Search Time         "<<"Avg Probing"<<endl;
for(int i=16,j=1;i<96;i+=24,j++)
{
    cout<<"Method"<<j<<"             "<<results[i]<<"                 "<<results[i+1]<<"    "<<results[i+2]<<"         "<<results[i+3]<<endl;
}
cout<<"Performance of Multiple methods under at 0.9 load factor: "<<endl;
cout<<"              "<<"Before Deletion"<<"                            "<<"After Deletion"<<endl;
cout<<"Load Factor   "<<"Avg Search Time"<<"     Avg Probing"<<"     "<<"Avg Search Time         "<<"Avg Probing"<<endl;
for(int i=20,j=1;i<96;i+=24,j++)
{
    cout<<"Method"<<j<<"             "<<results[i]<<"                 "<<results[i+1]<<"    "<<results[i+2]<<"         "<<results[i+3]<<endl;
}
*/
freopen("Table1.csv","w",stdout);
cout<<","<<"Before Deletion"<<","<<"After Deletion"<<endl;
cout<<"Load Factor"<<","<<"Avg Search Time"<<","<<"Avg Search Time"<<endl;
alpha=0.4;
for(int i=0;i<24;i+=4)
{
    cout<<alpha<<","<<results[i]<<","<<results[i+2]<<endl;
    alpha+=0.1;
}
fclose(stdout);
freopen("Table2.csv","w",stdout);
cout<<","<<"Before Deletion"<<","<<","<<"After Deletion"<<endl;
cout<<"Load Factor,"<<"Avg Search Time,"<<"Avg Probing,"<<"Avg Search Time,"<<"Avg Probing"<<endl;
alpha=0.4;
for(int i=24;i<48;i+=4)
{
    cout<<alpha<<","<<results[i]<<","<<results[i+1]<<","<<results[i+2]<<","<<results[i+3]<<endl;
    alpha+=0.1;
}
fclose(stdout);
freopen("Table3.csv","w",stdout);
cout<<","<<"Before Deletion"<<","<<","<<"After Deletion"<<endl;
cout<<"Load Factor,"<<"Avg Search Time,"<<"Avg Probing,"<<"Avg Search Time,"<<"Avg Probing"<<endl;
alpha=0.4;
for(int i=48;i<72;i+=4)
{
    cout<<alpha<<","<<results[i]<<","<<results[i+1]<<","<<results[i+2]<<","<<results[i+3]<<endl;
    alpha+=0.1;
}
fclose(stdout);
freopen("Table4.csv","w",stdout);
cout<<","<<"Before Deletion"<<","<<","<<"After Deletion"<<endl;
cout<<"Load Factor,"<<"Avg Search Time,"<<"Avg Probing,"<<"Avg Search Time,"<<"Avg Probing"<<endl;
alpha=0.4;
for(int i=72;i<96;i+=4)
{
    cout<<alpha<<","<<results[i]<<","<<results[i+1]<<","<<results[i+2]<<","<<results[i+3]<<endl;
    alpha+=0.1;
}
fclose(stdout);
freopen("Table5.csv","w",stdout);
cout<<","<<"Before Deletion,"<<","<<"After Deletion,"<<endl;
cout<<"Collision Process,"<<"Avg Search Time,"<<"Avg Probing,"<<"Avg Search Time,"<<"Avg Probing"<<endl;
for(int i=0,j=1;i<96;i+=24,j++)
{   string method;
switch(j)
{
    case 1:method="Separate Chaining";
    break;
    case 2:method="Linear Probing";
    break;
    case 3:method="Quadratic Probing";
    break;
    case 4:method="Double Hashing";
    break;
}
    cout<<method<<","<<results[i]<<","<<results[i+1]<<","<<results[i+2]<<","<<results[i+3]<<endl;
}
fclose(stdout);
freopen("Table6.csv","w",stdout);
cout<<","<<"Before Deletion,"<<","<<"After Deletion,"<<endl;
cout<<"Collision Process,"<<"Avg Search Time,"<<"Avg Probing,"<<"Avg Search Time,"<<"Avg Probing"<<endl;
for(int i=4,j=1;i<96;i+=24,j++)
{   string method;
switch(j)
{
    case 1:method="Separate Chaining";
    break;
    case 2:method="Linear Probing";
    break;
    case 3:method="Quadratic Probing";
    break;
    case 4:method="Double Hashing";
    break;
}
    cout<<method<<","<<results[i]<<","<<results[i+1]<<","<<results[i+2]<<","<<results[i+3]<<endl;
}
fclose(stdout);
freopen("Table7.csv","w",stdout);
cout<<","<<"Before Deletion,"<<","<<"After Deletion,"<<endl;
cout<<"Collision Process,"<<"Avg Search Time,"<<"Avg Probing,"<<"Avg Search Time,"<<"Avg Probing"<<endl;
for(int i=8,j=1;i<96;i+=24,j++)
{   string method;
switch(j)
{
    case 1:method="Separate Chaining";
    break;
    case 2:method="Linear Probing";
    break;
    case 3:method="Quadratic Probing";
    break;
    case 4:method="Double Hashing";
    break;
}
    cout<<method<<","<<results[i]<<","<<results[i+1]<<","<<results[i+2]<<","<<results[i+3]<<endl;
}
fclose(stdout);
freopen("Table8.csv","w",stdout);
cout<<","<<"Before Deletion,"<<","<<"After Deletion,"<<endl;
cout<<"Collision Process,"<<"Avg Search Time,"<<"Avg Probing,"<<"Avg Search Time,"<<"Avg Probing"<<endl;
for(int i=12,j=1;i<96;i+=24,j++)
{   string method;
switch(j)
{
    case 1:method="Separate Chaining";
    break;
    case 2:method="Linear Probing";
    break;
    case 3:method="Quadratic Probing";
    break;
    case 4:method="Double Hashing";
    break;
}
    cout<<method<<","<<results[i]<<","<<results[i+1]<<","<<results[i+2]<<","<<results[i+3]<<endl;
}
fclose(stdout);
freopen("Table9.csv","w",stdout);
cout<<","<<"Before Deletion,"<<","<<"After Deletion,"<<endl;
cout<<"Collision Process,"<<"Avg Search Time,"<<"Avg Probing,"<<"Avg Search Time,"<<"Avg Probing"<<endl;
for(int i=16,j=1;i<96;i+=24,j++)
{   string method;
switch(j)
{
    case 1:method="Separate Chaining";
    break;
    case 2:method="Linear Probing";
    break;
    case 3:method="Quadratic Probing";
    break;
    case 4:method="Double Hashing";
    break;
}
    cout<<method<<","<<results[i]<<","<<results[i+1]<<","<<results[i+2]<<","<<results[i+3]<<endl;
}
fclose(stdout);
freopen("Table10.csv","w",stdout);
cout<<","<<"Before Deletion,"<<","<<"After Deletion,"<<endl;
cout<<"Collision Process,"<<"Avg Search Time,"<<"Avg Probing,"<<"Avg Search Time,"<<"Avg Probing"<<endl;
for(int i=20,j=1;i<96;i+=24,j++)
{   string method;
switch(j)
{
    case 1:method="Separate Chaining";
    break;
    case 2:method="Linear Probing";
    break;
    case 3:method="Quadratic Probing";
    break;
    case 4:method="Double Hashing";
    break;
}
    cout<<method<<","<<results[i]<<","<<results[i+1]<<","<<results[i+2]<<","<<results[i+3]<<endl;
}
fclose(stdout);
return 0;
}
