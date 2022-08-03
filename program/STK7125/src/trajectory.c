/*
 *          trajectory.c            
 *                                     2008.02.02     jeong 
 */ 

#include "global.h"
#include "motor.h"
#include "control.h"
#include "trajectory.h"
#include "sci.h"


extern joint_type J[J_Num];

int uniVelTraj(char ch, double omega){

	/* ‘¬“x§ŒÀ@*/
	if( omega >= MaxVel) omega=MaxVel; 
	else if (omega <= -MaxVel) omega=-MaxVel;
	else omega = omega;


	/* “™‘¬‹O“¹¶¬*/
	J[ch].th_d += omega*CTIME;
	J[ch].dth_d = omega;
	
	return 1;
	 		
}

