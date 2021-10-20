#include "cordiccart2pol.h"
#include "ap_int.h"
#include "ap_fixed.h"
#include <iostream>

typedef ap_fixed<15,3> ang_fix;
typedef ap_fixed<16,3> data_fix;
//typedef float ang_fix;
//typedef float data_fix;

data_fix Kvalues[20] = {1				,	0.500000000000000,	0.250000000000000,	0.125000000000000,	0.0625000000000000,	0.0312500000000000,	0.0156250000000000,	0.00781250000000000,	0.00390625000000000,	0.00195312500000000,	0.000976562500000000,	0.000488281250000000,	0.000244140625000000,	0.000122070312500000,	6.10351562500000e-05,	3.05175781250000e-05, 	1.52587890625e-05,	7.62939453125e-06,	3.814697265625e-06,	1.9073486328125e-06 };
ang_fix angles[20]   = {0.7853981633974,	0.463647609000806,	0.244978663126864,	0.124354994546761,	0.0624188099959574,	0.0312398334302683,	0.0156237286204768,	0.00781234106010111,	0.00390623013196697,	0.00195312251647882,	0.000976562189559320,	0.000488281211194898,	0.000244140620149362,	0.000122070311893670,	6.10351561742088e-05,	3.05175781155261e-05,	1.52587890613e-05,	7.62939453110e-06,	3.814697265606e-06,	1.9073486328101e-06 };
//						1					2					3					4					5					6					7					8						9						10						11						12						13						14						15						16						17					18					19					20
using namespace::std;

void cordiccart2pol(data_t x, data_t y, data_t * r,  data_t * theta)
{
#pragma HLS INTERFACE mode=ap_ctrl_none port=return
	#pragma HLS INTERFACE mode=s_axilite port=r
	#pragma HLS INTERFACE mode=s_axilite port=theta
	#pragma HLS INTERFACE mode=s_axilite port=y
	#pragma HLS INTERFACE mode=s_axilite port=x
	// Write your code here
	ang_fix  pi_2 = 1.57079632;//(3.1415926535897932/2);
	data_fix x_new;
	data_fix y_new;
	data_fix x_val = x;
	data_fix y_val = y;
	data_fix k=0.6072529350088812561694;
	ap_uint<5> i;
	ang_fix  theta_fixed=0;

	if(y>=0)
	{
		theta_fixed=theta_fixed+pi_2;
		x_new=y_val;
		y_new=-x_val;
	}
	else
	{
		theta_fixed=theta_fixed-pi_2;
		x_new=-y_val;
		y_new=x_val;
	}

	x_val=x_new;
	y_val=y_new;

	for(i=0;i<NO_ITER;i++)
	{

		if(y_val>=0)
		{
			theta_fixed=theta_fixed+angles[i];
			x_new=x_val+(y_val>>i);
			y_new=y_val-(x_val>>i);
//			x_new=x_val+(y_val*Kvalues[i]);
//			y_new=y_val-(x_val*Kvalues[i]);
		}
		else
		{
			theta_fixed=theta_fixed-angles[i];
			x_new=x_val-(y_val>>i);
			y_new=y_val+(x_val>>i);
//			x_new=x_val-(y_val*Kvalues[i]);
//			y_new=y_val+(x_val*Kvalues[i]);
		}

//		if(i==8)
//			cout<<y_val<<endl;

		x_val=x_new;
		y_val=y_new;
	}
	*r=x_val*k;
	*theta=theta_fixed;
}
