/****************************************************************************/
/*																			*/
/*		STK-7125 �T���v���v���O����											*/
/*			�^�C�}����														*/
/*																			*/
/*			Copyright   :: ���A���t�@�v���W�F�N�g							*/
/*			Cpu         :: SH7125											*/
/*			Language    :: SH-C												*/
/*			File Name   :: timer.c											*/
/*																			*/
/****************************************************************************/
#include "global.h"
#include "7125s.h"
#include "timer.h"

#include "global.h"

/* 1ms�_�E���^�C�} */
unsigned long TMR1m[TMR_NUM];

/* �R�[���o�b�N�֐���` */
static TIMER_CALLBACK fnc = 0;

/************************************/
/*	CMT0�R�[���o�b�N�֐��ݒ�		*/
/************************************/
void timer_callback(TIMER_CALLBACK cback)
{
	fnc = cback;
}

/************************************/
/*	CMT0,MTU20������				*/
/************************************/
void timer_init(TIMER_CALLBACK cback)
{
	
	/* CMT0�̐ݒ� */
	timer_callback(cback);			// �R�[���o�b�N�֐��ݒ�

	CMT.CMSTR.WORD &= ~0x0001;		// �^�C�}��~

	INTC.IPRJ.WORD = (INTC.IPRJ.WORD & ~0xf000) | 0x7000;
									// �����݃��x���̐ݒ� : Lv7

	CMT0.CMCNT = 0;					// �J�E���^�l�ݒ�
	CMT0.CMCOR = (PCLK/8/CFREQ)-1;	// 1/CFREQ[s]�C���^�[�o��
	CMT0.CMCSR.WORD = 0x0040;       // �R���y�A�}�b�`���荞�݃C�l�[�u��(5: 1)
	                                // �N���b�N�Z���N�g ��/8 (0:0 1:0)   

	CMT.CMSTR.WORD |= 0x0001;		// �^�C�}�X�^�[�g


	/* MTU20�̐ݒ� */
	/* PWM�̐ݒ�*/
	MTU2.TSTR.BYTE &= ~0x01;		// ch0�̃J�E���g��~
	
	
	// MTU2 CH0�̐ݒ�(PWM0,1)
	MTU20.TCR.BYTE = 0x42;			// TGRB�̃R���y�A�}�b�`�ŃJ�E���^�N���A  01000010
									// �����オ��G�b�W�ŃJ�E���g,MP��/16�ŃJ�E���g
									// TCNT�̃J�E���g�A�b�v�ɂ����鎞�� TCNT(s) = 1/�iMP��/16�j
									// TCNT(S)�͖�0.67ns							
	MTU20.TIOR.BYTE.H = 0x12;		// TGR0B,TGR0A�̐ݒ�                              
    MTU20.TIOR.BYTE.L = 0x12;       // TGR0D,TGR0C�̐ݒ�
									// TGR0B,D�R���y�A�}�b�`��0, TGR0A,C�R���y�A�}�b�`��1
									// TGR0B,D�������ATGR0A,C��duty									 
	MTU20.TGRA = PERIOD;            // �����l�ݒ�
	MTU20.TGRB = PERIOD;
	MTU20.TGRC = PERIOD;
	MTU20.TGRD = PERIOD;
	
	MTU20.TCNT = 0x0000;			// �J�E���^�N���A
	MTU20.TSR.BYTE = 0xC0;			// �A�b�v�J�E���g

	MTU20.TMDR.BYTE = 0x02;			// PWM���[�h1
	
	// MTU2 CH3�̐ݒ�(PWM2,3)
	MTU23.TCR.BYTE = 0x42;			// TGRB�̃R���y�A�}�b�`�ŃJ�E���^�N���A  01000010
									// �����オ��G�b�W�ŃJ�E���g,MP��/16�ŃJ�E���g
									// TCNT�̃J�E���g�A�b�v�ɂ����鎞�� TCNT(s) = 1/�iMP��/16�j
									// TCNT(S)�͖�0.67ns							
	MTU23.TIOR.BYTE.H = 0x12;		// TGR3B,TGR3A�̐ݒ�                              
    MTU23.TIOR.BYTE.L = 0x12;       // TGR3D,TGR3C�̐ݒ�
									// TGR3B,D�R���y�A�}�b�`��0, TGR3A,C�R���y�A�}�b�`��1
									// TGR3B,D�������ATGR3A,C��duty									 
	MTU23.TGRA = PERIOD;            // �����l�ݒ�
	MTU23.TGRB = PERIOD;
	MTU23.TGRC = PERIOD;
	MTU23.TGRD = PERIOD;
	
	MTU23.TCNT = 0x0000;			// �J�E���^�N���A
	MTU23.TSR.BYTE = 0xC0;			// �A�b�v�J�E���g

	MTU23.TMDR.BYTE = 0x02;			// PWM���[�h1
	
		
	MTU2.TSTR.BYTE |= 0x41;			// ch0�̃J�E���g�n��
	
	
	/* Encoder�̐ݒ� */
	// MTU2 CH1,2�̐ݒ�(Enc0,1)
	MTU2.TSTR.BYTE &= ~0x06;        //ch1,2�̃J�E���g��~
	MTU21.TMDR.BYTE = 0x04;         //ch1���ʑ��W�����[�h1
	MTU22.TMDR.BYTE = 0x04;         //ch2���ʑ��W�����[�h1
	MTU2.TSTR.BYTE |= 0x06;         //ch1,2�̃J�E���g�J�n
	
	
	 	
	
}

/************************************/
/*	CMT0������(1ms����)				*/
/************************************/
#pragma interrupt (timer_int)
void timer_int(void)
{
	int	i;

	CMT0.CMCSR.WORD &= ~0x0080;      // �R���y�A�}�b�`�t���O�����Z�b�g

	for(i=0;i<TMR_NUM;i++) {
		if(TMR1m[i]) --TMR1m[i];
	}

	if(fnc)
		fnc();	// �R�[���o�b�N�֐��ďo��
}

