/****************************************************************************/
/*																			*/
/*		STK-7125 サンプルプログラム											*/
/*			タイマ処理														*/
/*																			*/
/*			Copyright   :: ㈱アルファプロジェクト							*/
/*			Cpu         :: SH7125											*/
/*			Language    :: SH-C												*/
/*			File Name   :: timer.c											*/
/*																			*/
/****************************************************************************/
#include "global.h"
#include "7125s.h"
#include "timer.h"

#include "global.h"

/* 1msダウンタイマ */
unsigned long TMR1m[TMR_NUM];

/* コールバック関数定義 */
static TIMER_CALLBACK fnc = 0;

/************************************/
/*	CMT0コールバック関数設定		*/
/************************************/
void timer_callback(TIMER_CALLBACK cback)
{
	fnc = cback;
}

/************************************/
/*	CMT0,MTU20初期化				*/
/************************************/
void timer_init(TIMER_CALLBACK cback)
{
	
	/* CMT0の設定 */
	timer_callback(cback);			// コールバック関数設定

	CMT.CMSTR.WORD &= ~0x0001;		// タイマ停止

	INTC.IPRJ.WORD = (INTC.IPRJ.WORD & ~0xf000) | 0x7000;
									// 割込みレベルの設定 : Lv7

	CMT0.CMCNT = 0;					// カウンタ値設定
	CMT0.CMCOR = (PCLK/8/CFREQ)-1;	// 1/CFREQ[s]インターバル
	CMT0.CMCSR.WORD = 0x0040;       // コンペアマッチ割り込みイネーブル(5: 1)
	                                // クロックセレクト φ/8 (0:0 1:0)   

	CMT.CMSTR.WORD |= 0x0001;		// タイマスタート


	/* MTU20の設定 */
	/* PWMの設定*/
	MTU2.TSTR.BYTE &= ~0x01;		// ch0のカウント停止
	
	
	// MTU2 CH0の設定(PWM0,1)
	MTU20.TCR.BYTE = 0x42;			// TGRBのコンペアマッチでカウンタクリア  01000010
									// 立ち上がりエッジでカウント,MPΦ/16でカウント
									// TCNTのカウントアップにかかる時間 TCNT(s) = 1/（MPΦ/16）
									// TCNT(S)は約0.67ns							
	MTU20.TIOR.BYTE.H = 0x12;		// TGR0B,TGR0Aの設定                              
    MTU20.TIOR.BYTE.L = 0x12;       // TGR0D,TGR0Cの設定
									// TGR0B,Dコンペアマッチで0, TGR0A,Cコンペアマッチで1
									// TGR0B,Dが周期、TGR0A,Cがduty									 
	MTU20.TGRA = PERIOD;            // 初期値設定
	MTU20.TGRB = PERIOD;
	MTU20.TGRC = PERIOD;
	MTU20.TGRD = PERIOD;
	
	MTU20.TCNT = 0x0000;			// カウンタクリア
	MTU20.TSR.BYTE = 0xC0;			// アップカウント

	MTU20.TMDR.BYTE = 0x02;			// PWMモード1
	
	// MTU2 CH3の設定(PWM2,3)
	MTU23.TCR.BYTE = 0x42;			// TGRBのコンペアマッチでカウンタクリア  01000010
									// 立ち上がりエッジでカウント,MPΦ/16でカウント
									// TCNTのカウントアップにかかる時間 TCNT(s) = 1/（MPΦ/16）
									// TCNT(S)は約0.67ns							
	MTU23.TIOR.BYTE.H = 0x12;		// TGR3B,TGR3Aの設定                              
    MTU23.TIOR.BYTE.L = 0x12;       // TGR3D,TGR3Cの設定
									// TGR3B,Dコンペアマッチで0, TGR3A,Cコンペアマッチで1
									// TGR3B,Dが周期、TGR3A,Cがduty									 
	MTU23.TGRA = PERIOD;            // 初期値設定
	MTU23.TGRB = PERIOD;
	MTU23.TGRC = PERIOD;
	MTU23.TGRD = PERIOD;
	
	MTU23.TCNT = 0x0000;			// カウンタクリア
	MTU23.TSR.BYTE = 0xC0;			// アップカウント

	MTU23.TMDR.BYTE = 0x02;			// PWMモード1
	
		
	MTU2.TSTR.BYTE |= 0x41;			// ch0のカウント始動
	
	
	/* Encoderの設定 */
	// MTU2 CH1,2の設定(Enc0,1)
	MTU2.TSTR.BYTE &= ~0x06;        //ch1,2のカウント停止
	MTU21.TMDR.BYTE = 0x04;         //ch1を位相係数モード1
	MTU22.TMDR.BYTE = 0x04;         //ch2を位相係数モード1
	MTU2.TSTR.BYTE |= 0x06;         //ch1,2のカウント開始
	
	
	 	
	
}

/************************************/
/*	CMT0割込み(1ms周期)				*/
/************************************/
#pragma interrupt (timer_int)
void timer_int(void)
{
	int	i;

	CMT0.CMCSR.WORD &= ~0x0080;      // コンペアマッチフラグをリセット

	for(i=0;i<TMR_NUM;i++) {
		if(TMR1m[i]) --TMR1m[i];
	}

	if(fnc)
		fnc();	// コールバック関数呼出し
}

