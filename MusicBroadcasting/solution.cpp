#include <unordered_map>
#define MAX 2000000
using namespace std;

struct EMP {
	int id;
	int s, e;
}emp[5002];

int eid, mt;
int tree[MAX];
unordered_map<int, int> M;

void init(int musicTime)
{
	eid = 0;
	mt = musicTime;
	M.clear();
	for (int i = 0; i < MAX; i++) tree[i] = 0;
}

void update(int i, int val) {
	while (i < MAX) {
		tree[i] += val;
		i += i & -i;
	}
}

int get(int i) {
	int cnt = 0;
	while (i) {
		cnt += tree[i];
		i -= i & -i;
	}
	return cnt;
}

void remove(int mID)
{
	int id = M[mID];
	if (id) {
		update(emp[id].s, -1);
		update(emp[id].e - mt + 1, 1);
		M[mID] = 0;
	}
}

void add(int mID, int mStart, int mEnd)
{
	remove(mID);
	M[mID] = ++eid;
	emp[eid] = { mID, mStart, mEnd };
	update(mStart, 1);
	update(mEnd - mt + 1, -1);
}

int getCnt(int mBSTime)
{
	return get(mBSTime);
}