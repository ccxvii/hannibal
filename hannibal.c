#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define USE_ENVELOPMENT 1
#define USE_PROPORTION 1
#define SHOW_ROUNDS 0

#define MIN(a,b) ((a)<(b)?(a):(b))

int DIE = 100;
int ITERATIONS = 10000;

enum { FRONTAL, LEFT, RIGHT, PROBE, ENVELOPMENT, RESERVE, BC_MAX };

const int card_count[] = { 12, 9, 9, 8, 6, 4 };

const char *card_abbr = "FLRPEr";

const char *card_name[] = {
	"Frontal Attack",
	"Left Flank",
	"Right Flank",
	"Probe",
	"Double Envelopment",
	"Reserve"
};

int roll_d6(void)
{
	return (int)floor(drand48() * 6) + 1;
}

void deal_battle_card(int hand[], int deck[])
{
	int i, n, k;
	for (i = 0, n = 0; i < BC_MAX; ++i)
		n += deck[i];
	k = (int)floor(drand48() * n);
	for (i = 0, n = 0; i < BC_MAX; ++i) {
		if (k >= n && k < n + deck[i]) {
			++hand[i];
			--deck[i];
			return;
		}
		n += deck[i];
	}
}

void print_hand(const char *name, int hand[])
{
	int i;
	printf("%s [", name);
	for (i = 0; i < BC_MAX; ++i)
		printf(" %c:%d", card_abbr[i], hand[i]);
	printf(" ]\n");
}

int preferred_card(int atk_general, int def_general, int hand[])
{
	int i, prefer = FRONTAL;
	int env = 2;
#ifdef USE_ENVELOPMENT
	if (def_general < atk_general) /* only use envelopment card if opposing general is weaker */
		env = 1;
#endif
	for (i = 0; i < BC_MAX - env; ++i)
#ifdef USE_PROPORTION
		if ((double)hand[i]/card_count[i] > (double)hand[prefer]/card_count[prefer])
#else
		if (hand[i] > hand[prefer])
#endif
			prefer = i;
	/* Use envelopment when we've run out of all other options */
	if (hand[prefer] == 0)
		prefer = ENVELOPMENT;
	return prefer;
}

int battle(int atk_general, int atk_BCs, int def_general, int def_BCs, int *rounds, int *final)
{
	int deck[BC_MAX] = { 12, 9, 9, 8, 6, 4 };
	int atk_hand[BC_MAX] = {0, 0, 0, 0, 0, 0 };
	int def_hand[BC_MAX] = {0, 0, 0, 0, 0, 0 };
	int victory = 1;

	int i, n;

	for (i = 0; i < atk_BCs; ++i)
		deal_battle_card(atk_hand, deck);
	for (i = 0; i < def_BCs; ++i)
		deal_battle_card(def_hand, deck);

	//printf("----\n");
	//print_hand("deck    ", deck);
	//print_hand("attacker", atk_hand);
	//print_hand("defender", def_hand);

	int *atk = atk_hand;
	int *def = def_hand;

	int atk_card = -1;
	int def_card = -1;
	int swap_roll;
	*rounds = 0;

	for (;;) {
		if (atk_card < 0 || (atk[atk_card] == 0 && atk[RESERVE] == 0))
			atk_card = preferred_card(atk_general, def_general, atk);

		//printf("Attack with %c (%d/%d)\n", card_abbr[atk_card], atk[atk_card], atk[RESERVE]);
		if (atk[atk_card] > 0)
			--atk[atk_card];
		else if (atk[RESERVE] > 0)
			--atk[RESERVE];
		else {
			//printf("Attack fails!\n");
			return !victory;
		}
		*rounds += 1;
		*final = atk_card;

		if (def[atk_card] > 0) {
			//printf("  successful defense!\n");
			--def[atk_card];
		} else if (def[RESERVE] > 0) {
			//printf("  successful defense with reserve!\n");
			--def[RESERVE];
		} else {
			//printf("  loss!\n");
			return victory;
		}

		n = 0;
		for (i = 0; i < BC_MAX; ++i)
			n += def[i];
		if (n > 0) {
			swap_roll = roll_d6();
			if (swap_roll <= def_general) {
				//printf("Counter-attack success (%d <= %d)\n", swap_roll, def_general);
			} else {
				//printf("Counter-attack failure (%d >  %d)\n", swap_roll, def_general);
			}
			if (atk_card == ENVELOPMENT || (swap_roll <= def_general)) {
				int *tmp = atk; atk = def; def = tmp;
				int tmp_card = atk_card; atk_card = def_card; def_card = tmp_card;
				int tmp_general = atk_general; atk_general = def_general; def_general = tmp_general;
				victory = !victory;
			}
		}
	}
}

int run_battle(int atk_general, int ac, int def_general, int dc, int *rp)
{
	int i, v = 0, r, f;
	*rp = 0;
	for (i = 0; i < ITERATIONS; ++i) {
		v += battle(atk_general, ac, def_general, dc, &r, &f);
		*rp += r;
	}
	return *rp = round((float)*rp / i), DIE * v / i;
}

void make_table(int atk_general, int def_general)
{
	int ac, dc, v, r;
	printf("%d/%d |", atk_general, def_general);
	for (ac = 1; ac <= 20; ++ac)
		printf(" %2d", ac);
	printf("\n");
	printf("----+------------------------------------------------------------\n");
	for (dc = 1; dc <= 20; ++dc) {
		printf("%3d |", dc);
		for (ac = 1; ac <= 20; ++ac) {
			v = run_battle(atk_general, ac, def_general, dc, &r);
#if SHOW_ROUNDS
			printf(" %2d", r);
#else
			if (v >= 100)
				printf(" 00");
			else
				printf(" %2d", v);
#endif
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
	if (argc >= 5)
		ITERATIONS = atoi(argv[4]);

	make_table(atk_general, def_general);

	return 0;
}
