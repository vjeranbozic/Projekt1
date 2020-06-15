#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include "header.h"

int main() {

	char* kupci = "kupci.bin";
	unsigned int brojKupaca = 1;

	kreiranjeDatoteke(kupci, &brojKupaca);
	menu(kupci, &brojKupaca);

	_getch();
	return 0;
}