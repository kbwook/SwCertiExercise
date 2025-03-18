//#include "dbg.h"
#define log

#define MAX_USER 1001
#define MAX_PKGS 40001
//#define MAX_PKGS 200001
//#define MAX_PKGS 800001
#define MAX_FRIENDS 21
#define MAX_AREA 11

typedef struct Pkg {
	int pid;
	int area;
	int price;
	int isReserved;
	struct Pkg *pNex;
}PKG;
PKG pkgs[MAX_PKGS];


PKG TB[MAX_PKGS];

int pkgIdx;
PKG* getPkgNode(int pid) {
	//pkgs[pkgIdx].area = 0;
	//pkgs[pkgIdx].isReserved = 0;
	//pkgs[pkgIdx].pid = 0;
	////pkgs[pkgIdx].pNex = 0;
	//pkgs[pkgIdx].price = 0;

	return &pkgs[pkgIdx++];
}

int hashAdd(PKG* pPkg) {

	PKG* pHeader;
	int pid;

	pid = pPkg->pid % MAX_PKGS;
	pHeader = &TB[pid];

	pPkg->pNex = pHeader->pNex;
	pHeader->pNex = pPkg;

	return 0;
}

PKG* findPkgNode(int pid) {
	int tmpPid;
	PKG* pHeader;

	tmpPid = pid % MAX_PKGS;
	pHeader = &TB[tmpPid];

	pHeader = pHeader->pNex;
	while (1) {
		if (pHeader->pid == pid) {
			//break;
			return pHeader;
		}

		pHeader = pHeader->pNex;
	}
	while (1);

	return pHeader;
}

typedef struct User {
	int uid;
	int friends[MAX_FRIENDS];
	int friendCnt;
	int reservedPkg[MAX_PKGS];
	int reservedCnt;
	//int visitArea[MAX_AREA];
	int visitArea[100];
	int areaCnt;
}USER;
USER users[MAX_USER];

//#define MAX_SIZE 100
//#define MAX_SIZE MAX_PKGS
#define MAX_SIZE 12000

//int heap[MAX_SIZE];
PKG* heap[MAX_AREA][MAX_SIZE];
//int heapSize = 0;
int heapSize[MAX_AREA] = { 0, };

void heapInit(void)
{
	for (int i = 0; i < MAX_AREA; i++) {
		heapSize[i] = 0;
	}
}

//int heapPush(int value)
int comparePkg(PKG* pkg1, PKG* pkg2) {
	int ret = 0;

	if (pkg1->price < pkg2->price) {
		ret = 1;
	}
	else if (pkg1->price == pkg2->price) {
		if (pkg1->pid < pkg2->pid) {
			ret = 1;
		}
	}

	return ret;
}

int heapPush(int area, PKG* value)
{
	//if (heapSize + 1 > MAX_SIZE)
	if (heapSize[area] + 1 > MAX_SIZE)
	{
		log("queue is full!");
		return 0;
	}

	//heap[heapSize] = value;
	heap[area][heapSize[area]] = value;

	//int current = heapSize;
	int current = heapSize[area];
	//while (current > 0 && heap[current] < heap[(current - 1) / 2])
	while (current > 0 && comparePkg(heap[area][current], heap[area][(current - 1) / 2]))
	{
		//int temp = heap[(current - 1) / 2];
		PKG* temp = heap[area][(current - 1) / 2];
		//heap[(current - 1) / 2] = heap[current];
		heap[area][(current - 1) / 2] = heap[area][current];
		heap[area][current] = temp;
		current = (current - 1) / 2;
	}

	//heapSize = heapSize + 1;
	heapSize[area] = heapSize[area] + 1;

	return 1;
}

//int heapPop(int* value)
int heapPop(int area, PKG** value)
{
	//if (heapSize <= 0)
	if (heapSize[area] <= 0)
	{
		return -1;
	}

	//*value = heap[0];

	//heapSize = heapSize - 1;
	heapSize[area] = heapSize[area] - 1;

	//heap[0] = heap[heapSize];
	heap[area][0] = heap[area][heapSize[area]];

	int current = 0;
	//while (current * 2 + 1 < heapSize)
	while (current * 2 + 1 < heapSize[area])
	{
		int child;
		//if (current * 2 + 2 == heapSize)
		if (current * 2 + 2 == heapSize[area])
		{
			child = current * 2 + 1;
		}
		else
		{
			//child = heap[current * 2 + 1] < heap[current * 2 + 2] ? current * 2 + 1 : current * 2 + 2;
			child = comparePkg(heap[area][current * 2 + 1], heap[area][current * 2 + 2]) ? current * 2 + 1 : current * 2 + 2;
		}

		//if (heap[current] < heap[child])
		if (comparePkg(heap[area][current], heap[area][child]))
		{
			break;
		}

		//int temp = heap[current];
		PKG* temp = heap[area][current];
		//heap[current] = heap[child];
		heap[area][current] = heap[area][child];
		//heap[child] = temp;
		heap[area][child] = temp;

		current = child;
	}
	return 1;
}

void init(int N, int M)
{
	for (int i = 0; i < MAX_USER; i++) {
		users[i].uid = 0;
		/*for (int j = 0; j < MAX_AREA; j++) {
			users[i].visitArea[j] = 0;
		}*/
	}

	for (int i = 0; i < MAX_PKGS; i++) {
		pkgs[i].pid = 0;
	}

	pkgIdx = 0;

	heapInit();

	for (int i = 0; i < MAX_PKGS; i++) {
		TB[i].pNex = 0;
	}

}

int befriend_cnt;
void befriend(int uid1, int uid2)
{
	log("%s(%d) uid1 %d uid2 %d\n", __func__, befriend_cnt++, uid1, uid2);

	if (!users[uid1].uid) {
		users[uid1].uid = uid1;
		users[uid1].friendCnt = 0;
		users[uid1].reservedCnt = 0;
		users[uid1].areaCnt = 0;
	}

	if (!users[uid2].uid) {
		users[uid2].uid = uid2;
		users[uid2].friendCnt = 0;
		users[uid2].reservedCnt = 0;
		users[uid2].areaCnt = 0;
	}

	users[uid1].friends[users[uid1].friendCnt++] = uid2;
	users[uid2].friends[users[uid2].friendCnt++] = uid1;
}



int add_cnt;
void add(int pid, int area, int price)
{
	log("\t%s(%d) pid %d area %d price %d\n", __func__, add_cnt++, pid, area, price);
	PKG* pPkg;

	pPkg = getPkgNode(pid);
	pPkg->pid = pid;
	pPkg->area = area;
	pPkg->price = price;
	pPkg->isReserved = 0;

	heapPush(area, pPkg);
	hashAdd(pPkg);
}

int reserve_cnt;
void reserve(int uid, int pid)
{
	PKG* pPkg;

	log("\t%s(%d) uid %d, pid %d\n", __func__, reserve_cnt++, uid, pid);

	if (!users[uid].uid) {
		users[uid].uid = uid;
		users[uid].friendCnt = 0;
		users[uid].reservedCnt = 0;
		users[uid].areaCnt = 0;
	}

	users[uid].reservedPkg[users[uid].reservedCnt++] = pid;

	pPkg = findPkgNode(pid);
	pPkg->isReserved = 1;

	//users[uid].visitArea[pPkg->area] = users[uid].visitArea[pPkg->area] + 1;
	users[uid].visitArea[users[uid].areaCnt] = pPkg->area;
	users[uid].areaCnt++;
}

int recommend_cnt;
int recommend(int uid)
{
	int ret = 0;
	int visitAreaSum[MAX_AREA] = { 0, };
	PKG* pTmp = 0, *pRecom = 0;

	log("\t%s(%d) uid %d\n", __func__, recommend_cnt++, uid);

	if (!users[uid].uid) {
		users[uid].uid = uid;
		users[uid].friendCnt = 0;
		users[uid].reservedCnt = 0;
		users[uid].areaCnt = 0;
	}

	for (int j = 0; j < users[uid].areaCnt; j++) {
		visitAreaSum[users[uid].visitArea[j]]++;
	}

	for (int i = 0; i < users[uid].friendCnt; i++) {
		for (int j = 0; j < users[users[uid].friends[i]].areaCnt; j++) {
			visitAreaSum[users[users[uid].friends[i]].visitArea[j]]++;
		}
	}

	for (int i = 0; i < MAX_AREA; i++) {
		if (!heapSize[i])
			continue;

		while (1) {
			if (heap[i][0]->isReserved) {
				ret = heapPop(i, 0);
				if (ret == -1) {
					break;
				}
			}
			else {
				break;
			}
		}

		if (ret == -1) {
			continue;
		}

		if (!pRecom) {
			pRecom = heap[i][0];
		}
		else {
			if (visitAreaSum[pRecom->area] < visitAreaSum[heap[i][0]->area]) {
				pRecom = heap[i][0];
			}
			else if (visitAreaSum[pRecom->area] == visitAreaSum[heap[i][0]->area]) {
				if (comparePkg(heap[i][0], pRecom)) {
					pRecom = heap[i][0];
				}
			}
		}
	}

	return pRecom->pid;
}