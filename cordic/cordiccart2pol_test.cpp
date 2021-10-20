#include "cordiccart2pol.h"
#include <math.h>
#include <stdio.h>
#include <cstdlib>


struct Rmse
{
	int num_sq;
	float sum_sq;
	float error;

	Rmse(){ num_sq = 0; sum_sq = 0; error = 0; }

	float add_value(float d_n)
	{
		num_sq++;
		sum_sq += (d_n*d_n);
		error = sqrtf(sum_sq / num_sq);
		return error;
	}

};


Rmse rmse_theta;
Rmse rmse_r;


void run_test(data_t x, data_t y, double golden_theta, double golden_r)
{
	data_t r;
	data_t theta;

	cordiccart2pol(x, y, &r,  &theta);

//	printf("Test: x=%.4f, y=%.4f, golden theta=%.4f, golden r=%.4f, your theta=%.4f, your r=%.4f\n",
//			(double)x, (double)y, golden_theta, golden_r, (double)theta, (double)r);
//	if(abs((double)theta - golden_theta) > 0.01 )
//	{
//		printf("Error");
//	}

	rmse_theta.add_value((float)theta - golden_theta);
	rmse_r.add_value((float)r - golden_r);
}

data_t radius (data_t x, data_t y)
{
	return sqrt(x*x + y*y);
}

data_t theta_calc (data_t x, data_t y)
{
	data_t pi = 3.14159265358979;
	data_t ang_temp = atan(y/x);
	if(x<0)
	{
		if(y<0)
			ang_temp = ang_temp - pi;
		else
			ang_temp = ang_temp + pi;
	}
	return ang_temp;
}

data_t rand_val ()
{
	data_t r = 0.001 + (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 0.999;
	return r;
}

int main()
{
	data_t x;
	data_t y;
	data_t r;
	data_t theta;


	printf("---Testing results----------------------------------\n");

	// Test1
	run_test(0.8147, 0.1269, 0.154521173805634, 0.824523923242982);

	// Test 2
	run_test(0.6323, -0.2785, -0.414888412375609, 0.690916449362729);

	// Test 3
	run_test(-0.5469, -0.9575, -2.089751217098013, 1.102681214132172);

	// Test 4
	run_test(-0.4854, 0.7003, 2.176897679895152, 0.852075847562880);

	data_t x_rand ;
	data_t y_rand ;
	data_t r_val;
	data_t theta_val;
	data_t pi = 3.14159265358979;
	for(int i =0 ; i< 10 ; i++)
	{
		x_rand = rand_val();
		y_rand = rand_val();
		r_val  = radius(x_rand, y_rand);
		theta_val  = theta_calc(x_rand , y_rand);
		run_test(x_rand, y_rand, theta_val, r_val);
		run_test(-x_rand, y_rand, pi - theta_val, r_val);
		run_test(x_rand, -y_rand, -theta_val, r_val);
		run_test(-x_rand, -y_rand, theta_val - pi, r_val);
	}

	for(int i =0 ; i< 10 ; i++)
	{
		for(int j=0 ; j<10; j++)
		{
			if(i>0 || j>0)
			{
				x_rand = i/10.0;
				y_rand = j/10.0;
				r_val  = radius(x_rand, y_rand);
				theta_val  = theta_calc(x_rand , y_rand);
				run_test(x_rand, y_rand, theta_val, r_val);
				run_test(-x_rand, y_rand, pi - theta_val, r_val);
				if(j!=0)
				{
					run_test(x_rand, -y_rand, -theta_val, r_val);
					run_test(-x_rand, -y_rand, theta_val - pi, r_val);
				}
			}
		}

	}



	printf("---RMS error----------------------------------\n");
	printf("----------------------------------------------\n");
	printf("   RMSE(R)           RMSE(Theta)\n");
	printf("%0.15f %0.15f\n", rmse_r.error, rmse_theta.error);
	printf("----------------------------------------------\n");


	float error_threshold = 0.001;

    int success = (rmse_r.error < error_threshold) && (rmse_theta.error < error_threshold);

    if (success) return 0;
    else return 1;

}
