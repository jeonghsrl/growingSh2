/****************************************************************************/
/*																			*/
/*		モータ速度制御用プログラム											*/
/*		　　　　　　　　													*/
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

extern joint_type J[J_Num];         // joint構造体外部宣言
extern sensor_type Sen;

int loopCount=1;                    // timerloop counter
char Transbuf[2];
char commText[10]={0,};      // コンマ度文字格納用変数
int commData=0;                     //コマンド格納用変数
int i=0;
int safeflag=1;
short rd;
char data='s';
int receiveData;
int DesirdVelSide=0;

/************************************/
/*	Timerloop関数				    */
/*	timer_intからコールバック		*/
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


	encoder_pool();                        //角度計算
	adConvert(); 				   //AD計算
	CalBodyAngle();				// ジャイロとか
	Kinematics();
	//SafeCompare();
	StateFeedBack();
    //uniVelTraj(0,300);	       //J0へ目標速度入力
	//uniVelTraj(0,DesirdVel);
	//pid_angle(0);                          //J0のpid制御
	
	pwm_output();                          //pwm出力 

   //rd = sci_getc();     
   //printDecimal1(loopCount);
 /*  
   if(rd!=sciEmpty){ // enterキーが押されるまで 
		if(!(rd=='\r')) {             // 文字を格納
			commText[i] = (char)rd;   // 押されたら表示
//			sci_putc((char)(rd));
			i++;
			} 	
			else {
            sputc((char)(rd));			
			commData = atoi(commText); // 文字をint型でコマンド変数に格納
			}
			for(j=0;j<10;j++)          // 文字格納変数をクリア
				commText[j]='\0';  
			i=0;
		}
	}

*/



 /* 角度、角速度送信 */

	if(!(loopCount%1)){	
		
//		sci0_putc(data);                //横移動角度要求
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
/*	MAIN関数						*/
/************************************/
int main(void)
{


	short rd=0;                    // 1文字格納用変数  
	char dummy = 0;

	/* スタートメッセージ */
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
	
	/* Joint parameter 設定 */
	parameter_set();
		
	/* 5ms周期で、timer_int割込からtimerloop関数呼出 */
	timer_callback(timerloop);
	
	/* 無限ループ */
	for (;;){
	 
		rd = sci_getc();     // 1文字ゲット
 	
		if(rd != sciEmpty){                 // enterキーが押されるまで
			sci_putc((char)(rd));	
		} 				
						
		if(rd=='w') setGoal_vf(5);		// 前
		if(rd=='z') setGoal_vf(-5);		// 後
		if(rd=='a') setGoal_dphi(30);	// 旋回
		if(rd=='s') setGoal_dphi(-30);	// 旋回左
//    	if(rd=='d') DesirdVelSide=30;	//side 
//	 	if(rd=='f') DesirdVelSide=-30;	// side
		if(rd=='x') {					// リセット
			setGoal_vf(0);
		    setGoal_dphi(0);
			DesirdVelSide=0;
		}			
		if(rd=='q') {                     // q が押されたら強制終了 
    		sputs("\r\nProgram stop!\r\n"); 
			break;			
		}
		
	}



	return 0;
}
