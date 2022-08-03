#include "global.h"
#include "motor.h"
#include "paraset.h"

extern joint_type J[J_Num];

/* Initialize and Parameter setting of each joint */
void parameter_set(){
  
  /* J0 encoder initialize */
  encoder_init(J0, J0_EN_DIR); 
  J[J0].en.PUL_TO_DEG = 360/(J0_EN_PPR*J0_EN_REDUC);     /*CNT to DEG*/
  /* J0 motor initialize */
  motor_init(J0, J0_MO_STATE, J0_MO_DIR); 
  J[J0].mo.duty_max = J0_MO_DUTY_MAX;                   /* Set Max duty */
  J[J0].kp_j = J0_KP_J;                                 /* Set PID gain for joint control */
  J[J0].kv_j = J0_KV_J;
  J[J0].ki_j = J0_KI_J;
  
  
/* J1 encoder initialize */
  encoder_init(J1, J1_EN_DIR); 
  J[J1].en.PUL_TO_DEG = 360/(J1_EN_PPR*J1_EN_REDUC);        /*CNT to DEG*/
  /* J0 motor initialize */
  motor_init(J1, J1_MO_STATE, J1_MO_DIR); 
  J[J1].mo.duty_max = J1_MO_DUTY_MAX;            /* Set Max duty */
  J[J1].kp_j = J1_KP_J;                          /* Set PID gain for joint control */
  J[J1].kv_j = J1_KV_J;
  J[J1].ki_j = J1_KI_J;
  

  
}

