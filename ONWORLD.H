typedef char CHAR8[8];

extern double         *__lpeur_;
extern int            *__lpeui_;
extern unsigned char  *__lpeuf_;
extern unsigned char  *__lpeufi_;
extern unsigned char  *__lpflg_;
extern unsigned char  *__lpdii_;
extern unsigned char  *__lpdoo_;
extern unsigned int   *__lpscan_;
extern unsigned int   *__lpout_;
extern CHAR8          *__lpstr_;
extern unsigned char  *__lpstf_;


#define WRLDERR_OK                     0
#define WRLDERR_ONSPEC_NOT_RUNNING   100
#define WRLDERR_QUEUE_ERROR          101
#define WRLDERR_BAD_PREFIX           102
#define WRLDERR_INDEX_OUT_OF_RANGE   103
#define WRLDERR_TAG_NOT_FOUND        104

#define EUR  1
#define EUI  2
#define FLG  3
#define EUF  4
#define EUFI 5
#define DII  6
#define DOO  7
#define SCAN 8
#define OUTT 9
#define STR  18
#define STF  19

double         *eur;
int            *eui;
unsigned char  *euf;
unsigned char  *eufi;
unsigned char  *flg;
unsigned char  *dii;
unsigned char  *doo;
unsigned int   *scan;
unsigned int   *out;
CHAR8          *str;
unsigned char  *stf;



#ifdef __cplusplus
extern "C" {
#endif

void APIENTRY delay (int seconds);
int APIENTRY tableidfromstr(char *name, int *table, int *index);
int APIENTRY readmem(int tbl,int index,double *val,unsigned char *status);
int APIENTRY writemem(int tbl,int index,double val,unsigned char status);
int APIENTRY writestat(int tbl,int index,unsigned char status);
int APIENTRY qwrite(int location, int index, double valu, unsigned char status);
int APIENTRY look_qwrite(int location, int index, double valu, unsigned char status);
int APIENTRY driver_qwrite(int location, int index, double valu, unsigned char status);
int APIENTRY readtag(char *tag1,double *tagvalue, unsigned char *stat);
int APIENTRY writetag(char *tag1,double tagvalue, unsigned char stat);
int APIENTRY readstring(int slen, char *s, int index);
int APIENTRY writestring(int slen, char *s, int index);
int APIENTRY initialize(void *reur, void *reui, void *rflg, void *reuf,
                void *reufi, void *rdii, void *rdoo, void *rscan, void *rout,
                void *rstr, void *rstf);
void APIENTRY uninitialize(void);

#ifdef __cplusplus
}
#endif
