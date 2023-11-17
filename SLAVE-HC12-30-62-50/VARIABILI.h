IPAddress controller(192, 168, 81, 90);

int contatore = 0;                          // per determinare ogni quanto stampare a video nel loop

boolean bloc = false;                    // per attivare lo stop
boolean fast = false;                    // per attivare il fast_ward();

float tensione = 1;                        // valore attuale di speed usato in limit() non deve mai essere 0

char Stato_Anta[1] = {'F'};                   // indica il movimento R> Fermo in Reset F> fermo V> veloce L> lento C> collisione S> emergenza T> tuttuchiuso A> tuttoaperto U> up anta alta D> down anta giù
char Stato_Alzata[1] = {'I'};                // indica lo stato del motore d'alzata I> indeterminato U> up anta alta D> down anta giù

volatile long pos = 0;                    // VALORE ATTUALE DELLA POSIZIONE
long pos_chiuso = 0;                      // VALORE DELLA POSIZIONE TUTTO CHIUSO
long pos_aperto = 4100;//7900; //16000;                  // VALORE DELLA POSIZIONE TUTTO APERTO
long pos_apri_fisso = 0;
long Pos_vecchio = 0;
long pos_precedente = 0;
boolean apri = false;                     // per il spingi e vai
boolean chiudi = false;                   // per il spingi e vai

boolean porta_tutta_aperta = false;       // stato porta aperta
boolean porta_tutta_chiusa = false;       // stato porta chiusa

int str;                                  // per il comando i2c
String str2;                              // per il valore associato al comando i2c
float str3;                               // per il valore associato al comando i2c
String str_old;

char azione[32] = "";                     // -- QUESTA E' LA STRINGA DA INTERPRETARE

int _Dir = 0;                             // sostituirà forw e back

int set = 0;                              // 1 SE IL SET-UP E' STATO FATTO
char cstr[7] = {'0', '0', '0', '0', '0', '0', '\0'};
char cstr_emergenza[16] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '\0'};
String str_emergenza = "";
int TopSpeed = 1200;    // riscalato nelle lib del MC33926

float imp = 6.60;  //2.8 ibrido;   6.6 intecno;  80 CBF;      // inpulsi per millimetro cambia in base al CPR del encoder
volatile int giro = 0;
volatile unsigned long T1 = 0;                  // PER MISURARE IL TEMPO TRA UN POS E L'ALTRO
volatile unsigned long T11 = 0;                  // PER MISURARE IL TEMPO TRA UN POS E L'ALTRO
volatile unsigned long T2 = 0;                  // PER MISURARE IL TEMPO TRA UN POS E L'ALTRO
volatile unsigned long T3 = 100000;             // PER MISURARE IL TEMPO TRA UN POS E L'ALTRO
volatile unsigned long T5 = 0;                  // PER MISURARE IL TEMPO TRA UN POS E L'ALTRO
float T4 = 0.00;                                // per convertire il formato

int corrente[20];
int conta_corrente = 0;

int amp;                                        // per misurare l'assorbimento di corrente sortituisce l

boolean zona_no_control = false ;

char _eccomi[10] = "";

int iCrocera = 0;                               // indica il valore della speed all'uscita da crocera()


float velocita_misurata;                        // VELOCITA' misurata da tachimetro()
volatile int v_attuale = 0 ;                    // VELOCITA' ATTUALE IN 0-400 PER L'ACCELERAZIONE E LA DECELLERAZIONE
float velocita_crocera = 0 ;                    // VELOCITA' ATTUALE DELL'ANTA IN cm/sec
float velocita_crocera_SET = 70;                //******** NON PIU' USATO           // VELOCITA'  IN mm/sec DURANTE IL SET-UP

float quasiChiuso = 5;                            // espresso in mm lo lascerei fisso non modificabile
float quasiChiuso_mm = 5;
float spazioRallenta = 300;        // si        // espresso in mm
float spazioRallenta_mm = 300;
float spazioRallenta_hold;                        // serve per salvare il valore iniziale
int spazio_no_chk_vel = 1;         // si            // espresso in mm
int spazio_no_chk_vel_mm = 1;

float velocita_crocera_COL = 70;   // si          // VELOCITA IN CASO DI COLLISIONE IN mm/sec
float velocita_crocera_MAX = 110;  // si         // VELOCITA' MASSIMA IN mm/sec
float velocita_crocera_MIN = 55;   // si          // VELOCITA' LENTO IN mm/sec
float velocita_crocera_CHI = 45;   // si          // VELOCITA' LENTO IN mm/sec

float V_Range_UP = 0.05;           // si                     // usata in controllo crociera per determinare il raggiongimento del targhet
float V_Range_Down = 0.05;
float attrito = 0.7;               // si                     // usata in controllo velocità per compensare attriti etc...
int motore = -1;                   // si                     // indica il cablaggio del motore di defoult 1
int encoder = -1;                  // si                     // indica il cablaggio del encoder di defoult 1
int inerzia = 1;                   // si                     // usato nel controllo accellrrazione
int peso = 10;                     // si


int Conto_Emergenze = 0;           // -- serve per contare le emergenze che avvengono nel Tempo_Reazione_Emergenza
long Tempo_Reazione_Emergenza = 0;
volatile float V_M = 0;
String str_limit = "";
int marginelimite = 1;//0-100 è la percentuale usata come limite del valore nel array limiti
int limiti[25] = {250, 250, 260, 270, 280, 290, 300, 390, 510, 690, 900, 1080, 1300, 1580, 1930, 2250, 2500, 2750, 2850, 2930, 2990, 3020, 3050, 3070, 3080}; //punti della spezzata
volatile float test_vel[5] = {0.00, 0.00, 0.00, 0.00, 0.00};


boolean  si_stop_sblocco = false;
int array_assorbimento[100];
float tensione_batteria = 0.00;
int conteggio = 0;
