#include "C:/Users/Usuario/Documents/GitHub/m41n/QBXPLRR/src/QBXPLRR.cpp"
//302
// indexing functions and move tables without symmetry reduction***********************************

// Indexes the twist of the 8 corners.
// 0 <= twist < 3^7.
unsigned short int twist(CubieCube cc)
{
	short int ret = 0;
	for (const auto& c : cc.co) ret = 3 * ret + c.o;
	return ret;
}

// Inverse of the above.
CubieCube invTwist(unsigned short int twist)
{
	CubieCube ccRet = idCube;
	int twistParity = 0;
	for (int c = DRB - 1; c >= URF; c--) { twistParity += ccRet.co[c].o = twist % 3; twist /= 3; }
	ccRet.co[DRB].o = (3 - twistParity % 3) % 3;
	return ccRet;
}

// Indexes the flip of the 12 edges.
// 0 <= flip < 2^11.
unsigned short int flip(CubieCube cc)
{
	short int ret = 0;
	for (const auto& e : cc.eo) ret = 2 * ret + e.o;
	return ret;
}

// Inverse of the above.
CubieCube invFlip(unsigned short int flip)
{
	CubieCube ccRet = idCube;
	int flipParity = 0;
	for (int e = BR - 1; e >= UR; e--) { flipParity += ccRet.eo[e].o = flip % 2; flip /= 2; }
	ccRet.eo[BR].o = (2 - flipParity % 2) % 2;
	return ccRet;
}

// Indexes the 495 unordered positions of the FR, FL, BL and BR UD-slice edges.
// 0 <= slice < (12 choose 4).
unsigned short int slice(CubieCube cc)
{
	int a = 0, x = 0;
	// Compute the index a for combination (< 12 choose 4).
	for (int j = BR; j >= UR; j--)
		if (FR <= cc.eo[j].e && cc.eo[j].e <= BR)
			a += Cnk(11 - j, x++ + 1);
	return a;
}

// Inverse of the above.
CubieCube invSlice(unsigned short int slice)
{
	int a = slice, j, x, perm[4] = { 8,9,10,11 };;
	CubieCube ccRet = idCube;
	for (auto& e : ccRet.eo) e.e = static_cast<Edge>(255);// Invalidate all edges.
	// Generate combination and set edges.
	x = 3;
	for (j = UR; j <= BR; j++) if (a - Cnk(11 - j, x + 1) >= 0) { ccRet.eo[j].e = static_cast<Edge>(perm[x]); a -= Cnk(11 - j, x-- + 1); }
	// Set the remaining edges 0..7.
	x = 0;
	for (auto& e : ccRet.eo) if (e.e == 255) e.e = static_cast<Edge>(x++);
	return ccRet;
}

/*int*/unsigned short int indexPos(
	CubieCube cc,
	int max,
	int perm_size,
	int second_j, int j_add,

	int returning)
{
	int a = 0, b = 0, x = 0, j, k, perm[perm_size];
	for (j = 0; j <= max; j++)
	{
		if (max == 7 && cc.co[j].c <= 5) { a += Cnk(j, x + 1); perm[x++] = cc.co[j].c; }
		if (max == 11 && cc.eo[j].e <= 5) { a += Cnk(j, x + 1); perm[x++] = cc.eo[j].e; }
	}
	for (j = second_j; j > 0; j--)
	{
		for (k = 0; perm[j] != j + j_add; k++) rotateLeft(perm, 0, j);
		b = (j + 1) * b + k;
	}
	return returning * a + b;
}

void indexPos_test(CubieCube cc)
{
	indexPos(cc, 7, 6, 5, 0, 720);
	indexPos(cc, 11, 6, 5, 0, 720);
	indexPos(cc, 11, 4, 3, 8, 24);
}
int edge4Pos(CubieCube cc)
{
	for (j = BR; j >= UR; j--) if (FR <= cc.eo[j].e && cc.eo[j].e <= BR) { a += Cnk(11 - j, x + 1); perm[3 - x++] = cc.eo[j].e; }
}

CubieCube invPos(
	unsigned short int idx,
	int perm[],
	int returning,
	int second_j,
	int third_j,
	int xx)
{
	int j, k, x;
	int b = idx % returning;
	int a = idx / returning;
	CubieCube ccRet = idCube;
	if (third_j == 7) for (auto& c : ccRet.co) c.c = static_cast<Corner>(255);
	if (third_j == 11) for (auto& e : ccRet.eo) e.e = static_cast<Edge>(255);
	for (j = 1; j < second_j; j++)
	{
		for (k = b % (j + 1); k > 0; k--) rotateRight(perm, 0, j);
		b /= j + 1;
	}
	x = xx;
	if (xx == 5)
	{
		for (j = third_j; j >= 0; j--)
		{
			if (third_j == 7 && a - Cnk(j, x + 1) >= 0) { ccRet.co[j].c = static_cast<Corner>(perm[x]); a -= Cnk(j, x-- + 1); }
			if (third_j == 11 && a - Cnk(j, x + 1) >= 0) { ccRet.eo[j].e = static_cast<Edge>(perm[x]); a -= Cnk(j, x-- + 1); }
		}
	}
	if (xx == 3)
	{
		for (j = UR; j <= BR; j++)
		{//mayb same but reverse order?
			if (a - Cnk(11 - j, x + 1) >= 0) { ccRet.eo[j].e = static_cast<Edge>(perm[3 - x]); a -= Cnk(11 - j, x-- + 1); }
		}
		x = 0;
		for (auto& e : ccRet.eo)if (e.e == 255) e.e = static_cast<Edge>(x++);
	}
	return ccRet;
}
void invPos_test(unsigned short int idx)
{
	invPos(idx, { 0,1,2,3,4,5 }, 720, 6, 7, 5);
	invPos(idx, { 0,1,2,3,4,5 }, 720, 6, 11, 5);
	invPos(idx, { 8,9,10,11 }, 24, 4, 11, 3);
}

// Returns 0 if cube has even corner parity, 1 if it has odd parity.
int cornerParity(CubieCube cc)
{
	int s = 0;
	for (int i = DRB; i >= URF + 1; i--)
		for (int j = i - 1; j >= URF; j--)
			if (cc.co[j].c > cc.co[i].c) s++;
	return s % 2;
}

// Returns 0 if cube has even edge parity, 1 if it has odd parity.
int edgeParity(CubieCube cc)
{
	int s = 0;
	for (int i = BR; i >= UR + 1; i--)
		for (int j = i - 1; j >= UR; j--)
			if (cc.eo[j].e > cc.eo[i].e) s++;
	return s % 2;
}

// Initializes the table[][] which gives the new ()index of a cube ()A with index() ()a after applying move ()m.
// table[a][m] = index(A*moveCube[m])
void initMove(
	int loop,
	int ppp,
	CubieCube(*invIndex)(unsigned short int twist),
	void (*multiply)(const CubieCube* a, const CubieCube* b, CubieCube* ab),
	unsigned short int (*index)(CubieCube cc),
	unsigned short int table)
{
	CubieCube a, b, c, d;
	int i, j, m;
	for (i = 0; i < loop; i++)
	{
		if (!(i % ppp)) pp();
		a = invIndex(i);
		for (j = mU1; j <= mB3; j += 2)
		{
			m = j >> 1;//extract the move axis
			multiply(&a, &basicCubeMove[m], &b);
			table[i][j] = index(b);
			multiply(&b, &basicCubeMove[m], &c);
			multiply(&c, &basicCubeMove[m], &d);
			table[i][j + 1] = index(d);
			multiply(&d, &basicCubeMove[m], &a);
		}
	}
}

void test()
{
	initMove(NTWIST, 999, &invTwist, &cornerMultiply, &twist, twistMove);
	initMove(NCORN6POS, 999, &invCorn6Pos, &cornerMultiply, &corn6Pos, corn6PosMove);
	initMove(NEDGE6POS, 40000, &invEdge6Pos, &edgeMultiply, &edge6Pos, edge6PosMove);
	initMove(NEDGE4POS, 999, &invEdge4Pos, &edgeMultiply, &edge4Pos, edge4PosMove);
}

// end indexing functions and move tables without symmetry reduction*******************************



// Creates a CubieCube from a FaceletCube.
CubieCube FaceletCubeToCubieCube(FaceletCube fc)
{
	int ori;
	CubieCube ccRet; Color col1, col2;
	for (int c1 = URF; c1 <= DRB; c1++)
	{
		//get the colors of the cubie at corner i, starting with the U/D color
		for (ori = 0; ori < 3; ori++)
		{
			if (fc.f[cornerFacelet[c1][ori]] == U ||
				fc.f[cornerFacelet[c1][ori]] == D)
			{
				break;
			}
		}
		col1 = fc.f[cornerFacelet[c1][(ori + 1) % 3]];
		col2 = fc.f[cornerFacelet[c1][(ori + 2) % 3]];

		for (int c2 = URF; c2 <= DRB; c2++)
		{
			if (col1 == FaceletToColor[cornerFacelet[c2][1]] &&
				col2 == FaceletToColor[cornerFacelet[c2][2]])
			{
				// in cornerposition i we have cornercubie j
				ccRet.co[c1].c = static_cast<Corner>(c2);
				ccRet.co[c1].o = ori;
				break;
			}
		}
	}
	for (int e1 = UR; e1 <= BR; e1++)
	{
		for (int e2 = UR; e2 <= BR; e2++)
		{
			if (fc.f[edgeFacelet[e1][0]] == FaceletToColor[edgeFacelet[e2][0]] &&
				fc.f[edgeFacelet[e1][1]] == FaceletToColor[edgeFacelet[e2][1]])
			{
				ccRet.eo[e1].e = static_cast<Edge>(e2);
				ccRet.eo[e1].o = 0;
				break;
			}
			if (fc.f[edgeFacelet[e1][0]] == FaceletToColor[edgeFacelet[e2][1]] &&
				fc.f[edgeFacelet[e1][1]] == FaceletToColor[edgeFacelet[e2][0]])
			{
				ccRet.eo[e1].e = static_cast<Edge>(e2);
				ccRet.eo[e1].o = 1;
				break;
			}
		}
	}

	return ccRet;
};