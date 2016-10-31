
/* c201.c *********************************************************************}
{* T�ma: Jednosm�rn� line�rn� seznam
**
**                     N�vrh a referen�n� implementace: Petr P�ikryl, ��jen 1994
**                                          �pravy: Andrea N�mcov� listopad 1996
**                                                   Petr P�ikryl, listopad 1997
**                                P�epracovan� zad�n�: Petr P�ikryl, b�ezen 1998
**                                  P�epis do jazyka C: Martin Tu�ek, ��jen 2004
**	                                      �pravy: Bohuslav K�ena, ��jen 2015
**
** Implementujte abstraktn� datov� typ jednosm�rn� line�rn� seznam.
** U�ite�n�m obsahem prvku seznamu je cel� ��slo typu int.
** Seznam bude jako datov� abstrakce reprezentov�n prom�nnou typu tList.
** Definici konstant a typ� naleznete v hlavi�kov�m souboru c201.h.
** 
** Va��m �kolem je implementovat n�sleduj�c� operace, kter� spolu s v��e
** uvedenou datovou ��st� abstrakce tvo�� abstraktn� datov� typ tList:
**
**      InitList ...... inicializace seznamu p�ed prvn�m pou�it�m,
**      DisposeList ... zru�en� v�ech prvk� seznamu,
**      InsertFirst ... vlo�en� prvku na za��tek seznamu,
**      First ......... nastaven� aktivity na prvn� prvek,
**      CopyFirst ..... vrac� hodnotu prvn�ho prvku,
**      DeleteFirst ... zru�� prvn� prvek seznamu,
**      PostDelete .... ru�� prvek za aktivn�m prvkem,
**      PostInsert .... vlo�� nov� prvek za aktivn� prvek seznamu,
**      Copy .......... vrac� hodnotu aktivn�ho prvku,
**      Actualize ..... p�ep�e obsah aktivn�ho prvku novou hodnotou,
**      Succ .......... posune aktivitu na dal�� prvek seznamu,
**      Active ........ zji��uje aktivitu seznamu.
**
** P�i implementaci funkc� nevolejte ��dnou z funkc� implementovan�ch v r�mci
** tohoto p��kladu, nen�-li u dan� funkce explicitn� uvedeno n�co jin�ho.
**
** Nemus�te o�et�ovat situaci, kdy m�sto leg�ln�ho ukazatele na seznam 
** p�ed� n�kdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodn� komentujte!
**
** Terminologick� pozn�mka: Jazyk C nepou��v� pojem procedura.
** Proto zde pou��v�me pojem funkce i pro operace, kter� by byly
** v algoritmick�m jazyce Pascalovsk�ho typu implemenov�ny jako
** procedury (v jazyce C procedur�m odpov�daj� funkce vracej�c� typ void).
**/

#include "c201.h"

int solved;
int errflg;

void Error() {
/*
** Vytiskne upozorn�n� na to, �e do�lo k chyb�.
** Tato funkce bude vol�na z n�kter�ch d�le implementovan�ch operac�.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;                      /* glob�ln� prom�nn� -- p��znak chyby */
}

void InitList (tList *L) {
/*
** Provede inicializaci seznamu L p�ed jeho prvn�m pou�it�m (tzn. ��dn�
** z n�sleduj�c�ch funkc� nebude vol�na nad neinicializovan�m seznamem).
** Tato inicializace se nikdy nebude prov�d�t nad ji� inicializovan�m
** seznamem, a proto tuto mo�nost neo�et�ujte. V�dy p�edpokl�dejte,
** �e neinicializovan� prom�nn� maj� nedefinovanou hodnotu.
**/

	/**
	 * Inicializacia zoznamu, aktivnemu a poslednemu prvku priradime NULL
	 */
	L->Act = NULL;
	L->First = NULL;
}

void DisposeList (tList *L) {
/*
** Zru�� v�echny prvky seznamu L a uvede seznam L do stavu, v jak�m se nach�zel
** po inicializaci. Ve�ker� pam� pou��van� prvky seznamu L bude korektn�
** uvoln�na vol�n�m operace free.
***/

	tElemPtr pom_val;
 	pom_val = NULL;

 	/**
 	 * Cyklus prechadza zoznamu od prveho po posledny prvok
 	 * Presuva ukazatel first vzdy na nasledujuci prvok a uvolnuje
 	 * aktivny prvok pokial nie je First ukazatelom s hodnotou NULL 
 	 */
 	while(L->First != NULL)
 	{
 		L->Act = L->First;
 		L->First = L->First->ptr;
 		pom_val = L->Act;
 		free(pom_val);
 	}

 	L->Act = NULL;
}

void InsertFirst (tList *L, int val) {
/*
** Vlo�� prvek s hodnotou val na za��tek seznamu L.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci Error().
**/
 
 	tElemPtr pom_val;
 	pom_val = NULL;

 	/**
 	 * Alokujeme pamat pre pomocny ukazatel pom_val, inak SegFault
 	 * Pomocou pomecnej premennej priradime prvok zoznamu
 	 * Prva polozka zoznamu bude nova polozka pomocnej premennej
 	 */
 	if((pom_val = malloc(sizeof(struct tElem))) == NULL)
		Error();
	else
	{
 		pom_val->data = val;		
 		pom_val->ptr = L->First;
 		L->First = pom_val;
	}
}

void First (tList *L) {
/*
** Nastav� aktivitu seznamu L na jeho prvn� prvek.
** Funkci implementujte jako jedin� p��kaz, ani� byste testovali,
** zda je seznam L pr�zdn�.
**/
	L->Act = L->First;
}

void CopyFirst (tList *L, int *val) {
/*
** Prost�ednictv�m parametru val vr�t� hodnotu prvn�ho prvku seznamu L.
** Pokud je seznam L pr�zdn�, vol� funkci Error().
**/
	
	/**
	 * Ak je zoznam prazdny, tak je to chyba
	 * Inak uloz hodnotu prveho prvku zoznamu
	 */
	if(L->First == NULL)
		Error();
	else
		*val = L->First->data;
}

void DeleteFirst (tList *L) {
/*
** Zru�� prvn� prvek seznamu L a uvoln� j�m pou��vanou pam�.
** Pokud byl ru�en� prvek aktivn�, aktivita seznamu se ztr�c�.
** Pokud byl seznam L pr�zdn�, nic se ned�je.
**/
	
	tElemPtr pom_val;
 	pom_val = NULL;

 	/**
 	 * Ak je zoznam prazdnuy, tak sa nid nedeje
 	 * Ak je ruseny prvok aktivny, aktivita zaznamu sa straca
 	 */
 	if(L->First != NULL)
 	{
 		if(L->First == L->Act)
 			L->Act = NULL;
 		/**
 		 * Uprava pointeru na prvy prvok
 		 * Uvolnenie pamati
 		 */
 		pom_val = L->First;
 		L->First = pom_val->ptr;
 		free(pom_val);
 	}
}	

void PostDelete (tList *L) {
/* 
** Zru�� prvek seznamu L za aktivn�m prvkem a uvoln� j�m pou��vanou pam�.
** Pokud nen� seznam L aktivn� nebo pokud je aktivn� posledn� prvek seznamu L,
** nic se ned�je.
**/

	tElemPtr pom_val;
 	pom_val = NULL;

 	/**
 	 * Kontrola ci je zoznam aktivny, ak nie nic sa nedeje
 	 * Kontrola ci je aktivny posledny prvok zoznamu, ak nie nic sa nedeje
 	 */
 	if (L->Act != NULL)
 	{
 		if (L->Act->ptr != NULL)
 		{
 			/**
 			 * Ulozenie odkazu na dalsi prvok do aktivneho prvku
 			 * Uvolnenie pamati
 			 */
 			pom_val = L->Act->ptr;
 			L->Act->ptr = pom_val->ptr;
 			free(pom_val);
 		}
 	}
}

void PostInsert (tList *L, int val) {
/*
** Vlo�� prvek s hodnotou val za aktivn� prvek seznamu L.
** Pokud nebyl seznam L aktivn�, nic se ned�je!
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** zavol� funkci Error().
**/
	
	tElemPtr pom_val;
 	pom_val = NULL;

 	/**
 	 * Ak je zoznam aktivny preved alokaciu pamate pre pomocnu premennu
 	 * V pripade nedostatku pamati pri alokacii chyba
 	 */
 	if (L->Act != NULL)
 	{
 	 	if((pom_val = malloc(sizeof(struct tElem))) == NULL)
			Error();
		else	
 		/**
 		 * Novy prvok ukazuje na nasledujuci
 		 * Data noveho prvku maju hodnotu val
 	 	 * Uprava odkazu na novy prvok
         */
        {
 		pom_val->data = val;
 		pom_val->ptr = L->Act->ptr;
 		L->Act->ptr = pom_val;
 		}
 	}
}

void Copy (tList *L, int *val) {
/*
** Prost�ednictv�m parametru val vr�t� hodnotu aktivn�ho prvku seznamu L.
** Pokud seznam nen� aktivn�, zavol� funkci Error().
**/
	
	/**
	 * Ak zoznam nie je aktivny jedna sa o chybu
	 * inak preved operaciu Copy
	 */
	if (L->Act == NULL)
		Error();
	else
		*val = L->Act->data;
}

void Actualize (tList *L, int val) {
/*
** P�ep�e data aktivn�ho prvku seznamu L hodnotou val.
** Pokud seznam L nen� aktivn�, ned�l� nic!
**/

	/**
	 * Kontrola ci je zoznam aktivny
	 * ak ano preved Actualize inak sa nic nedeje
	 */
	if (L->Act != NULL)
		L->Act->data = val;
}

void Succ (tList *L) {
/*
** Posune aktivitu na n�sleduj�c� prvek seznamu L.
** V�imn�te si, �e touto operac� se m��e aktivn� seznam st�t neaktivn�m.
** Pokud nen� p�edan� seznam L aktivn�, ned�l� funkce nic.
**/

	/**
	 * Kontrola ci je zoznam aktivny
	 * ak ano preved Succ inak sa nic nedeje
	 */
	if (L->Act != NULL)
		L->Act = L->Act->ptr;
}

int Active (tList *L) {		
/*
** Je-li seznam L aktivn�, vrac� nenulovou hodnotu, jinak vrac� 0.
** Tuto funkci je vhodn� implementovat jedn�m p��kazem return. 
**/
	
	return((L->Act != NULL) ? TRUE : FALSE);
}

/* Konec c201.c */
