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

void make_table(int aBR, int aMod, int dBR, int dMod)
{
	int ac, dc, v;
	printf("%d%+d/%d%+d |", aBR, aMod, dBR, dMod);
	for (ac = 1; ac <= 5; ++ac)
		printf(" %3d", ac);
	printf("\n");
	printf("-------+---------------------\n");
	for (dc = 1; dc <= 10; ++dc) {
		printf("    %3d |", dc);
		for (ac = 1; ac <= 5; ++ac) {
			v = run_battle(aBR, ac, aMod, dBR, dc, dMod);
			printf(" %3d", v);
		}
		printf("\n");
	}
}

int main(int argc, char **argv)
{
	int atk_general = 1;
	int def_general = 0;
	int atk_mod = 0;
	int def_mod = 0;

	if (argc >= 3) {
		atk_general = atoi(argv[1]);
		def_general = atoi(argv[2]);
	}
	if (argc >= 5) {
		atk_mod = atoi(argv[3]);
		def_mod = atoi(argv[4]);
	}
	if (argc >= 6)
		DIE = atoi(argv[5]);

	make_table(atk_general, atk_mod, def_general, def_mod);

	return 0;
}
