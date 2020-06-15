#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include "header.h"

void kreiranjeDatoteke(char* datoteka, unsigned int* pBrojKupaca) {

	FILE* provjera = fopen(datoteka, "rb");

	if (provjera == NULL) {
		perror("          Datoteka ne postoji u mapi prilikom prvog pokretanja");

		provjera = fopen(datoteka, "wb");

		if (provjera == NULL) {
			perror("\n          Datoteka se ne moze kreirati");
			exit(EXIT_FAILURE);
		}
		else {
			fwrite(pBrojKupaca, sizeof(unsigned int), 1, provjera);
			fclose(provjera);
			printf("\n            Datoteka kreirana, prilikom prvog pokretanja");
		}
	}
	else {
		fread(pBrojKupaca, sizeof(unsigned int), 1, provjera);
		printf("\n                    Datoteka postoji");
		fclose(provjera);
	}
}

void unos(char* datoteka, unsigned int* pBrojKupaca) {
	
	FILE* unosUDatoteku = NULL;
	unosUDatoteku = fopen(datoteka, "rb+");

	if (unosUDatoteku == NULL) {
		perror("              Izbornik 1 - Greska dodavanja novog clana u datoteku\n");
		return;
	}
	else {
		RACUN unosClan = { 0 };
		printf("\n\n   ================================================\n\n");
		printf("          Unos novih korisnickih racuna\n");
		printf("\n   ================================================\n\n");
		printf("          Unesite podatke o korisniku:\n\n");
		printf("          Ime: ");
		scanf("%21s", unosClan.ime);
		printf("          Prezime: ");
		scanf("%21s", unosClan.prezime);
		printf("          Adresa stanovanja: ");
		scanf(" %50[^\n]", unosClan.adresa_stanovanja);
		printf("          Broj mobitela: ");
		char privremeniBroj[10] = { '\0' };
		scanf("%9s", privremeniBroj);
		strcpy(unosClan.broj_mobitela, "+385");
		strcat(unosClan.broj_mobitela, privremeniBroj);
		unosClan.id = (*pBrojKupaca)++;
		printf("          Dugovanje: ");
		char privremeniDug[10] = { '\0' };
		scanf("%9s", privremeniDug);
		strcat(unosClan.dug, privremeniDug);
		unosClan.noviRed = '\n';

		fseek(unosUDatoteku, sizeof(unsigned int) + ((*pBrojKupaca - 1) * sizeof(RACUN)), SEEK_SET);
		fwrite(&unosClan, sizeof(RACUN), 1, unosUDatoteku);
		rewind(unosUDatoteku);
		fwrite(pBrojKupaca, sizeof(unsigned int), 1, unosUDatoteku);
		fclose(unosUDatoteku);
	}
}

void pretraga(char* datoteka, unsigned int* pBrojKupaca) {
	FILE* citajIzDatoteke = NULL;
	citajIzDatoteke = fopen(datoteka, "r");

	if (citajIzDatoteke == NULL) {
		perror("Izbornik 2 - Citanje datoteke\n");
		return;
	}
	else {
		
		RACUN* sviKupci = NULL;

		fread(pBrojKupaca, sizeof(unsigned int), 1, citajIzDatoteke);

		if (*pBrojKupaca == 0) {

			printf("Nema unesenih clanova!\n");
			fclose(citajIzDatoteke);
			return;
		}
		else {
			int izbor;

			printf("\n   ================================================\n\n");
			printf("        Pretraga postojecih korisnickih racuna\n");
			printf("\n   ================================================\n\n");
			printf("          1. Ispis svih postojecih korisnika\n");
			printf("          2. Pretrazi po imenu\n");
			printf("          3. Pretrazi po ID-u\n");
			printf("          0. Povratak na izbornik\n\n");

			scanf("%d", &izbor);

			switch (izbor) {
			case 1:
				system("cls");
				ispisSvihClanova(datoteka, pBrojKupaca);
				break;
			case 2:
				system("cls");
				pretragaPoImenu(datoteka, pBrojKupaca);
				break;
			case 3:
				system("cls");
				pretragaPoID(datoteka, pBrojKupaca);
				break;
			case 0:
				break;
			default:
				system("cls");
				printf("\n   Krivo odabrana opcija, molimo pokusajte ponovno\n");
			}

		}
	}
}

void ispisSvihClanova(char* datoteka, unsigned int* pBrojKupaca) {
	
	FILE* pDatotekaProcitaj = NULL;
	pDatotekaProcitaj = fopen(datoteka, "rb");

	if (pDatotekaProcitaj == NULL) {

		perror("Izbornik 2 - Citanje datoteke");
		return;
	}
	else {

		RACUN* sviKorisnici = NULL;

		fread(pBrojKupaca, sizeof(unsigned int), 1, pDatotekaProcitaj);

		if (*pBrojKupaca == 0) {

			printf("Nema unesenih clanova!\n");
			fclose(pDatotekaProcitaj);
			return;
		}
		else {

			sviKorisnici = (RACUN*)calloc(*pBrojKupaca, sizeof(RACUN));

			if (sviKorisnici == NULL) {

				perror("Citanje svih korisnika");
				exit(EXIT_FAILURE);
			}
			else {

				fread(sviKorisnici, sizeof(RACUN), *pBrojKupaca, pDatotekaProcitaj);
				fclose(pDatotekaProcitaj);

				unsigned int i;

				for (i = 0; i < *pBrojKupaca; i++) {
					if (i != 0) {
						printf("%u\t", (sviKorisnici + i)->id);
						printf("%s ", (sviKorisnici + i)->ime);
						printf("%s ", (sviKorisnici + i)->prezime);
						printf("%s ", (sviKorisnici + i)->adresa_stanovanja);
						printf("%s  ", (sviKorisnici + i)->broj_mobitela);
						printf("%s kn\n", (sviKorisnici + i)->dug);
					}
				}
				free(sviKorisnici);
			}
		}
	}
	char izbor[11];
	printf("\n\n\n          Za povratak na izbornik unesite bilo koji broj:  ");
	scanf("%s", izbor);
}

void pretragaPoImenu(char* datoteka, unsigned int* pBrojKupaca) {

	FILE* pDatotekaProcitaj = NULL;
	pDatotekaProcitaj = fopen(datoteka, "rb");

	if (pDatotekaProcitaj == NULL) {

		perror("Izbornik 2 - Citanje datoteke");
		return;
	}
	else {

		RACUN* sviKorisnici = NULL;

		fread(pBrojKupaca, sizeof(unsigned int), 1, pDatotekaProcitaj);

		if (*pBrojKupaca == 0) {

			printf("          Nema unesenih clanova!\n");
			fclose(pDatotekaProcitaj);
			return;
		}
		else {

			sviKorisnici = (RACUN*)calloc(*pBrojKupaca, sizeof(RACUN));

			if (sviKorisnici == NULL) {

				perror("          Citanje svih korisnika");
				exit(EXIT_FAILURE);
			}
			else {

				fread(sviKorisnici, sizeof(RACUN), *pBrojKupaca, pDatotekaProcitaj);
				fclose(pDatotekaProcitaj);

				unsigned int i;

				printf("          Unesite ime korisnika: \n");
				char privremenoIme[31] = { '\0' };
				scanf("%30s", privremenoIme);
				unsigned int statusPronalaska = 0;
				unsigned int indeksPronalaska = -1;

				for (i = 0; i < *pBrojKupaca; i++){
					
					if (!strcmp((sviKorisnici + i)->ime, privremenoIme)) {
						statusPronalaska = 1;
						indeksPronalaska = i;
					}
				}
				if (statusPronalaska) {
					system("cls");
					printf("          Korisnik pronadjen:\n\n");
					printf("          %u\t", (sviKorisnici + indeksPronalaska)->id);
					printf("          %s ", (sviKorisnici + indeksPronalaska)->ime);
					printf("          %s ", (sviKorisnici + indeksPronalaska)->prezime);
					printf("          %s ", (sviKorisnici + indeksPronalaska)->adresa_stanovanja);
					printf("          %s", (sviKorisnici + indeksPronalaska)->broj_mobitela);
					printf("          %s kn\n", (sviKorisnici + i)->dug);
					int pom;
					scanf("%d", &pom);
				}
				else {
					int pom;
					printf("Nepostojeci korisnik\n");
					scanf("%d", &pom);
				}
				free(sviKorisnici);
			}
		}
	}
}

void pretragaPoID(char* datoteka, unsigned int* pBrojKupaca) {
	
	FILE* pDatotekaProcitaj = NULL;
	pDatotekaProcitaj = fopen(datoteka, "rb");

	if (pDatotekaProcitaj == NULL) {

		perror("Izbornik 2 - Citanje datoteke");
		return;
	}
	else {

		RACUN* sviKorisnici = NULL;

		fread(pBrojKupaca, sizeof(unsigned int), 1, pDatotekaProcitaj);

		if (*pBrojKupaca == 0) {

			printf("          Nema unesenih clanova!\n");
			fclose(pDatotekaProcitaj);
			return;
		}
		else {

			sviKorisnici = (RACUN*)calloc(*pBrojKupaca, sizeof(RACUN));

			if (sviKorisnici == NULL) {

				perror("          Citanje svih korisnika");
				exit(EXIT_FAILURE);
			}
			else {

				fread(sviKorisnici, sizeof(RACUN), *pBrojKupaca, pDatotekaProcitaj);
				fclose(pDatotekaProcitaj);

				unsigned int i;

				printf("          Unesite ID trazenog korisnika: ");
				int privremeniID = 0;
				scanf("%d", &privremeniID);
				unsigned int statusPronalaska = 0;
				unsigned int indeksPronalaska = -1;

				for (i = 0; i < *pBrojKupaca; i++) {

					if (privremeniID == (sviKorisnici + i)->id) {
						statusPronalaska = 1;
						indeksPronalaska = i;
					}
				}
				if (statusPronalaska) {
					system("cls");
					printf("Korisnik pronadjen:\n\n");
					printf("%u\t", (sviKorisnici + indeksPronalaska)->id);
					printf("%s ", (sviKorisnici + indeksPronalaska)->ime);
					printf("%s ", (sviKorisnici + indeksPronalaska)->prezime);
					printf("%s ", (sviKorisnici + indeksPronalaska)->adresa_stanovanja);
					printf("%s ", (sviKorisnici + indeksPronalaska)->broj_mobitela);
					printf(" %s kn\n", (sviKorisnici + indeksPronalaska)->dug);
					int pom;
					printf("\n\nPritisnite bilo koju tipku za nastavak: ");
					scanf("%d", &pom);
				}
				else {
					int pom;
					printf("          Nepostojeci korisnik\n");
					printf("\n\n          Unesite bilo koji broj za nastavak: ");
					scanf("%d", &pom);
				}
				free(sviKorisnici);
			}
		}
	}
}

void izmjenaPodataka(char* datoteka, unsigned int* pBrojKupaca) {

	FILE* pDatotekaProcitaj = NULL;
	pDatotekaProcitaj = fopen(datoteka, "rb");

	if (pDatotekaProcitaj == NULL) {

		perror("Izbornik 2 - Citanje datoteke");
		return;
		//exit(EXIT_FAILURE);
	}
	else {

		RACUN* sviKorisnici = NULL;

		fread(pBrojKupaca, sizeof(unsigned int), 1, pDatotekaProcitaj);

		if (*pBrojKupaca == 0) {

			printf("          Nema unesenih clanova!\n");
			fclose(pDatotekaProcitaj);
			return;
		}
		else {

			sviKorisnici = (RACUN*)calloc(*pBrojKupaca, sizeof(RACUN));

			if (sviKorisnici == NULL) {

				perror("          Citanje svih korisnika");
				exit(EXIT_FAILURE);
			}
			else {

				RACUN privremeniRacun = { 0 };
				fread(sviKorisnici, sizeof(RACUN), *pBrojKupaca, pDatotekaProcitaj);

				unsigned int i;

				printf("          Unesite ID trazenog korisnika: ");
				int privremeniID = 0;
				scanf("%d", &privremeniID);
				unsigned int statusPronalaska = 0;
				unsigned int indeksPronalaska = -1;

				for (i = 0; i < *pBrojKupaca; i++) {

					if (privremeniID == (sviKorisnici + i)->id) {
						statusPronalaska = 1;
						indeksPronalaska = i;
					}
				}
				if (statusPronalaska) {
					system("cls");
					printf("          Korisnik pronadjen:\n\n");
					printf("          %u\t", (sviKorisnici + indeksPronalaska)->id);
					printf("          %s ", (sviKorisnici + indeksPronalaska)->ime);
					printf("          %s ", (sviKorisnici + indeksPronalaska)->prezime);
					printf("          %s ", (sviKorisnici + indeksPronalaska)->adresa_stanovanja);
					printf("          %s", (sviKorisnici + indeksPronalaska)->broj_mobitela);
					printf("          %s\n", (sviKorisnici + indeksPronalaska)->dug);
					fclose(pDatotekaProcitaj);
					pDatotekaProcitaj = fopen(datoteka, "rb+");
				
					printf("\n   ================================================\n\n");
					printf("          Izmjena podataka o korisniku\n");
					printf("\n   ================================================\n\n");
					
					printf("          Novo ime: ");
					scanf(" %50[^\n]", privremeniRacun.ime);
					printf("          Novo prezime: ");
					scanf(" %50[^\n]", privremeniRacun.prezime);
					printf("          Nova adresa: ");
					scanf(" %50[^\n]", privremeniRacun.adresa_stanovanja);
					printf("          Novi broj mobitela: ");
					char privremeniBroj[10] = { '\0' };
					scanf("%9s", privremeniBroj);
					strcpy(privremeniRacun.broj_mobitela, "+385");
					strcat(privremeniRacun.broj_mobitela, privremeniBroj);
					printf("          Novi iznos duga: ");
					char privremeniDug[10] = { '\0' };
					scanf("%9s", privremeniDug);
					strcat(privremeniRacun.dug, privremeniDug);
					privremeniRacun.noviRed = '\n';
					privremeniRacun.id = indeksPronalaska;

					fseek(pDatotekaProcitaj, sizeof(unsigned int) + ((indeksPronalaska) * sizeof(RACUN)), SEEK_SET);
					fwrite(&privremeniRacun, sizeof(RACUN), 1, pDatotekaProcitaj);
					rewind(pDatotekaProcitaj);
					fwrite(pBrojKupaca, sizeof(unsigned int), 1, pDatotekaProcitaj);
					fclose(pDatotekaProcitaj);
					int pom;
					printf("\n\nPritisnite bilo koju tipku za nastavak: ");
					scanf("%d", &pom);
				}
				else {
					int pom;
					printf("          Nepostojeci korisnik\n");
					printf("\n\n          Unesite bilo koji broj za nastavak: ");
					scanf("%d", &pom);
				}
				free(sviKorisnici);
			}
		}
	}
}

void sortiranje(char* datoteka, unsigned int* pBroj) {
	
	FILE* pDatotekaProcitaj = NULL;
	pDatotekaProcitaj = fopen(datoteka, "rb");

	if (pDatotekaProcitaj == NULL) {
		printf("Citanje datoteke %s je neuspjesno", datoteka);
		return;
	}
	else {
		fread(pBroj, sizeof(unsigned int), 1, pDatotekaProcitaj);

		if (*pBroj == 0) {
			printf("Popis je prazan!\n");
			fclose(pDatotekaProcitaj);
			return;
		}

		RACUN* sviKorisnici = NULL;
		RACUN* temp = NULL;
		sviKorisnici = (RACUN*)calloc(*pBroj, sizeof(RACUN));
		temp = (RACUN*)calloc(*pBroj, sizeof(RACUN));

		if (sviKorisnici == NULL || temp == NULL) {
			perror("Greska!");
			exit(EXIT_FAILURE);
		}
		else {
			fread(sviKorisnici, sizeof(RACUN), *pBroj, pDatotekaProcitaj);
			fclose(pDatotekaProcitaj);
			
					for (unsigned int i = 0; i <= *pBroj - 1; i++)
						for (unsigned int j = i + 1; j <= (*pBroj) - 1; j++) {
							int num = atoi((sviKorisnici + i)->dug);
							int num1 = atoi((sviKorisnici + j)->dug);
							if (num > num1) {
								memcpy(temp, (sviKorisnici + i), sizeof(RACUN));
								memcpy((sviKorisnici + i), (sviKorisnici + j), sizeof(RACUN));
								memcpy((sviKorisnici + j), temp, sizeof(RACUN));
							}
						}
		}
		printf("\n   ================================================\n\n");
		printf("          Uzlazno sortiranje po iznosu duga\n");
		printf("\n   ================================================\n\n");
			
			for (unsigned int i = 1; i <= *pBroj - 1; i++) {
				printf("%u\t", (sviKorisnici + i)->id);
				printf("%s ", (sviKorisnici + i)->ime);
				printf("%s ", (sviKorisnici + i)->prezime);
				printf("%s ", (sviKorisnici + i)->adresa_stanovanja);
				printf("%s  ", (sviKorisnici + i)->broj_mobitela);
				printf("%s kn\n", (sviKorisnici + i)->dug);
			}
			int broj;
			printf("\n\n          Upisite bilo koji broj za nastavak: ");
			scanf("%d", &broj);
			free(sviKorisnici);
			free(temp);
	}
}

void brisanjeDatoteke(char* datoteka) {

	FILE* pDatotekaObrisi = NULL;
	pDatotekaObrisi = fopen(datoteka, "rb");

	if (pDatotekaObrisi == NULL) {

		perror("Izbornik 2 - Citanje datoteke");
		return;
		//exit(EXIT_FAILURE);
	}
	else {
		printf("\n   ================================================\n\n");
		printf("            Brisanje datoteke\n");
		printf("\n   ================================================\n\n");

		printf("       Zelite li obrisati datoteku kupci.bin?");
		printf("\n\n               1. Da");
		printf("\n               2. Ne");

		int izbor = -1;

		printf("\n\n               Unesite izbor: ");
		scanf("%d", &izbor);
		fclose(pDatotekaObrisi);
		switch (izbor) {
		case 1: 
			remove(datoteka);
			exit(EXIT_FAILURE);
		case 2:
			break;
		}
	}
}

void menu(char* datoteka, unsigned int* pBrojKupaca) {

	int izbor;

	while (1) {
		system("cls");
		printf("\n   ================================================\n\n");
		printf("          Sustav pracenja racuna trgovine\n");
		printf("\n   ================================================\n\n");
		printf("          1. Unos novih korisnickih racuna\n");
		printf("          2. Pretraga postojecih korisnickih racuna\n");
		printf("          3. Izmjena korisnickih podataka\n");
		printf("          4. Sortiranje korisnika po iznosu duga\n");
		printf("          5. Brisanje datoteke\n");
		printf("          0. Kraj programa\n\n");

		printf("          Odaberite jednu od ponudenih opcija: ");
		scanf("%d", &izbor);
			
		switch (izbor) {
		case 1:
			system("cls");
			unos(datoteka, pBrojKupaca);
			break;
		case 2:
			system("cls");
			pretraga(datoteka, pBrojKupaca);
			break;
		case 3:
			system("cls");
			izmjenaPodataka(datoteka, pBrojKupaca);
			break;
		case 4:
			system("cls");
			sortiranje(datoteka, pBrojKupaca);
			break;
		case 5:
			system("cls");
			brisanjeDatoteke(datoteka);
			break;
		case 0:
			system("cls");
			izlazakIzPrograma();
			break;
		default:
			system("cls");
			printf("\n   Krivo odabrana opcija, molimo pokusajte ponovno\n");
		}
	}
}

void izlazakIzPrograma(void) {
	printf("Dali ste sigurni kako zelite zavrsiti program? [da/ne]: ");
	char izbor[3] = { '\0' };
	scanf(" %2s", izbor);
	if (!strcmp("da", izbor)) {
		exit(EXIT_FAILURE);
	}
	return;
}