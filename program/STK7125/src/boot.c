/****************************************************************************/
/*																			*/
/*		STK-7125 �T���v���v���O����											*/
/*			�u�[�g����														*/
/*																			*/
/*			Copyright   :: ���A���t�@�v���W�F�N�g							*/
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

/* �O���Q�� */
extern void	set_imask(int);
extern int	main(void);

/************************************/
/*	�Z�N�V����������				*/
/************************************/
void sectionInit(void)
{
	extern int	*_D_ROM, *_B_BGN, *_B_END, *_D_BGN, *_D_END;
	register int	*p, *q;

	/* ���������f�[�^�̈���[���ŏ����� */
	for ( p = _B_BGN; p < _B_END; p++ ) {
		*p = 0;
	}
	/* �������f�[�^��ROM�ォ��RAM��փR�s�[ */
	for ( p = _D_BGN, q = _D_ROM; p < _D_END; p++, q++ ) {
		*p = *q;
	}
}

/************************************/
/*	CPU������						*/
/************************************/
#pragma	section BOOT
void boot(void)
{
	
	/* �N���b�N���U��(CPG) : PLL=96MHz */
	CPG.FRQCR.WORD = 0x16db;		// I��=PLL/2,B��=PLL/4,P��=PLL/4,MP��=PLL/4
									// I��=48MHz,B��=24MHz,P��=24MHz,MP��=24MHz
	/* �s���t�@���N�V����(PFC) */
	//�V���A���ʐM�ɐݒ�
	PFC.PACRL1.WORD |= 0x1011;		// PA1 :TXD0(14),PA0 :RXD0(13), PA3:RXD1(16)
	PFC.PACRL2.WORD |= 0x0001;		// PA4 :TXD1(17)
	
	
	// �ʑ��W�����[�h�ɐݒ�(�G���R�[�_)
	PFC.PACRL2.WORD |= 0x1100;      // PA6: TCLKA, PA7: TCLKB(motor1)
	PFC.PACRL3.WORD |= 0x0011;      // PA8: TCLKC, PA9: TCLKD(motor2)
	
		
	// PWM�ݒ�
	PFC.PEIORL.WORD |= 0xffff;		// PE0 :OUT(PWM0)	1111 1111 1111 0101
									// PE2 :OUT(PWM1)	�i�s�����o�͂ɐݒ�
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
	
	
	
	//AD�ϊ�
	STB.CR4.BIT._AD0 = 0;			//port�̃X�^���o�C������
	// AD�ϊ���ҋ@��Ԃɐݒ�   1�ɂȂ�ƕϊ��J�n
	AD0.ADCR.BIT.ADST = 0; 
	// AD�G���h�t���O�@���Z�b�g
	AD0.ADCSR.BIT.ADF = 0; 
	// ADI���荞�݁@����
	AD0.ADCSR.BIT.ADIE = 0;
	// �g���K�C�l�[�u���i�O���g���K�j�@����
	AD0.ADCSR.BIT.TRGE = 0; 
	// ADF�R���g���[���@����
	AD0.ADCSR.BIT.CONADF = 0;
	// �X�e�[�g�R���g���[���@50�X�e�[�g
	AD0.ADCSR.BIT.STC = 0;
	// A/D�ϊ����Ԑݒ�
	AD0.ADCSR.BIT.CKSL = 0; ;
	
	AD0.ADCSR.BIT.ADM = 1;	// �V���O���X�L�������[�h
	AD0.ADCSR.BIT.ADCS = 0;	// 1�T�C�N���X�L����
	AD0.ADCSR.BIT.CH = 3;	// �`�����l���Z���N�g?????
	
	
	/* �X�^���o�C����(STB) */
	STB.CR3.BYTE &= ~0x18;			// SCI0,SCI1�X�^���o�C����
	STB.CR4.BYTE &= ~0x20;			// CMT�X�^���o�C����
	STB.CR4.BYTE &= ~0x40;			// MTU2�̃X�^���o�C���[�h������

		
	/* ������������ */
	sectionInit();

	/* �����f�o�C�X������ */
	timer_init(0);					// CMT0 : 1mS interval,callback����,MTU20 : PWM ���[�h1
	sci_init();						// SCI1 : 38400,8,1,N

	/* �����݉��� */
	set_imask(0);
	
	/* ���C���֐��ďo�� */
	main();
}






#pragma	section
