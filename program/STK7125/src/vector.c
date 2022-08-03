/****************************************************************************/
/*																			*/
/*		STK-7125サンプルプログラム ベクタテーブル				 			*/
/*																			*/
/*			Copyright   :: ㈱アルファプロジェクト							*/
/*			Language    :: SH-C												*/
/*			File Name   :: vector.c											*/
/*																			*/
/****************************************************************************/
#include "global.h"
#include "timer.h"
#include "sci.h"

/* プロトタイプ宣言 */
void dummy_int(void);
extern void boot(void);

#pragma	section STACK		/*スタックポインタ：Linker Option設定*/
long stack;

/************************************/
/*	ベクターテーブル				*/
/************************************/
// VBR=0が前提条件。RAM上でのデバッグの場合はデバッガの機能にてVBRを適切な値に設定の事

#pragma	section	VECTTBL
void (* const vectors[])(void) = {
	boot,					/*	パワーＯＮリセット			*/
	(void(*)(void))&stack,	/*	スタックポインタ			*/
	boot,					/*	マニュアルリセット			*/
	(void(*)(void))&stack,	/*	スタックポインタ			*/

							/*  [ベクタ番号]				*/
	dummy_int,				/*	[  4]一般不当命令			*/
	dummy_int,				/*	[  5] Reserved				*/
	dummy_int,				/*	[  6]スロット不当命令		*/
	dummy_int,				/*	[  7] Reserved				*/
	dummy_int,				/*	[  8] Reserved				*/
	dummy_int,				/*	[  9]CPUアドレスエラー		*/
	dummy_int,				/*	[ 10]DMAC/DTCアドレスエラー	*/
	dummy_int,				/*	[ 11]ＮＭＩ					*/
	dummy_int,				/*	[ 12]ユーザーブレーク		*/
	dummy_int,				/*	[ 13] Reserved				*/
	dummy_int,				/*	[ 14]ＨＵＤＩ				*/

	dummy_int,				/*	[ 15] Reserved				*/
	(void(*)(void))0,		/*	[ 16] Reserved				*/
	dummy_int,				/*	[ 17] Reserved				*/
	dummy_int,				/*	[ 18] Reserved				*/
	dummy_int,				/*	[ 19] Reserved				*/
	dummy_int,				/*	[ 20] Reserved				*/
	dummy_int,				/*	[ 21] Reserved				*/
	dummy_int,				/*	[ 22] Reserved				*/
	dummy_int,				/*	[ 23] Reserved				*/
	dummy_int,				/*	[ 24] Reserved				*/
	dummy_int,				/*	[ 25] Reserved				*/
	dummy_int,				/*	[ 26] Reserved				*/
	dummy_int,				/*	[ 27] Reserved				*/
	dummy_int,				/*	[ 28] Reserved				*/
	dummy_int,				/*	[ 29] Reserved				*/
	dummy_int,				/*	[ 30] Reserved				*/
	dummy_int,				/*	[ 31] Reserved				*/

	dummy_int,				/*	[ 32] TRAPA (User Vector)	*/
	dummy_int,				/*	[ 33] TRAPA (User Vector)	*/
	dummy_int,				/*	[ 34] TRAPA (User Vector)	*/
	dummy_int,				/*	[ 35] TRAPA (User Vector)	*/
	dummy_int,				/*	[ 36] TRAPA (User Vector)	*/
	dummy_int,				/*	[ 37] TRAPA (User Vector)	*/
	dummy_int,				/*	[ 38] TRAPA (User Vector)	*/
	dummy_int,				/*	[ 39] TRAPA (User Vector)	*/
	dummy_int,				/*	[ 40] TRAPA (User Vector)	*/
	dummy_int,				/*	[ 41] TRAPA (User Vector)	*/
	dummy_int,				/*	[ 42] TRAPA (User Vector)	*/
	dummy_int,				/*	[ 43] TRAPA (User Vector)	*/
	dummy_int,				/*	[ 44] TRAPA (User Vector)	*/
	dummy_int,				/*	[ 45] TRAPA (User Vector)	*/
	dummy_int,				/*	[ 46] TRAPA (User Vector)	*/
	dummy_int,				/*	[ 47] TRAPA (User Vector)	*/
	dummy_int,				/*	[ 48] TRAPA (User Vector)	*/
	dummy_int,				/*	[ 49] TRAPA (User Vector)	*/
	dummy_int,				/*	[ 50] TRAPA (User Vector)	*/
	dummy_int,				/*	[ 51] TRAPA (User Vector)	*/
	dummy_int,				/*	[ 52] TRAPA (User Vector)	*/
	dummy_int,				/*	[ 53] TRAPA (User Vector)	*/
	dummy_int,				/*	[ 54] TRAPA (User Vector)	*/
	dummy_int,				/*	[ 55] TRAPA (User Vector)	*/
	dummy_int,				/*	[ 56] TRAPA (User Vector)	*/
	dummy_int,				/*	[ 57] TRAPA (User Vector)	*/
	dummy_int,				/*	[ 58] TRAPA (User Vector)	*/
	dummy_int,				/*	[ 59] TRAPA (User Vector)	*/
	dummy_int,				/*	[ 60] TRAPA (User Vector)	*/
	dummy_int,				/*	[ 61] TRAPA (User Vector)	*/
	dummy_int,				/*	[ 62] TRAPA (User Vector)	*/
	dummy_int,				/*	[ 63] TRAPA (User Vector)	*/

	dummy_int,				/*	[ 64]				*/
	dummy_int,				/*	[ 65]				*/
	dummy_int,				/*	[ 66]				*/
	dummy_int,				/*	[ 67]				*/
	dummy_int,				/*	[ 68]				*/
	dummy_int,				/*	[ 69]				*/
	dummy_int,				/*	[ 70]				*/
	dummy_int,				/*	[ 71]				*/
	dummy_int,				/*	[ 72]				*/
	dummy_int,				/*	[ 73]				*/
	dummy_int,				/*	[ 74]				*/
	dummy_int,				/*	[ 75]				*/
	dummy_int,				/*	[ 76]				*/
	dummy_int,				/*	[ 77]				*/
	dummy_int,				/*	[ 78]				*/
	dummy_int,				/*	[ 79]				*/
	dummy_int,				/*	[ 80]				*/
	dummy_int,				/*	[ 81]				*/
	dummy_int,				/*	[ 82]				*/
	dummy_int,				/*	[ 83]				*/
	dummy_int,				/*	[ 84]				*/
	dummy_int,				/*	[ 85]				*/
	dummy_int,				/*	[ 86]				*/
	dummy_int,				/*	[ 87]				*/
	dummy_int,				/*	[ 88]				*/
	dummy_int,				/*	[ 89]				*/
	dummy_int,				/*	[ 90]				*/
	dummy_int,				/*	[ 91]				*/
	dummy_int,				/*	[ 92]				*/
	dummy_int,				/*	[ 93]				*/
	dummy_int,				/*	[ 94]				*/
	dummy_int,				/*	[ 95]				*/
	dummy_int,				/*	[ 96]				*/
	dummy_int,				/*	[ 97]				*/
	dummy_int,				/*	[ 98]				*/
	dummy_int,				/*	[ 99]				*/
	dummy_int,				/*	[100]				*/
	dummy_int,				/*	[101]				*/
	dummy_int,				/*	[102]				*/
	dummy_int,				/*	[103]				*/
	dummy_int,				/*	[104]				*/
	dummy_int,				/*	[105]				*/
	dummy_int,				/*	[106]				*/
	dummy_int,				/*	[107]				*/
	dummy_int,				/*	[108]				*/
	dummy_int,				/*	[109]				*/
	dummy_int,				/*	[110]				*/
	dummy_int,				/*	[111]				*/
	dummy_int,				/*	[112]				*/
	dummy_int,				/*	[113]				*/
	dummy_int,				/*	[114]				*/
	dummy_int,				/*	[115]				*/
	dummy_int,				/*	[116]				*/
	dummy_int,				/*	[117]				*/
	dummy_int,				/*	[118]				*/
	dummy_int,				/*	[119]				*/
	dummy_int,				/*	[120]				*/
	dummy_int,				/*	[121]				*/
	dummy_int,				/*	[122]				*/
	dummy_int,				/*	[123]				*/
	dummy_int,				/*	[124]				*/
	dummy_int,				/*	[125]				*/
	dummy_int,				/*	[126]				*/
	dummy_int,				/*	[127]				*/
	dummy_int,				/*	[128]				*/
	dummy_int,				/*	[129]				*/
	dummy_int,				/*	[130]				*/
	dummy_int,				/*	[131]				*/
	dummy_int,				/*	[132]				*/
	dummy_int,				/*	[133]				*/
	dummy_int,				/*	[134]				*/
	dummy_int,				/*	[135]				*/
	dummy_int,				/*	[136]				*/
	dummy_int,				/*	[137]				*/
	dummy_int,				/*	[138]				*/
	dummy_int,				/*	[139]				*/
	dummy_int,				/*	[140]				*/
	dummy_int,				/*	[141]				*/
	dummy_int,				/*	[142]				*/
	dummy_int,				/*	[143]				*/
	dummy_int,				/*	[144]				*/
	dummy_int,				/*	[145]				*/
	dummy_int,				/*	[146]				*/
	dummy_int,				/*	[147]				*/
	dummy_int,				/*	[148]				*/
	dummy_int,				/*	[149]				*/
	dummy_int,				/*	[150]				*/
	dummy_int,				/*	[151]				*/
	dummy_int,				/*	[152]				*/
	dummy_int,				/*	[153]				*/
	dummy_int,				/*	[154]				*/
	dummy_int,				/*	[155]				*/
	dummy_int,				/*	[156]				*/
	dummy_int,				/*	[157]				*/
	dummy_int,				/*	[158]				*/
	dummy_int,				/*	[159]				*/
	dummy_int,				/*	[160]				*/
	dummy_int,				/*	[161]				*/
	dummy_int,				/*	[162]				*/
	dummy_int,				/*	[163]				*/
	dummy_int,				/*	[164]				*/
	dummy_int,				/*	[165]				*/
	dummy_int,				/*	[166]				*/
	dummy_int,				/*	[167]				*/
	dummy_int,				/*	[168]				*/
	dummy_int,				/*	[169]				*/
	dummy_int,				/*	[170]				*/
	dummy_int,				/*	[171]				*/
	dummy_int,				/*	[172]				*/
	dummy_int,				/*	[173]				*/
	dummy_int,				/*	[174]				*/
	dummy_int,				/*	[175]				*/
	dummy_int,				/*	[176]				*/
	dummy_int,				/*	[177]				*/
	dummy_int,				/*	[178]				*/
	dummy_int,				/*	[179]				*/
	dummy_int,				/*	[180]				*/
	dummy_int,				/*	[181]				*/
	dummy_int,				/*	[182]				*/
	dummy_int,				/*	[183]				*/
	timer_int,				/*	[184] CMT0			*/
	dummy_int,				/*	[185]				*/
	dummy_int,				/*	[186]				*/
	dummy_int,				/*	[187]				*/
	dummy_int,				/*	[188]				*/
	dummy_int,				/*	[189]				*/
	dummy_int,				/*	[190]				*/
	dummy_int,				/*	[191]				*/
	dummy_int,				/*	[192]				*/
	dummy_int,				/*	[193]				*/
	dummy_int,				/*	[194]				*/
	dummy_int,				/*	[195]				*/
	dummy_int,				/*	[196]				*/
	dummy_int,				/*	[197]				*/
	dummy_int,				/*	[198]				*/
	dummy_int,				/*	[199]				*/
	dummy_int,				/*	[200]				*/
	dummy_int,				/*	[201]				*/
	dummy_int,				/*	[202]				*/
	dummy_int,				/*	[203]				*/
	dummy_int,				/*	[204]				*/
	dummy_int,				/*	[205]				*/
	dummy_int,				/*	[206]				*/
	dummy_int,				/*	[207]				*/
	dummy_int,				/*	[208]				*/
	dummy_int,				/*	[209]				*/
	dummy_int,				/*	[210]				*/
	dummy_int,				/*	[211]				*/
	dummy_int,				/*	[212]				*/
	dummy_int,				/*	[213]				*/
	dummy_int,				/*	[214]				*/
	dummy_int,				/*	[215]				*/
	eri0_int,				/*	[216] SCI0	eri0_int*/
	rxi0_int,				/*	[217] SCI0	rxi0_int*/
	dummy_int,				/*	[218] SCI0			*/
	dummy_int,				/*	[219] SCI0			*/
	eri_int,				/*	[220] SCI1			*/
	rxi_int,				/*	[221] SCI1			*/
	txi_int,				/*	[222] SCI1			*/
	tei_int,				/*	[223] SCI1			*/
	dummy_int,				/*	[224]				*/
	dummy_int,				/*	[225]				*/
	dummy_int,				/*	[226]				*/
	dummy_int,				/*	[227]				*/
	dummy_int,				/*	[228]				*/
	dummy_int,				/*	[229]				*/
	dummy_int,				/*	[230]				*/
	dummy_int,				/*	[231]				*/
	dummy_int,				/*	[232]				*/
	dummy_int,				/*	[233]				*/
	dummy_int,				/*	[234]				*/
	dummy_int,				/*	[235]				*/
	dummy_int,				/*	[236]				*/
	dummy_int,				/*	[237]				*/
	dummy_int,				/*	[238]				*/
	dummy_int,				/*	[239]				*/
	dummy_int,				/*	[240]				*/
	dummy_int,				/*	[241]				*/
	dummy_int,				/*	[242]				*/
	dummy_int,				/*	[243]				*/
	dummy_int,				/*	[244]				*/
	dummy_int,				/*	[245]				*/
	dummy_int,				/*	[246]				*/
	dummy_int,				/*	[247]				*/
	dummy_int,				/*	[248]				*/
	dummy_int,				/*	[249]				*/
	dummy_int,				/*	[250]				*/
	dummy_int,				/*	[251]				*/
	dummy_int,				/*	[252]				*/
	dummy_int,				/*	[253]				*/
	dummy_int,				/*	[254]				*/
	dummy_int,				/*	[255]				*/
};


/************************************/
/*	想定外の例外発生処理			*/
/************************************/
#pragma	section
#pragma interrupt (dummy_int)
/*	ダミー関数					*/
void dummy_int(void)
{
	while(1) ;	
}
