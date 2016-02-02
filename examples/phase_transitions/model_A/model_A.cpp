// model_A.cpp
// Algorithms for 2D and 3D implementation of "model A"
// Questions/comments to gruberja@gmail.com (Jason Gruber)

#ifndef MODELA_UPDATE
#define MODELA_UPDATE
#include"MMSP.hpp"
#include<cmath>
#include"model_A.hpp"

namespace MMSP{

double gaussian(double ave, double std)
{
	static double u = 0;
	static double v = 0;
	static bool saved = false;

	if (not saved) {
		start:
		double x = 2.0*double(rand())/double(RAND_MAX)-1.0;
		double y = 2.0*double(rand())/double(RAND_MAX)-1.0;

		double r = x*x+y*y;
		if (r<=0.0 or r>1.0) goto start;
		double d = sqrt(-2.0*log(r)/r);

		u = d*x;
		v = d*y;

		saved = true;
		return ave+u*std;
	}
	else {
		saved = false;
		return ave+v*std;
	}
}

void generate(int dim, const char* filename)
{
	if (dim==1) {
		GRID1D initGrid(1,0,128);

		for (int i=0; i<nodes(initGrid); i++)
			initGrid(i) = 0.0;

		output(initGrid,filename);
	}

	if (dim==2) {
		GRID2D initGrid(1,0,128,0,128);

		for (int i=0; i<nodes(initGrid); i++)
			initGrid(i) = 0.0;

		output(initGrid,filename);
	}

	if (dim==3) {
		GRID3D initGrid(1,0,64,0,64,0,64);

		for (int i=0; i<nodes(initGrid); i++)
			initGrid(i) = 0.0;

		output(initGrid,filename);
	}
}

template <int dim, typename T> void update(grid<dim,T>& oldGrid, int steps)
{
	grid<dim,T> newGrid(oldGrid);

	T r = 1.0;
	T u = 1.0;
	T K = 1.0;
	T M = 1.0;
	T dt = 0.01;
	T kT = 0.01;
	T dV = 1.0;

	for (int step=0; step<steps; step++) {
		for (int i=0; i<nodes(oldGrid); i++) {
			T phi = oldGrid(i);
			T noise = gaussian(0.0,sqrt(2.0*kT/(dt*dV)));
			newGrid(i) = phi-dt*M*(-r*phi+u*pow(phi,3)-K*laplacian(oldGrid,i)+noise);
		}
		swap(oldGrid,newGrid);
		ghostswap(oldGrid);
	}
}

} // namespace MMSP

#endif

#include"MMSP.main.hpp"
