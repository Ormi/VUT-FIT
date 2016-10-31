
/* c401.c: **********************************************************}
{* Téma: Rekurzivní implementace operací nad BVS
**                                         Vytvořil: Petr Přikryl, listopad 1994
**                                         Úpravy: Andrea Němcová, prosinec 1995
**                                                      Petr Přikryl, duben 1996
**                                                   Petr Přikryl, listopad 1997
**                                  Převod do jazyka C: Martin Tuček, říjen 2005
**                                         Úpravy: Bohuslav Křena, listopad 2009
**                                         Úpravy: Karel Masařík, říjen 2013
**                                         Úpravy: Radek Hranický, říjen 2014
**                                         Úpravy: Radek Hranický, listopad 2015
**
** Implementujte rekurzivním způsobem operace nad binárním vyhledávacím
** stromem (BVS; v angličtině BST - Binary Search Tree).
**
** Klíčem uzlu stromu je jeden znak (obecně jím může být cokoliv, podle
** čeho se vyhledává). Užitečným (vyhledávaným) obsahem je zde integer.
** Uzly s menším klíčem leží vlevo, uzly s větším klíčem leží ve stromu
** vpravo. Využijte dynamického přidělování paměti.
** Rekurzivním způsobem implementujte následující funkce:
**
**   BSTInit ...... inicializace vyhledávacího stromu
**   BSTSearch .... vyhledávání hodnoty uzlu zadaného klíčem
**   BSTInsert .... vkládání nové hodnoty
**   BSTDelete .... zrušení uzlu se zadaným klíčem
**   BSTHeight .... výpočet výšky stromu
**   BSTDispose ... zrušení celého stromu
**
** ADT BVS je reprezentován kořenovým ukazatelem stromu (typ tBSTNodePtr).
** Uzel stromu (struktura typu tBSTNode) obsahuje klíč (typu char), podle
** kterého se ve stromu vyhledává, vlastní obsah uzlu (pro jednoduchost
** typu int) a ukazatel na levý a pravý podstrom (LPtr a RPtr). Přesnou definici typů 
** naleznete v souboru c401.h.
**
** Pozor! Je třeba správně rozlišovat, kdy použít dereferenční operátor *
** (typicky při modifikaci) a kdy budeme pracovat pouze se samotným ukazatelem 
** (např. při vyhledávání). V tomto příkladu vám napoví prototypy funkcí.
** Pokud pracujeme s ukazatelem na ukazatel, použijeme dereferenci.
**/

#include "c401.h"
int solved;

void BSTInit (tBSTNodePtr *RootPtr) {
/*   -------
** Funkce provede počáteční inicializaci stromu před jeho prvním použitím.
**
** Ověřit, zda byl již strom předaný přes RootPtr inicializován, nelze,
** protože před první inicializací má ukazatel nedefinovanou (tedy libovolnou)
** hodnotu. Programátor využívající ADT BVS tedy musí zajistit, aby inicializace
** byla volána pouze jednou, a to před vlastní prací s BVS. Provedení
** inicializace nad neprázdným stromem by totiž mohlo vést ke ztrátě přístupu
** k dynamicky alokované paměti (tzv. "memory leak").
**	
** Všimněte si, že se v hlavičce objevuje typ ukazatel na ukazatel.	
** Proto je třeba při přiřazení přes RootPtr použít dereferenční operátor *.
** Ten bude použit i ve funkcích BSTDelete, BSTInsert a BSTDispose.
**/
	
	*RootPtr = NULL;
	return;

}	

int BSTSearch (tBSTNodePtr RootPtr, char K, int *Content)	{
/*  ---------
** Funkce vyhledá uzel v BVS s klíčem K.
**
** Pokud je takový nalezen, vrací funkce hodnotu TRUE a v proměnné Content se
** vrací obsah příslušného uzlu.´Pokud příslušný uzel není nalezen, vrací funkce
** hodnotu FALSE a obsah proměnné Content není definován (nic do ní proto
** nepřiřazujte).
**
** Při vyhledávání v binárním stromu bychom typicky použili cyklus ukončený
** testem dosažení listu nebo nalezení uzlu s klíčem K. V tomto případě ale
** problém řešte rekurzivním volání této funkce, přičemž nedeklarujte žádnou
** pomocnou funkci.
**/

	if (RootPtr != NULL)
	{
	/**
	 * Ak som polozku nasiel pomocou hladaneho kluca tak priradim obsah
	 * Ak som polozku nenasiel a kluc sa nerovna volam funkciu rekurzivne
	 * na lavo od katualneho korena alebo napravo a prehladavam na nizsie urovne
	 */
		if(RootPtr->Key == K)
		{
			*Content = RootPtr->BSTNodeCont;
			return true;
		}
		else if(RootPtr->Key != K)
		{
			if(RootPtr->Key < K)
				return BSTSearch(RootPtr->RPtr, K, Content);
			else
				return BSTSearch(RootPtr->LPtr, K, Content); 
		}
	}			   
	return false;
} 


void BSTInsert (tBSTNodePtr* RootPtr, char K, int Content)	{	
/*   ---------
** Vloží do stromu RootPtr hodnotu Content s klíčem K.
**
** Pokud již uzel se zadaným klíčem ve stromu existuje, bude obsah uzlu
** s klíčem K nahrazen novou hodnotou. Pokud bude do stromu vložen nový
** uzel, bude vložen vždy jako list stromu.
**
** Funkci implementujte rekurzivně. Nedeklarujte žádnou pomocnou funkci.
**
** Rekurzivní implementace je méně efektivní, protože se při každém
** rekurzivním zanoření ukládá na zásobník obsah uzlu (zde integer).
** Nerekurzivní varianta by v tomto případě byla efektivnější jak z hlediska
** rychlosti, tak z hlediska paměťových nároků. Zde jde ale o školní
** příklad, na kterém si chceme ukázat eleganci rekurzivního zápisu.
**/
		
tBSTNodePtr tree_temp;

	/**
	 * Ak najdeny koren nie je null a kluce suhlasia idem
	 * bud vlavo alebo vpravo a priradim prvok
	 * Ak koren je null tak vytvaram novy prvok
	 */

	if(*RootPtr != NULL)
	{
		if((*RootPtr)->Key == K)
		{
			(*RootPtr)->BSTNodeCont = Content;
			return;			
		}
		else if ((*RootPtr)->Key > K)
		{
            BSTInsert((&(*RootPtr)->LPtr), K, Content);
            return;
		}
		else if ((*RootPtr)->Key < K)
		{
            BSTInsert((&(*RootPtr)->RPtr), K, Content);
            return;
		}
	}
	else if (*RootPtr == NULL)
	{
		tree_temp = malloc(sizeof(struct tBSTNode));

		if(tree_temp != NULL)
		{
			*RootPtr = tree_temp;
			(*RootPtr)-> LPtr = NULL;
			(*RootPtr)-> RPtr = NULL;
			(*RootPtr)->Key = K;
			(*RootPtr)->BSTNodeCont = Content;
		}
		else if (tree_temp == NULL)
			return;
	}			
	return;
}

void ReplaceByRightmost (tBSTNodePtr PtrReplaced, tBSTNodePtr *RootPtr) {
/*   ------------------
** Pomocná funkce pro vyhledání, přesun a uvolnění nejpravějšího uzlu.
**
** Ukazatel PtrReplaced ukazuje na uzel, do kterého bude přesunuta hodnota
** nejpravějšího uzlu v podstromu, který je určen ukazatelem RootPtr.
** Předpokládá se, že hodnota ukazatele RootPtr nebude NULL (zajistěte to
** testováním před volání této funkce). Tuto funkci implementujte rekurzivně. 
**
** Tato pomocná funkce bude použita dále. Než ji začnete implementovat,
** přečtěte si komentář k funkci BSTDelete(). 
**/

tBSTNodePtr tree_temp;
	
	if(*RootPtr == NULL)
		return;
	else
	{
		tree_temp = NULL;

		if((*RootPtr)->RPtr == NULL)
		{
			tree_temp = *RootPtr;
			PtrReplaced->BSTNodeCont = tree_temp->BSTNodeCont;
			PtrReplaced->Key = tree_temp->Key;
			*RootPtr = tree_temp->LPtr;
			free(tree_temp);
			return;
		}
		else if ((*RootPtr)->RPtr != NULL)
		{
            ReplaceByRightmost(PtrReplaced ,&(*RootPtr)->RPtr);
            return ;
		}
	}	
	return;
}

void BSTDelete (tBSTNodePtr *RootPtr, char K) 		{
/*   ---------
** Zruší uzel stromu, který obsahuje klíč K.
**
** Pokud uzel se zadaným klíčem neexistuje, nedělá funkce nic. 
** Pokud má rušený uzel jen jeden podstrom, pak jej zdědí otec rušeného uzlu.
** Pokud má rušený uzel oba podstromy, pak je rušený uzel nahrazen nejpravějším
** uzlem levého podstromu. Pozor! Nejpravější uzel nemusí být listem.
**
** Tuto funkci implementujte rekurzivně s využitím dříve deklarované
** pomocné funkce ReplaceByRightmost.
**/

tBSTNodePtr tree_temp;
	
	/**
	 * ak je co mazat tak pokracujem inak vraciam return
	 * Ak najdem kluc, ktory hladam tak pomocoou pomocnej premennej
	 * mazem bud vlavo alebo vpravo a korektne uvolnujem
	 */

	if(*RootPtr != NULL)
	{
		if((*RootPtr)->Key == K)
		{
			tree_temp = *RootPtr;

			if((*RootPtr)->LPtr == NULL)
			{
                *RootPtr = (*RootPtr)->RPtr;
                free(tree_temp);
                return;				
			}
			else if ((*RootPtr)->RPtr == NULL)
			{
                *RootPtr = (*RootPtr)->LPtr;
                free(tree_temp);
                return;			
			}
			else ReplaceByRightmost(*RootPtr, &((*RootPtr)->LPtr));
		}
		else if ((*RootPtr)->Key > K)
		{
            BSTDelete((&(*RootPtr)->LPtr), K);
            return;
		}
		else if ((*RootPtr)->Key < K)
		{
            BSTDelete((&(*RootPtr)->RPtr), K);
            return;
		}
	}
	return;
} 

int BSTHeight (tBSTNodePtr NodePtr, bool IsRoot) {	
/*   ----------
** Vypočítá výšku BVS. Výška stromu délka (počet hran) nejdelší cesty
** od kořene k listu. Vzhledem k rekurzivní implementaci je třeba rozlišit,
** zda funkci voláme pro samotný kořen stromu nebo rekurzivně pro některý
** z ostatních uzlů.
** Výpočet výšky stromu se tedy bude provádět voláním:
**   BSTHeight(ukazatel_na_kořen, TRUE)
**
** Návratová hodnota je výška stromu. Výška prázdného stromu však není definována.
** V případě prázdného stromu bude funkce vracet hodnotu -1. 
** 
** Tuto funkci implementujte bez deklarování pomocné funkce.
**/
	
int tree_temp_left, tree_temp_right, tree_max;

	if (NodePtr == NULL)
		return -1;
	{
		tree_temp_left = BSTHeight((NodePtr)->LPtr, true);
		tree_temp_right = BSTHeight((NodePtr)->RPtr, true);

		if (tree_temp_left > tree_temp_right)
			tree_max = tree_temp_left + 1;
		else
			tree_max = tree_temp_right + 1;
	}
	return tree_max;
}

void BSTDispose (tBSTNodePtr *RootPtr) {	
/*   ----------
** Zruší celý binární vyhledávací strom a korektně uvolní paměť.
**
** Po zrušení se bude BVS nacházet ve stejném stavu, jako se nacházel po
** inicializaci. Tuto funkci implementujte rekurzivně bez deklarování pomocné
** funkce.
**/
	
	if(*RootPtr == NULL)
		return;
	else
	{
	    /**
	     * Ak mozem ist dolava tak idem kolko mozem a rusim listy
	     * Ak mozem ist doprava idem kolko mozem a rusim listy
	     * ak uz nemozem ist dolava ani doprava tak zrusim koren
	     */
			if((*RootPtr)->LPtr != NULL)
			{
                BSTDispose(&(*RootPtr)->LPtr);
			}
			if ((*RootPtr)->RPtr != NULL)
			{
                BSTDispose(&(*RootPtr)->RPtr);	
			}
			if (((*RootPtr)->LPtr == NULL) && ((*RootPtr)->LPtr == NULL))
			{
				free(*RootPtr);
			}
			*RootPtr = NULL;
	}	
	return;
}

/* konec c401.c */

