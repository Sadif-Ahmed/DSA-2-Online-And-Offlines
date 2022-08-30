#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<vector>
using namespace std;

int n;
int m;



struct characteristics
{
	int A;
	int H;
	int D;

};

characteristics Men[70];
characteristics Women[70];
int Already_Checked[70];
int Husband_Id[70];

int Suitable_check(int i, int j)
{

	if((Men[i].D == Women[j].D) && (abs(Men[i].H - Women[j].H) <= 12) && (abs(Men[i].A - Women[j].A) <= 5)) {
		return true;
	}

	return false;

}

int Matching(int man_index)
{

	for (int i = 0; i < m; i++) {
		if(Suitable_check(man_index, i)) {
			if(Already_Checked[i])
				continue;

			Already_Checked[i] = 1;

			if(Husband_Id[i] < 0 or Matching(Husband_Id[i])) {
				Husband_Id[i] = man_index;
				return true;
			}

		}

	}

	return false;

}



int main()
{

	int t;
	int count;
	scanf("%d", &t);

	for (int cs = 1; cs <= t; cs++) {
		scanf("%d", &n);
		scanf("%d", &m);

		for (int i = 0; i < n; i++) {
			scanf("%d", &Men[i].H);
			scanf("%d", &Men[i].A);
			scanf("%d", &Men[i].D);
		}

		for (int i = 0; i < m; i++) {
			scanf("%d", &Women[i].H);
			scanf("%d", &Women[i].A);
			scanf("%d", &Women[i].D);
		}


		memset(Husband_Id, -1, sizeof Husband_Id);
		count = 0;

		for (int i = 0; i < n; i++) {
			memset(Already_Checked, 0, sizeof Already_Checked);

			if(Matching(i)) {
				count++;
			}

		}

		printf("Case %d: %d\n", cs, count);

	}

}
