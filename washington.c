#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

int DIE = 100;

int battle(int aBR, int aCU, int aMod, int dBR, int dCU, int dMod, int a2, int a10, int d2, int d10)
{
	int aStr = aCU + aMod;
	int dStr = dCU + dMod;
	if (a2)
		aStr += MIN(aBR, aCU);
	else
		aStr += MIN((int)(aBR+0.5/2), aCU);
	if (d2)
		dStr += MIN(dBR, dCU);
	else
		dStr += MIN((int)(dBR+0.5/2), dCU);
	int aFinal = aStr + a10;
	int dFinal = dStr + d10;
	return aFinal >= dFinal;
}

int run_battle(int aBR, int aCU, int aMod, int dBR, int dCU, int dMod)
{
	int a2, a10, d2, d10;
	int v = 0;
	for (a2 = 0; a2 <= 1; ++a2) {
		for (d2 = 0; d2 <= 1; ++d2) {
			for (a10 = 1; a10 <= 10; ++a10) {
				for (d10 = 1; d10 <= 10; ++d10) {
					v += battle(aBR, aCU, aMod, dBR, dCU, dMod, a2, a10, d2, d10);
				}
			}
		}
	}
	return DIE * v / (2*2*10*10);
}

void make_table(int atk_general, int def_general)
{
	int ac, dc, v;
	printf("%d/%d |", atk_general, def_general);
	for (ac = 1; ac <= 5; ++ac)
		printf(" %2d", ac);
	printf("\n");
	printf("----+---------------\n");
	for (dc = 1; dc <= 10; ++dc) {
		printf("%3d |", dc);
		for (ac = 1; ac <= 5; ++ac) {
			v = run_battle(atk_general, ac, 0, def_general, dc, 0);
			if (v >= 100)
				printf(" 00");
			else
				printf(" %2d", v);
		}
		printf("\n");
	}
}

int main(int argc, char **argv)
{
	int atk_general = 1;
	int def_general = 0;

	srand48(time(NULL));

	if (argc >= 3) {
		atk_general = atoi(argv[1]);
		def_general = atoi(argv[2]);
	}
	if (argc >= 4)
		DIE = atoi(argv[3]);

	make_table(atk_general, def_general);

	return 0;
}
