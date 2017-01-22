#include <cstdio>

void UpdateWave(const double timeInterval, double* x, double* speed)
{
	(*x) += timeInterval * (*speed);
}

int main()
{
	double x = 0.0;
	double y = 0.0;
	double speedX = 1.0;
	double speedY = -0.5;

	const int fps = 100;
	const double timeInterval = 1.0 / fps;

	for (int i = 0; i < 1000; ++i)
	{
		// Update waves
		UpdateWave(timeInterval, &x, &speedX);
	}

	return 0;
}