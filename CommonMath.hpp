#ifndef COMMON_MATH_HPP
#define COMMON_MATH_HPP
#include <stdio.h>
#include <iostream>
#include <algorithm>
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
		static double CrossProductValue(MathVector a, MathVector b);
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
		static int CalculateAngleBetweenNorthVectorAndVectorGoingThroughTwoPoints(Coordinates point_a,
			Coordinates point_b);
};

class Distance
{
    private:

    public:
		static double CalculateDistanceBetweenPoints(Coordinates a, Coordinates b);
};

class Segment
{
    private:

    public:
		static bool CheckIfEndOfOneSegmentIsPlacedOnAnotherSegment(Coordinates a, Coordinates b, Coordinates c);
		static double CalculateDetFromMatrix(Coordinates a, Coordinates b, Coordinates c);
		static bool DoSegmentsIntersect(Coordinates a, Coordinates b, Coordinates c, Coordinates d);

};

class MathLine
{
    private:

    public:
		static double  CalculateSlope(Coordinates a, Coordinates b);
		static double CalculateIntercept(Coordinates a, Coordinates b);
		static MathLineParams CalculateLineParams(Coordinates point_a, Coordinates point_b);
		static MathLineParams CalculateParamsForOrthogonalLineCrossingGivenPoint(MathLineParams base_line, Coordinates point);
		static std::pair<double,double> CalculateXesForPointLyingOnGivenLineInGivenDistanceFromGivenX(MathLineParams line, double distance, double starting_x);
		static double CalculateYForGivenX(MathLineParams my_params, double my_x);
};

class Collisions
{
    private:

    public:
	   static bool IsThisPointInsideRectangle(Coordinates point, PreciseRect rectangle);
	   static bool DoTheseRectanglesOverlap(PreciseRect a, PreciseRect b);
	   static bool DoesSegmentIntersectRectangle(Coordinates segment_start, Coordinates segment_end, PreciseRect rectanlge);
};

#endif // COMMON_MATH_HPP

