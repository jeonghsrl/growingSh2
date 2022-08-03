/* global.h :  gloval variables definition
*/


#ifndef _GLOBAL_H
#define _GLOBAL_H


#define	PCLK		24000000		// 周辺クロック周波数


#define ENABLE 1
#define CW -1
#define CCW 1
#define ON 1
#define OFF 0

#define SHPI  0x0003243f         /* PI  */
#define SH_2PI  0x0006487e       /* 2PI */


#define T_m  5000                       /* Task operation time [ms]*/
#define CFREQ   200                     /* Cycle Frequency Timerloop用 */
#define CTIME   0.005                   /* Cycle Time[ms] Timerloop用*/ 
  
  
  
#define MaxVel 1500                       /* 最大回転速度 [deg/s] */ 
  
  
#endif
