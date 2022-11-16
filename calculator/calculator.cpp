#define _CRT_SECURE_NO_WARNINGS

#define bool int
#define true 1
#define false 0
#define size 1024
#define gets_s(v) gets(v)


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//const int size = 1024;

#ifndef __cplusplus
#define bool int
#define true  1
#define false 0
#endif

bool q;

//st = (char*)malloc(size * sizeof(int)); s = (char*)malloc(size * sizeof(int)); s1 = (char*)malloc(size * sizeof(int)); l = (char*)malloc(size * sizeof(int)); x = (char*)malloc(size * sizeof(int));
char st[size], s[size], s1[size], l[size], x[size];

//Объединить строки
void concat(char* s1, char* s2) {
	int n1 = strlen(s1);
	int n2 = strlen(s2);
	for (int k = 0; k <= n2; k++)
		s1[n1 + k] = s2[k];
}

char* pred(int a, char* st) {
	int l = strlen(s);
	s[l++] = st[a - 1];
	s[l] = 0;
	return s;
}

char* sled(int a, char* st) {
	int l = strlen(s1);
	s1[l++] = st[a];
	s1[l] = 0;
	return s1;
}

void perevorot(char* s) {
	char h[size];
	int j = 0;
	int hi = 0;
	int l = strlen(s);
	for (j = l - 1; j >= 0; j--)
		h[hi++] = s[j];
	for (int k = 0; k < l; k++)
		s[k] = h[k];
}

void copy(char* to, char* from) {
	while (*from) {
		*to = *from;
		*to++;
		*from++;
	}
	*to = 0;
}

int pos(char c, char* s) {
	for (int k = 0; k < strlen(s); k++)
		if (s[k] == c)
			return k;
	return -1;
}

//Удалить из s все начиная с start длиной len
void Delete(char* s, int start, int len) {
	int last = strlen(s);
	while (start < last - len) {
		s[start] = s[start + len];
		start++;
	}
	s[start] = 0;
}

//вставить подстроку substr в строку s с позиции p
//предполагается, что s достаточно длинна
void Insert(char* substr, char* s, int p) {
	int ls = strlen(s);
	int lsb = strlen(substr);
	//Сдвинуть все что позже
	for (int k = ls; k >= p; k--)
		s[k + lsb] = s[k];
	s[ls + lsb] = 0;

	for (int k = 0; k < lsb; k++)
		s[k + p] = substr[k];
}

bool Error;
double Add(double a, double b) {
	Error = false;
	return a + b;
}

double Sub(double a, double b) {
	Error = false;
	return a - b;
}

double Mul(double a, double b) {
	Error = false;
	return a * b;
}

double Div(double a, double b) {
	Error = b == 0;
	if (Error) return 0;
	return a / b;
}

void Operation(char sign, double(*F)(double a, double b), char* stopper, char* stopper2, bool all) {
	double b, c, d;

	for (int i = 0; i < strlen(st) / 2; i++) {
		int k = pos(sign, st);
		if (k < 0)
			return;
		s[0] = s1[0] = 0;
		if (k >= 0) {
			for (; k > 0 && pos(st[k - 1], stopper) < 0; k--)
			{
				pred(k, st);
			};
			perevorot(s);
			if (s[0] == '.')
			{
				Error = true;
				return;
			};
			b = atof(s);
		};

		int a = pos(sign, st);
		if (a >= 0)
		{
			while (pos(st[a + 1], stopper2) < 0)
			{
				a++;
				sled(a, st);
				if (a >= (int)strlen(st)) break;
			};
			c = atof(s1);
			d = F(b, c);
			sprintf(l, "%lf", d); //str(d, l);
			int lishn = strlen(l) - 1;
			//Убрать лишние 0
			while (l[lishn] == '0') { l[lishn--] = 0; }
			while (l[lishn] == '.') { l[lishn--] = 0; }

			Delete(st, k, a - k + 1);
			Insert(l, st, k);
			//printf("%s\n", st);
			if (!all)
				return;
		};
	};
}

void PrintError(char* s) {
	Error = true;
	printf("Ошибка %s\n", s);
	getchar();
}

void ChangeSigns() {
	if (st[0] != '-')
		return;
	q = !q;
	for (int j = 0; j < strlen(st); j++)
		if (st[j] == '-')
			st[j] = '+';
		else
			if (st[j] == '+') st[j] = '-';
	Delete(st, 0, 1);
}

int main() {
	system("chcp 1251>nul");
	printf("Введите выражение:");
	gets_s(st);
	copy(x, st);
	//нахождение букв
	for (int i = 0; i < strlen(st); i++)
	{
		if (pos(st[i], (char*)"+-*/0.,123456789") < 0)
		{
			PrintError((char*)"Есть не числа");
			return 0;
		};
	};

	//сложение
	//вычитание
	q = false;
	while (true)
	{
		//умножение 
		Error = false;
		//деление
		Operation('/', Div, (char*)"+-/", (char*)"+/", true);
		if (Error) {
			PrintError((char*)"");
			return 0;
		}
		Operation('*', Mul, (char*)"+*-/", (char*)"+*-/", true);
		if (Error) {
			PrintError((char*)"");
			return 0;
		}
		ChangeSigns();
		int minus = pos('-', st);
		int plus = pos('+', st);
		if (minus < 0 && plus < 0)
			break; //Нет больше аддитивных операций

		if (minus < 0) {
			Operation('+', Add, (char*)"+-", (char*)" +-", false);
			continue;
		}

		if (plus < 0) {
			Operation('-', Sub, (char*)"+-", (char*)"+-", false);
			continue;
		}

		if (minus < plus)
			Operation('-', Sub, (char*)"+-", (char*)"+-", false);
		else
			Operation('+', Add, (char*)"+-", (char*)"+-", false);
	}
	//Восстановить знак
	if (q)
		if (st[0] == '-')
			Delete(st, 0, 1);
		else
			Insert((char*)"-", st, 0);
	if (Error)
		printf("Ошибка\n");
	else
		printf_s("%s = %s\n", x, st);
	getchar();
}