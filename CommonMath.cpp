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