// motor.h 


#define J_Num 3              //joint number
#define OffsetDuty 5     //offset duty for double motor actuating

/* Encoder Struct */
typedef struct ___encoder_type{

  int dir;               /* CW or CCW */
  int cnt;               /* count number */
  int diff_cnt;          /* change of count */
  
  int cnt_d, dcnt_d;     /* desired count value */
  
  double PUL_TO_DEG;        /* convert pulse to degree */

}encoder_type;


/* Motor Struct */
typedef struct ___motor_type{

  int state;          /* ENABLE or DISABLE*/
  int dir;            /* CW or CCW */
  int duty;           /* duty value */
  int duty_max;       /* max duty value */
  
}motor_type;


/* Joint Struct */
typedef struct __joint_type{

  encoder_type en;                /* Declare encoder struct */
  motor_type mo;
  double th,dth;                  /* angle [deg] */ 
  double th_d,dth_d;              /* desired andgle[deg] */
  double kp_j, kv_j, ki_j;           /* PID gain for joint control */ 
   
}joint_type;


/* state valuable struct */
typedef struct __state{
  int n;
  int old;
  int i;
  int d;
  int f;
}state;


typedef struct __sensor{
	
	int ch0;
	int ch1;
	int ch2;
	int ch3;
	
}sensor_type;




/*--- Motor related declaration -------*/
void motor_init( int Ch, int State, int Dir);  /* Motor initialize */
void set_duty( int ch, int d);                 /* Set duty */
int  get_duty( int ch);                        /* Get duty */  
void pwm_output(void);                         /* Output PWM */

// Encoder 
int encoder_pool();
void encoder_init(int jn, int Dir);
double getJointAngVel(int ch);

// Sensor
void adConvert();
void CalBodyAngle();
int MoveAverage();

// PID
void set_pidGain(char ch, double kp, double kv, double ki);


int SafeCompare();





