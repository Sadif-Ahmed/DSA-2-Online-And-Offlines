#include<iostream>
#include<vector>
#include<unordered_map>
#include<unordered_set>
#include <bits/stdc++.h>
#include<chrono>
#include<ctime>

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
    long long no_of_collisions;
    long long no_of_probes;
public:
    void reset()
    {
        no_of_collisions=0;
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
                this->no_of_collisions++;
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
                no_of_collisions++;
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
            this->no_of_probes++;
            if(table[idx]->flag!=1)
            {
                return -INT_MAX;
            }
            else
            {
                this->no_of_probes--;
                node * head = table[idx];
                while(head)
                {
                    this->no_of_probes++;
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
        if(!count)
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
    long long get_collisions()
    {
        return no_of_collisions;
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
int main()
{
    long long N;
    cin>>N;
//    count_unique_hash(N);


    vector<string>dump=wordgenerator(N);
    Hash_table t1 = Hash_table(N,1);
    int l=N*0.4;
    vector<string>tempr;
    for(int i=0; i<l; i++)
    {
      //  cout<<dump[i]<<"   "<<i<<endl;
        if(i%5==0)
        {
            tempr.push_back(dump[i]);
        }
        t1.insert(dump[i],i+1);
    }
    cout<<"Collisions :  "<<t1.get_collisions()<<endl;
    auto start_time=high_resolution_clock::now();
    for(int i=0; i<tempr.size(); i++)
    {
        cout<<t1.search(tempr[i])<<endl;
    }
      auto end_time=high_resolution_clock::now();
      auto dur_time=duration_cast<nanoseconds>(end_time-start_time);
    cout<<"Before Deletion Search Time:  "<<dur_time.count()<<endl;
  //  cout<<"Probes:  "<<t1.get_probes()<<endl;
    for(int i=0; i<tempr.size(); i+=2)
    {
        t1.remove(tempr[i]);
    }
    start_time=high_resolution_clock::now();
    for(int i=0; i<tempr.size(); i++)
    {
        cout<<t1.search(tempr[i])<<endl;
    }
    end_time=high_resolution_clock::now();
    dur_time=duration_cast<nanoseconds>(end_time-start_time);
    cout<<"Probes:  "<<t1.get_probes()<<endl;
    cout<<"After Deletion Search Time:  "<<dur_time.count()<<endl;
    //t1.print();

    /*
        Hash_table h1 = Hash_table(N*0.4,1);
        h1.insert("baba",1);
        h1.insert("ma",2);
        h1.insert("dada",3);
        h1.insert("baba",4);
        h1.insert("ma",5);
        h1.insert("dada",6);
        cout<<h1.search("baba")<<endl;
        h1.remove("baba");
        cout<<h1.search("baba")<<endl;
        cout<<h1.search("ma")<<endl;
        cout<<h1.search("dada")<<endl;
        cout<<"col   "<<h1.get_collisions()<<endl;
        cout<<"prob   "<<h1.get_probes();
    */


}
