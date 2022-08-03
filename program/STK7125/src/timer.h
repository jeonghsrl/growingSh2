/****************************************************************************/
/*																			*/
/*		STK-7125 CMT サンプルプログラム ヘッダファイル			 			*/
/*																			*/
/*			Copyright   :: ㈱アルファプロジェクト							*/
/*			Language    :: SH-C												*/
/*			File Name   :: timer.h											*/
/*																			*/
/****************************************************************************/
#ifndef __TIMER_H__
#define __TIMER_H__

// PWM周期用カウンタ
#define PERIOD	100   //100: 15kHz

// 1msec ダウンタイマ
#define	TMR_NUM			4				// タイマ数
extern unsigned long TMR1m[TMR_NUM];

// コールバック関数
typedef void (*TIMER_CALLBACK)(void);	// コールバック関数定義

// プロトタイプ宣言
void timer_init(TIMER_CALLBACK);
void timer_int(void);
void timer_callback(TIMER_CALLBACK);

#endif		// END __TIMER_H__
