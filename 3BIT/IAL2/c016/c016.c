
/* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Položkami
**                      První implementace: Petr Přikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masařík, říjen 2014
**                      Úpravy: Radek Hranický, říjen 2014
**                      Úpravy: Radek Hranický, listopad 2015
**
** Vytvořete abstraktní datový typ
** TRP (Tabulka s Rozptýlenými Položkami = Hash table)
** s explicitně řetězenými synonymy. Tabulka je implementována polem
** lineárních seznamů synonym.
**
** Implementujte následující procedury a funkce.
**
**  HTInit ....... inicializuje tabulku před prvním použitím
**  HTInsert ..... vložení prvku
**  HTSearch ..... zjištění přítomnosti prvku v tabulce
**  HTDelete ..... zrušení prvku
**  HTRead ....... přečtení hodnoty prvku
**  HTClearAll ... zrušení obsahu celé tabulky (inicializace tabulky
**                 poté, co již byla použita)
**
** Definici typů naleznete v souboru c016.h.
**
** Tabulka je reprezentována datovou strukturou typu tHTable,
** která se skládá z ukazatelů na položky, jež obsahují složky
** klíče 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na další synonymum 'ptrnext'. Při implementaci funkcí
** uvažujte maximální rozměr pole HTSIZE.
**
** U všech procedur využívejte rozptylovou funkci hashCode.  Povšimněte si
** způsobu předávání parametrů a zamyslete se nad tím, zda je možné parametry
** předávat jiným způsobem (hodnotou/odkazem) a v případě, že jsou obě
** možnosti funkčně přípustné, jaké jsou výhody či nevýhody toho či onoho
** způsobu.
**
** V příkladech jsou použity položky, kde klíčem je řetězec, ke kterému
** je přidán obsah - reálné číslo.
*/

#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíč a přidělit
** mu index v rozmezí 0..HTSize-1.  V ideálním případě by mělo dojít
** k rovnoměrnému rozptýlení těchto klíčů po celé tabulce.  V rámci
** pokusů se můžete zamyslet nad kvalitou této funkce.  (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku). }
*/

int hashCode ( tKey key ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}

/*
** Inicializace tabulky s explicitně zřetězenými synonymy.  Tato procedura
** se volá pouze před prvním použitím tabulky.
*/

void htInit ( tHTable* ptrht ) {

	int count;

	if ((*ptrht) == NULL)
		return;

	for(count = 0; count < HTSIZE; count++)
	{
		((*ptrht)[count]) = NULL;
	}
	return;
}

/* TRP s explicitně zřetězenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíče key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není, 
** vrací se hodnota NULL.
**
*/

tHTItem* htSearch ( tHTable* ptrht, tKey key ) {

int hashVar;
tHTItem *temp;

	if ((*ptrht) == NULL)
		return NULL;

	hashVar = hashCode(key);

	temp = (*ptrht)[hashVar];

	if(temp != NULL)
	{
		while((temp) != NULL)
		{
			if(temp->key == key)
				return temp;
			else
				temp = temp->ptrnext;
		}
	}
	return NULL;
}

/* 
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vkládá do tabulky ptrht položku s klíčem key a s daty
** data.  Protože jde o vyhledávací tabulku, nemůže být prvek se stejným
** klíčem uložen v tabulce více než jedenkrát.  Pokud se vkládá prvek,
** jehož klíč se již v tabulce nachází, aktualizujte jeho datovou část.
**
** Využijte dříve vytvořenou funkci htSearch.  Při vkládání nového
** prvku do seznamu synonym použijte co nejefektivnější způsob,
** tedy proveďte.vložení prvku na začátek seznamu.
**/

void htInsert ( tHTable* ptrht, tKey key, tData data ) {

int hashVar;
tHTItem *temp_item;
tHTItem *new_item;

	if ((*ptrht) == NULL)
		return;

	hashVar = hashCode(key);
	temp_item = ((*ptrht)[hashVar]);

	/**
	 * Pustam sa do hladania polozky
	 * Ak sa mi podari nasjt hladanu polozku tak prepisem data
	 * Ak sa mi to nepodari tak polozku vytvorim a pridam ju na
	 * zaciatok (pridam klic a data)
	 * Naviazem zbytok
	 * Naviazem to datublky
	 */
	{
		temp_item = htSearch(ptrht, key);
		if (temp_item != NULL) //polozka najdena
		{
			temp_item->data = data;
			return;
		}
		else //polozka nenajdena
		{
			new_item = malloc(sizeof(struct tHTItem));
			new_item->key = key;
			new_item->data = data;
			new_item->ptrnext = (*ptrht)[hashVar];
			(*ptrht)[hashVar] = new_item;
			return;	
		}
	}
	return;
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato funkce zjišťuje hodnotu datové části položky zadané klíčem.
** Pokud je položka nalezena, vrací funkce ukazatel na položku
** Pokud položka nalezena nebyla, vrací se funkční hodnota NULL
**
** Využijte dříve vytvořenou funkci HTSearch.
*/

tData* htRead ( tHTable* ptrht, tKey key ) {

tHTItem *temp_item;
tData *data_pointer;

	if ((*ptrht) == NULL)
		return NULL;

	temp_item = htSearch(ptrht, key);
	if(temp_item == NULL)
		return NULL;
	else
	{
		data_pointer = &((*temp_item).data);
		return data_pointer;
	}

}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vyjme položku s klíčem key z tabulky
** ptrht.  Uvolněnou položku korektně zrušte.  Pokud položka s uvedeným
** klíčem neexistuje, dělejte, jako kdyby se nic nestalo (tj. nedělejte
** nic).
**
** V tomto případě NEVYUŽÍVEJTE dříve vytvořenou funkci HTSearch.
*/

void htDelete ( tHTable* ptrht, tKey key ) {

int hashVar;
tHTItem *temp_item;
tHTItem *next_temp_item;

	/**
	 * Ak je ukazatel na prvok prazdny nerob nic
	 * Inak prehladaj tabulky pokial nenajdes prvok s pozadovanym klucom
	 * Korektne ho uvol a uvolni pamat
	 */
	if ((*ptrht) == NULL)
		return;

	hashVar = hashCode(key);
	temp_item = (*ptrht)[hashVar];
	next_temp_item = temp_item;

	while(temp_item != NULL)
	{
		if(temp_item->key == key)
		{
			if(temp_item != (*ptrht)[hashVar])
				next_temp_item->ptrnext = temp_item->ptrnext;
			else
				(*ptrht)[hashVar] = temp_item->ptrnext;
			free(temp_item);
			return;
		}
		next_temp_item = temp_item;
		temp_item = temp_item->ptrnext;
	}
	return;
}

/* TRP s explicitně zřetězenými synonymy.
** Tato procedura zruší všechny položky tabulky, korektně uvolní prostor,
** který tyto položky zabíraly, a uvede tabulku do počátečního stavu.
*/

void htClearAll ( tHTable* ptrht ) {

int count;
tHTItem *temp_item, *next_temp_item;
 
	/**
	 * Ak je tabulka prazdna nemam co robit return null
	 * Ak nie je prazdna inicializujem pomocne premenne
	 * uvolnujem pokial po maxsize tabulky
	 * pokial je co korektne to uvolnim
	 */

	if ((*ptrht) == NULL)
		return;

	temp_item = NULL;
	next_temp_item = NULL;

	for(count = 0; count < HTSIZE; count++)
	{
		temp_item = (*ptrht)[count];
		while (temp_item != NULL)
		{
			next_temp_item = temp_item->ptrnext;
			free(temp_item);
			temp_item = next_temp_item;
		}
		(*ptrht)[count] = NULL;
	}
}
