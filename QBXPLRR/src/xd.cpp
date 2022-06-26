#include <iostream>
#include <fstream>

enum Corners { A, B, C, D, G, H, K, O };
enum Edges { a, b, c, d, f, g, h, j, k, n, o, s };
static const char Axis[6] = { 'U','F','R','B','L','D' };

struct cube
{
	Corners cp[8]; char co[8];
	Edges ep[12]; char eo[12];
};
static const cube solved {
	{A, B, C, D, G, H, K, O},{0,0,0,0,0,0,0,0},
	{a, b, c, d, f, g, h, j, k, n, o, s},{0,0,0,0,0,0,0,0,0,0,0,0}
};
static const cube qtmMoves[12] =
{
	/*U*/{{D, A, B, C, G, H, K, O},{0,0,0,0,0,0,0,0},{d, a, b, c, f, g, h, j, k, n, o, s},{0,0,0,0,0,0,0,0,0,0,0,0}},
	/*U'*/{{B, C, D, A, G, H, K, O},{0,0,0,0,0,0,0,0},{b, c, d, a, f, g, h, j, k, n, o, s},{0,0,0,0,0,0,0,0,0,0,0,0}},
	/*F*/{{A, B, D, H, C, G, K, O},{0,0,2,1,1,2,0,0},{a, b, h, d, c, f, g, j, k, n, o, s},{0,0,0,0,0,0,0,0,0,0,0,0}},
	/*F'*/{{A, B, G, C, H, D, K, O},{0,0,1,1,1,1,0,0},{a, b, f, d, g, h, c, j, k, n, o, s},{0,0,0,0,0,0,0,0,0,0,0,0}},
	/*R*/{{A, C, G, D, K, H, B, O},{0,2,1,0,2,0,1,0},{a, f, c, d, k, g, h, b, j, n, o, s},{0,0,0,0,0,0,0,0,0,0,0,0}},
	/*R'*/{{A, K, B, D, C, H, G, O},{0,1,1,0,1,0,1,0},{a, j, c, d, b, g, h, k, f, n, o, s},{0,0,0,0,0,0,0,0,0,0,0,0}},
	/*B*/{{B, K, C, D, G, H, O, A},{2,1,0,0,0,0,2,1},{j, b, c, d, f, g, h, o, k, a, n, s},{0,0,0,0,0,0,0,0,0,0,0,0}},
	/*B'*/{{O, A, C, D, G, H, B, K},{1,1,0,0,0,0,1,1},{n, b, c, d, f, g, h, a, k, o, j, s},{0,0,0,0,0,0,0,0,0,0,0,0}},
	/*L*/{{O, B, C, A, G, D, K, H},{1,0,0,2,0,1,0,2},{a, b, c, o, f, g, d, j, k, n, s, h},{0,0,0,0,0,0,0,0,0,0,0,0}},
	/*L'*/{{D, B, C, H, G, O, K, A},{1,0,0,1,0,1,0,1},{a, b, c, h, f, g, s, j, k, n, d, o},{0,0,0,0,0,0,0,0,0,0,0,0}},
	/*D*/{{A, B, C, D, H, O, G, K},{0,0,0,0,0,0,0,0},{a, b, c, d, f, s, h, j, g, n, k, o},{0,0,0,0,0,0,0,0,0,0,0,0}},
	/*D'*/{{A, B, C, D, K, G, O, H},{0,0,0,0,0,0,0,0},{a, b, c, d, f, k, h, j, o, n, s, g},{0,0,0,0,0,0,0,0,0,0,0,0}}
};

static const char* max = "43252003274489856000";//479001600 * 2048 * 40320 * 2187 * 0.5
struct i_cube { unsigned __int32 ep; unsigned short eo, cp, co; };
static const i_cube i_solved{ 0,0,0,0 };//

void CornerMultiply(const cube& a, const cube& b, cube& c)
{
	char o, oA, oB;
	for (unsigned int corner = 0; corner <= 7; ++corner)
	{
		c.cp[corner] = a.cp[b.cp[corner]];
		oA = a.co[b.cp[corner]];
		oB = b.co[corner];
		o = oA + oB * (1 - 2 * !(oA < 3));
		o += (oA < 3) * (-3 * (((oB < 3) && (o >= 3)) || (o >= 6)))
			+ !(oA < 3) * (3 * ((oB < 3) && ((o < 3) || (o < 0))));
		c.co[corner] = o;
	}
}
void EdgeMultiply(const cube& a, const cube& b, cube& c)
{
	for (unsigned int edge = 0; edge <= 11; ++edge)
	{
		c.ep[edge] = a.ep[b.ep[edge]];
		c.eo[edge] = (b.eo[edge] + a.eo[b.ep[edge]]) % 2;
	}
}
void Multiply(const cube& a, const cube& b, cube& c)
{
	CornerMultiply(a, b, c);
	EdgeMultiply(a, b, c);
}
cube MoveAx(cube& _qb, const unsigned char ax)
{
	cube qb;
	CornerMultiply(_qb, qtmMoves[ax], qb);
	EdgeMultiply(_qb, qtmMoves[ax], qb);
	//Multiply(_qb, qtmMoves[ax], qb);
	return qb;
}

cube ManeuverNotation(const char* maneuvers, unsigned int length)
{
	cube qb = solved;
	unsigned char ax = 0;
	int pow;
	for (unsigned int i = 0; i < length; ++i)
	{
		switch (toupper(maneuvers[i]))
		{
		case 'U': ax = 0; break;
		case 'F': ax = 1; break;
		case 'R': ax = 2; break;
		case 'B': ax = 3; break;
		case 'L': ax = 4; break;
		case 'D': ax = 5; break;
		default: continue;
		}
		if (i == length - 1) { MoveAx(qb, ax); break; }
		switch (maneuvers[i + 1])
		{
		case '3':case'\'': pow = 3; break;
		case '2': pow = 2; break;
		default: pow = 1;
		}
		for (int j = 0; j < pow; ++j) MoveAx(qb, ax);
	}
	return qb;
}

int fact(int x)
{
	int r = 1;
	for (int i = 1; i <= x; ++i) r *= i;
	return r * (x >= 0);
}

// Binomial Coefficient nCk = n!/(k!-(n-k)!);
int nCk(int n, int k)
{
	if (k > n) return 0;
	if (k == 0 || k == n) return 1;
	return nCk(n - 1, k - 1) + nCk(n - 1, k);
	//return (k < n) * (C(n - 1, k - 1) + C(n - 1, k));
}

//m^(n-1)
unsigned __int32 i_o(const char* o, const unsigned int n, const unsigned int m)
{
	unsigned __int32 i = 0;
	for (unsigned int ii = 0; ii < n; ++ii) i = i * m + o[ii];
	return i;
}
char* inv_i_o(unsigned __int32 i, const unsigned int n, const unsigned int m)
{
	char* o = new char[n];
	int parity = 0;
	for (int ii = n - 2; ii >= 0; --ii) { parity += o[i] = i % m; i /= m; }
	o[n - 1] = (m - parity % m) % m;
	return o;
}

unsigned __int32 i_p(const int p[], const unsigned int n)
{
	unsigned __int32 i = 0;
	int x = 0;
	for (int ii = n - 1; ii >= 0; --ii) i += nCk(n - 1 - p[ii], ++x);
	return i;
}
int* inv_i_p(unsigned __int32 i, const unsigned int n, const unsigned int m)
{
	int* p = new int[n];

	return p;
}

#define NTWIST 2187
#define NFLIPSLICE 64430
static short movesCloserToTarget[NTWIST][NFLIPSLICE * 2];
#define NGOAL 281816820
void initMovesCloserToTarget()
{
	std::fstream file;
	file.open("pruning", std::ios::in);
	if (file.is_open())
	{
		std::cout << "loading pruning table ..." << std::endl;
		for (unsigned int i = 0; i < NTWIST; ++i)
		{
			for (unsigned int j = 0; j < NFLIPSLICE * 2; ++j)
			{
				file >> movesCloserToTarget[i][j];
			}
		}
		file.close();//unn
	}
	else
	{
		std::cout << "creating pruning table ..." << std::endl;
		file.open("pruning", std::ios::out | std::ios::trunc);
		if (file.is_open())//unn
		{
			char* visitedA = (char*)calloc(NGOAL / 8 + 1, 1);
			char* visitedB = (char*)calloc(NGOAL / 8 + 1, 1);
			visitedA[0] = 1;
			visitedB[0] = 1;
			unsigned long count = 1;
			while (count != NGOAL)
			{
				
				std::cout << (100 * count / NGOAL) << " % entries done" << std::endl;
			}
			file << movesCloserToTarget;
			file.close();//unn
			free(visitedA);
			free(visitedB);
		}
		else std::cout << "error" << std::endl;
	}
}

void solveOptimal(const cube& qb)
{
	
}

int main()
{
	char maneuvers[100];
	cube qb;
	while (1)
	{
		std::cout << "input maneuvers: ";
		std::cin >> maneuvers;
		if (maneuvers[0] == '0') break;
		qb = ManeuverNotation(maneuvers, 100);

		solveOptimal(qb);
	}
	for (const auto& i : qtmMoves)
	{
		std::cout << i_o(i.eo, 12, 2) << " " << i_o(i.co, 8, 3) << std::endl;
	}
	return 0;
}
//