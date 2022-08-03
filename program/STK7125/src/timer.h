/****************************************************************************/
/*																			*/
/*		STK-7125 CMT �T���v���v���O���� �w�b�_�t�@�C��			 			*/
/*																			*/
/*			Copyright   :: ���A���t�@�v���W�F�N�g							*/
/*			Language    :: SH-C												*/
/*			File Name   :: timer.h											*/
/*																			*/
/****************************************************************************/
#ifndef __TIMER_H__
#define __TIMER_H__

// PWM�����p�J�E���^
#define PERIOD	100   //100: 15kHz

// 1msec �_�E���^�C�}
#define	TMR_NUM			4				// �^�C�}��
extern unsigned long TMR1m[TMR_NUM];

// �R�[���o�b�N�֐�
typedef void (*TIMER_CALLBACK)(void);	// �R�[���o�b�N�֐���`

// �v���g�^�C�v�錾
void timer_init(TIMER_CALLBACK);
void timer_int(void);
void timer_callback(TIMER_CALLBACK);

#endif		// END __TIMER_H__
