/*
* 프로젝트명 : BLACKJACK
* 제작자 : TEAM WHEEL
* 제작시작일 : 2015. 12. 21
* 최근수정일 : 2016. 01. 25
* 버전 : 1.5
*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <string.h>
#include <Windows.h>

#define LINES 20
#define COLS 100
#define NUM_OF_DECK 3
#define CARD_NUM ((NUM_OF_DECK) * 52)
#define CARD_NUM_MIN_LIMIT 15
#define CARD_SHUFFLING_NUM 1000
#define HAND_MAX 12

/* FLAGS */
#define VISIBLE 1
#define INVISIBLE 0
#define VALID 1
#define INVALID 0
#define YES 1
#define NO 0
#define DEALER 0
#define PLAYER 1
#define EMPTY -1
#define WIN 1
#define LOSE -1

/* KEYS */
#define BKSP 8
#define ENTER 13
#define EXTKEY 224
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80

/* CARDS */
#define SPADE 0
#define DIAMOND 1
#define HEART 2
#define CLUB 3
#define ACE 1
#define JACK 11
#define QUEEN 12
#define KING 13

/* COMMANDS */
#define MENU 0
#define HIT 1
#define STAND 2
#define DOUBLE_DOWN 3
#define SURRENDER 4
#define INSURANCE 5

/* 시스템 세팅 함수 */
void sysSetting();
void curSetting(int mode);

/* Windows API 함수 */
void gotoxy(int x, int y);
void textColor(int color_num);

/* 주요 함수 */
void title();
void menu();
void newgame();
int blackjack();

/* 보조 함수 */
void initHand();
void initCard();
void swapCard(int m, int n);
void shuffleCard();
int getStake();
int getStakeInput(int y);
void assignCard(int toWhom);
int getCardNum(int card);
int getCardShape(int card);
void printCard(int x, int y, int card, int visibility);
void printHands(int dealer_first_card_visibility);
void printField(int dealer_first_card_visibility);
int getCardScore(int card);
int blackjack_menu();
int isValidBlackjackMenu(int choice);
int isBlackJack(int who);

/* 기타 함수 */
void printMid(int y, const char* s);
void clearLine(int y);
void textColorReset();
int power(int base, int n);

/* 전역 변수 */
char version[] = "1.5";
int money;
int stake;
int insurance;
int card[CARD_NUM];
int cardNum;
int player_hand[HAND_MAX];
int dealer_hand[HAND_MAX];
int player_hand_num;
int dealer_hand_num;
int player_hand_sum;
int dealer_hand_sum;
int player_ace_num;
int dealer_ace_num;

int main()
{
	sysSetting();

	title();

	while (1)
	{
		menu();
	}
}

void title()
{
	char version_info[10] = "Ver.";
	sprintf_s(version_info, sizeof(version_info), "%s %s", version_info, version);
	
	printMid(3, "■■■　　■　　　　　■■　　　■■■　■　　■　　　■■■■　　■■　　　■■■　■　　■");
	printMid(4, "■　　■　■　　　　■　　■　■　　　　■　■　　　　　　■　　■　　■　■　　　　■　■　");
	printMid(5, "■■■　　■　　　　■■■■　■　　　　■■　　　　　　　■　　■■■■　■　　　　■■　　");
	printMid(6, "■　　■　■　　　　■　　■　■　　　　■　■　　　　■　■　　■　　■　■　　　　■　■　");
	printMid(7, "■■■　　■■■■　■　　■　　■■■　■　　■　　　　■■　　■　　■　　■■■　■　　■");
	printMid(9, version_info);
	printMid(11, "Team WHEEL");

	while (!_kbhit())
	{
		printMid(15, "Press any key to continue");

		if (_kbhit()) { break; }

		Sleep(700);

		if (_kbhit()) { break; }

		printMid(15, "                         ");

		if (_kbhit()) { break; }

		Sleep(300);
	}

	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
}

void sysSetting()
{
	system("cls");

	//제목 설정
	system("title BLACKJACK");

	//커서 안보이게 하기
	curSetting(INVISIBLE);

	//LINES, COLS 설정
	char cmd[30] = "mode con:";
	char lines[] = "lines";
	char cols[] = "cols";
	sprintf_s(cmd, sizeof(cmd), "%s%s%d %s%d", cmd, lines, LINES, cols, COLS);
	system(cmd);
}

void curSetting(int mode)
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);

	if (mode == VISIBLE)
	{
		curInfo.bVisible = 1;
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	}
	else if (mode == INVISIBLE)
	{
		curInfo.bVisible = 0;
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	}
}

void gotoxy(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void textColor(int color_num)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_num);
}

void printMid(int y, const char* s)
{
	gotoxy((COLS / 2) - (strlen(s) / 2), y); printf(s);
}

void clearLine(int y)
{
	gotoxy(0, y); printf("　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　");
}

void textColorReset()
{
	textColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
}

int power(int base, int n)
{
	if (n == 0)
	{
		return 1;
	}

	return base * power(base, n - 1);
}

void menu()
{
	int input;
	int result;

	system("cls");

	int choice = 1;

	gotoxy(4, 4); printf("<Menu>");
	gotoxy(7, 6); printf("1. New Game");
	gotoxy(7, 7); printf("0. Exit");

	while (1)
	{
		switch (choice)
		{
		case 1:
			gotoxy(4, 6); printf("▶");
			gotoxy(4, 7); printf("　");
			break;
		case 0:
			gotoxy(4, 6); printf("　");
			gotoxy(4, 7); printf("▶");
			break;
		}

		input = _getch();

		if (input == ENTER)
		{
			result = choice;
			break;
		}
		else if (input == EXTKEY)
		{
			input = _getch();

			switch (input)
			{
			case UP:
			case LEFT:
				choice = (choice + 1) % 2;
				break;
			case DOWN:
			case RIGHT:
				choice = (choice + 1) % 2;
				break;
			}
		}
	}

	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

	switch (result)
	{
	case 1:
		newgame();
		break;
	case 0:
		exit(0);
		break;
	}
}

void newgame()
{
	money = 100;

	initCard();
	shuffleCard();

	int result;
	while (1)
	{
		system("cls");

		if (cardNum < CARD_NUM_MIN_LIMIT)
		{
			printMid(7, "Shuffling Card...");
			shuffleCard();
			Sleep(1000);
			continue;
		}

		stake = getStake();

		initHand();

		result = blackjack();

		if (result == WIN)
		{
			Sleep(1000);

			clearLine(5); clearLine(6); clearLine(7); clearLine(8); clearLine(9); clearLine(10); clearLine(11); clearLine(12);

			printMid(5, "──────────────────────────────────────────────────");
			printMid(6, "<GAME OVER>");
			printMid(8, "PLAYER");
			printMid(9, "WIN!");
			printMid(11, "Press Any Key to Continue...");
			printMid(12, "──────────────────────────────────────────────────");

			_getch();
		}
		else if (result == LOSE)
		{
			Sleep(1000);

			clearLine(5); clearLine(6); clearLine(7); clearLine(8); clearLine(9); clearLine(10); clearLine(11); clearLine(12);

			printMid(5, "──────────────────────────────────────────────────");
			printMid(6, "<GAME OVER>");
			printMid(8, "PLAYER");
			printMid(9, "LOSE!");
			printMid(11, "Press Any Key to Continue...");
			printMid(12, "──────────────────────────────────────────────────");

			_getch();
		}
		else
		{
			return;
		}

		stake = 0;
		FlushConsoleInputBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
	}
}

void initHand()
{
	player_hand_num = 0;
	dealer_hand_num = 0;

	player_hand_sum = 0;
	dealer_hand_sum = 0;

	player_ace_num = 0;
	dealer_ace_num = 0;
}

void initCard()
{
	int i;
	for (i = 0; i < CARD_NUM; i++)
	{
		card[i] = i;
	}
	cardNum = CARD_NUM;
}

void swapCard(int m, int n)
{
	int temp = card[m];
	card[m] = card[n];
	card[n] = temp;
}

void shuffleCard()
{
	int i;
	int m, n;

	srand((unsigned)time(NULL));

	for (i = 0; i < CARD_SHUFFLING_NUM; i++)
	{
		m = rand() % CARD_NUM;
		n = rand() % CARD_NUM;

		swapCard(m, n);
	}

	cardNum = CARD_NUM;
}

int getStake()
{
	int result;
	char str_money[COLS - 1] = "";
	sprintf_s(str_money, sizeof(str_money), "$ %d", money);

	system("cls");
	printMid(3, "[Current Money]");
	printMid(4, str_money);

	printMid(7, "<STAKE SET>");
	printMid(9, "How much will you bet?");

	result = getStakeInput(11);

	while (result > money || result < 0)
	{
		printMid(13, "Invalid stake input");
		result = getStakeInput(11);
	}

	money -= result;

	return result;
}

int getStakeInput(int y)
{
	clearLine(y);
	gotoxy(50, y);
	curSetting(VISIBLE);

	int i;
	char num[COLS] = { '\0' };
	int input;
	int len = 0;
	int result = 0;

	while (1)
	{
		input = _getch();

		if (input == ENTER)
		{
			break;
		}
		else if (input == BKSP)
		{
			if (len <= 0)
			{
				clearLine(y);
				printMid(y, "");
				continue;
			}
			else
			{
				num[len] = '\0';
				len--;
				clearLine(y);
				printMid(y, num);
			}
		}
		else if (input >= '0' && input <= '9')
		{
			num[len] = input;
			len++;
			num[len] = '\0';
			clearLine(y);
			printMid(y, num);
		}
		else
		{
			continue;
		}
	}

	curSetting(INVISIBLE);

	for (i = 0; i < len; i++)
	{
		result += power(10, len - i - 1) * (num[i] - '0');
	}

	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

	return result;
}

int blackjack()
{
	int choice;

	system("cls");

	assignCard(DEALER);
	printField(INVISIBLE);
	Sleep(500);
	assignCard(PLAYER);
	printField(INVISIBLE);
	Sleep(500);
	assignCard(DEALER);
	printField(INVISIBLE);
	Sleep(500);
	assignCard(PLAYER);
	printField(INVISIBLE);

	while (1) //player's turn
	{
		if (player_hand_sum > 21)
		{
			return LOSE;
		}

		choice = blackjack_menu();

		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

		if (choice == HIT)
		{
			assignCard(PLAYER);
			printField(INVISIBLE);
			continue;
		}
		else if (choice == STAND)
		{
			break;
		}
		else if (choice == DOUBLE_DOWN)
		{
			money += stake;
			stake *= 2;
			money -= stake;

			assignCard(PLAYER);
			printField(INVISIBLE);

			if (player_hand_sum > 21)
			{
				return LOSE;
			}

			break;
		}
		else if (choice == SURRENDER)
		{
			money += (stake / 2);

			return LOSE;
		}
		else if (choice == INSURANCE)
		{
			insurance = stake / 2;
			stake += insurance;
			money -= insurance;

			break;
		}
		else if (choice == MENU)
		{
			return MENU;
		}
	}

	printField(VISIBLE);

	while (dealer_hand_sum < 17) //dealer's turn
	{
		Sleep(1000);
		assignCard(DEALER);
		printField(VISIBLE);
	}

	if (dealer_hand_sum <= 21)
	{
		if (player_hand_sum >= dealer_hand_sum)
		{
			if (choice == STAND)
			{
				money += (2 * stake);
			}
			else if (choice == DOUBLE_DOWN)
			{
				money += (2 * stake);
			}
			else if (choice == INSURANCE)
			{
				stake -= insurance;
				if (isBlackJack(DEALER) == YES)
				{
					money += 2 * (stake + insurance);
				}
				else
				{
					money += (2 * stake);
				}
			}

			return WIN;
		}
		else
		{
			if (choice == INSURANCE)
			{
				stake -= insurance;
				if (isBlackJack(DEALER) == YES)
				{
					money += (2 * insurance);
				}
			}

			return LOSE;
		}
	}
	else
	{
		if (choice == STAND)
		{
			money += (2 * stake);
		}
		else if (choice == DOUBLE_DOWN)
		{
			money += (2 * stake);
		}
		else if (choice == INSURANCE)
		{
			stake -= insurance;
			if (isBlackJack(DEALER) == YES)
			{
				money += 2 * (stake + insurance);
			}
			else
			{
				money += (2 * stake);
			}
		}

		return WIN;
	}
}

void assignCard(int toWhom)
{
	cardNum--;

	int curCard = card[cardNum];

	if (toWhom == PLAYER)
	{
		player_hand[player_hand_num] = curCard;
		player_hand_num++;

		if (getCardNum(curCard) == ACE)
		{
			player_ace_num++;
		}

		player_hand_sum += getCardScore(curCard);

		if (player_hand_sum > 21 && player_ace_num > 0)
		{
			player_hand_sum -= 10;
			player_ace_num--;
		}
	}
	else if (toWhom == DEALER)
	{
		dealer_hand[dealer_hand_num] = curCard;
		dealer_hand_num++;

		if (getCardNum(curCard) == ACE)
		{
			dealer_ace_num++;
		}

		dealer_hand_sum += getCardScore(curCard);

		if (dealer_hand_sum > 21 && dealer_ace_num > 0)
		{
			dealer_hand_sum -= 10;
			dealer_ace_num--;
		}
	}
}

int getCardNum(int card)
{
	return (card % 52) % 13 + 1;
}

int getCardShape(int card)
{
	return (card % 52) / 13;
}

void printCard(int x, int y, int card, int visibility)
{
	textColor(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);

	gotoxy(x, y + 0); printf("　　　");
	gotoxy(x, y + 1); printf("　　　");
	gotoxy(x, y + 2); printf("　　　");

	if (visibility == INVISIBLE)
	{
		gotoxy(x + 2, y + 1); printf("？");
	}
	else if (visibility == VISIBLE)
	{
		int num = getCardNum(card);
		int shape = getCardShape(card);

		switch (num)
		{
		case ACE:
			gotoxy(x, y); printf("Ａ");
			gotoxy(x + 4, y + 2); printf("Ａ");
			break;
		case 2:
			gotoxy(x, y); printf("２");
			gotoxy(x + 4, y + 2); printf("２");
			break;
		case 3:
			gotoxy(x, y); printf("３");
			gotoxy(x + 4, y + 2); printf("３");
			break;
		case 4:
			gotoxy(x, y); printf("４");
			gotoxy(x + 4, y + 2); printf("４");
			break;
		case 5:
			gotoxy(x, y); printf("５");
			gotoxy(x + 4, y + 2); printf("５");
			break;
		case 6:
			gotoxy(x, y); printf("６");
			gotoxy(x + 4, y + 2); printf("６");
			break;
		case 7:
			gotoxy(x, y); printf("７");
			gotoxy(x + 4, y + 2); printf("７");
			break;
		case 8:
			gotoxy(x, y); printf("８");
			gotoxy(x + 4, y + 2); printf("８");
			break;
		case 9:
			gotoxy(x, y); printf("９");
			gotoxy(x + 4, y + 2); printf("９");
			break;
		case 10:
			gotoxy(x, y); printf("10");
			gotoxy(x + 4, y + 2); printf("10");
			break;
		case JACK:
			gotoxy(x, y); printf("Ｊ");
			gotoxy(x + 4, y + 2); printf("Ｊ");
			break;
		case QUEEN:
			gotoxy(x, y); printf("Ｑ");
			gotoxy(x + 4, y + 2); printf("Ｑ");
			break;
		case KING:
			gotoxy(x, y); printf("Ｋ");
			gotoxy(x + 4, y + 2); printf("Ｋ");
			break;
		}

		switch (shape)
		{
		case SPADE:
			gotoxy(x + 2, y + 1); printf("♠");
			break;
		case DIAMOND:
			textColor(FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
			gotoxy(x + 2, y + 1); printf("◆");
			break;
		case HEART:
			textColor(FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
			gotoxy(x + 2, y + 1); printf("♥");
			break;
		case CLUB:
			gotoxy(x + 2, y + 1); printf("♣");
			break;
		}
	}

	textColorReset();
}

void printHands(int dealer_first_card_visibility)
{
	int i;

	if (dealer_first_card_visibility == VISIBLE)
	{
		for (i = 0; i < dealer_hand_num; i++)
		{
			printCard(8 * i + 4, 3, dealer_hand[i], VISIBLE);
		}
	}
	else if (dealer_first_card_visibility == INVISIBLE)
	{
		for (i = 0; i < dealer_hand_num; i++)
		{
			if (i == 0)
			{
				printCard(4, 3, dealer_hand[i], INVISIBLE);
			}
			else
			{
				printCard(8 * i + 4, 3, dealer_hand[i], VISIBLE);
			}
		}
	}

	for (i = 0; i < player_hand_num; i++)
	{
		printCard(90 - (8 * i), 7, player_hand[i], VISIBLE);
	}
}

void printField(int dealer_first_card_visibility)
{
	clearLine(1); clearLine(11); clearLine(13);
	gotoxy(2, 1); printf("DEALER");
	gotoxy(73, 1); printf("Remaining Card : %d", cardNum);
	gotoxy(93, 1); printf("／%d", CARD_NUM);
	gotoxy(2, 11); printf("Stake : $ %d", stake);
	gotoxy(COLS - strlen("PLAYER") - 2, 11); printf("PLAYER");
	gotoxy(0, 12); printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
	gotoxy(2, 13); printf("Money you have : $ %d", money);

	gotoxy(1, 14); printf("|      HIT      |     STAND     |  DOUBLE DOWN  |   SURRENDER   |   INSURANCE   ||     MENU      |");

	gotoxy(2, 17); printf("[HELP]");

	if (dealer_first_card_visibility == VISIBLE)
	{
		printHands(VISIBLE);
	}
	else
	{
		printHands(INVISIBLE);
	}
}

int getCardScore(int card)
{
	int card_num = getCardNum(card);

	switch (card_num)
	{
	case ACE:
		return 11;
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
		return card_num;
	case JACK:
	case QUEEN:
	case KING:
		return 10;
	}
}

int blackjack_menu()
{
	int input;
	int choice = HIT;

	while (1)
	{
		clearLine(15); clearLine(18); clearLine(19);
		switch (choice)
		{
		case HIT:
			gotoxy(9, 15); printf("▲                                                                                 ");
			gotoxy(2, 18); printf("Take a card from the dealer");
			break;
		case STAND:
			gotoxy(9, 15); printf("                ▲                                                                 ");
			gotoxy(2, 18); printf("Take no more cards");
			break;
		case DOUBLE_DOWN:
			gotoxy(9, 15); printf("                                ▲                                                 ");
			gotoxy(2, 18); printf("Double the initial bet and receive only one more card");
			break;
		case SURRENDER:
			gotoxy(9, 15); printf("                                                ▲                                 ");
			gotoxy(2, 18); printf("Give up the game and receive 50%% of the initial bet");
			textColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
			gotoxy(2, 19); printf("(Only available as first decision of a hand)");
			textColorReset();
			break;
		case INSURANCE:
			gotoxy(9, 15); printf("                                                                ▲                 ");
			gotoxy(2, 18); printf("Put additional 50%% of the initial bet as insurance. If the dealer makes blackjack, receive");
			gotoxy(2, 19); printf("double of the insurance.");
			textColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
			printf("(Only available when the dealer’s first open card is Ace)");
			textColorReset();
			break;
		case MENU:
			gotoxy(9, 15); printf("                                                                                 ▲");
			gotoxy(2, 18); printf("Go back to menu");
			break;
		}

		input = _getch();

		if (input == ENTER)
		{
			if (isValidBlackjackMenu(choice) == VALID)
			{
				return choice;
			}
			else
			{
				continue;
			}
		}
		else if (input == EXTKEY)
		{
			input = _getch();

			switch (input)
			{
			case UP:
			case LEFT:
				choice = (choice + 5) % 6;
				break;
			case DOWN:
			case RIGHT:
				choice = (choice + 1) % 6;
				break;
			}
		}
		else
		{
			continue;
		}
	}
}

int isValidBlackjackMenu(int choice)
{
	switch (choice)
	{
	case HIT:
	case STAND:
		return VALID;
	case DOUBLE_DOWN:
		if (money - stake >= 0)
		{
			return VALID;
		}
		else
		{
			return INVALID;
		}
	case SURRENDER:
		if (player_hand_num == 2)
		{
			return VALID;
		}
		else
		{
			return INVALID;
		}
	case INSURANCE:
		if (player_hand_num == 2 && getCardNum(dealer_hand[1]) == ACE && (money - (stake / 2) >= 0))
		{
			return VALID;
		}
		else
		{
			return INVALID;
		}
	case MENU:
		return VALID;
	}
}

int isBlackJack(int who)
{
	if (who == PLAYER)
	{
		if (player_hand_num > 2)
		{
			return NO;
		}
		else
		{
			if ((getCardNum(player_hand[0]) == ACE && (getCardNum(player_hand[1]) == 10 || getCardNum(player_hand[1]) == JACK || getCardNum(player_hand[1]) == QUEEN || getCardNum(player_hand[1]) == KING))
				|| (getCardNum(player_hand[1]) == ACE && (getCardNum(player_hand[0]) == 10 || getCardNum(player_hand[0]) == JACK || getCardNum(player_hand[0]) == QUEEN || getCardNum(player_hand[0]) == KING)))
			{
				return YES;
			}
			else
			{
				return NO;
			}
		}
	}
	else if (who == DEALER)
	{
		if (dealer_hand_num > 2)
		{
			return NO;
		}
		else
		{
			if ((getCardNum(dealer_hand[0]) == ACE && (getCardNum(dealer_hand[1]) == 10 || getCardNum(dealer_hand[1]) == JACK || getCardNum(dealer_hand[1]) == QUEEN || getCardNum(dealer_hand[1]) == KING))
				|| (getCardNum(dealer_hand[1]) == ACE && (getCardNum(dealer_hand[0]) == 10 || getCardNum(dealer_hand[0]) == JACK || getCardNum(dealer_hand[0]) == QUEEN || getCardNum(dealer_hand[0]) == KING)))
			{
				return YES;
			}
			else
			{
				return NO;
			}
		}
	}
}