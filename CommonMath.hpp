#ifndef COMMON_MATH_HPP
#define COMMON_MATH_HPP
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <CustomDefinedTypes.hpp>

struct MathVectorValue 
{
	double x = 0;
	double y = 0;
};

class MathVector
{
    private:

		double start_x = 0;
		double start_y = 0;
		double end_x = 0;
		double end_y = 0;
		double value_x = 0;
		double value_y = 0;

    public:

		MathVector(double my_start_x, double my_start_y, double my_end_x, double my_end_y);
		double TellStartingX();
		double TellStartingY();
		MathVectorValue TellValue();
		double TellLength();
		//MathVector operator* (const MathVector a);
		//MathVector operator+ (const MathVector a);
		static double ScalarProduct(MathVector a, MathVector b);
		static double TellRadianAngleBetweenVectors(MathVector a, MathVector b);
};

class Angle
{
    private:
		//Guess what
		static double PI;
    public:
		static double DegreeToRadian(int angle_degree);
		static int RadianToDegree(double angle_radian);
		static int NormalizeAngle(int angle);
};

class Distance
{
    private:

    public:
		static double CalculateDistanceBetweenPoints(Coordinates a, Coordinates b);
};

#endif // COMMON_MATH_HPP

