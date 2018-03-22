/**
 * Subor:         main.c
 * Verzia:        original 1.0
 * Datum:		      December 2016
 *
 * Predmet:       IMP (2016)
 * Projekt:       Konfigurovatelny OCT citac
 *
 * Author:        Michal Ormos(xormos00), 3BIT
 * 
 * Fakulta:       Fakulta Informacnych Technologii,
 *                Vysoke uceni technicke v Brne,
 *                Czech Republic
 *
 * E-mails:       xormos00@stud.fit.vutbr.cz
 *
 * Popis:         Implementacia oktaloveho citaca s rezimami
 *                startu, nastaveni a behu.
 */

#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */


unsigned char *seg1;
unsigned char *seg2;
unsigned char *dlsw;

//Seg7 numbers
#define ZERO  0b00111111
#define ONE   0b00000110
#define TWO   0b01011011
#define THREE 0b01001111
#define FOUR  0b01100110
#define FIVE  0b01101101
#define SIX   0b01111101
#define SEVEN 0b00000111

#define SIZE 4
//   0    1    2    3
// 0000 0000 0000 0000
int CNTN[SIZE];

int MODE;
int BLIKANIE = 1;
int FIRSTSTART = 0;
int bliknutie;
int setCNT = 0;

int i;

/**
 * Namapovanie portov segmentacnych displejov a DILswitchu
 *  do premennych
 */
void setAddress() {
  seg1 = (char *) 617;
  seg2 = (char *) 618;
  dlsw = (char *) 619;  
}

/**
 * Inicializacia, obe seg displeje na nulu, vsetky DILswitch prepinace
 * na nulu, CNTN vynulovane
 */
void init1() {
  *seg1 = 0x00; 
  *seg2 = 0x00;
  *dlsw = 0x00;
  
  for(i=0; i<4; i++) {
    CNTN[i] = 0; 
  }
}  

/**
 * Prvotna inicializacia po stpusteni programu
 * Rozsvietenie displejov a este raz vynulovanie CNTN
 */
void init2() {
  *seg1 = ZERO; 
  *seg2 = ZERO;
  *dlsw = 0b10000000;
  
  for(i=0; i<4; i++) {
    CNTN[i] = 0; 
  }
} 

void CNTNmax() {
  for(i=0; i<4; i++) {
    CNTN[i] = 7; 
  }
}
 
/**
 * Pomocna funkcia pre vypisovanie na seg displeje
 * @param int num cislica aku chcem vypisat
 * @param unsugned char *seg, na ktory displej to chcem vypisat
 */ 
void segShow(int num, unsigned char *seg) {
    switch(num) {
      case 0:
        *seg = ZERO;
        break;
      case 1:
        *seg = ONE;
        break;
      case 2:
        *seg = TWO;
        break;
      case 3:
        *seg = THREE;
        break;
      case 4:
        *seg = FOUR;
        break;
      case 5:
        *seg = FIVE;
        break;
      case 6:
        *seg = SIX;
        break; 
      case 7:
        *seg = SEVEN;
        break;
      default:
        break;
    }
}

void blikanie() {
  if (BLIKANIE == 1) {
    *seg1 = 0x00;
    *seg2 = 0x00;
    BLIKANIE = 0;
  } else {
    segShow(0, seg1);
    segShow(7, seg2);    
    BLIKANIE = 1;
  }
  bliknutie++;
  if(bliknutie == 3) {
    bliknutie = 0;  
    *dlsw = 0b10000000;
    MODE = 1; 
    FIRSTSTART = 0; 
    
  }  
}

/*********************************************************/

/**
 * Funkcia pre oblushu START s pocitanim smerom hore
 * Inkrementuje CNTN od najnizsieho (laveho) po pravy 
 * v sedmickovej sustave
 */
void countUp() {
 if(CNTN[3] < 7) {
  CNTN[3] = CNTN[3] + 1;
 } else if(CNTN[2] < 7) {
  CNTN[2] = CNTN[2] + 1;
  CNTN[3] = 0;
 } else if(CNTN[1] < 7) {
  CNTN[1] = CNTN[1] + 1;
  CNTN[3] = 0;
  CNTN[2] = 0;
 } else if(CNTN[0] < 7) {
  CNTN[0] = CNTN[0] + 1;
  CNTN[3] = 0;
  CNTN[2] = 0;
  CNTN[1] = 0;  
 } else if(CNTN[0] == 7) {
  blikanie();
 }
 for(i=0; i<4; i++) {
  if(CNTN[i] > 0) {
    segShow(i, seg1);
    segShow(CNTN[i], seg2);
    break;
  }
 }  
}

/**
 * Pracuje obobne ako countUp, ale pocita smerom dole
 */
void countDown() {
 if(CNTN[3] > 0) {
  CNTN[3] = CNTN[3] - 1;
 } else if(CNTN[2] > 0) {
  CNTN[2] = CNTN[2] - 1;
  CNTN[3] = 7;
 } else if(CNTN[1] > 0) {
  CNTN[1] = CNTN[1] - 1;
  CNTN[3] = 7;
  CNTN[2] = 7;
 } else if(CNTN[0] > 0) {
  CNTN[0] = CNTN[0] - 1;
  CNTN[3] = 7;
  CNTN[2] = 7;
  CNTN[1] = 7;  
 } else if (CNTN[0] == 0){
  blikanie();
 }
  for(i=0; i<4; i++) {
  if(CNTN[i] != 7) {
    segShow(i, seg1);
    segShow(CNTN[i], seg2);
    break;
  }
 }  
}

/**
 * Funkcia pre zmenu laveho Seg
 * urcuje index CNTN, ktoreho chceme menit
 * Cita hodnoty DIL switchu pre obsluhu SET 
 */ 
void setCNTN() {
  switch(*dlsw) {
    case 0b10000000:
      setCNT = 0;
      break;
    case 0b10000001:     
      setCNT = 1;
      break;
    case 0b10000010:    
      setCNT = 2;
      break;
    case 0b10000011:
      setCNT = 3;
      break;                                  
  }
  if (BLIKANIE == 0) {
    *seg1 = 0x00;
    BLIKANIE = 1;
  } else {    
    segShow(setCNT, seg1);
    BLIKANIE = 0;
  }
}

/**
 * Funkcia pre zmenu praveho Seg
 * urcuje hodnotu indexu CNTN, ktoreho chceme menit
 * Cita hodnoty DIL switchu pre obsluhu SET 
 * Uklada ich do CNTN pre buduce pouzitie a zobrazuje ich an Seg
 * @param int cntn index CNTN ktoremu ideme menit hodnotu
 */
void setCNTNvalue(int cntn) {
  switch(*dlsw) {
    case 0b10010000:
      CNTN[cntn] = 0;
      break; 
    case 0b10010001:      
      CNTN[cntn] = 1;
      break;
    case 0b10010010:    
      CNTN[cntn] = 2;
      break;
    case 0b10010011:    
      CNTN[cntn] = 3;
      break;                  
    case 0b10010100:
      CNTN[cntn] = 4;
      break;
    case 0b10010101:    
      CNTN[cntn] = 5;
      break;
    case 0b10010110:    
      CNTN[cntn] = 6;
      break;
    case 0b10010111:
      CNTN[cntn] = 7;
      break;                  
  }
  segShow(setCNT, seg1);
  if (BLIKANIE == 0) {
    *seg2 = 0x00;
    BLIKANIE = 1;
  } else {    
    segShow(CNTN[cntn], seg2);
    BLIKANIE = 0;
  }  
}

/**
 * Hlavne jadro, ktore rozhoduje aka akcia
 * bude vykonana vzhladom na stav DIL switchu
 */
void checkMode() {
  switch(*dlsw) {
    //STOP - counter off '0XXX XXXX'
    case 0b00000000:
      MODE = 1;
      break;
    
    //SET - counter on '10X0 XXXX' seg on  
    case 0b10000000:
      if(FIRSTSTART == 0) { //rozsvietenie displejov
        init2(); 
        FIRSTSTART = 1;      
      }
      MODE = 2;
      break; 

    // SET - settings for RSeg7 '10X1 XXXX'
    case 0b10010000:
    case 0b10010001:
    case 0b10010010:
    case 0b10010011:
      MODE = 3;
      break;              
  
    //START - count up '110X XXXX'
    case 0b11000000:
      MODE = 4;
      break;
    
    //START - count down '111X XXXX'
    case 0b11100000:
      MODE = 5;
      break;
      
    case 0b10001000:
      MODE = 6;
      break;
    
    default:
      break;
  }
} 


void run() {
  checkMode();
  switch(MODE) {
    case 1:
      break;
    case 2: 
      setCNTN();
      break;
    case 3:     
      setCNTNvalue(setCNT);
      break;      
    case 4:
      countDown();
      break;
    case 5:
      countUp();    
      break;
    case 6:
      CNTNmax();
      break;
    default:
      break;
  }
} 

void spomalenie() {
  for(i=0; i<10000; i++) {
  }  
}

/**
 * Zachytavanie prerusenia od RTCSC
 * Deklarovane v Project.prm
 */
interrupt 29 void RTC_ISR(void) {
     run(); 
     //spomalenie();
}
  
void main(void) {

  EnableInterrupts; /* enable interrupts */
  /* include your code here */     
 
  RTCSC = 0b00010100;

  setAddress();
  init1();
  
  SOPT1 = 0;

  for(;;) {
   
    __RESET_WATCHDOG(); /* feeds the dog */

  } /* loop forever */
  /* please make sure that you never leave main */
}
