
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

class Matt
{
public:
    int mat_size;
    vector<vector<int>> *store;
    vector<int> rows;
    vector<int> collumns;
    int fixed_col_ = 0;
    int fixed_rc = 0;
    int lvl = 0;
    bool last_row_fix = false;
    int bd;

    class rowcheck
    {
    private:
        int rc;
        const Matt *temp_matt;
    public:
        rowcheck(int rowcheck, const Matt *m)
        {
            rc=rowcheck;
            temp_matt=m;
        }

        bool operator[](int c)
        {
            assert(temp_matt->store);
            assert(c < temp_matt->mat_size);
            return (*temp_matt->store)[temp_matt->rows[rc]]
                   [temp_matt->collumns[c]];
        }


    };


    Matt(vector<vector<int>> *store)
    {
        mat_size=store->size();
        this->store=store;
        rows=vector<int>(mat_size);
        collumns=vector<int>(mat_size);
        for (int i = 0; i < mat_size; i++)
        {
            collumns[i] = rows[i] = i;
        }
        bd = calculate_bound(*this);
    }



    Matt next(int val) const
    {
        Matt temp = *this;
        temp.lvl++;
        if (!last_row_fix)
        {
            std::swap(temp.rows[temp.fixed_rc], temp.rows[val]);
            temp.fixed_rc++;
        }
        else
        {
            std::swap(temp.collumns[temp.fixed_col_], temp.collumns[val]);
            temp.fixed_col_++;
        }
        temp.last_row_fix = !last_row_fix;
        temp.bd = calculate_bound(temp);
        return temp;
    }

    rowcheck operator[](size_t r) const
    {
        return rowcheck(r, this);
    }

    int level() const
    {
        return lvl;
    }

    bool final() const
    {
        return (fixed_rc == mat_size - 1) && (fixed_col_ == mat_size - 1);
    }

    int calculate_bound() const
    {
        return bd;
    }

    int band() const
    {
        assert(final());
        assert(lvl == (2 * (mat_size - 1)));
        return bd;
    }

private:
    static int calculate_bound(const Matt &m);

public:
    friend auto operator<(const Matt &a, const Matt &b)
    {
        if (a.bd == b.bd)
        {
            return a.lvl < b.lvl;
        }
        else
        {
            return b.bd < a.bd;
        }
    }




};
int swap_choice(Matt m)
    {
        if (m.last_row_fix!=false)
        {
            return m.fixed_rc;
        }
        else
        {
            return m.fixed_col_;
        }
    }
void print(const Matt &temp,int n)
{

    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            if(temp[i-1][j-1]==1)
            {
                cout<<"X"<<" ";
            }
            else
            {
                cout<<0<<" ";
            }
        }
        cout << endl;
    }
}

int Matt::calculate_bound(const Matt &m)
{
    int bound = 1;
    //Checking For Fixed Region
    //For Rows
    for (int i = 0; i < m.fixed_rc; i++)
    {
        // Adjusting Non fixed Elements
        int ex = 0;
        for (int j = m.fixed_col_; j < m.mat_size; j++)
        {
            if (m[i][j])
            {
                ex++;
            }
        }
        if (ex)
        {
            bound = max(bound, m.fixed_col_ - i + ex);
        }
        else
        {
            //Calculating Fixed Region Bounds
            int last = i;
            for (int j = i + 1; j < m.fixed_col_; j++)
            {
                if (m[i][j])
                {
                    last = j;
                }
            }
            bound = max(bound, last - i + 1);
        }
    }
    //Checking For Fixed Region
    //For Rows
    for (int j = 0; j < m.fixed_col_; j++)
    {
        // Adjusting Non fixed Elements
        int ex = 0;
        for (int i = m.fixed_rc; i < m.mat_size; i++)
        {
            if (m[i][j])
            {
                ex++;
            }
        }
        if (ex)
        {
            bound = max(bound, m.fixed_rc - j + ex);
        }
        else
        {
             //Calculating Fixed Region Bounds
            int last = j;
            for (int i = j + 1; i < m.fixed_rc; i++)
            {
                if (m[i][j])
                {
                    last = i;
                }
            }
            bound = max(bound, last - j + 1);
        }
    }
    //Checking For Unfixed Region
    //Rows
    for (int i = m.fixed_rc; i < m.mat_size; i++)
    {
        int cnt = 0;
        for (int j = 0; j < m.mat_size; j++)
        {
            if (m[i][j])
            {
                cnt++;
            }
        }
        bound = max(bound, (cnt + 1) / 2);
    }
    // Collumns
    for (int j = m.fixed_col_; j < m.mat_size; j++)
    {
        int cnt = 0;
        for (int i = 0; i < m.mat_size; i++)
        {
            if (m[i][j])
            {
                cnt++;
            }
        }
        bound = max(bound, (cnt + 1) / 2);
    }
    return bound;
}


int main()
{
    int n;
    char c;
    cin >> n;
    vector<vector<int>> store(n, vector<int>(n));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cin >> c;
            if(c=='X')
            {
                store[i][j]=1;
            }
            else
            {
                store[i][j]=0;
            }
        }
    }


    Matt X(&store);
    priority_queue<Matt> temp;
    temp.push(X);

    while (!temp.empty())
    {
        auto m = temp.top();
        temp.pop();
        if (m.final())
        {
            X = m;
            break;
        }
        int s = swap_choice(X);
        for (int i = s; i < n; i++)
        {
            temp.push(m.next(i));
        }
    }


    cout << X.band() << std::endl;
    print(X,n);

}
