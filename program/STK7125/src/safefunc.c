// Safety fuction
//  
//                  written by jeong 2010.06.08


#include "motor.h"


#define ErrRange 20          //Error range 10[deg/s]

/*
void SafeCheck(){
	
	int safeflag[2];
	int safesum=0;
	
	safeflag[0]=SafeCompare();
	safeflag[1]=SafeCrossCheck();	
	
	safesum = safeflag[0]+safeflag[1];
	
	if(safesum) SafeSignalOutput();
	
}

*/

int SafeCompare(){
	
	//Compare joint angular velocity of two encoder of a double motor 
	double vel1=0;
	double vel2=0;
	double err=0;
	
	vel1 = getJointAngVel(0);
	vel2 = getJointAngVel(1);

	if(vel1 > 200 || vel2 > 200) return 1;
//	if(vel1 > 3000 || vel2 > 3000) return 1;
	else return 0;
	
	//err = vel1-vel2;
		
	//if(err < -ErrRange | err > ErrRange) return 1;
	//else return 0;	
	
}

/*
void SafeCrossCheck(){
	
    // Cross check the angular velocity	between sh2
	
}

void SafeSignalOutput(){
	
	// output safe signal 
	// sputs("over speed")
	
}

*/


