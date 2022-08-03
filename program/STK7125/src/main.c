/****************************************************************************/
/*																			*/
/*		���[�^���x����p�v���O����											*/
/*		�@�@�@�@�@�@�@�@													*/
/*																			*/
/*			Copyright   :: HSRL  by jeong          							*/
/*			Cpu         :: SH7125											*/
/*			Language    :: SH-C												*/
/*			File Name   :: main.c											*/
/*																			*/
/****************************************************************************/
#include <machine.h>
#include <stdlib.h>
#include <stdio.h>
#include "7125s.h"
#include "global.h"
#include "timer.h"
#include "sci.h"
#include "motor.h"
#include "trajectory.h"
#include "control.h"

extern joint_type J[J_Num];         // joint�\���̊O���錾
extern sensor_type Sen;

int loopCount=1;                    // timerloop counter
char Transbuf[2];
char commText[10]={0,};      // �R���}�x�����i�[�p�ϐ�
int commData=0;                     //�R�}���h�i�[�p�ϐ�
int i=0;
int safeflag=1;
short rd;
char data='s';
int receiveData;
int DesirdVelSide=0;

/************************************/
/*	Timerloop�֐�				    */
/*	timer_int����R�[���o�b�N		*/
/************************************/
void timerloop(void)
{
	
	int j;

	double read;
	//int safeflag;
    static double DesirdVel=0;    

	//set_duty(0,10);
	//set_duty(1,10);
		 
	//set_duty(0,commData);
    //set_duty(1,commData);


	encoder_pool();                        //�p�x�v�Z
	adConvert(); 				   //AD�v�Z
	CalBodyAngle();				// �W���C���Ƃ�
	Kinematics();
	//SafeCompare();
	StateFeedBack();
    //uniVelTraj(0,300);	       //J0�֖ڕW���x����
	//uniVelTraj(0,DesirdVel);
	//pid_angle(0);                          //J0��pid����
	
	pwm_output();                          //pwm�o�� 

   //rd = sci_getc();     
   //printDecimal1(loopCount);
 /*  
   if(rd!=sciEmpty){ // enter�L�[���������܂� 
		if(!(rd=='\r')) {             // �������i�[
			commText[i] = (char)rd;   // �����ꂽ��\��
//			sci_putc((char)(rd));
			i++;
			} 	
			else {
            sputc((char)(rd));			
			commData = atoi(commText); // ������int�^�ŃR�}���h�ϐ��Ɋi�[
			}
			for(j=0;j<10;j++)          // �����i�[�ϐ����N���A
				commText[j]='\0';  
			i=0;
		}
	}

*/



 /* �p�x�A�p���x���M */

	if(!(loopCount%1)){	
		
//		sci0_putc(data);                //���ړ��p�x�v��
		printDecimal(J[0].th);
		sputs("\t");
		printDecimal(J[1].th);
		sputs("\t");
		printDecimal(J[2].th);
//		sputs("\t");





		
		sputs("\n\r");	
	}



	loopCount++;
	

}




/************************************/
/*	MAIN�֐�						*/
/************************************/
int main(void)
{


	short rd=0;                    // 1�����i�[�p�ϐ�  
	char dummy = 0;

	/* �X�^�[�g���b�Z�[�W */
	sputs("\n\r----------------------------------------------\n\r");
	sputs("Motor control program for sh2-7125 \n\r");
	sputs("HSRL by jeong (2010.03.20) \n\r");
	sputs("-----------------------------------------------\n\r");
	sputs("\n\rHit 's' to start \n\r");

		
	while(!(rd=='s')){
		rd = sci_getc();
		if(rd != sciEmpty){
		sci_putc((char)(rd));
		}
	};

	sputs("Program start....\n\r");	
	
	/* Joint parameter �ݒ� */
	parameter_set();
		
	/* 5ms�����ŁAtimer_int��������timerloop�֐��ďo */
	timer_callback(timerloop);
	
	/* �������[�v */
	for (;;){
	 
		rd = sci_getc();     // 1�����Q�b�g
 	
		if(rd != sciEmpty){                 // enter�L�[���������܂�
			sci_putc((char)(rd));	
		} 				
						
		if(rd=='w') setGoal_vf(5);		// �O
		if(rd=='z') setGoal_vf(-5);		// ��
		if(rd=='a') setGoal_dphi(30);	// ����
		if(rd=='s') setGoal_dphi(-30);	// ����
//    	if(rd=='d') DesirdVelSide=30;	//side 
//	 	if(rd=='f') DesirdVelSide=-30;	// side
		if(rd=='x') {					// ���Z�b�g
			setGoal_vf(0);
		    setGoal_dphi(0);
			DesirdVelSide=0;
		}			
		if(rd=='q') {                     // q �������ꂽ�狭���I�� 
    		sputs("\r\nProgram stop!\r\n"); 
			break;			
		}
		
	}



	return 0;
}
