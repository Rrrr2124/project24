
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>
#include <string.h>

#define BUF_SIZE 69 //Не меньше 36
#define COM 5
#define OUTS 128
#define ISGOOD(x) (isalpha(x)||x==' ')

char *sErrors[] = {
	/*01*/ "Файл не найден",
	/*02*/ "Команда не найдена",
	/*03*/ "Животные друг друга съели",
	/*04*/ "Волк либо уже на лодке либо места больше нет",
	/*05*/ "Волк и лодка на разных берегах",
	/*06*/ "Коза либо уже на лодке либо места больше нет",
	/*07*/ "Коза и лодка на разных берегах",
	/*08*/ "Капуста либо уже на лодке либо места больше нет ",
	/*09*/ "Капуста и лодка на разных берегах",
	/*10*/ "Нечего выкладывать, лодка пуста",
	/*11*/ "Волк съел козу",
	/*12*/ "Коза съела обед рыбака(капусту)", };

char *com[COM] = {
	"TAKE WOLF", "TAKE GOAT", "TAKE CABBAGE", "MOVE", "PUT"
}, **comPtr = com;

unsigned acc, Abcdef[26]; //Аккумулятор && Регистры
FILE*fIn;     //Входные/выходные файлы
char outs[OUTS];
//Флаги//
int lgoat=1,//наличие goat на левом берегу
rgoat=0,//наличие goat на правом берегу
bgoat=0,//налчие goat на лодке
lwolf=1,//наличие wolf на левом берегу
rwolf=0,//наличие wolf на правом берегу
bwolf=0,//налчие wolf на лодке
lcab=1,//наличие cab на левом берегу
rcab=0,//наличие cab на правом берегу
bcab=0,//налчие cab на лодке
fboat=0,//заполненность лодки
bboat=1;//местоположение лодки 0 - правый берег, 1 - левый
	


void HelloWorld(int);
void Error(int);

int main(int argc, char *argv[])
{
	int  k, osn = 0;
	char *sIn = "in.txt";
	char *p = argv[1], c, *ar1 = NULL, *ar2 = NULL, buf[BUF_SIZE] = { 0 }, *b = buf, tmp[10] = { 0 };

	//Делаем локализацию
	setlocale(LC_ALL, "Russian");
	if (!(fIn = fopen(sIn, "r"))) Error(1, sIn);
	/*Основной цикл*/
	while (!feof(fIn)){
		c = fgetc(fIn);
		if (ISGOOD(c))
				*b++ = toupper(c); //Записываем в буфер
			else if (c == ';' || c == EOF){
				if (*buf){  //Буфер не пуст
					*b = 0;
					for (comPtr = com; comPtr - com<COM; comPtr++) //Ищем команду
						if (strstr(buf, *comPtr) == buf) break;
					if ((k = comPtr - com) == COM) Error(2, buf); //Не нашли
					if (k<6){ 
						HelloWorld(k);
					}
					b = buf;
					*b = 0;
				}
				else{ //Пустой буфер
					printf("Команды закончились, если ошибок не было, значит никто не погиб\n");
					}
		}
		if (c == EOF) break;
	} /*Конец основного цикла*/

	return 0;
}
//Выполнение команд
void HelloWorld(int k)
{
	int osn = 0;
	switch (k){
	case 0:	// TAKE WOLF
		if (bwolf == 0 && fboat == 0) {
			if ((bboat == 1 && lwolf == 1) || (bboat == 0 && rwolf == 1)) {
				if (bboat == 1 && lwolf == 1) { fboat = 1; lwolf = 0; bwolf = 1; 
				printf("Отладочные значения:%d%d%d\n", fboat, lwolf, bwolf);
				}
				if (bboat == 0 && rwolf == 1) {
						fboat = 1; rwolf = 0; bwolf = 1;
						printf("Отладочные значения:%d%d%d\n", fboat, rwolf, bwolf);
				}
			}
			else Error(5);
		}
		else Error(4);
		break;
	case 1:	// TAKE GOAT
		if (bgoat == 0 && fboat == 0) {
			if ((bboat == 1 && lgoat == 1) || (bboat == 0 && rgoat == 1)) {
				if (bboat == 1 && lgoat == 1) {
					fboat = 1; lgoat = 0; bgoat = 1;
					printf("Отладочные значения:%d%d%d\n", fboat, lgoat, bgoat);
				}
				if (bboat == 0 && rgoat == 1) {
					fboat = 1; rgoat = 0; bgoat = 1;
					printf("Отладочные значения:%d%d%d\n", fboat, rgoat, bgoat);
				}
			}
			else Error(7);
		}
		else Error(6);
		break;
	case 2:	// TAKE CABBAGE
		if (bcab == 0 && fboat == 0) {
			if ((bboat == 1 && lcab == 1) || (bboat == 0 && rcab == 1)) {
				if (bboat == 1 && lcab == 1) {
					fboat = 1; lcab = 0; bcab = 1;
					printf("Отладочные значения:%d%d%d\n", fboat, lcab, bcab);
				}
				if (bboat == 0 && rcab == 1) {
					fboat = 1; rcab = 0; bcab = 1;
					printf("Отладочные значения:%d%d%d\n", fboat, rcab, bcab);
				}
			}
			else Error(9);
		}
		else Error(8);
		break;
	case 3:	// MOVE
		if (bboat == 0) bboat = 1; else bboat = 0;
		if (lgoat == 1 && lwolf == 1 && lcab == 1 && bboat == 0) Error(3);
		if (rgoat == 1 && rwolf == 1 && rcab == 1 && bboat == 1) Error(3);
		if (lgoat == 1 && lwolf == 1 && bboat == 0) Error(11);
		if (lgoat == 1 && lcab == 1 && bboat == 0) Error(12);
		if (rgoat == 1 && rwolf == 1 && bboat == 1) Error(11);
		if (rgoat == 1 && rcab == 1 && bboat == 1) Error(12);
		break;
	case 4:	// PUT
		if (fboat == 1){
			if (bboat == 1 && (bgoat == 1 || bwolf == 1 || bcab == 1)) {
				if (bgoat == 1) {
					bgoat = 0; lgoat = 1; fboat = 0;
				}
				if (bwolf == 1) {
					bwolf = 0; lwolf = 1; fboat = 0;
				}
				if (bcab == 1) {
					bcab = 0; lcab = 1; fboat = 0;
				}
				//printf("%d%d%d\n", bgoat, lgoat, rgoat);
			}
			if (bboat == 0 && (bgoat == 1 || bwolf == 1 || bcab == 1)) {
				if (bgoat == 1) {
					bgoat = 0; rgoat = 1; fboat = 0;
				}
				if (bwolf == 1) {
					bwolf = 0; rwolf = 1; fboat = 0;
				}
				if (bcab == 1) {
					bcab = 0; rcab = 1; fboat = 0;
				}
			}
		}
		else Error(10);
		break;
	}
}
//Ошибки
void Error(int n)
{
	printf("Error #%d:\n%s.\n", n, sErrors[n - 1]);
	exit(n);
}