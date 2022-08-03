/****************************************************************************/
/*																			*/
/*		STK-7125 サンプルプログラム											*/
/*			ブート処理														*/
/*																			*/
/*			Copyright   :: ㈱アルファプロジェクト							*/
/*			Cpu         :: SH7125											*/
/*			Language    :: SH-C												*/
/*			File Name   :: boot.c											*/
/*																			*/
/****************************************************************************/
#include <machine.h>
#include "7125s.h"
#include "global.h"
#include "timer.h"
#include "sci.h"

/* 外部参照 */
extern void	set_imask(int);
extern int	main(void);

/************************************/
/*	セクション初期化				*/
/************************************/
void sectionInit(void)
{
	extern int	*_D_ROM, *_B_BGN, *_B_END, *_D_BGN, *_D_END;
	register int	*p, *q;

	/* 未初期化データ領域をゼロで初期化 */
	for ( p = _B_BGN; p < _B_END; p++ ) {
		*p = 0;
	}
	/* 初期化データをROM上からRAM上へコピー */
	for ( p = _D_BGN, q = _D_ROM; p < _D_END; p++, q++ ) {
		*p = *q;
	}
}

/************************************/
/*	CPU初期化						*/
/************************************/
#pragma	section BOOT
void boot(void)
{
	
	/* クロック発振器(CPG) : PLL=96MHz */
	CPG.FRQCR.WORD = 0x16db;		// Iφ=PLL/2,Bφ=PLL/4,Pφ=PLL/4,MPφ=PLL/4
									// Iφ=48MHz,Bφ=24MHz,Pφ=24MHz,MPφ=24MHz
	/* ピンファンクション(PFC) */
	//シリアル通信に設定
	PFC.PACRL1.WORD |= 0x1011;		// PA1 :TXD0(14),PA0 :RXD0(13), PA3:RXD1(16)
	PFC.PACRL2.WORD |= 0x0001;		// PA4 :TXD1(17)
	
	
	// 位相係数モードに設定(エンコーダ)
	PFC.PACRL2.WORD |= 0x1100;      // PA6: TCLKA, PA7: TCLKB(motor1)
	PFC.PACRL3.WORD |= 0x0011;      // PA8: TCLKC, PA9: TCLKD(motor2)
	
		
	// PWM設定
	PFC.PEIORL.WORD |= 0xffff;		// PE0 :OUT(PWM0)	1111 1111 1111 0101
									// PE2 :OUT(PWM1)	格ピンを出力に設定
									// PE8 :OUT(PWM2)
									// PE10:OUT(PWM3)
									// PE4 :OUT(M0_dirA)
									// PE5 :OUT(M0_dirB)
									// PE6 :OUT(M1_dirA)
									// PE7 :OUT(M1_dirB)
									// PE12 :OUT(M2_dirA)
									// PE13:OUT(M2_dirB)
									// PE14:OUT(M3_dirA)
									// PE15:OUT(M3_dirB)
									
	PFC.PECRL1.WORD |= 0x0101;		// PE0,PE2: TIOC0A,TIOC0C(PWM) 0000 0001 0000 0001 
	PFC.PECRL2.WORD |= 0x0000;		// PE4 :OUT(M0_dirA),PE5 :OUT(M0_dirB),PE6 :OUT(M1_dirA),PE7 :OUT(M1_dirB)
	PFC.PECRL3.WORD |= 0x0101;		// PE8 :TIOC3A, PE10: TIOC3C(PWM)         
	PFC.PECRL4.WORD |= 0x0000;		// PE12:OUT(M3_dirA),PE13:OUT(M3_dirB),PE14:OUT(M4_dirA),PE15:OUT(M4_dirB)
	
	
	
	//AD変換
	STB.CR4.BIT._AD0 = 0;			//portのスタンバイを解除
	// AD変換を待機状態に設定   1になると変換開始
	AD0.ADCR.BIT.ADST = 0; 
	// ADエンドフラグ　リセット
	AD0.ADCSR.BIT.ADF = 0; 
	// ADI割り込み　無効
	AD0.ADCSR.BIT.ADIE = 0;
	// トリガイネーブル（外部トリガ）　無効
	AD0.ADCSR.BIT.TRGE = 0; 
	// ADFコントロール　無効
	AD0.ADCSR.BIT.CONADF = 0;
	// ステートコントロール　50ステート
	AD0.ADCSR.BIT.STC = 0;
	// A/D変換時間設定
	AD0.ADCSR.BIT.CKSL = 0; ;
	
	AD0.ADCSR.BIT.ADM = 1;	// シングルスキャンモード
	AD0.ADCSR.BIT.ADCS = 0;	// 1サイクルスキャン
	AD0.ADCSR.BIT.CH = 3;	// チャンネルセレクト?????
	
	
	/* スタンバイ制御(STB) */
	STB.CR3.BYTE &= ~0x18;			// SCI0,SCI1スタンバイ解除
	STB.CR4.BYTE &= ~0x20;			// CMTスタンバイ解除
	STB.CR4.BYTE &= ~0x40;			// MTU2のスタンバイモードを解除

		
	/* メモリ初期化 */
	sectionInit();

	/* 内蔵デバイス初期化 */
	timer_init(0);					// CMT0 : 1mS interval,callback無し,MTU20 : PWM モード1
	sci_init();						// SCI1 : 38400,8,1,N

	/* 割込み解除 */
	set_imask(0);
	
	/* メイン関数呼出し */
	main();
}






#pragma	section
