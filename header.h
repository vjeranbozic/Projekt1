#ifndef HEADER_H
#define HEADER_H

typedef struct racuni {
	int id;
	char ime[21];
	char prezime[21];
	char broj_mobitela[15];
	char adresa_stanovanja[51];
	char dug[27];
	char noviRed;
}RACUN;

void kreiranjeDatoteke(char* datoteka, unsigned int* pBrojKupaca);
void unos(char* datoteka, unsigned int* pBrojKupaca);
void menu(char* datoteka, unsigned int* pBrojKupaca);
void pretraga(char* datoteka, unsigned int* pBrojkupaca);
void pretragaPoImenu(char* datoteka, unsigned int* pBrojKupaca);
void ispisSvihClanova(char* datoteka, unsigned int* pBrojKupaca);
void pretragaPoID(char* datoteka, unsigned int* pBrojKupaca);
void izmjenaPodataka(char* datoteka, unsigned int* pBrojKupaca);
void sortiranje(char* datoteka, unsigned int* pBroj);
void brisanjeDatoteke(char* datoteka);
void izlazakIzPrograma(void);

#endif