#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <inttypes.h>

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++CUBEDEFS_H

#define Color			Axis

#define NSYM_Oh			48
#define NSYM_D4h		16
#define NMOVE			12
#define NFLIP			2048
#define NSLICE			495
#define NTWIST			2187
#define NFLIPSLICE		64430
#define NCORN6POS		20160
#define NEDGE6POS		665280
#define NEDGE4POS		11880
#define NGOAL			281816820
#define NCOSETBUF		1219276800
#define NCOSET			9754214400ULL
#define NNODE			40
#define NMBITS			4096

enum Corner { URF, UFL, ULB, UBR, DFR, DLF, DBL, DRB };
Corner& operator++(Corner& corner) { return corner = static_cast<Corner>(static_cast<int>(corner) + 1); }
Corner& operator++(Corner& corner, int) { Corner corner2 = corner; ++corner; return corner2; }

enum Edge { UR, UF, UL, UB, DR, DF, DL, DB, FR, FL, BL, BR };
Edge& operator++(Edge& edge) { return edge = static_cast<Edge>(static_cast<int>(edge) + 1); }
Edge& operator++(Edge& edge, int) { Edge edge2 = edge; ++edge; return edge2; }

enum Facelet
{
	U1, U2, U3, U4, U5, U6, U7, U8, U9, R1, R2, R3, R4, R5, R6, R7, R8, R9,
	F1, F2, F3, F4, F5, F6, F7, F8, F9, D1, D2, D3, D4, D5, D6, D7, D8, D9,
	L1, L2, L3, L4, L5, L6, L7, L8, L9, B1, B2, B3, B4, B5, B6, B7, B8, B9
};

// The 12 moves in the QTM (quarterturn metric)
enum Move { mU1, mU3, mR1, mR3, mF1, mF3, mD1, mD3, mL1, mL3, mB1, mB3 };
Move& operator++(Move& move) { return move = static_cast<Move>(static_cast<int>(move) + 1); }
Move& operator++(Move& move, int) { Move move2 = move; ++move; return move2; }

// Used both for the 6 Facelet colors and the 6 axes of faceturns
enum Axis { U, R, F, D, L, B };
Axis& operator++(Axis& axis) { return axis = static_cast<Axis>(static_cast<int>(axis) + 1); }
Axis& operator++(Axis& axis, int) { Axis axis2 = axis; ++axis; return axis2; }

// all 48 symmetries of the cube are compositions of these 4 symmetries
enum BasicSym { S_URF3, S_F2, S_U4, S_LR2 };

// A cube on the Facelet level
struct FaceletCube { Color f[54]; };

// Cubies with an attached orientation
struct corner_o { Corner c; char o; };
struct edge_o { Edge e; char o; };

// A cube on the cubie level. co[x] and eo[x] gives the corner and edge cubie in position x
struct CubieCube { struct corner_o co[8]; struct edge_o eo[12]; };

// A cube on the coordinate level, see below for the definitions of the different coordinates.
struct CoordCube
{
	int symFlipSlice;		// < NFLIPSLICE*NSYM_D4h	// < 16 in target group				// < 16 for idCube
	int edge6Pos;			// < NEDGE6POS				// < 20160 in target group			// 0 for idCube
	short int edge4Pos;		// < NEDGE4POS				// < 24 in target group				// 0 for idCube
	short int twist;		// < NTWIST					// 0 in target group				// 0 for idCube
	short int corn6Pos;		// < NCORN6POS				// < NCORN6POS in target group		// 0 for idCube
	short int parity;		// < 2						// 0 in target group				// 0 for idCube
};

struct SearchNode
{
	unsigned short int flipSliceU, flipSliceR, flipSliceF;
	unsigned short int symU, symR, symF;
	unsigned short int parity;
	unsigned short int twistU, twistR, twistF;
	unsigned short int corn6Pos;
	unsigned short int edge4Pos;
	int edge6Pos;
	short int movesCloserTargetU, movesCloserTargetR, movesCloserTargetF;
	short int movesAllowed;
	short int move;
	short int distU, distR, distF;
	unsigned long long mSym;
};

// global arrays and pointers

CubieCube symCube[NSYM_Oh];
CubieCube moveCube[NMOVE];
int invSymIdx[NSYM_Oh];
int symIdxMultiply[NSYM_Oh][NSYM_Oh];
int moveConjugate[NMOVE][NSYM_Oh];
int twistConjugate[NTWIST][NSYM_D4h];
int rawFlipSliceRep[NFLIPSLICE];
char nextMove[NMBITS][NMOVE + 1];
unsigned short int twistMove[NTWIST][NMOVE];
unsigned short int corn6PosMove[NCORN6POS][NMOVE];
unsigned short int edge4PosMove[NEDGE4POS][NMOVE];
int edge6PosMove[NEDGE6POS][NMOVE];
int symFlipSliceClassMove[NFLIPSLICE][NMOVE];
short* movesCloserToTarget[NTWIST];
short moveBitsConjugate[4096][NSYM_Oh];
unsigned long long ESymmetries[NMOVE], GESymmetries[NMOVE];

char* coset;

char* visitedA;
char* visitedB;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++CUBEDEFS_H-

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++CubieCube_H

/*
The layout of the Facelets on the cube
*          |U1|U2|U3|                   *
*          |U4|U5|U6|                   *
*          |U7|U8|U9|                   *
* |L1|L2|L3|F1|F2|F3|R1|R2|R3|B1|B2|B3| *
* |L4|L5|L6|F4|F5|F6|R4|R5|R6|B4|B5|B6| *
* |L7|L8|L9|F7|F8|F9|R7|R8|R9|B7|B8|B9| *
*          |D1|D2|D3|                   *
*          |D4|D5|D6|                   *
*          |D7|D8|D9|                   *
*/

// Mapping corners {URF,UFL,ULB,UBR,DFR,DLF,DBL,DRB} to Facelets
const Facelet cornerFacelet[8][3] =
{ {U9,R1,F3},{U7,F1,L3},{U1,L1,B3},{U3,B1,R3},{D3,F9,R7},{D1,L9,F7},{D7,B9,L7},{D9,R9,B7} };

// Mapping edges {UR,UF,UL,UB,DR,DF,DL,DB,FR,FL,BL,BR} to Facelets
const Facelet edgeFacelet[12][2] =
{ {U6,R2},{U8,F2},{U4,L2},{U2,B2},{D6,R8},{D2,F8},{D4,L8},{D8,B8},{F6,R4},{F4,L6},{B6,L4},{B4,R6} };

// Mapping this definition string to Facelets
// The definiton string of the Identity cube in Singmaster notation is
// UF UR UB UL DF DR DB DL FR FL BR BL UFR URB UBL ULF DRF DFL DLB DBR
const Facelet SingmasterToFacelet[48] =
{ U8,F2,U6,R2,U2,B2,U4,L2,D2,F8,D6,R8,D8,B8,D4,L8,F6,R4,F4,L6,B4,R6,B6,L4,U9,F3,R1,U3,R3,B1,U1,B3,L1,U7,L3,F1,D3,R7,F9,D1,F7,L9,D7,L7,B9,D9,B7,R9 };

// Mapping the Facelets {U1.....B9} to the FaceletColor
const Color FaceletToColor[54] =
{ U,U,U,U,U,U,U,U,U,R,R,R,R,R,R,R,R,R,F,F,F,F,F,F,F,F,F,D,D,D,D,D,D,D,D,D,L,L,L,L,L,L,L,L,L,B,B,B,B,B,B,B,B,B };

const char ColorToChar[] = { "URFDLB" };

// Cubie level faceturns in the "is replaced by" representation
const CubieCube basicCubeMove[6] =
{
	{{{UBR,0},{URF,0},{UFL,0},{ULB,0},{DFR,0},{DLF,0},{DBL,0},{DRB,0}},
	{{UB,0},{UR,0},{UF,0},{UL,0},{DR,0},{DF,0},{DL,0},{DB,0},{FR,0},{FL,0},{BL,0},{BR,0}}},// U
	{{{DFR,2},{UFL,0},{ULB,0},{URF,1},{DRB,1},{DLF,0},{DBL,0},{UBR,2}},
	{{FR,0},{UF,0},{UL,0},{UB,0},{BR,0},{DF,0},{DL,0},{DB,0},{DR,0},{FL,0},{BL,0},{UR,0}}},// R
	{{{UFL,1},{DLF,2},{ULB,0},{UBR,0},{URF,2},{DFR,1},{DBL,0},{DRB,0}},
	{{UR,0},{FL,1},{UL,0},{UB,0},{DR,0},{FR,1},{DL,0},{DB,0},{UF,1},{DF,1},{BL,0},{BR,0}}},// F
	{{{URF,0},{UFL,0},{ULB,0},{UBR,0},{DLF,0},{DBL,0},{DRB,0},{DFR,0}},
	{{UR,0},{UF,0},{UL,0},{UB,0},{DF,0},{DL,0},{DB,0},{DR,0},{FR,0},{FL,0},{BL,0},{BR,0}}},// D
	{{{URF,0},{ULB,1},{DBL,2},{UBR,0},{DFR,0},{UFL,2},{DLF,1},{DRB,0}},
	{{UR,0},{UF,0},{BL,0},{UB,0},{DR,0},{DF,0},{FL,0},{DB,0},{FR,0},{UL,0},{DL,0},{BR,0}}},// L
	{{{URF,0},{UFL,0},{UBR,1},{DRB,2},{DFR,0},{DLF,0},{ULB,2},{DBL,1}},
	{{UR,0},{UF,0},{UL,0},{BR,1},{DR,0},{DF,0},{DL,0},{BL,1},{FR,0},{FL,0},{UB,1},{DB,1}}},// B
};

// The Identitiy Cube
const CubieCube idCube =
{
	{{URF,0},{UFL,0},{ULB,0},{UBR,0},{DFR,0},{DLF,0},{DBL,0},{DRB,0}},
	{{UR,0},{UF,0},{UL,0},{UB,0},{DR,0},{DF,0},{DL,0},{DB,0},{FR,0},{FL,0},{BL,0},{BR,0}}
};

// Computes the composition of the permutations a and b including orientations.
void cornerMultiply(const CubieCube* a, const CubieCube* b, CubieCube* ab)
//The result of the multiplication is the composition of the the permutations
//a and b. Because we also describe reflections of the whole cube by
//permutations, we get a complication with the corners. The orientation of
//mirrored corners is described by the numbers 3, 4 and 5. The composition of
//the orientations cannot be computed by addition modulo three in the cyclic
//group C3 any more. Instead the rules below give an addition in the dihedral
//group D3 with 6 elements.
{
	char ori, oriA, oriB;
	Corner crn;
	ori = 0;
	for (crn = URF; crn <= DRB; crn++)
	{
		ab->co[crn].c = a->co[b->co[crn].c].c;
		oriA = a->co[b->co[crn].c].o;
		oriB = b->co[crn].o;
		if (oriA < 3 && oriB < 3)       //if both cubes are regular cubes... 
		{
			ori = oriA + oriB;     //just do an addition modulo 3 here  
			if (ori >= 3) ori -= 3;    //the composition is a regular cube 
		}
		else if (oriA < 3 && oriB >= 3) //if cube b is in a mirrored state...
		{
			ori = oriA + oriB;
			if (ori >= 6) ori -= 3;	    //the composition is a mirrored cube
		}
		else if (oriA >= 3 && oriB < 3) //if cube a is an a mirrored state...
		{
			ori = oriA - oriB;
			if (ori < 3) ori += 3;		//the composition is a mirrored cube	
		}
		else if (oriA >= 3 && oriB >= 3) //if both cubes are in mirrored states...
		{
			ori = oriA - oriB;
			if (ori < 0) ori += 3;		//the composition is a regular cube
		}
		ab->co[crn].o = ori;
	};
}

// Computes the composition of the permutations a and b including orientations.
void edgeMultiply(const CubieCube* a, const CubieCube* b, CubieCube* ab)
// With edges we have no complications with mirror images,
// because the mirror image of an edgecubie looks the same as the edgecubie itself.
// So we only have the orientations 0 and 1 and do an addition modulo 2 in all cases.
{
	for (Edge edg = UR; edg <= BR; edg++)
	{
		ab->eo[edg].e = a->eo[b->eo[edg].e].e;
		ab->eo[edg].o = (b->eo[edg].o + a->eo[b->eo[edg].e].o) % 2;
	};
}

// Do a U,R,F,D,L, or B move on the CubieCube level.
CubieCube cubeAxMove(CubieCube cc, Axis ax)
{
	CubieCube ccRet;
	cornerMultiply(&cc, &basicCubeMove[ax], &ccRet);
	edgeMultiply(&cc, &basicCubeMove[ax], &ccRet);
	return ccRet;
};

// Initialize the 12 QTM-moves on the CubieCube level.
void initMoveCubes()
{
	CubieCube cc;
	for (Axis i = U; i <= B; i++)
	{
		cc = moveCube[2 * i] = basicCubeMove[i];//ax
		cc = cubeAxMove(cc, i);//ax^2
		moveCube[2 * i + 1] = cubeAxMove(cc, i);//ax^3
	}
}

// Creates a FaceletCube from a CubieCube.
FaceletCube CubieCubeToFaceletCube(CubieCube cc)
{
	FaceletCube fcRet;
	Corner c2; Edge e2; unsigned char o;
	int n;
	for (Corner c1 = URF; c1 <= DRB; c1++)
	{
		c2 = cc.co[c1].c; o = cc.co[c1].o;
		for (n = 0; n < 3; n++) fcRet.f[cornerFacelet[c1][(n + o) % 3]] = FaceletToColor[cornerFacelet[c2][n]];
	}
	for (Edge e1 = UR; e1 <= BR; e1++)
	{
		e2 = cc.eo[e1].e; o = cc.eo[e1].o;
		for (n = 0; n < 2; n++) fcRet.f[edgeFacelet[e1][(n + o) % 2]] = FaceletToColor[edgeFacelet[e2][n]];
	}
	// Set center colors
	fcRet.f[U5] = U; fcRet.f[R5] = R; fcRet.f[F5] = F; fcRet.f[D5] = D; fcRet.f[L5] = L; fcRet.f[B5] = B;

	return fcRet;
}

// Creates a CubieCube from a FaceletCube.
CubieCube FaceletCubeToCubieCube(FaceletCube fc)
{
	Corner i, j; Edge k, m; int ori;
	CubieCube ccRet; Color col1, col2;
	for (i = URF; i <= DRB; i++)
	{
		//get the colors of the cubie at corner i, starting with the U/D color
		for (ori = 0; ori < 3; ori++)
			if (fc.f[cornerFacelet[i][ori]] == U || fc.f[cornerFacelet[i][ori]] == D)
				break;
		col1 = fc.f[cornerFacelet[i][(ori + 1) % 3]];
		col2 = fc.f[cornerFacelet[i][(ori + 2) % 3]];

		for (j = URF; j <= DRB; j++)
		{
			if (col1 == FaceletToColor[cornerFacelet[j][1]]
				&& col2 == FaceletToColor[cornerFacelet[j][2]])
			{
				// in cornerposition i we have cornercubie j
				ccRet.co[i].c = j;
				ccRet.co[i].o = ori;
				break;
			}
		}
	}
	for (k = UR; k <= BR; k++)
		for (m = UR; m <= BR; m++)
		{
			if (fc.f[edgeFacelet[k][0]] == FaceletToColor[edgeFacelet[m][0]] &&
				fc.f[edgeFacelet[k][1]] == FaceletToColor[edgeFacelet[m][1]])
			{
				ccRet.eo[k].e = m;
				ccRet.eo[k].o = 0;
				break;
			}
			if (fc.f[edgeFacelet[k][0]] == FaceletToColor[edgeFacelet[m][1]] &&
				fc.f[edgeFacelet[k][1]] == FaceletToColor[edgeFacelet[m][0]])
			{
				ccRet.eo[k].e = m;
				ccRet.eo[k].o = 1;
				break;
			}
		}
	return ccRet;
};

// Creates a description string in Singmaster notation from a CubieCube.
void CubieCubeToString(CubieCube cc, char* defString)
{
	FaceletCube fc = CubieCubeToFaceletCube(cc);
	int i, j = 0;
	for (i = 0; i < 48; i++)
	{
		defString[i + j] = ColorToChar[fc.f[SingmasterToFacelet[i]]];
		// Add spaces if necessary
		if ((i < 24 && i & 1) || (i > 24 && (i + 1) % 3 == 0)) defString[i + ++j] = ' ';
	}
	defString[i + j - 1] = 0;
}

// Creates a CubieCube from a string. The string may be a maneuver or a description of the cube Facelets in Singmaster notation.
CubieCube StringToCubieCube(char* defString)
{
	CubieCube ccRet = idCube;
	int i, j, n, fCount[7] = { 0,0,0,0,0,0,0 };

	if (char* sp = strstr(defString, "(")) *sp = 0;// Remove maneuver length if present
	if (char* sp = strstr(defString, "//")) *sp = 0;// Remove comment if present

	n = strlen(defString);
	for (i = 0; i < n; i++)
	{// Check if definition string has Singmaster notation by counting chars
		switch (toupper(defString[i]))
		{
		case 'U': fCount[0]++; break;
		case 'R': fCount[1]++; break;
		case 'F': fCount[2]++; break;
		case 'D': fCount[3]++; break;
		case 'L': fCount[4]++; break;
		case 'B': fCount[5]++; break;
		case '\'':case '2': fCount[6]++; break;
		}
	}

	if (fCount[0] == 8 && fCount[1] == 8 && fCount[2] == 8 && fCount[3] == 8 && fCount[4] == 8 && fCount[5] == 8 && fCount[6] == 0)
	{// Singmaster notation
		FaceletCube fc;
		j = 0;
		for (i = 0; i < n; i++)
			switch (toupper(defString[i]))
			{
			case 'U': fc.f[SingmasterToFacelet[j++]] = U; break;
			case 'R': fc.f[SingmasterToFacelet[j++]] = R; break;
			case 'F': fc.f[SingmasterToFacelet[j++]] = F; break;
			case 'D': fc.f[SingmasterToFacelet[j++]] = D; break;
			case 'L': fc.f[SingmasterToFacelet[j++]] = L; break;
			case 'B': fc.f[SingmasterToFacelet[j++]] = B; break;
			}
		ccRet = FaceletCubeToCubieCube(fc);
	}
	else
	{// Maneuver notation
		Axis ax = U;
		int pow;
		for (i = 0; i < n; i++)
		{
			switch (toupper(defString[i]))
			{
			case 'U': ax = U; break;
			case 'R': ax = R; break;
			case 'F': ax = F; break;
			case 'D': ax = D; break;
			case 'L': ax = L; break;
			case 'B': ax = B; break;
			default: continue;
			}
			if (i == n) { ccRet = cubeAxMove(ccRet, ax); break; }
			switch (defString[i + 1])
			{
			case '3':case'\'':case'-': pow = 3; break;
			case '2': pow = 2; break;
			default: pow = 1;
			}
			for (j = 0; j < pow; j++) ccRet = cubeAxMove(ccRet, ax);
		}
	}

	return ccRet;
}

// Computes the composition of the permutations a and b including orientations.
void multiply(const CubieCube* a, const CubieCube* b, CubieCube* ab)
{
	cornerMultiply(a, b, ab);
	edgeMultiply(a, b, ab);
}

// Computes the inverse cc^-1 of cc, so we have cc^-1*cc = idCube.
CubieCube invCubieCube(CubieCube cc)
{
	CubieCube ccInv;
	Edge edg; Corner crn;
	int o;// Orientation

	for (edg = UR; edg <= BR; edg++) ccInv.eo[cc.eo[edg].e].e = edg;
	for (edg = UR; edg <= BR; edg++) ccInv.eo[edg].o = cc.eo[ccInv.eo[edg].e].o;

	for (crn = URF; crn <= DRB; crn++) ccInv.co[cc.co[crn].c].c = crn;
	for (crn = URF; crn <= DRB; crn++)
	{
		o = cc.co[ccInv.co[crn].c].o;
		if (o >= 3) ccInv.co[crn].o = o;// Just for completeness, no need to invert mirrored cubes here.
		else // The usual case
		{
			ccInv.co[crn].o = -o;
			if (ccInv.co[crn].o < 0) ccInv.co[crn].o += 3;
		}
	}

	return ccInv;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++CubieCube_H-

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++COORDCUBE_H

//extern CubieCube basicCubeMove[6];
//extern CubieCube idCube;

// Helper function
// Binomial coefficient n choose k.
int Cnk(unsigned char n, unsigned char k)
{
	int s;
	unsigned char i, j;
	if (n < k) return 0;
	if (k > n / 2) k = n - k;
	for (s = 1, i = n, j = 1; i != n - k; i--, j++) { s *= i; s /= j; }
	return s;
}

void rotateLeft(int arr[], int l, int r)
//Left rotation of all array elements between l and r
{
	int temp = arr[l];
	for (int i = l; i < r; i++) arr[i] = arr[i + 1];
	arr[r] = temp;
}
void rotateRight(int arr[], int l, int r)
//Right rotation of all array elements between l and r
{
	int temp = arr[r];
	for (int i = r; i > l; i--) arr[i] = arr[i - 1];
	arr[l] = temp;
}

// Helper function
// Rotation of all array elements between l and r to the right(b==true) or to the left(b==false).
void RotateAE(int arr[], int l, int r, bool b)
{
	int temp = arr[l];
	if (b) for (int i = r; i > l; i--) arr[i] = arr[i - 1];
	else for (int i = l; i < r; i++) arr[i] = arr[i + 1];
	arr[r] = temp;
}

//**************************end helper functions*******************************
//*****************************************************************************


//********indexing functions and move tables without symmetry reduction********
//*****************************************************************************

// Indexes the twist of the 8 corners.
// 0 <= twist < 3^7.
unsigned short int twist(CubieCube cc)
{
	short int ret = 0;
	for (Corner c = URF; c < DRB; c++) ret = 3 * ret + cc.co[c].o;
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
	for (Edge e = UR; e < BR; e++) ret = 2 * ret + cc.eo[e].o;
	return ret;
}

// Inverse of the above.
CubieCube invFlip(unsigned short int flip)
{
	CubieCube ccRet = idCube;;
	int flipParity = 0;
	for (int e = BR - 1; e >= UR; e--) { flipParity += ccRet.eo[e].o = flip % 2; flip /= 2; }
	ccRet.eo[BR].o = (2 - flipParity % 2) % 2;
	return ccRet;
}

// Indexes the 495 unordered positions of the FR, FL, BL and BR UD-slice edges.
// 0 <= slice < (12 choose 4).
unsigned short int slice(CubieCube cc)
{
	int a = 0, x = 0, j;
	// Compute the index a for combination (< 12 choose 4).
	for (j = BR; j >= UR; j--)
		if (FR <= cc.eo[j].e && cc.eo[j].e <= BR)
			a += Cnk(11 - j, x++ + 1);
	return a;
}

// Inverse of the above.
CubieCube invSlice(unsigned short int slice)
{
	int a = slice, j, x, perm[4] = { 8,9,10,11 };;
	CubieCube ccRet = idCube;
	for (j = UR; j <= BR; j++) ccRet.eo[j].e = static_cast<Edge>(255);// Invalidate all edges.

	x = 3;// Generate combination and set edges.
	for (j = UR; j <= BR; j++)
		if (a - Cnk(11 - j, x + 1) >= 0) { ccRet.eo[j].e = static_cast<Edge>(perm[x]); a -= Cnk(11 - j, x-- + 1); }
	for (j = UR, x = 0; j <= BR; j++)// Set the remaining edges 0..7.
		if (ccRet.eo[j].e == 255) ccRet.eo[j].e = static_cast<Edge>(x++);
	return ccRet;
}

// Indexes the ordered positions of the URF,UFL,ULB,UBR,DFR and DLF corner.
// 0 <= corn6Pos < 20160.
unsigned short int corn6Pos(CubieCube cc)
{
	int a = 0, b = 0, x = 0, j, k, perm[6];
	// Compute the index a < (8 choose 6) and the permutation array perm.
	for (j = URF; j <= DRB; j++)
		if (cc.co[j].c <= DLF) { a += Cnk(j, x + 1); perm[x++] = cc.co[j].c; }

	for (j = 5; j > 0; j--)// Compute the index b < 6! for the permutation in perm.
	{
		k = 0;
		while (perm[j] != j) { rotateLeft(perm, 0, j); k++; }
		b = (j + 1) * b + k;
	}

	return 720 * a + b;
}

// Inverse of the above.
CubieCube invCorn6Pos(unsigned short int idx)
{
	int j, k, x, perm[6] = { 0,1,2,3,4,5 };
	int b = idx % 720;	// Permutation.
	int a = idx / 720;	// Combination.
	CubieCube ccRet = idCube;
	for (j = URF; j <= DRB; j++) ccRet.co[j].c = static_cast<Corner>(255);// Invalidate all corners.

	for (j = 1; j < 6; j++)// Generate permutation from index b.
	{
		k = b % (j + 1); b /= j + 1;
		while (k-- > 0) rotateRight(perm, 0, j);
	}
	x = 5;// Generate combination and set corners.
	for (j = DRB; j >= 0; j--)
		if (a - Cnk(j, x + 1) >= 0) { ccRet.co[j].c = static_cast<Corner>(perm[x]); a -= Cnk(j, x-- + 1); }

	return ccRet;
}

// Indexes the 665280 ordered positions of the UR,UF,UL,UB,DR and DF edges.
// If the six edges are all in the U-face and D-face the index is <20160.
int edge6Pos(CubieCube cc)
{
	int a = 0, b, x = 0, j, k, perm[6];
	// Compute the index a < (12 choose 6) and the permutation array perm.
	for (j = UR; j <= BR; j++)
		if (cc.eo[j].e <= DF) { a += Cnk(j, x + 1); perm[x++] = cc.eo[j].e; }

	for (j = 5, b = 0; j > 0; j--)// Compute the index b < 6! for the permutation in perm.
	{
		k = 0;
		while (perm[j] != j) { rotateLeft(perm, 0, j); k++; }
		b = (j + 1) * b + k;
	}

	return 720 * a + b;
}
// Inverse of the above.
CubieCube invEdge6Pos(int idx)
{
	int j, k, x, perm[6] = { 0,1,2,3,4,5 };
	int b = idx % 720;	// Permutation 
	int a = idx / 720;	// Combination
	CubieCube ccRet = idCube;
	for (j = UR; j <= BR; j++) ccRet.eo[j].e = static_cast<Edge>(255);// Invalidate all edges.

	for (j = 1; j < 6; j++)// Generate permutation from index b.
	{
		k = b % (j + 1); b /= j + 1;
		while (k-- > 0) rotateRight(perm, 0, j);
	}
	x = 5;// Generate combination and set edges.
	for (j = BR; j >= 0; j--)
		if (a - Cnk(j, x + 1) >= 0) { ccRet.eo[j].e = static_cast<Edge>(perm[x]); a -= Cnk(j, x-- + 1); }

	return ccRet;
}

// Indexes the 11880 ordered positions of the FR,FL,BL and BR edges.
// Iff the 4 edges are all in the UD-slice the index is <24.
int edge4Pos(CubieCube cc)
{
	int a = 0, b, x = 0, j, k, perm[4];
	//compute the index a < (12 choose 4) and the permutation array perm.
	//for (j=UR;j<=BR;j++)
	//if (cc.eo[j].e<=DF) {a += Cnk(j,x+1);perm[x++] = cc.eo[j].e;}
	for (j = BR; j >= UR; j--)
		if (FR <= cc.eo[j].e && cc.eo[j].e <= BR) { a += Cnk(11 - j, x + 1); perm[3 - x++] = cc.eo[j].e; }

	for (j = 3, b = 0; j > 0; j--)// Compute the index b < 4! for the permutation in perm.
	{
		k = 0;
		while (perm[j] != j + 8) { rotateLeft(perm, 0, j); k++; }
		b = (j + 1) * b + k;
	}

	return 24 * a + b;
}

// Inverse of the above.
CubieCube invEdge4Pos(int idx)
{
	int j, k, x, perm[4] = { 8,9,10,11 };
	int b = idx % 24;	// Permutation 
	int a = idx / 24;	// Combination
	CubieCube ccRet = idCube;
	for (j = UR; j <= BR; j++) ccRet.eo[j].e = static_cast<Edge>(255);// Invalidate all edges.

	for (j = 1; j < 4; j++)// Generate permutation from index b.
	{
		k = b % (j + 1); b /= j + 1;
		while (k-- > 0) rotateRight(perm, 0, j);
	}

	x = 3;// Generate combination and set edges.
	for (j = UR; j <= BR; j++)
		if (a - Cnk(11 - j, x + 1) >= 0) { ccRet.eo[j].e = static_cast<Edge>(perm[3 - x]); a -= Cnk(11 - j, x-- + 1); }

	for (j = UR, x = 0; j <= BR; j++)// Set the remaining edges 0..7.
		if (ccRet.eo[j].e == 255) ccRet.eo[j].e = static_cast<Edge>(x++);

	return ccRet;
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

//Initializes the twistMove table which gives the new twist of a cube A
//with twist a after applying move m.
//twist(A*moveCube[m]) = twistMove[a][m]
void initTwistMove()
{
	CubieCube a, b, c, d;
	int i, j, m;
	for (i = 0; i < NTWIST; i++)
	{
		a = invTwist(i);
		for (j = mU1; j <= mB3; j += 2)
		{
			m = j >> 1;//extract the move axis
			cornerMultiply(&a, &basicCubeMove[m], &b);
			twistMove[i][j] = twist(b);
			cornerMultiply(&b, &basicCubeMove[m], &c);
			cornerMultiply(&c, &basicCubeMove[m], &d);
			twistMove[i][j + 1] = twist(d);
			cornerMultiply(&d, &basicCubeMove[m], &a);
		}
	}
}

// Similar to initTwistMove().
void initCorn6PosMove()
{
	CubieCube a, b, c, d;
	int i, j, m;
	for (i = 0; i < NCORN6POS; i++)
	{
		a = invCorn6Pos(i);
		for (j = mU1; j <= mB3; j += 2)
		{
			m = j >> 1;// Extract the move axis.
			cornerMultiply(&a, &basicCubeMove[m], &b);
			corn6PosMove[i][j] = corn6Pos(b);
			cornerMultiply(&b, &basicCubeMove[m], &c);
			cornerMultiply(&c, &basicCubeMove[m], &d);
			corn6PosMove[i][j + 1] = corn6Pos(d);
			cornerMultiply(&d, &basicCubeMove[m], &a);
		}
	}
}

// Print a "." to stdout.
void pp() { printf("."); fflush(stdout); }

// Similar to initTwistMove().
void initEdge6PosMove()
{
	CubieCube a, b, c, d;
	int i, j, m;
	for (i = 0; i < NEDGE6POS; i++)
	{
		if (!(i % 40000)) pp();
		a = invEdge6Pos(i);
		for (j = mU1; j <= mB3; j += 2)
		{
			m = j >> 1;// Extract the move axis.
			edgeMultiply(&a, &basicCubeMove[m], &b);
			edge6PosMove[i][j] = edge6Pos(b);
			edgeMultiply(&b, &basicCubeMove[m], &c);
			edgeMultiply(&c, &basicCubeMove[m], &d);
			edge6PosMove[i][j + 1] = edge6Pos(d);
			edgeMultiply(&d, &basicCubeMove[m], &a);
		}
	}
}

// Similar to initTwistMove().
void initEdge4PosMove()
{
	CubieCube a, b, c, d;
	int i, j, m;
	for (i = 0; i < NEDGE4POS; i++)
	{
		a = invEdge4Pos(i);
		for (j = mU1; j <= mB3; j += 2)
		{
			m = j >> 1;//extract the move axis
			edgeMultiply(&a, &basicCubeMove[m], &b);
			edge4PosMove[i][j] = edge4Pos(b);
			edgeMultiply(&b, &basicCubeMove[m], &c);
			edgeMultiply(&c, &basicCubeMove[m], &d);
			edge4PosMove[i][j + 1] = edge4Pos(d);
			edgeMultiply(&d, &basicCubeMove[m], &a);
		}
	}
}

//******end indexing functions and move tables without symmetry reduction******
//*****************************************************************************


//*********indexing functions and move tables with symmetry reduction**********
//*****************************************************************************

CubieCube edgeConjugate(CubieCube cc, int symIdx);//HUH¿

// Indexes the 2048*495 possible configurations of the flip + slice coordinates.
// These are divided into 64430 equivalence classes with respect to the 16 symmetries of the cube (subgroup Dh4), which preserve the UD-axis.
// The value 16*classIndex+symmmetryIndex is returned.
// 2048*495=1013670 while 64430*16=1030880. This shows that some symFLipSlice coordinates describe the same configuration.
// This happens because there are equivalence classes where the cubes have some symmetry itself.
int symFlipSlice(CubieCube cc)
{
	int symIdx = 0, classIdx, rawFlipSlice, rep = NFLIP * NSLICE, rBound, lBound, i;
	CubieCube ccRep;
	for (i = 0; i < NSYM_D4h; i++)
	{
		ccRep = edgeConjugate(cc, i);
		rawFlipSlice = NFLIP * slice(ccRep) + flip(ccRep);
		if (rawFlipSlice < rep) { rep = rawFlipSlice; symIdx = i; }
	}
	//rep not holds the rawFlipSlice coordinate of the representant ccRep
	//in the equivalence class of cube cc.
	//We have cc = symCube[idx]^-1*ccRep*symCube[idx]
	//Get the index of the ccRep-equivalence class from table rawFlipSliceRep now:
	rBound = NFLIPSLICE;
	lBound = 0;
	classIdx = -1;
	do
	{
		classIdx = (lBound + rBound) / 2;
		if (rep < rawFlipSliceRep[classIdx]) rBound = classIdx;
		else if (rep > rawFlipSliceRep[classIdx]) lBound = classIdx;
	} while (rep != rawFlipSliceRep[classIdx]);
	return (classIdx << 4) + symIdx;
}

// Initializes the symFlipSliceClassMove table.
// If idx is the index of a equivalence class and m a move to apply,
// symFlipSliceClassMove[idx][m] gives the symFlipSlice coordinate of the resulting cube.
void initSymFlipSliceClassMove()
{
	int i, j, rep, flip, slice, m, n;
	CubieCube ccFlip, ccSlice, b, c, d;
	for (i = 0; i < NFLIPSLICE; i++)
	{
		if (!(i % 2000)) pp();
		rep = rawFlipSliceRep[i];
		flip = rep % NFLIP;
		slice = rep / NFLIP;
		ccSlice = invSlice(slice);
		ccFlip = invFlip(flip);
		for (n = 0; n < 12; n++) ccFlip.eo[n].e = ccSlice.eo[n].e;//merge cubes
		for (j = mU1; j <= mB3; j += 2)
		{
			m = j >> 1;// extract move axis.
			edgeMultiply(&ccFlip, &basicCubeMove[m], &b);
			symFlipSliceClassMove[i][j] = symFlipSlice(b);
			edgeMultiply(&b, &basicCubeMove[m], &c);
			edgeMultiply(&c, &basicCubeMove[m], &d);
			symFlipSliceClassMove[i][j + 1] = symFlipSlice(d);
			edgeMultiply(&d, &basicCubeMove[m], &ccFlip);
		}
	}
}

//computes the new symFlipSlice coordinate after applying move m.
//Because there are 64430*16 different symFlipSlice coordinates, the value is
//computed by a function and not realized as a table like twistMove.
int symFlipSliceMove(int a, int m)
{
	int aSym, aClass, bSym, bClass, mConj, b;
	//With the abbreviations A = invSymFlipSlice(a),
	//M = moveCube[m] and ASYM = symCube[aSym]
	//we have A = ASYM^-1*ACLASS*ASYM with some representant ACLASS
	//so A*M = ASYM^-1*ACLASS*ASYM*M =  ASYM^-1*ACLASS*(ASYM *M*ASYM^-1)*ASYM
	//Defining MCONJ = ASYM *M*ASYM^-1
	//we have A*M = ASYM^-1*ACLASS*MCONJ*ASYM
	//Define B = ACLASS*MCONJ then we have A*M = ASYM^-1*B*ASYM
	//With the abbreviation BSYM = symCube[bSym]
	//We have B = BSYM^-1*BCLASS*BSYM with some representant BCLASS
	//So A*M = ASYM^-1* BSYM^-1*BCLASS*BSYM*ASYM 
	//= (BSYM*ASYM)^-1*BCLASS*(BSYM*ASYM)
	aClass = a >> 4; aSym = a & 15;
	mConj = moveConjugate[m][aSym];
	b = symFlipSliceClassMove[aClass][mConj];
	bClass = b >> 4; bSym = b & 15;
	return (bClass << 4) + symIdxMultiply[bSym][aSym];
}

// Creates a coordCube from a CubieCube.
CoordCube CubieCubeToCoordCube(CubieCube cc)
{
	CoordCube co;
	co.corn6Pos = corn6Pos(cc);
	co.edge6Pos = edge6Pos(cc);
	co.edge4Pos = edge4Pos(cc);
	co.parity = cornerParity(cc);
	co.symFlipSlice = symFlipSlice(cc);
	co.twist = twist(cc);
	return co;
}

//*******end indexing functions and move tables with symmetry reduction********
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++COORDCUBE_H-

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++SYMMETRY_H

//extern CubieCube idCube;

// Basic cubie level rotations and reflections.
// The orientation of a reflected corner has values from 3 to 5.
const CubieCube basicSymCube[4] =
{
	{{{URF,1},{DFR,2},{DLF,1},{UFL,2},{UBR,2},{DRB,1},{DBL,2},{ULB,1}},
	{{UF,1},{FR,0},{DF,1},{FL,0},{UB,1},{BR,0},{DB,1},{BL,0},{UR,1},{DR,1},{DL,1},{UL,1}}},// S_URF3
	{{{DLF,0},{DFR,0},{DRB,0},{DBL,0},{UFL,0},{URF,0},{UBR,0},{ULB,0}},
	{{DL,0},{DF,0},{DR,0},{DB,0},{UL,0},{UF,0},{UR,0},{UB,0},{FL,0},{FR,0},{BR,0},{BL,0}}},// S_F2
	{{{UBR,0},{URF,0},{UFL,0},{ULB,0},{DRB,0},{DFR,0},{DLF,0},{DBL,0}},
	{{UB,0},{UR,0},{UF,0},{UL,0},{DB,0},{DR,0},{DF,0},{DL,0},{BR,1},{FR,1},{FL,1},{BL,1}}},// S_U4
	{{{UFL,3},{URF,3},{UBR,3},{ULB,3},{DLF,3},{DFR,3},{DRB,3},{DBL,3}},
	{{UL,0},{UF,0},{UR,0},{UB,0},{DL,0},{DF,0},{DR,0},{DB,0},{FL,0},{FR,0},{BR,0},{BL,0}}},// S_LR2
};

// Initializes the 48 cube symmetries of the cube on the CubieCube level.
// For an index x < 48, symCube[x] gives the corresponding symmetry of the cube.
void initSymCubes()
{
	int a, b, c, d, idx = 0;
	CubieCube cb, ci = idCube;
	for (a = 0; a < 3; a++)
	{// S_URF3
		for (b = 0; b < 2; b++)
		{// S_F2
			for (c = 0; c < 4; c++)
			{// S_U4
				for (d = 0; d < 2; d++)
				{// S_LR2
					symCube[idx++] = ci;
					multiply(&ci, &basicSymCube[S_LR2], &cb);
					ci = cb;
				}
				multiply(&ci, &basicSymCube[S_U4], &cb);
				ci = cb;
			}
			multiply(&ci, &basicSymCube[S_F2], &cb);
			ci = cb;
		}
		multiply(&ci, &basicSymCube[S_URF3], &cb);
		ci = cb;
	}
}

// Initializes the invSymIdx table.
// For a symmetry index x, invSymIdx[x] gives the index of the inverse symmetry:
// symCube[x]*symCube[invSymIdx[x]] = IdCube.
void initInvSymIdx()
{
	CubieCube cc;
	for (int j = 0; j < NSYM_Oh; j++)
		for (int k = 0; k < NSYM_Oh; k++)
		{
			cornerMultiply(&symCube[j], &symCube[k], &cc);
			if (cc.co[URF].c == URF && cc.co[UFL].c == UFL && cc.co[ULB].c == ULB)
			{
				invSymIdx[j] = k; break;
			}
		}
}

// Initalizes the symIdxMultiply table.
// For two symmetry indices x and y, we compute the index of the product:
// symCube[symIdxMultiply[x][y]] = symCube[x]*symCube[y].
void initSymIdxMultiply()
{
	CubieCube cc;
	for (int i = 0; i < NSYM_Oh; i++)
		for (int j = 0; j < NSYM_Oh; j++)
		{
			edgeMultiply(&symCube[i], &symCube[j], &cc);
			for (int k = 0; k < NSYM_Oh; k++)//find the product cube
			{
				if (symCube[k].eo[UR].e == cc.eo[UR].e &&
					symCube[k].eo[UF].e == cc.eo[UF].e)
				{
					symIdxMultiply[i][j] = k; break;
				}
			}
		}
}

// Computes the conjugate cube symCube(idx)*cc*symCube(idx)^-1 for edges only.
CubieCube edgeConjugate(CubieCube cc, int symIdx)
{
	CubieCube ccTmp;
	edgeMultiply(&symCube[symIdx], &cc, &ccTmp);
	edgeMultiply(&ccTmp, &symCube[invSymIdx[symIdx]], &cc);
	return cc;
}

// Computes the conjugate cube symCube(idx)*cc*symCube(idx)^-1 for corners only.
CubieCube cornerConjugate(CubieCube cc, int symIdx)
{
	CubieCube ccTmp;
	cornerMultiply(&symCube[symIdx], &cc, &ccTmp);
	cornerMultiply(&ccTmp, &symCube[invSymIdx[symIdx]], &cc);
	return cc;
}

//Initializes the moveConjugate table. If m is a move and x the index of a
//symmetry, moveConjugate[m][x] gives the move m' which is the conjugation of
//move m by the the symmetry x:
//moveCube[m'] = symCube[x]*moveCube[m]*symCube[x]^-1
void initMoveConjugate()
{
	CubieCube cc;
	int i, j, k, l, match;

	for (i = 0; i < NMOVE; i++)
		for (j = 0; j < NSYM_Oh; j++)
		{
			cc = edgeConjugate(moveCube[i], j);
			for (k = 0; k < NMOVE; k++)//find the conjugate cube cube
			{
				match = 1;
				//test all edges
				for (l = UR; l <= BR; l++) if (moveCube[k].eo[l].e != cc.eo[l].e) { match = 0; break; }
				if (match == 1) { moveConjugate[i][j] = k; break; }
			}
		}
}

//Initializes the twistConjugate table. If A is a cube with twist t and s is a
//symmetry, twistConjugate[t][s] gives the twist t' of the conjugate cube.
//invTwist(t') = symCube[s]*invTwist(t)*symCube[s]^-1
void initTwistConjugate()
{
	CubieCube cc, ccConj;
	int i, j;

	for (i = 0; i < NTWIST; i++)
	{
		cc = invTwist(i);
		for (j = 0; j < NSYM_D4h; j++)
		{
			ccConj = cornerConjugate(cc, j);
			twistConjugate[i][j] = twist(ccConj);
		}
	}
}


//Two cubes A and B are called equivalent, if conjugation of one cube by a
//symmetry s in D4h (16 symmetries which preserve the UD-axis) gives the other
//cube: symCube[s]*A*symCube[s]^-1 = B
//The rawflipSlice coordinate is computed by 2048*slice + flip.
//The symFlipSliceRep table maps the index x < 64430 of a flipSlice
//equivalence class to the smallest rawflipSlice coordinate of all cubes in
//this class. In other words rawFlipSliceRep[x] gives the rawFlipSlice
//coordinate of the representant of this class. 
void initRawFLipSliceRep()
{
	int i, j, k, n, min, idx = 0;
	CubieCube ccK, ccJ, ccI;
	char flag[NSLICE * NFLIP];
	for (j = 0; j < NSLICE * NFLIP; j++) flag[j] = 0;
	for (j = 0; j < NSLICE; j++)
	{
		ccJ = invSlice(j);
		for (k = 0; k < NFLIP; k++)
		{
			min = NFLIP * j + k;
			if (flag[min] == 1) continue;//no representant
			ccK = invFlip(k);
			for (n = 0; n < 12; n++) ccK.eo[n].e = ccJ.eo[n].e; //merge ccK and ccJ
			for (i = 0; i < NSYM_D4h; i++)
			{
				ccI = edgeConjugate(ccK, i);
				n = NFLIP * slice(ccI) + flip(ccI);
				flag[n] = 1;//no representant
			}
			rawFlipSliceRep[idx++] = min;
		}
	}
}

//Computes the symmetries of the cube cc. This is relevant if we want to reduce
//the maneuver searchspace exploiting the symmetries of the given cube.
unsigned long long getSymmetries(CubieCube cc)
{
	int hasThisSym, symIdx;
	unsigned long long symRet = 0;
	CubieCube ccTmp1, ccTmp;
	Corner i; Edge j;
	for (symIdx = 0; symIdx < 48; symIdx++)
	{
		hasThisSym = 1;
		cornerMultiply(&symCube[symIdx], &cc, &ccTmp1);
		cornerMultiply(&ccTmp1, &symCube[invSymIdx[symIdx]], &ccTmp);
		edgeMultiply(&symCube[symIdx], &cc, &ccTmp1);
		edgeMultiply(&ccTmp1, &symCube[invSymIdx[symIdx]], &ccTmp);
		for (i = URF; i <= DRB; i++) if (ccTmp.co[i].c != cc.co[i].c || ccTmp.co[i].o != cc.co[i].o) { hasThisSym = 0; break; }
		if (!hasThisSym) continue;
		for (j = UR; j <= BR; j++) if (ccTmp.eo[j].e != cc.eo[j].e || ccTmp.eo[j].o != cc.eo[j].o) { hasThisSym = 0; break; }
		if (hasThisSym) symRet |= 1ULL << symIdx;
	}
	return symRet;
}

// sym is an 48 bit vector where a set bit corresponds to a given symmetry.
// symCount computes the number of symmetries given by sym.
int symCount(unsigned long long sym)
{
	int i = 0;
	while (sym != 0) { if (sym & 1) i++; sym = sym >> 1; }
	return i;
}

//Initializes the ESymmetries and GESymmetries table. initESymmetries[m] is a
//48 bit vector where bit s is set if moveConjugate[m][s]=m, in other words m
//is a fixpoint of the symmetry s. GESymmetries is similar to ESymmetries.
//The bit s is set if moveConjugate[m][s]>=m.
//These table are needed if we want to reduce the searchspace by symmetry
//in the case the cube we solve has symmetry itself.
void initGESymmetries()
{
	Move m;
	int i;

	for (m = mU1; m <= mB3; m++) { ESymmetries[m] = 0; GESymmetries[m] = 0; }

	for (i = 0; i < NSYM_Oh; i++)
		for (m = mU1; m <= mB3; m++)
		{
			if (moveConjugate[m][i] == m) ESymmetries[m] |= 1ULL << i;
			if (moveConjugate[m][i] >= m) GESymmetries[m] |= 1ULL << i;
		}
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++SYMMETRY_H-

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++PRUNING_H

extern int subOptLev;
extern int symRed;

// movesDefault[m] gives the moves which are allowed after move m by default.
// Each entry is a 12-bit array with the bit m* set if move m* is allowed after move m.
// U:U'; U':U,U'; R:R'; R':R,R'; F:F'; F':F,F'; D:D',U,U'; D':D,D',U,U'; L:L',R,R'; L':L,L',R,R'; B:B',F,F'; B':B,B',F,F';
const short int movesDefault[12] =
{ 0xffd,0xffc,0xff7,0xff3,0xfdf,0xfcf,0xf7c,0xf3c,0xdf3,0xcf3,0x7cf,0x3cf };
// L':L,L',R,R' means for example: after a L' move, the moves L,L'R,R' are not allowed.
// moveBitsDefault[mL3]=moveBitsDefault[9]=0xcf3 = 0b110011110011 has the corresponding four bits cleared.

const char* mv[] = { "U","U'","R","R'","F","F'","D","D'","L","L'","B","B'" };

//Computes the movesCloserToTarget table which acts as a pruning table. The target
//subgroup has the following property: the twist coordinate is 0 (all corner
//twist are ok), the flip cooordinate is 0 (all edge flips are ok), the UD-
//slice edges are in their slice and the edge/corner parity is even.
//Every coset is indexed by its symFlipSlice coordinate (16*classIndex +
//symmetryIndex), its twist coordinate and its parity.
//movesCloserToTarget[twist][2*classIndex+parity] is a short int where bit m with
//0<=m<12 is set when move m carries the cube closer to the target subgroup. 
void initMovesCloserToTarget()
{
	FILE* tableSave;
	char fileName[] = { "pruntable" };
	int flipSlice, twist, parity, symFlipSlice1, sym1, flipSlice1, twist1, parity1, i;
	unsigned long idx, idx1, count = 1;
	Move m;

#pragma warning(suppress : 4996)
	tableSave = fopen(fileName, "r+b");
	if (tableSave == NULL) goto MAKETABLE;
	fseek(tableSave, 0, SEEK_END);
	if (ftell(tableSave) != NFLIPSLICE * NTWIST * 4) { fclose(tableSave); goto MAKETABLE; }
	fseek(tableSave, 0, SEEK_SET);
	printf("\nloading pruning table (538 MB) from disk...");
	for (i = 0; i < NTWIST; i++)
	{
		if (!(i % 200)) pp();
		fread(movesCloserToTarget[i], NFLIPSLICE * 4, 1, tableSave);
	}
	fclose(tableSave);
	free(visitedA); free(visitedB);
	return;

MAKETABLE:
	printf("\ngenerating pruning table (538 MB), this may take half an hour.\n");
	fflush(stdout);
	visitedA[0] = 1; visitedB[0] = 1;
	while (count != NGOAL)
	{
		for (flipSlice = 0; flipSlice < NFLIPSLICE; flipSlice++)//Index of equivalence class
			for (twist = 0; twist < NTWIST; twist++)
				for (parity = 0; parity < 2; parity++)
				{
					idx = ((NTWIST * flipSlice + twist) << 1) + parity;
					if (visitedA[idx >> 3] & 1 << (idx & 7)) continue;//occupied
					else
					{
						for (m = mU1; m <= mB3; m++)
						{
							parity1 = parity ^ 1;//quarter turn changes parity			
							symFlipSlice1 = symFlipSliceClassMove[flipSlice][m];
							sym1 = symFlipSlice1 & 15;
							flipSlice1 = symFlipSlice1 >> 4;
							twist1 = twistMove[twist][m];
							twist1 = twistConjugate[twist1][sym1];
							idx1 = ((NTWIST * flipSlice1 + twist1) << 1) + parity1;
							if (visitedA[idx1 >> 3] & 1 << (idx1 & 7))//occupied, so closer to goal
							{
								movesCloserToTarget[twist][(flipSlice << 1) + parity] |= 1 << m;//set bit for this move
								if (!(visitedB[idx >> 3] & 1 << (idx & 7)))
								{
									visitedB[idx >> 3] |= 1 << (idx & 7); count++;
								}
							}
						}
					}
				}
		for (i = 0; i < NGOAL / 8 + 1; i++) visitedA[i] |= visitedB[i];
		printf("%9lu of %u entries done.\n", count, NGOAL);
		fflush(stdout);
	}
	printf("saving pruning table (538 MB) to disk...");
	fflush(stdout);
#pragma warning(suppress : 4996)
	tableSave = fopen(fileName, "w+b");
	for (i = 0; i < NTWIST; i++) fwrite(movesCloserToTarget[i], NFLIPSLICE * 4, 1, tableSave);
	fclose(tableSave);

	free(visitedA); free(visitedB);
	printf(" done.\non the next run the table will be loaded from disk.\n");
	fflush(stdout);
}

// movesCloserToTarget applies only if symmetryIndex s=0. For s<>0
//the corresponding moves have to be conjugated by symCube(s).
void initMoveBitsConjugate()
{
	int m, s, n;

	for (n = 0; n < NMBITS; n++)
		for (s = 0; s < NSYM_Oh; s++)
		{
			moveBitsConjugate[n][s] = 0;
			for (m = mU1; m <= mB3; m++) if (n & 1 << m) moveBitsConjugate[n][s] |= 1 << moveConjugate[m][invSymIdx[s]];
		}
}

//computes the distance of the cube in QTM to the target subgroup.
int distanceToTarget(CoordCube co)
{
	int symFlipSlice = co.symFlipSlice;
	int flipSlice = symFlipSlice >> 4;
	int sym = symFlipSlice & 15;
	int twist = co.twist;
	int twistConj = twistConjugate[co.twist][sym];
	int parity = co.parity;
	int dist = 0;
	short int move, a;

	while (1)
	{
		a = movesCloserToTarget[twistConj][(flipSlice << 1) + parity];
		if (a == 0) break;//no more moves which reduce distance, so we are finished
		a = moveBitsConjugate[a][sym];
		for (move = 0; move < NMOVE; move++)
		{
			if (a & 1 << move)
			{
				dist++;
				symFlipSlice = symFlipSliceMove(symFlipSlice, move);
				sym = symFlipSlice & 15;
				flipSlice = symFlipSlice >> 4;
				twist = twistMove[twist][move];
				twistConj = twistConjugate[twist][sym];
				parity ^= 1;
				break;
			}
		}
	}
	return dist;
}

//Computes the nextMove table. Given an 12-bit movesAllowed vector z
//and a move m, nextMove[z][m] returns the next move m' in z with m'>m.
//If no such move exists, -1 is returned.
void initNextMove()
{
	int i, j, k;
	for (i = 0; i < NMBITS; i++)
		for (j = 0; j <= NMOVE; j++)//<=NMOVE is correct!
			nextMove[i][j] = -1; //no next move is default

	for (i = 0; i < NMBITS; i++)
		for (j = 0; j < NMOVE; j++)
			for (k = j; k < NMOVE; k++)
			{
				if (i & 1 << k) { nextMove[i][j] = k; break; }
			}
}

// The most important routine of the program.
// Checks all maneuvers with increasing length which transform the cube to the target group.
// Uses the movesCloserToTarget pruningtable in three different orientations for more effective pruning.
// Keeps also track of the corn6Pos, edge6Pos and edge4Pos coordinates.
// The cube is solved, if these coordinates are 0 when the cube is in the target group.
void solveOptimal(CubieCube cu)
{
	//------------------------initialisation---------------------------------------
	int b, mConjU, mConjR, mConjF, twistConjU, twistConjR, twistConjF;
	CoordCube coU, coR, coF;
	int r_depth, optimalDist = 99;
	short int m;
	SearchNode sn[NNODE];//36 is enough
	SearchNode* snP, * snPNew;
	int manLength;
	unsigned long long sym, sym1, nodes = 0, tests = 0;

	sym = getSymmetries(cu);
	if (sym == 1) printf("Cube has no symmetry.\n");
	else printf("Cube has %u symmetries.\n", symCount(sym));

	if (subOptLev == 0)  printf("Computing all optimal solutions ");
	else if (subOptLev == 1) printf("Computing suboptimal solutions (+%d level) ", subOptLev);
	else if (subOptLev > 1) printf("Computing suboptimal solutions (+%d levels) ", subOptLev);
	if (symRed == 1 && subOptLev != -1) printf("up to symmetry");
	if (symRed == 0 && subOptLev != -1) printf("without symmetry reduction");
	printf("\n"); fflush(stdout);

	//rotate the cube along URF-diagonal, do get three different orientations
	coU = CubieCubeToCoordCube(cu);
	cu = edgeConjugate(cu, 16);//conjugation S_URF3*cu*S_URF3^-1
	cu = cornerConjugate(cu, 16);
	coR = CubieCubeToCoordCube(cu);//R-face now shows up
	cu = edgeConjugate(cu, 16);
	cu = cornerConjugate(cu, 16);
	coF = CubieCubeToCoordCube(cu);//F-face now shows up

	snP = sn;
	snP->distU = distanceToTarget(coU);
	snP->distR = distanceToTarget(coR);
	snP->distF = distanceToTarget(coF);

	snP->flipSliceU = coU.symFlipSlice >> 4;
	snP->flipSliceR = coR.symFlipSlice >> 4;
	snP->flipSliceF = coF.symFlipSlice >> 4;

	snP->symU = coU.symFlipSlice & 15;
	snP->symR = coR.symFlipSlice & 15;
	snP->symF = coF.symFlipSlice & 15;

	snP->parity = coU.parity;//the same for all three cube directions

	snP->twistU = coU.twist;
	snP->twistR = coR.twist;
	snP->twistF = coF.twist;

	snP->corn6Pos = coU.corn6Pos;//sufficient to use only U-direction
	snP->edge6Pos = coU.edge6Pos;
	snP->edge4Pos = coU.edge4Pos;

	for (b = 0; b < NNODE; b++) sn[b].move = mU1 - 1;

	twistConjU = twistConjugate[snP->twistU][snP->symU];
	twistConjR = twistConjugate[snP->twistR][snP->symR];
	twistConjF = twistConjugate[snP->twistF][snP->symF];


	snP->movesCloserTargetU = moveBitsConjugate[movesCloserToTarget[twistConjU]
		[(snP->flipSliceU << 1) + snP->parity]][snP->symU];
	//movesCloserTargetU now holds a 12-bit array with m set if move m
	//reduces the distance to the target group by 1.
	//the same in the other two orientations:
	snP->movesCloserTargetR = moveBitsConjugate[movesCloserToTarget[twistConjR]
		[(snP->flipSliceR << 1) + snP->parity]][symIdxMultiply[snP->symR][16]];
	snP->movesCloserTargetF = moveBitsConjugate[movesCloserToTarget[twistConjF]
		[(snP->flipSliceF << 1) + snP->parity]][symIdxMultiply[snP->symF][32]];

	snP->movesAllowed = 0xfff;//all moves are allowed for the first node
	snP->mSym = sym;

	if (snP->distU == 0 && snP->distR == 0 && snP->distF == 0) r_depth = manLength = 2;
	else
	{   //find maximum of the three distances
		r_depth = manLength =
			snP->distU > snP->distR ? (snP->distU > snP->distF ? snP->distU : snP->distF)
			: (snP->distR > snP->distF ? snP->distR : snP->distF);


		if (snP->distU == manLength) snP->movesAllowed &= snP->movesCloserTargetU;
		if (snP->distR == manLength) snP->movesAllowed &= snP->movesCloserTargetR;
		if (snP->distF == manLength) snP->movesAllowed &= snP->movesCloserTargetF;

	}
	//------------------------end of initialisation--------------------------------

MOVE:
	snP->move = nextMove[snP->movesAllowed][++(snP->move)];
	if (snP->move == -1)//no more possible move, go to left node
	{
		if (r_depth == manLength)
		{
			printf("depth %2u completed, %14llu nodes, %14llu tests\n", manLength, nodes, tests);
			fflush(stdout);
			r_depth += 2;
			manLength = r_depth;
			if (manLength > optimalDist + 2 * subOptLev) return;
			snP->movesAllowed = 0xfff;
			snP->mSym = sym;
		}
		else { r_depth++; snP--; }
		goto MOVE;
	}

	m = snP->move;
	sym1 = snP->mSym;
	//check if a symmetry transformation of the maneuver leads to a maneuver
	//which is lexically earlier. Ignore those moves.
	if (symRed == 1 && (sym1 & GESymmetries[m]) != sym1) goto MOVE;

	if (manLength - r_depth >= 2)//Prevent UUU, RRR, FFF...
		if (m == (snP - 1)->move && m == (snP - 2)->move) goto MOVE;

	snPNew = snP + 1;

	//coordinates relevant if cube is in target subgroup (all zero -> Id-cube)
	snPNew->edge6Pos = edge6PosMove[snP->edge6Pos][m];
	snPNew->edge4Pos = edge4PosMove[snP->edge4Pos][m];
	snPNew->corn6Pos = corn6PosMove[snP->corn6Pos][m];

	if (r_depth == 1)//maneuver complete
	{
		tests++;
		if (snPNew->corn6Pos == 0 && snPNew->edge6Pos == 0 && snPNew->edge4Pos == 0)
		{
			if (optimalDist > manLength) optimalDist = manLength;
			for (b = 0; b < manLength; b++) printf("%s ", mv[sn[b].move]);
			printf(" (%uq*)\n", manLength);
			if (subOptLev == -1) { printf("\n"); return; }
		}
		goto MOVE;
	}

	//fill in information for right node

	//distances to target for all three cube orientations
	if (snP->movesCloserTargetU & 1 << m)
		snPNew->distU = snP->distU - 1;
	else snPNew->distU = snP->distU + 1;
	if (snP->movesCloserTargetR & 1 << m)
		snPNew->distR = snP->distR - 1;
	else snPNew->distR = snP->distR + 1;
	if (snP->movesCloserTargetF & 1 << m)
		snPNew->distF = snP->distF - 1;
	else snPNew->distF = snP->distF + 1;

	//new coset coordinates for right node
	snPNew->parity = 1 - snP->parity;
	snPNew->mSym = snP->mSym & ESymmetries[m];

	mConjU = moveConjugate[m][snP->symU];
	b = symFlipSliceClassMove[snP->flipSliceU][mConjU];
	snPNew->flipSliceU = b >> 4;
	snPNew->symU = symIdxMultiply[b & 15][snP->symU];
	snPNew->twistU = twistMove[snP->twistU][m];

	m = moveConjugate[m][16];
	mConjR = moveConjugate[m][snP->symR];
	b = symFlipSliceClassMove[snP->flipSliceR][mConjR];
	snPNew->flipSliceR = b >> 4;
	snPNew->symR = symIdxMultiply[b & 15][snP->symR];
	snPNew->twistR = twistMove[snP->twistR][m];

	m = moveConjugate[m][16];
	mConjF = moveConjugate[m][snP->symF];
	b = symFlipSliceClassMove[snP->flipSliceF][mConjF];
	snPNew->flipSliceF = b >> 4;
	snPNew->symF = symIdxMultiply[b & 15][snP->symF];
	snPNew->twistF = twistMove[snP->twistF][m];

	twistConjU = twistConjugate[snPNew->twistU][snPNew->symU];
	twistConjR = twistConjugate[snPNew->twistR][snPNew->symR];
	twistConjF = twistConjugate[snPNew->twistF][snPNew->symF];

	//12bit vectors hold information which moves carry the cube closer
	//to the target group.
	snPNew->movesCloserTargetU =
		moveBitsConjugate[movesCloserToTarget[twistConjU][(snPNew->flipSliceU << 1) + snPNew->parity]][snPNew->symU];
	snPNew->movesCloserTargetR =
		moveBitsConjugate[movesCloserToTarget[twistConjR][(snPNew->flipSliceR << 1) + snPNew->parity]][symIdxMultiply[snPNew->symR][16]];
	snPNew->movesCloserTargetF =
		moveBitsConjugate[movesCloserToTarget[twistConjF][(snPNew->flipSliceF << 1) + snPNew->parity]][symIdxMultiply[snPNew->symF][32]];

	//compute which moves are allowed for this node
	snPNew->movesAllowed = movesDefault[snP->move];
	if (snPNew->distU == --r_depth) snPNew->movesAllowed &= snPNew->movesCloserTargetU;
	if (snPNew->distR == r_depth) snPNew->movesAllowed &= snPNew->movesCloserTargetR;
	if (snPNew->distF == r_depth) snPNew->movesAllowed &= snPNew->movesCloserTargetF;

	//now go to right move
	nodes++; snP++;
	goto MOVE;
	//-----------------------------------------------------------------------------
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++PRUNING_H-

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++OPTIQTM_H

static char manString[256];
int subOptLev;
int symRed;

CubieCube cc_a;

int main(int argc, char* argv[])
{
	int i, l;
	subOptLev = -1;
	symRed = 1;

	char config[256];
	printf("config "); fflush(stdout);
	std::cin >> config;
	if (config[0] == '+' && config[1] > '0' && config[1] <= '9') subOptLev = config[1] - 48;
	if (config[2] == '-' && config[3] == 's') symRed = 0;
	printf("// subOptLev: %d // symRed: %d\n", subOptLev, symRed);

	printf("initializing memory.\n");
	visitedA = (char*)calloc(NGOAL / 8 + 1, 1);//initialized to 0 by default
	visitedB = (char*)calloc(NGOAL / 8 + 1, 1);
	for (l = 0; l < NTWIST; l++) movesCloserToTarget[l] = (short*)calloc(NFLIPSLICE * 2, 2);

	printf("initializing tables"); fflush(stdout);
	initSymCubes();
	initMoveCubes();
	initInvSymIdx();
	initSymIdxMultiply();
	initMoveConjugate();
	initMoveBitsConjugate();
	initGESymmetries();
	initTwistConjugate(); pp();
	initRawFLipSliceRep(); pp();
	initTwistMove(); pp();
	initCorn6PosMove(); pp();
	initEdge4PosMove(); pp();
	initEdge6PosMove(); pp();
	initSymFlipSliceClassMove(); pp();
	initMovesCloserToTarget(); pp();
	initNextMove(); pp();
	printf("\r\n");

	while (1)
	{
		printf("enter cube (x to exit): "); fflush(stdout);
		if (fgets(manString, sizeof(manString), stdin) == NULL) break;
		if (manString[0] == 'x') exit(EXIT_SUCCESS);
		l = strlen(manString);
		if (manString[l - 1] == '\n') manString[l - 1] = 0;//remove LF
		if (l > 1 && manString[l - 2] == '\r') manString[l - 2] = 0;//remove CR if present
		if (strlen(manString) == 0) continue;//ignore empty lines
		printf("\nsolving optimal: %s\n", manString); fflush(stdout);
		cc_a = StringToCubieCube(manString);

		solveOptimal(cc_a);
	}

	exit(EXIT_SUCCESS);
	return 0;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++OPTIQTM_H-

//752,629 possible error at static_cast<Edge>(x++)
//operator++ not working¿ // maybe no need to overload if used temp ints example initMoveBitsConjugate()