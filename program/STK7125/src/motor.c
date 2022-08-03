// motor.c


#include "7125s.h"
#include "global.h"
#include "timer.h"
#include "sci.h"
#include "motor.h"

#define AveNum 10                            /* Moving Average Number */

joint_type J[J_Num];                         /* Define joint struct */
sensor_type Sen;

void encoder_init( int jn, int Dir)
{
  J[jn].en.dir = Dir;
  J[jn].en.cnt = 0;
  J[jn].en.diff_cnt = 0;
}

int encoder_pool(void){

  int jn;
  short cnt_data[2]={0,0};
  static short old_data[2];


  cnt_data[0] = J[0].en.dir* MTU21.TCNT;     // read J0 counter 
  cnt_data[1] = J[1].en.dir* MTU22.TCNT;     // read J1 counter 
  
  for(jn=0; jn<J_Num; jn++){ 
    
    J[jn].en.diff_cnt = cnt_data[jn] - old_data[jn];
	old_data[jn] = cnt_data[jn];                 // save present conter data
	
	// Overflow ëŒçÙ    
    /* case: over flow  */
    if((J[jn].en.diff_cnt > 0) && (J[jn].en.diff_cnt > 0x4000))
      J[jn].en.diff_cnt = J[jn].en.diff_cnt - 0x10000;  
    /* case: under flow */    
    if((J[jn].en.diff_cnt < 0) && (-J[jn].en.diff_cnt > 0x4000))
      J[jn].en.diff_cnt = 0x10000 + J[jn].en.diff_cnt;
     
        
    /* Save encoder conter (overflow ëŒçÙå„) */
    J[jn].en.cnt += J[jn].en.diff_cnt;          
	J[jn].th = J[jn].en.cnt*J[jn].en.PUL_TO_DEG;
  	J[jn].dth = J[jn].en.diff_cnt*J[jn].en.PUL_TO_DEG/CTIME;
	}
   
	
	return 1;
}


double getJointAngVel(int ch){ return J[ch].dth;  }


/*----------------------------- 
 *  MOTOR Related Function  
 *-----------------------------*/
/* motor initialize */
void motor_init( int jn, int State, int Dir) 
{
  J[jn].mo.state = State;
  J[jn].mo.dir = Dir;
  J[jn].mo.duty = 0;

}


/* interface */
void set_duty(int jn, int d){
	
	J[jn].mo.duty = d;
}

int get_duty( int jn){ return(J[jn].mo.duty); }


/* Output PWM duty ratio */
void pwm_output(void)
{
  int jn;
  int mo1,mo2,mo3,mo4;
  
  /* Limit PWM duty ratio */
  for (jn=0; jn<2; jn++){
	  J[jn].mo.duty = J[jn].mo.dir*J[jn].mo.duty;
    if(J[jn].mo.state==1){
      if(J[jn].mo.duty > J[jn].mo.duty_max) 
	J[jn].mo.duty = J[jn].mo.duty_max;
      else if(J[jn].mo.duty < -J[jn].mo.duty_max) 
	J[jn].mo.duty = -J[jn].mo.duty_max;
      else J[jn].mo.duty = J[jn].mo.duty;
    }
    else J[jn].mo.duty = 0;
  }


	mo1=J[0].mo.duty-OffsetDuty;
    mo2=J[0].mo.duty+OffsetDuty;
    	 
    if(mo1 < 0){
	MTU20.TGRA = PERIOD+mo1;
	PE.DRL.BIT.B4 = 0;
	PE.DRL.BIT.B5 = 1;
	}
	else if(mo1> 0){
    MTU20.TGRA = PERIOD-mo1;
	PE.DRL.BIT.B4 = 1;
	PE.DRL.BIT.B5 = 0;
	}
	else {
	MTU20.TGRA = PERIOD;
	PE.DRL.BIT.B4 = 0;
	PE.DRL.BIT.B5 = 0;
	}
		 
	if(mo2 < 0){
    MTU20.TGRC = PERIOD+mo2;
	PE.DRL.BIT.B6 = 0;
	PE.DRL.BIT.B7 = 1;	
	}
	else if(mo2 > 0){
	MTU20.TGRC = PERIOD-mo2;
	PE.DRL.BIT.B6 = 1;
	PE.DRL.BIT.B7 = 0;
    } 
    else {
    MTU20.TGRC = PERIOD;	
	PE.DRL.BIT.B6 = 0;
	PE.DRL.BIT.B7 = 0;	
    }	 
	



   	mo3=J[1].mo.duty-OffsetDuty;
    mo4=J[1].mo.duty+OffsetDuty;

    if(mo3 < 0){
    MTU23.TGRA = PERIOD+mo3;
	PE.DRL.BIT.B12 = 0;
	PE.DRL.BIT.B13 = 1;
	}	
    else if(mo3 > 0){
    MTU23.TGRA = PERIOD-mo3;
	PE.DRL.BIT.B12 = 1;
	PE.DRL.BIT.B13 = 0;
    }	
	else {
	MTU23.TGRA = PERIOD;
    PE.DRL.BIT.B12 = 0;
	PE.DRL.BIT.B13 = 0;
	}
	
    if(mo4 < 0){
   	MTU23.TGRC = PERIOD+mo4;
	PE.DRL.BIT.B14 = 0;
	PE.DRL.BIT.B15 = 1;
    }
	else if(mo4 > 0){
	MTU23.TGRC = PERIOD-mo4;
	PE.DRL.BIT.B14 = 1;
	PE.DRL.BIT.B15 = 0;	
    }
    else {
	MTU23.TGRC = PERIOD;	   
	PE.DRL.BIT.B14 = 0;
	PE.DRL.BIT.B15 = 0;
    }
	
 }

   
/*
void pwmOutput(){
	
	// PWM1,2 duty ê›íË
	MTU20.TGRA = PERIOD-50;
	MTU20.TGRC = PERIOD-100;
    // DirópI/Oê›íË
	PE.DRL.BIT.B4 = 0;
	PE.DRL.BIT.B5 = 1;
	
}
*/

void set_pidGain(char ch, double kp, double kv, double ki){

	J[ch].kp_j = kp;
	J[ch].kv_j = kv;
	J[ch].ki_j = ki;	
	
}


/************************************/
/*		ADïœä∑						*/
/************************************/
void adConvert(void){
	

	double read[3];
	
	AD0.ADCR.BIT.ADST = 1;      // ADäJén
    while(AD0.ADCSR.BIT.ADF==0);  // ADäÆóπë“Çø
    read[0] = AD0.ADDR0 >> 6; // AN0 (òcorìdó¨)ÇÃèÛë‘ÇéÊìæ (21)
	read[1] = AD0.ADDR1 >> 6; // AN1(òcorìdó¨) ÇÃèÛë‘ÇéÊìæ (22)
    AD0.ADCSR.BIT.ADF = 0;      // ADäÆóπÉtÉâÉOÉNÉäÉA
	
	Sen.ch0 = (int)(read[0] * 5000 / 1024);  //
	Sen.ch1 = (int)(read[1] * 5000 / 1024);  //Gyroë™íË
			
}

int MoveAverage(){
	
	double ave=0;
	int s=0,i=0;
	static int count=0;
	static data[AveNum]={0,};
	
	data[count]= Sen.ch1;
	
	if(count< AveNum) count++;
	else count=0;
	
	for(i=0;i<AveNum;i++){
		s=s+data[i];
	}
	ave=s/AveNum;
	
	return (int)ave;
	
}


/***********************************/
/*		äpë¨ìxÅïäpìx			   */
/***********************************/
void CalBodyAngle(){
	
	double ang,angvel;
    static double ang_old=0;
	int senVal=0;
	
	//senVal=MoveAverage();	
	
    //angvel= (1/4. * Sen.ch1/5. - 125);		//äpë¨ìxÇÃåvéZ

	angvel= 1/4. * (Sen.ch1 + 7) / 5. - 125;		//äpë¨ìxÇÃåvéZ
	//angvel= 1/4. * (MoveAverage())/5. - 125;		//äpë¨ìxÇÃåvéZ

	ang = angvel * CTIME + ang_old;	// êœï™-->äpìx
	ang_old = ang;		     		// êœï™-->äpìx
	
	J[2].th = ang;
	J[2].dth = angvel;
	
}

