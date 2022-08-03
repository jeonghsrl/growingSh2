/*  
 *                 control.c                  
 *                                    2008.02.02,  jeong        
 */


#include "motor.h"
#include "sci.h"
#include "7125s.h"
#include "global.h"
#include "trajectory.h"
#include "control.h"


#define GaAdj 10
#define bw 0.530            // distance between wheels
#define rw 0.04            // wheel radius             

extern joint_type J[J_Num];
double Pf,Ps,phi,X,Y;
double vf,dphi,vs,Vx,Vy;
double Goal_vf, Goal_dphi;
	
	
// for i gain
int val_err_i[J_Num] = {0,0};
int dval_err_i[J_Num] = {0,0};




/* PID control */
void pid_count(char ch){

  int val_n, dval_n;
  int val_d, dval_d;
  int val_err, dval_err;
 
 	// load value  
	val_n = J[ch].en.cnt;        //現在の円コーダの値を読む
  	dval_n = J[ch].en.diff_cnt;
	val_d = J[ch].en.cnt_d;
	dval_d = J[ch].en.dcnt_d;
 
    // pid control
    val_err = val_d - val_n;
    dval_err = dval_d - dval_n;
	
    val_err_i[ch]+= val_err;
    dval_err_i[ch]+= dval_err;
    
    J[ch].mo.duty = (J[ch].kp_j*val_err)/GaAdj;
    J[ch].mo.duty+= (J[ch].kv_j*dval_err)/GaAdj;
    J[ch].mo.duty+= J[ch].ki_j*val_err_i[ch];
  
 }

/* PID control */
void pid_angle(char ch){

  double val_n, dval_n;
  double val_d, dval_d;
  double val_err, dval_err;
 
 	// load value  
	val_n = J[ch].th;
	dval_n = J[ch].dth;
	val_d = J[ch].th_d;        
		dval_d = J[ch].dth_d;
 
    // pid control
    val_err = val_d - val_n;     
	dval_err = dval_d - dval_n;
	val_err_i[ch]+= val_err;
    dval_err_i[ch]+= dval_err;
    J[ch].mo.duty = (J[ch].kp_j*val_err);
    J[ch].mo.duty+= (J[ch].kv_j*dval_err);
    J[ch].mo.duty+= J[ch].ki_j*val_err_i[ch];
  
 }


/* 状態フィードバック */
double StateFeedBack(void)
{
	
	double x[6]={0,0,0,0,0,0};     // psi,dpsi,Pf,vf,phi,dphi
    double xd[6]={0,0,0,0,0,0};
	double K[6]={15,0.2,0,0,0,0};
//	double K[6]={15,0.01,0,0,0,0};


    double u_psi, u_f, u_s, u_phi;


    xd[0]=0;                      // Desired inclination angle of a body
    xd[1]=0;                      // Desired inclination angvel of a body
    xd[2]=getGoal_Pf();                      // Desired foward position
    xd[3]=0;//getGoal_vf();       // Desired foward velocity (defined in ode.h)
    xd[4]=getGoal_Phi();
    xd[5]=0;//getGoal_dphi();         // defined in ode.h

	x[0] = J[2].th;      // Inclination angle of a body
    x[1] = J[2].dth;     // Incl. angular vel. of a body 
	x[2] = get_Pf();     // Forward position   
    x[3] = get_vf();     // Forward velocity
    x[4] = get_phi();
    x[5] = get_dphi();
	
	
	u_psi=K[0]*(x[0]-xd[0])+K[1]*x[1];
    u_f=K[2]*(x[2]-xd[2])+K[3]*x[3];
    u_phi=K[4]*(x[4]-xd[4])+K[5]*x[5];

    J[0].mo.duty = u_psi+u_f-u_phi;    //right wheel
    J[1].mo.duty = u_psi+u_f+u_phi;    //left wheel

/*	
    double xc[2];
    double xcd[2]={0,0};
    double Kc[2]={3,3};

    xcd[0]=getGoal_sidev();            //defined in ode.h
    xcd[1]=0;                          //defined in ode.h
    xc[0] = get_vs();
   // xc[1] = get_sidea();

    J[2].mo.u = Kc[0]*(xcd[0]-xc[0]);//+Kc[1]*(xc[1]-xdc[1]);
*/
	
/*	
	phi = J[2].th;		    //body angle
	dphi = J[2].dth;	    //body anglevel		
	lw_th = J[1].th;
	lw_dth = J[1].dth;
	rw_th = J[0].th;
	rw_dth = J[0].dth;
		
	// 状態フィードバック
	u_lt = k1*(phi-phi_d) + k2*(dphi-dphi_d) + k3*(lw_th-lw_thd) + k4*(lw_dth-0);
	u_rt = k1*(phi-phi_d) + k2*(dphi-dphi_d) + k3*(rw_th-rw_thd) + k4*(rw_dth-0);
		
	// モータへ出力
	J[0].mo.duty=u_rt;
    J[1].mo.duty=u_lt;
*/
	return 0;
}
 
 
 void Kinematics(){

    vf= rw*(J[0].dth+J[1].dth)/2.;  //foward velocity
    Pf+=vf*CTIME;                 //foward position
    dphi=rw*(J[0].dth - J[1].dth)/(bw); //steering velocity
    phi+=dphi*CTIME;            //steering angle
	
/*
    vs= Wh_R*(J[2].dth);
    Ps+=vs*SAMP_TIME;

    //Pos. and Vel. of a robot in a world coordinate 
    Vx=Wh_R*cos(phi)*(J[0].dth+J[1].dth)/2-Wh_R*sin(phi)*J[2].dth;
    Vy=Wh_R*sin(phi)*(J[0].dth+J[1].dth)/2+Wh_R*cos(phi)*J[2].dth;
    X+=Vx*SAMP_TIME;
    Y+=Vy*SAMP_TIME;
*/
}


double getGoal_Pf(){

 static double GoalPf=0;
 double GoalVf;

 GoalPf+=Goal_vf*CTIME;

 return GoalPf;

}

double getGoal_Phi(){

 static double GoalPhi=0;
 double GoaldPhi;

 GoalPhi+=Goal_dphi*CTIME;

 return GoalPhi;

}


// Get foward velocity of a robot
double get_vf(){ return vf; }
double get_Pf(){ return Pf;}
// Get rotational velocity of a robot
double get_dphi(){return dphi;}
/* steering angle */
double get_phi(){return phi;}
//double get_vs(){return vs;}

//double get_Ps(){return Ps; } /* Side moving distance */
/*
double get_Vx(){return Vx;}
double get_Vy(){return Vy;}
double get_X(){return X;}
double get_Y(){return Y;}
*/


void setGoal_vf(double val){
	
	Goal_vf = val;
	
}
void setGoal_dphi(double val){
	
	Goal_dphi = val;
	
}
	
//	double 
	
	
	
//}









/*
void Third_traj(){

  double T_m =1;
  static double ti=0;

  if(ti<T_m)  ti = ti+SAMP_TIME;
  else ti = T_m;

  J[0].th_d = (J[0].th_g-0)*(-2.*pow((ti/T_m),3)+3.*(pow((ti/T_m),2)));
  J[1].th_d = (J[1].th_g-0)*(-2.*pow((ti/T_m),3)+3.*(pow((ti/T_m),2)));

}

 */