/*
 *               paraset.h                      
 *                                     2008.02.02.    jeong        
 */

#ifndef _PARASET_H
#define _PARASET_H

/* Joint 0 */
#define J0             0                     /* joint number */

#define J0_EN_ADDR     0x00                  /* Encoder adress */ 
#define J0_EN_DIR      CCW                    /* Rotational direction of encoder*/
#define J0_EN_PPR      2000.                  /* PPR of encoder */
#define J0_EN_INIT     0                     /* Initial angle of encoder */ 
#define J0_EN_REDUC    11                   /* 4.4x2.5  Reduction ratio for encoder */

#define J0_MO_STATE    ENABLE                /* Motor state */
#define J0_MO_DIR      CW                    /* Rotational direction of motor */
#define J0_MO_REDUC    1                     /* Reduction ration for motor */
#define J0_MO_DUTY_MAX 95                   /* Max duty */

#define J0_KP_J        5                     /* P gain for joint control */
#define J0_KV_J        0.3                     /* D gain for joint control */
#define J0_KI_J        0                     /* I gain for joint control */

/* Joint 1 */
#define J1			   1                     /* joint number */
#define J1_EN_ADDR     0x00                  /* Encoder adress */ 
#define J1_EN_DIR      CW                    /* Rotational direction of encoder*/
#define J1_EN_PPR      2000.                  /* PPR of encoder  */
#define J1_EN_INIT     0                     /* Initial angle of encoder */ 
#define J1_EN_REDUC    11                     /* Reduction ratio for encoder */

#define J1_MO_STATE    ENABLE                /* Motor state */
#define J1_MO_DIR      CCW                    /* Rotational direction of motor */
#define J1_MO_REDUC    1                     /* Reduction ration for motor */
#define J1_MO_DUTY_MAX 95                     /* Max duty */

#define J1_KP_J        5                     /* P gain for joint control */
#define J1_KV_J        0.3                   /* D gain for joint control */
#define J1_KI_J        0                     /* I gain for joint control */


void parameter_set();                        /* Initialize and Parameter setting  */


#endif 
