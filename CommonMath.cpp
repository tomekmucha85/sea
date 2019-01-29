#include <CommonMath.hpp>

double Angle::PI = 3.14159265;

MathVector::MathVector(double my_start_x, double my_start_y, double my_end_x, double my_end_y)
{
	start_x = my_start_x;
	start_y = my_start_y;
	value_x = my_end_x - start_x;
	value_y = my_end_y - start_y;
}

MathVectorValue MathVector::TellValue()
{
	return {value_x, value_y};
}

double MathVector::TellStartingX()
{
	return start_x;
}

double MathVector::TellStartingY()
{
	return start_y;
}

double MathVector::TellLength()
{
	return sqrt(pow(value_x, 2) + pow(value_y, 2));
}

//MathVector operator* (const MathVector a)
//{
//	MathVector result = { 0,0,0,0 };
//	return result;
//}

//MathVector MathVector::operator+ (const MathVector a)
//{
//	MathVector result = {0,0,0,0};
//	double result_x = a.start_x;
//	return result;
//}

//#################################
//STATIC METHODS
//#################################

double MathVector::ScalarProduct(MathVector a, MathVector b)
{
	MathVectorValue a_value = a.TellValue();
	MathVectorValue b_value = b.TellValue();
	return (a_value.x*b_value.x + a_value.y*b_value.y);
}

double MathVector::TellRadianAngleBetweenVectors(MathVector a, MathVector b)
{
	double cosinus_between_vectors = ScalarProduct(a,b) / (a.TellLength() * b.TellLength());
	double angle = acos(cosinus_between_vectors);
	return acos(cosinus_between_vectors);
}

double Angle::DegreeToRadian(int angle_degree)
{
	double angle_radian = (angle_degree * PI) / 180;
	return angle_radian;
}

int Angle::RadianToDegree(double angle_radian)
{
	int angle_degree = static_cast<int>((angle_radian * 180) / PI);
	return angle_degree;
}

//Preventing angle from exceeding 360 degrees

int Angle::NormalizeAngle(int angle)
{
	if (angle < 0)
	{
		angle = 360 + angle;
	}
	else if (angle >= 360)
	{
		angle = 360 - angle;
	}
	else if (angle >= 2 * 360 || angle <= -2 * 360)
	{
		printf("Angle exceeds 360 twice! Something went wrong.\n");
		throw std::invalid_argument("Angle outside acceptable range.\n");
	}
	else
	{
		;
	}
	return angle;
}

double Distance::CalculateDistanceBetweenPoints(Coordinates a, Coordinates b)
{
	double distance_x = std::abs(a.x - b.x);
	double distance_y = std::abs(a.y - b.y);
	double distance = sqrt(pow(distance_x,2) + pow(distance_y,2));
	return distance;
}