/****************************************************************************/
/*																			*/
/*		STK-7125 SCI サンプルプログラム ヘッダファイル			 			*/
/*																			*/
/*			Copyright   :: ㈱アルファプロジェクト							*/
/*			Language    :: SH-C												*/
/*			File Name   :: sci.h											*/
/*																			*/
/****************************************************************************/
#ifndef __SCI_H__
#define __SCI_H__

// 使用するSCIのチャネル指定
#define	SCICH		1		// SCIチャネル番号 0 or 1

#if SCICH==0
#define	SCI			SCI0	// SCIチャネル0
#define	SCI_IPRMSK	0xf000	// SCIチャネル0 IPRL MASK
#define	SCI_IPRSFT	12		// SCIチャネル0 IPRL SHIFT
#else
#define	SCI			SCI1	// SCIチャネル1
#define	SCI_IPRMSK	0x0f00	// SCIチャネル1 IPRL MASK
#define	SCI_IPRSFT	8		// SCIチャネル1 IPRL SHIFT
#endif

#define	sciEmpty	-1		// 受信バッファ空

// SCIパリティ定義
typedef enum _sciPr {
	sciPrNone = 0,
	sciPrEven = 2,
	sciPrOdd  = 3,
} sciPr;

// SCIバッファ構造体
typedef struct _sciBuf {
	char * buf;				// バッファ本体へのポインタ
	unsigned long size;		// バッファサイズ
	unsigned long wp;		// 書込みポインタ
	unsigned long rp;		// 読出しポインタ
} sciBuf;

// SCI初期化パラメータ構造体
typedef struct _sciPara {
	sciBuf * rec;			// 受信SCIバッファ構造体へのポインタ
	sciBuf * send;			// 送信SCIバッファ構造体へのポインタ
	long bps;				// レート   : 2400,4800,9600,19200,38400
	short bit_len;			// ビット長	: 8 or 7
	short stop_bit;			// ストップ	: 1 or 2
	sciPr parity;			// パリティ	: sciPrNone,sciPrEven,sciPrOdd
	short int_lv;			// 割込順位 : 0-15
} sciPara;

// プロトタイプ宣言
void sci_init(void);
void open_sci(sciPara *);
void close_sci(void);
short sci_putc(char);
short sci_getc(void);
void printDecimal(int d);
void sputs(char *p);
double adhen(void);
double get_keisan(void);
double feed_back(void);

void eri_int(void);
void rxi_int(void);
void txi_int(void);
void tei_int(void);

#endif		// END _SCI_H_
