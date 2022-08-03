/*
 *      control.h
 *                                      2008.02.02.  jeong
 */

#ifndef _CONTROL_H
#define _CONTROL_H


void pid_count(char ch);      // IN: desired encoder counter
void pid_angle(char ch);      // IN: desired encoder counter
double StateFeedBack(void);
void Kinematics();
double getGoal_Pf();
double getGoal_Phi();
double get_vf();
double get_Pf(); 
double get_dphi();
double get_phi();
void setGoal_vf(double val);        //set goal foward velocity
void setGoal_dphi(double val);      //set goal rotational velocity


#endif 
