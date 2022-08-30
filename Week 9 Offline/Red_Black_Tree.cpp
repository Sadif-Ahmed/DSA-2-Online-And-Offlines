#include<iostream>
#include<bits/stdc++.h>
using namespace std;



#define RED   1
#define BLACK 0


struct node
{
    int  val;////Key value of node
    node * l = nullptr;
    node * r = nullptr;
    node * p = nullptr;
    bool  col = 0;//By default red colored
    int size=1;//No. of nodes in the subtree rooted at the node(including root)
    //Counting it as a root so value starts from 1
};

class RBT
{
    node * root;
public:
    RBT();
    node * getroot();
    void lrotate(node * x);
    void rrotate(node * x);
    void insert(int val);
    void rbtinsertfixup(node * x);
    void remove(int val);
    void fixdeletenode(node * x);
    void rbtdeletefixup(node * par, node * x);
    bool find(int val);
    void printpreorder(node * x);
    void successor_update(node *x);
    int check_left_priority(int key);
};

RBT::RBT() {root = nullptr;}

node * RBT::getroot() {return root;}
void RBT::successor_update(node *x)
    {
        int a = 0, b = 0;
        if(x->l != nullptr)
        {
            a=x->l->size;
        }
        if(x->r != nullptr)
        {
            b=x->r->size;
        }
        x->size = a + b + 1;

    }


void RBT::lrotate(node *x)
{
    node * xpar = x->p;
    node * rbranch = x->r;
    node * rbranchl = rbranch->l;
    x->r = rbranchl;
    if(rbranchl) rbranchl->p = x;
    rbranch->l = x;
    rbranch->p = xpar;
    x->p = rbranch;
    if(!xpar) root = rbranch;
    else
    {
        if(x == xpar->l) xpar->l = rbranch;
        else xpar->r = rbranch;
    }
    successor_update(x);
    successor_update(x->p);

}

void RBT::rrotate(node *x)
{
    node * xpar = x->p;
    node * lbranch = x->l;
    node * lbranchr = lbranch->r;
    x->l = lbranchr;
    if(lbranchr) lbranchr->p = x;
    lbranch->r = x;
    lbranch->p = xpar;
    x->p = lbranch;
    if(!xpar) root = lbranch;
    else
    {
        if(x == xpar->l) xpar->l = lbranch;
        else xpar->r = lbranch;
    }
    successor_update(x);
    successor_update(x->p);
}

void RBT::insert(int val)
{//Insertion same as BST
    node * x = new node();
    x->val = val;
    x->col = RED;

    node * cur = root;
    node * par = nullptr;
    while(cur)
    {
        par = cur;
        cur->size++;//Increasing the number of successors
        if(x->val <= cur->val) cur = cur->l;
        else cur = cur->r;
    }
    x->p = par;
    if(par == nullptr) root = x;
    else
    {
        if(x->val <= par->val) par->l = x;
        else par->r = x;
    }
    //Fixing up the red black property
    rbtinsertfixup(x);
}

void RBT::rbtinsertfixup(node * x)
{
    while(x != root && x->p->col == RED)
    {
        node * par = x->p;
        node * gp = par->p;
        node * uncle;
        if(gp->l == par)
        {   //uncle red so only recoloring
            uncle = gp->r;
            if(uncle != nullptr && uncle->col == RED)
            {
                uncle->col = par->col = BLACK;
                gp->col = RED;
                x = gp;
            }
            else
            { //Uncle Black parent left child
                if(x == par->r)
                { //Left-Right Case
                    x = par;
                    lrotate(x);
                }
                //Left-Left Case
                gp->col = RED;
                x->p->col = BLACK;
                rrotate(gp);
            }
        }
        else
        {
            uncle = gp->l;
             //Uncle red so only recoloring
            if(uncle != nullptr && uncle->col == RED)
            {
                uncle->col = par->col = BLACK;
                gp->col = RED;
                x = gp;
            }
            else
            {//uncle black parent right child
                if(x == par->l)
                {//Right left case
                    x = par;
                    rrotate(x);
                }
                //Right Right case
                gp->col = RED;
                x->p->col = BLACK;
                lrotate(gp);
            }
        }
    }
    root->col = BLACK;
}

void RBT::fixdeletenode(node * cur)
{   //Finding true successor and swapping it with the to be deleted node
    node * rep;
    node * repbaccha;
    if(cur->l == nullptr || cur->r == nullptr) rep = cur;
    else
    {
        rep = cur->r;
        while(rep->l) rep = rep->l;
    }

    if(rep->l) repbaccha = rep->l;
    else repbaccha = rep->r;
    if(repbaccha) repbaccha->p = rep->p;

    if(rep->p == nullptr) root = repbaccha;
    else if(rep == rep->p->l) rep->p->l = repbaccha;
    else rep->p->r = repbaccha;

    if(rep != cur) cur->val = rep->val;
     //Decreasing the number of successors of the predecessors of the to be deleted node
    node * temp=rep->p;
    while(temp)
    {
        temp->size--;
        temp=temp->p;
    }
    if(rep->col == BLACK)
    {
        rbtdeletefixup(rep->p, repbaccha);
    }
    if(repbaccha == nullptr && repbaccha == root) root = nullptr;
    else delete rep;
}

void RBT::remove(int val)
{
    node * cur = root;

    while(cur)
    {
        if(val == cur->val)
        {

            fixdeletenode(cur);
            break;
        }
        else if(val < cur->val) cur = cur->l;
        else cur = cur->r;
    }
}

void RBT::rbtdeletefixup(node * par, node * x)
{
    node * sibling;
    while((x == nullptr || x->col == BLACK) && x != root)
    {
        if(x == par->l)
        {
            sibling = par->r;
            if(sibling == nullptr) continue;
            if(sibling->col == RED)
            {  //recoloring when sibling color red
                sibling->col = BLACK;
                par->col = RED;
                lrotate(par);
                sibling = par->r;
            }
//Fixing double black made in the previous step
            if((sibling->l == nullptr || sibling->l->col == BLACK) && (sibling->r == nullptr || sibling->r->col == BLACK))
            {
                sibling->col = RED;
                x = par;
                par = x->p;
            }
            else
            {
                if(sibling->r == nullptr || sibling->r->col == BLACK)
                {//left left case
                    if(sibling->l) sibling->l->col = BLACK;
                    sibling->col = RED;
                    rrotate(sibling);
                    sibling = par->r;
                }
                sibling->col = par->col;
                par->col = BLACK;
                //left right case
                if(sibling->r) sibling->r->col = BLACK;
                lrotate(par);
                x = root;
            }
        }
        else
        {//fixing red sibling
            sibling = par->l;
            if(sibling == nullptr) continue;
            if(sibling->col == RED)
            {
                sibling->col = BLACK;
                par->col = RED;
                rrotate(par);
                sibling = par->l;
            }

            if((sibling->l == nullptr || sibling->l->col == BLACK) && (sibling->r == nullptr || sibling->r->col == BLACK))
            { //right right
                sibling->col = RED;
                x = par;
                par = x->p;
            }
            else
            {
                if(sibling->l == nullptr || sibling->l->col == BLACK)
                {
                    if(sibling->r) sibling->r->col = BLACK;
                    sibling->col = RED;
                    lrotate(sibling);
                    sibling = par->l;
                }
                sibling->col = par->col;
                par->col = BLACK;
                 //right left
                if(sibling->l) sibling->l->col = BLACK;
                rrotate(par);
                x = root;
            }
        }
    }
    if(x) x->col = BLACK;
}

bool RBT::find(int val)
{
    node * cur = root;
    node * par = nullptr;

    while(cur)
    {
        if(val == cur->val)

        {
        return true;
        }
        else if(val < cur->val)
        {
            par = cur;
            cur = cur->l;
        }
        else
        {
            par = cur;
            cur = cur->r;
        }
    }
    return false;
}
int RBT::check_left_priority(int key)
    {
        int left = 0;
        node *current = root;
        node *temp = NULL;
        while (current)
        {
            temp = current;

            if (key < current->val)
            {
                current = current->l;
            }
            else if (key == current->val)
            {

                left += (current->l ? current->l->size : 0);
                return left;
            }
            else
            {

                left += ((current->l ? current->l->size : 0) + 1);
                current = current->r;
            }
        }
        return left;
    }

void RBT::printpreorder(node * x)
{
    if(!x) return;
    cout<<x->val<<":"<<((x->col == RED) ? 'r' : 'b');
    if(!x->l && !x->r) return;
    cout<<"(";
    printpreorder(x->l);
    cout<<")";
    cout<<"(";
    printpreorder(x->r);
    cout<<")";
}

int main()
{
    RBT X;
    freopen("in.txt","r",stdin);
    freopen("output.txt","w",stdout);
    long long t, order, value;
    cin >> t;
    cout << t << endl;
    while (t--)
    {
        cin >> order >> value;
        cout << order << "   " << value << "   ";
        switch (order)
        {
        case 0:
            if (X.find(value))
            {
                cout << 1 << endl;
                X.remove(value);
            }
            else
            {
                cout << 0 << endl;
            }
            break;
        case 1:
            if (X.find(value))
            {
                cout << 0 << endl;
            }
            else
            {
                cout << 1 << endl;
                X.insert(value);
            }
            break;
        case 2:
            if (X.find(value))
            {
                cout << 1 << endl;
            }
            else
            {
                cout << 0 << endl;
            }
            break;
        case 3:
            cout << X.check_left_priority(value) << endl;
            break;
        default:
            cout << "Not a valid Order" << endl;
            break;
        }
    }
    fclose(stdin);
    fclose(stdout);


    return 0;
}
