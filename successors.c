#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

int DIE = 100;

static const int battle_table[8][16] = {
//	1  2  3  4  5  6  7  8  9   10  11-12 13-14 15-17 18-20 21-24 25+
	{ 0, 0, 0, 1, 1, 2, 2, 2, 3,  3,  4,    5,    6,    7,    8,    9  },	// 2,3,4
	{ 0, 0, 1, 1, 2, 2, 3, 3, 4,  4,  5,    6,    7,    8,    9,    10 },	// 5,6
	{ 0, 1, 1, 2, 2, 3, 3, 4, 4,  5,  6,    7,    8,    9,    10,   11 },	// 7
	{ 1, 1, 2, 3, 3, 4, 4, 5, 5,  6,  7,    8,    9,    10,   11,   12 },	// 8
	{ 1, 2, 3, 3, 4, 5, 5, 6, 6,  7,  8,    9,    10,   11,   12,   13 },	// 9*
	{ 2, 3, 4, 4, 5, 5, 6, 7, 7,  8,  9,    10,   11,   12,   13,   14 },	// 10
	{ 2, 4, 4, 5, 6, 6, 7, 8, 9,  9,  10,   11,   12,   13,   14,   15 },	// 11
	{ 3, 4, 5, 6, 7, 7, 8, 9, 10, 10, 11,   12,   13,   14,   15,   16 },	// 12
};

static const char *label[16] = {
	"  1   ",
	"  2   ",
	"  3   ",
	"  4   ",
	"  5   ",
	"  6   ",
	"  7   ",
	"  8   ",
	"  9   ",
	" 10   ",
	"11,12 ",
	"13,14 ",
	"15-17 ",
	"18-20 ",
	"21-24 ",
	" 25+  "
};

int roll_battle(int BR, int str, int r1, int r2)
{
	int dr = MAX(BR, r1) + MAX(BR, r2);
	if (dr <= 4) return battle_table[0][str];
	if (dr <= 6) return battle_table[1][str];
	return battle_table[dr-5][str];
}

int battle(int aBR, int aSTR, int dBR, int dSTR, int a1, int a2, int d1, int d2)
{
	int aR = roll_battle(aBR, aSTR, a1, a2);
	int dR = roll_battle(dBR, dSTR, d1, d2);
	if (aR > dR) return 1;
	if (aR == dR) return 0;
	return -1;
}

void run_battle(int aBR, int aSTR, int dBR, int dSTR, int *win, int *draw, int *loss)
{
	int a1, a2, d1, d2;
	*win = *draw = *loss = 0;
	for (a1 = 1; a1 <= 6; ++a1) {
		for (a2 = 1; a2 <= 6; ++a2) {
			for (d1 = 1; d1 <= 6; ++d1) {
				for (d2 = 1; d2 <= 6; ++d2) {
					int r = battle(aBR, aSTR, dBR, dSTR, a1, a2, d1, d2);
					if (r < 0) *loss += 1;
					if (r > 0) *win += 1;
					if (r == 0) *draw += 1;
				}
			}
		}
	}
	*win = DIE * *win / (6*6*6*6);
	*draw = DIE * *draw / (6*6*6*6);
	*loss = DIE * *loss / (6*6*6*6);
}

void make_table(int aBR, int dBR)
{
	int aSTR, dSTR;
	int w, d, l;
	printf("%d/%d   | ", aBR, dBR);
	for (aSTR = 0; aSTR < 16; ++aSTR)
		printf("%s", label[aSTR]);
	printf("\n");
	printf("------+------------------------------------------------------------------------------------------------\n");
	for (dSTR = 0; dSTR < 16; ++dSTR) {
		printf("%s| ", label[dSTR]);
		for (aSTR = 0; aSTR < 16; ++aSTR) {
			run_battle(aBR, aSTR, dBR, dSTR, &w, &d, &l);
			if (DIE > 20)
			{
				if (w == 100)
					printf("00/%02d ", d);
				else
					printf("%2d/%02d ", w,d);
			}
			else
			{
				if (w == 100)
					printf("00/%-2d ", d);
				else
					printf("%2d/%-2d ", w,d);
			}
		}
		printf("\n");
	}
}

int main(int argc, char **argv)
{
	int aBR = 2;
	int dBR = 0;

	srand48(time(NULL));

	if (argc >= 3) {
		aBR = atoi(argv[1]);
		dBR = atoi(argv[2]);
	}
	if (argc >= 4)
		DIE = atoi(argv[3]);

	make_table(aBR, dBR);

	return 0;
}
