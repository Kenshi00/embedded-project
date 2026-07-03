
#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define DO_01   1911
#define DO_01_H 1817
#define RE_01   1703
#define RE_01_H 1607
#define MI_01   1517
#define FA_01   1432
#define FA_01_H 1352
#define SO_01   1276
#define SO_01_H 1199
#define LA_01   1136
#define LA_01_H 1073
#define TI_01   1012
#define DO_02   956
#define DO_02_H 909
#define RE_02   851
#define RE_02_H 804
#define MI_02   758
#define FA_02   716
#define FA_02_H 676
#define SO_02   638
#define SO_02_H 602
#define LA_02   568
#define LA_02_H 536
#define TI_02   506
#define DO_03   478
#define DO_03_H 450
#define RE_03	425
#define RE_03_H 401
#define MI_03	378

#define F_CLK       16000000L //클럭
#define F_SCALER	8 //프리스케일러
#define BEAT_1_32	42
#define BEAT_1_16	86
#define BEAT_1_8	170
#define BEAT_1_4	341
#define BEAT_1_2	682
#define BEAT_1		1364

/*#define DO_01   (F_CLK/(2*16.3516*F_SCALER)-1)
#define DO_01s  (F_CLK/(2*17.3239*F_SCALER)-1)
#define RE_01   (F_CLK/(2*18.3541*F_SCALER)-1)
#define RE_01s  (F_CLK/(2*19.4454*F_SCALER)-1)
#define MI_01   (F_CLK/(2*20.6017*F_SCALER)-1)
#define FA_01   (F_CLK/(2*21.8268*F_SCALER)-1)
#define FA_01s  (F_CLK/(2*23.1247*F_SCALER)-1)
#define SO_01   (F_CLK/(2*24.4997*F_SCALER)-1)
#define SO_01s  (F_CLK/(2*25.9565*F_SCALER)-1)
#define LA_01   (F_CLK/(2*27.5*F_SCALER)-1)
#define LA_01s  (F_CLK/(2*29.1352*F_SCALER)-1)
#define SI_01   (F_CLK/(2*30.8677*F_SCALER)-1)
#define DO_02   (F_CLK/(2*32.7032*F_SCALER)-1)
#define DO_02s  (F_CLK/(2*34.6478*F_SCALER)-1)
#define RE_02   (F_CLK/(2*36.7081*F_SCALER)-1)
#define RE_02s  (F_CLK/(2*38.8909*F_SCALER)-1)
#define MI_02   (F_CLK/(2*41.2034*F_SCALER)-1)
#define FA_02   (F_CLK/(2*43.6535*F_SCALER)-1)
#define FA_02s  (F_CLK/(2*46.2493*F_SCALER)-1)
#define SO_02   (F_CLK/(2*48.9994*F_SCALER)-1)
#define SO_02s  (F_CLK/(2*51.913*F_SCALER)-1)
#define LA_02   (F_CLK/(2*55*F_SCALER)-1)
#define LA_02s  (F_CLK/(2*58.2705*F_SCALER)-1)
#define SI_02   (F_CLK/(2*61.7354*F_SCALER)-1)
#define DO_03   (F_CLK/(2*65.4064*F_SCALER)-1)
#define DO_03s  (F_CLK/(2*69.2957*F_SCALER)-1)
#define RE_03   (F_CLK/(2*73.4162*F_SCALER)-1)
#define RE_03s  (F_CLK/(2*77.7817*F_SCALER)-1)
#define MI_03   (F_CLK/(2*82.4069*F_SCALER)-1)
#define FA_03   (F_CLK/(2*87.3071*F_SCALER)-1)
#define FA_03s  (F_CLK/(2*92.4986*F_SCALER)-1)
#define SO_03   (F_CLK/(2*97.9989*F_SCALER)-1)
#define SO_03s  (F_CLK/(2*103.8262*F_SCALER)-1)
#define LA_03   (F_CLK/(2*110*F_SCALER)-1)
#define LA_03s  (F_CLK/(2*116.5409*F_SCALER)-1)
#define SI_03   (F_CLK/(2*123.4708*F_SCALER)-1)
#define DO_04   (F_CLK/(2*130.8128*F_SCALER)-1)
#define DO_04s  (F_CLK/(2*138.5913*F_SCALER)-1)
#define RE_04   (F_CLK/(2*146.8324*F_SCALER)-1)
#define RE_04s  (F_CLK/(2*155.5635*F_SCALER)-1)
#define MI_04   (F_CLK/(2*164.8138*F_SCALER)-1)
#define FA_04   (F_CLK/(2*174.6141*F_SCALER)-1)
#define FA_04s  (F_CLK/(2*184.9972*F_SCALER)-1)
#define SO_04   (F_CLK/(2*195.9977*F_SCALER)-1)
#define SO_04s  (F_CLK/(2*207.6523*F_SCALER)-1)
#define LA_04   (F_CLK/(2*220*F_SCALER)-1)
#define LA_04s  (F_CLK/(2*233.0819*F_SCALER)-1)
#define SI_04   (F_CLK/(2*246.9417*F_SCALER)-1)
#define DO_05   (F_CLK/(2*261.6256*F_SCALER)-1)
#define DO_05s  (F_CLK/(2*277.1826*F_SCALER)-1)
#define RE_05   (F_CLK/(2*293.6648*F_SCALER)-1)
#define RE_05s  (F_CLK/(2*311.127*F_SCALER)-1)
#define MI_05   (F_CLK/(2*329.6276*F_SCALER)-1)
#define FA_05   (F_CLK/(2*349.2282*F_SCALER)-1)
#define FA_05s  (F_CLK/(2*369.9944*F_SCALER)-1)
#define SO_05   (F_CLK/(2*391.9954*F_SCALER)-1)
#define SO_05s  (F_CLK/(2*415.3047*F_SCALER)-1)
#define LA_05   (F_CLK/(2*440*F_SCALER)-1)
#define LA_05s  (F_CLK/(2*466.1638*F_SCALER)-1)
#define SI_05  (F_CLK/(2*493.8833*F_SCALER)-1)
#define DO_06   (F_CLK/(2*523.2511*F_SCALER)-1)
#define DO_06s  (F_CLK/(2*554.3653*F_SCALER)-1)
#define RE_06   (F_CLK/(2*587.3295*F_SCALER)-1)
#define RE_06s  (F_CLK/(2*622.254*F_SCALER)-1)
#define MI_06   (F_CLK/(2*659.2551*F_SCALER)-1)
#define FA_06   (F_CLK/(2*698.4565*F_SCALER)-1)
#define FA_06s  (F_CLK/(2*739.9888*F_SCALER)-1)
#define SO_06   (F_CLK/(2*783.9909*F_SCALER)-1)
#define SO_06s  (F_CLK/(2*830.6094*F_SCALER)-1)
#define LA_06   (F_CLK/(2*880*F_SCALER)-1)
#define LA_06s  (F_CLK/(2*932.3275*F_SCALER)-1)
#define SI_06   (F_CLK/(2*987.7666*F_SCALER)-1)
#define DO_07   (F_CLK/(2*1046.502*F_SCALER)-1)
#define DO_07s  (F_CLK/(2*1108.731*F_SCALER)-1)
#define RE_07   (F_CLK/(2*1174.659*F_SCALER)-1)
#define RE_07s  (F_CLK/(2*1244.508*F_SCALER)-1)
#define MI_07   (F_CLK/(2*1318.51*F_SCALER)-1)
#define FA_07   (F_CLK/(2*1396.913*F_SCALER)-1)
#define FA_07s  (F_CLK/(2*1479.978*F_SCALER)-1)
#define SO_07   (F_CLK/(2*1567.982*F_SCALER)-1)
#define SO_07s  (F_CLK/(2*1661.219*F_SCALER)-1)
#define LA_07   (F_CLK/(2*1760*F_SCALER)-1)
#define LA_07s  (F_CLK/(2*1864.655*F_SCALER)-1)
#define SI_07   (F_CLK/(2*1975.533*F_SCALER)-1)*/
int School_Bell_Tune[] = {SO_01, SO_01, LA_01, LA_01, SO_01, SO_01, MI_01, 
					 	  SO_01, SO_01, MI_01, MI_01, RE_01, 
					 	  SO_01, SO_01, LA_01, LA_01, SO_01, SO_01, MI_01, 
					 	  SO_01, MI_01, RE_01, MI_01, DO_01,'/0'};
const int School_Bell_Beats[] = {BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_2,
						   BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1,
						   BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_2, 
						   BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1};
						   
// const int i = 0;
// i =20; 안되 ..... 읽는 작업만 가능하고 write는 안되  
// int j;
// j = i;


/*int He_Pirate[] = {RE_04, RE_04, RE_04, RE_04,
	RE_04, RE_04, RE_04, LA_03, DO_04,
	RE_04, RE_04, RE_04, MI_04,
	FA_04, FA_04, FA_04, SO_04,
	MI_04, MI_04, RE_04, DO_04,
	DO_04, RE_04, 0, LA_03, DO_04,
	RE_04, RE_04, RE_04, MI_04,
	FA_04, FA_04, FA_04, SO_04,
	MI_04, MI_04, RE_04, DO_04,
	RE_04, 0, 0, LA_03, DO_04,
	RE_04, RE_04, RE_04, FA_04,
	SO_04, SO_04, SO_04, LA_04,
	LA_04s, LA_04s, LA_04, SO_04,
	
	LA_04, RE_04, 0, RE_04, MI_04,
	FA_04, FA_04, SO_04,
	LA_04, RE_04, 0, RE_04, FA_04,
	MI_04, MI_04, FA_04, RE_04,
	MI_04, 0, 0, LA_04, DO_05,
	RE_05, RE_05, RE_05, MI_05,
	FA_05, FA_05, FA_05, SO_05,
	MI_05, MI_05, RE_05, DO_05,
	DO_05, RE_05, 0, LA_04, DO_05,
	RE_05, RE_05, RE_05, MI_05,
	FA_05, FA_05, FA_05, SO_05,
	MI_05, MI_05, RE_05, DO_05,
	RE_05, 0, 0, LA_04, DO_05,
	RE_05, RE_05, RE_05, FA_05,
	SO_05, SO_05, SO_05, LA_05,
	LA_05s, LA_05s, LA_05, SO_05,
	LA_05, RE_05, 0, RE_05, MI_05,
	FA_05, FA_05, SO_05,
	LA_05, RE_05, 0, RE_05, FA_05,
	MI_05, MI_05, RE_05, DO_05,
	RE_05, RE_05, MI_05,
	
	FA_05, FA_05, FA_05, SO_05,
	LA_05, FA_05, 0, 0, FA_05, RE_05,
	LA_04, 0, 0, 0,
	LA_04s, 0, 0, SO_05, RE_05,
	LA_04s, 0, 0, 0,
	MI_05, MI_05, RE_05,
	FA_04, 0, FA_04, SO_04,
	LA_04, LA_04, LA_04,
	LA_04s, LA_04,0 , 0,
	SO_04, SO_04, SO_04,
	SO_04, LA_04, 0, 0,
	LA_04, LA_04, LA_04,
	LA_04s, LA_04, 0, 0,
	SO_04, FA_04, MI_04,
	RE_04, 0, 0, RE_04, MI_04,
	FA_04, SO_04, LA_04,
	SO_04, FA_04, MI_04,
	FA_04, SO_04, LA_04,
	SO_04, 0, 0, FA_04, SO_04,
	LA_04, 0, 0, SO_04, FA_04,
	MI_04, FA_04, MI_04,
	RE_04, 0, 0, MI_04, DO_04,
	RE_04, 0, 0, RE_05, MI_05,
	
	FA_05, 0, 0, MI_05, FA_05,
	SO_05, FA_05, SO_05,
	LA_05, SO_05, FA_05,
	RE_05, 0, 0, RE_05, MI_05,
	FA_05, SO_05, LA_05,
	LA_05s, RE_05, SO_05,
	FA_05, 0, 0, SO_05, MI_05,
	RE_05, 0, 0, MI_05, DO_05s,
	LA_05, 0, 0, LA_05s, 0, 0,
	LA_05, LA_05, LA_05,
	LA_05, SO_05, 0, 0,
	SO_05, 0, 0,
	FA_05, 0, 0,
	FA_05, SO_05, MI_05,
	RE_05, RE_05, MI_05, FA_05,
	LA_05, RE_05, MI_05, FA_05,
	LA_05s, RE_05, MI_05, FA_05,
	LA_05, LA_05, DO_06,
	LA_05, SO_05, 0, 0,
	SO_05, 0, 0,
	FA_05, 0, 0,
	FA_05, SO_05, MI_05,
	RE_05, 0, 0,
RE_04, '/0'};
int He_Pirate1_Beat[] = {BEAT_1_4, BEAT_1_8, BEAT_1_4, BEAT_1_8,
	BEAT_1_4, BEAT_1_8, BEAT_1_16, BEAT_1_16, BEAT_1_16,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	
	BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	
	BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_4, BEAT_1_2,
	BEAT_1_2, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	
	BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_2, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_2, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_2, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_2, BEAT_1_8, BEAT_1_4,
BEAT_1};*/

const int Elise_Tune[] = {MI_02, RE_02_H, MI_02, RE_02_H, MI_02, TI_01, RE_02, DO_02, LA_01, 0,
	DO_01, MI_01, LA_01, TI_01, 0, MI_01, SO_01_H, TI_01, DO_02, 0,
	MI_01, MI_02, RE_02_H, MI_02, RE_02_H, MI_02, TI_01, RE_02, DO_02, LA_01, 0,
	DO_01, MI_01, LA_01, TI_01, 0, MI_01, DO_02, TI_01, LA_01,
	MI_02, RE_02_H, MI_02, RE_02_H, MI_02, TI_01, RE_02, DO_02, LA_01, 0,
	DO_01, MI_01, LA_01, TI_01, 0, MI_01, SO_01_H, TI_01, DO_02, 0,
	MI_01, MI_02, RE_02_H, MI_02, RE_02_H, MI_02, TI_01, RE_02, DO_02, LA_01, 0,
	DO_01, MI_01, LA_01, TI_01, 0, MI_01, DO_02, TI_01, LA_01, 0,
	TI_01, DO_02, RE_02, MI_02, 0, SO_01, FA_02, MI_02, RE_02, 0, FA_01, MI_02, RE_02, DO_02, 0,
	MI_01, RE_02, DO_02, TI_01, 0, MI_01, MI_02, MI_03,
	RE_02_H, MI_02, RE_02_H, MI_02, RE_02, MI_02, RE_01_H, MI_02, TI_01, RE_01, DO_02, LA_01, 0,
	DO_01, MI_01, LA_01, TI_01, 0, MI_01, SO_01_H, TI_01, DO_02, 0,
	MI_01, MI_02, RE_02_H, MI_02, RE_02_H, MI_02, TI_01, RE_02, DO_02, LA_01, 0, DO_01, MI_01, LA_01, TI_01, 0,
RE_01, DO_02, TI_01, LA_01,'/0'};

const int Elise_Beats[] = 
   { BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_2,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_2, BEAT_1_8,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8};

int GrandFa_Tune[] = {SO_01, DO_02, TI_01, DO_02, RE_02, DO_02, RE_02, MI_02, FA_02, MI_02, LA_01, RE_02, RE_02, DO_02, DO_02, DO_02, TI_01, LA_01, TI_01, DO_02, 0,
	SO_01, DO_02, TI_01, DO_02, RE_02, DO_02, RE_02, MI_02, FA_02, MI_02, LA_01, RE_02, RE_02, DO_02, DO_02, DO_02, TI_01, LA_01, TI_01, DO_02, 0,
	DO_02, MI_02, SO_02, MI_02, RE_02, DO_02, TI_01, DO_02, RE_02, DO_02, TI_01, LA_01, SO_01, DO_02, MI_02, SO_02, MI_02, RE_02, DO_02, TI_01, DO_02, RE_02,
SO_01, DO_02, RE_02, MI_02, FA_02, MI_02, LA_01, RE_02, RE_02, DO_02, DO_02, DO_02, TI_01, LA_01, TI_01, DO_02, '/0'};

const int GrandFa_Beats[] = {BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_2, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_2, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4,
BEAT_1_4, BEAT_1_2, BEAT_1_2, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_2};

void Music_Player(int *tone, int *Beats)
{
	while(*tone != '/0')
	{
		OCR3A = *tone;
		delay_ms(*Beats);
		tone++;
		Beats++;
		OCR3A = 0;
		_delay_ms(10);		
	}
	return;
}  
// Timer3 위상교정 PWM
void init_speaker(void)
{
	DDRE |= 0x08;   // PWM CHANNEL  OC3A(PE3) 출력 모드로 설정 한다. 
	TCCR3A = (1<<COM3A0); // COM3A0 : 비교일치시 PE3 출력 반전 (P328 표14-6 참고)
	TCCR3B = (1<<WGM32) | (1<<CS31);   // WGM32 : CTC 4(P327 표14-5) CS31: 8분주(P318)
	// CTC mode : 비교일치가 되면 카운터는 reset되면서 PWM 파형 출력 핀의 출력이 반전 됨. 
	// 정상모드와 CTC모드의 차이점은 비교일치 발생시 TCNT1의 레지스터값을 0으로 설정 하는지 여부 이다. 
	// 정상모드를 사용시 TCNT1이 자동으로 0으로 설정 되지 않아 인터럽트 루틴에서 TCNT1을 0으로 설정 해 주었다. 
	// 위상교정 PWM mode4  CTC 분주비 8  16000000/8 ==> 2,000,000HZ(2000KHZ) : 
	// up-dounting: 비교일치시 LOW, down-counting시 HIGH출력
	// 1/2000000 ==> 0.0000005sec (0.5us)
	// P599 TOP 값 계산 참고
	// PWM주파수 = OSC(16M) / ( 2(up.down)x N(분주율)x(1+TOP) ) 
	// TOP =  (fOSC(16M) /  2(up.down)x N(분주율)x 원하는주파수 )) -1 
	//-----------------------------------------------------------
	// - BOTTOM :  카운터가 0x00/0x0000 일때를 가리킨다.
    // - MAX : 카운터가 0xFF/0xFFFF 일 때를 가리킨다.
    // - TOP?:  카운터가 가질 수 있는 최대값을 가리킨다. 오버플로우 인터럽트의 경우 TOP은 0xFF/0xFFFF
    //          이지만 비교일치 인터럽트의 경우 사용자가 설정한 값이 된다. 
    
	TCCR3C = 0;  // P328 그림 14-11 참고 
	OCR3A = 0;   // 비교 일치값을 OCR3A에 넣는다. 
	
	return;
}

void Beep(int repeat)
{
	int  i;
	
	for(i=0; i < repeat; i++)
	{
		OCR3A = 500;  // 0.00025sec (250us) : 0.0000005 * 500
		_delay_ms(200);
		OCR3A = 0;
		_delay_ms(200);
	}
}    

void Siren(int repeat)
{
	int i, j;
	
	OCR3A = 900;
	
	for(i=0; i < repeat; i++)
	{
		for(j=0; j < 100; j++)
		{
			OCR3A += 10;
			_delay_ms(20);
		}
		for(j=0; j < 100; j++)
		{
			OCR3A -= 10;
			_delay_ms(20);
		}
	}
}

void RRR(void)
{
	int i;
	
	for(i=0; i<20; i++)
	{
		OCR3A = 1136;
		_delay_ms(100);
		OCR3A = 0;
		_delay_ms(20);
	}
}

void delay_ms(int ms)
{
	while(ms-- != 0)_delay_ms(1);
}