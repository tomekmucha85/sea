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

/*MathVector operator* (const MathVector& a, const MathVector& b)
{
	double result = 0;


	return result;
}*/

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

double MathVector::CrossProductValue(MathVector a, MathVector b)
{
	return (a.TellValue().x*a.TellValue().y - b.TellValue().x*a.TellValue().y);
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

int Angle::CalculateAngleBetweenNorthVectorAndVectorGoingThroughTwoPoints(Coordinates point_a,
	Coordinates point_b)
{
	//Vector pointing north
	MathVector* ptr_vector_pointing_north = new MathVector(0, 0, 0, 0 - 100);
	//Vector pointing towards given point
	MathVector* ptr_vector_pointing_from_point_a_towards_point_b = 
		new MathVector(point_a.x, point_a.y, point_b.x, point_b.y);

	double angle_between_vectors_radian = MathVector::TellRadianAngleBetweenVectors(*ptr_vector_pointing_north, *ptr_vector_pointing_from_point_a_towards_point_b);
	int angle_between_vectors_degrees = Angle::RadianToDegree(angle_between_vectors_radian);

	//Calculate if angle should be positive or negative
	int real_angle_between_vectors_degrees = 0;
	if (ptr_vector_pointing_from_point_a_towards_point_b->TellValue().x <= 0)
	{
		real_angle_between_vectors_degrees = 360 - angle_between_vectors_degrees;
	}
	else
	{
		real_angle_between_vectors_degrees = angle_between_vectors_degrees;
	}

	//Cleanup
	delete ptr_vector_pointing_north;
    delete ptr_vector_pointing_from_point_a_towards_point_b;

	return real_angle_between_vectors_degrees;
}

double Distance::CalculateDistanceBetweenPoints(Coordinates a, Coordinates b)
{
	double distance_x = std::abs(a.x - b.x);
	double distance_y = std::abs(a.y - b.y);
	double distance = sqrt(pow(distance_x,2) + pow(distance_y,2));
	/*printf("Calculated distance between x1: %f y1: %f and x2: %f y2: %f is %f.\n",
		a.x, a.y, b.x, b.y, distance);*/
	return distance;
}

Coordinates Distance::CalculatePointInGivenDistanceAndAngleFromNorthPointingVectorFromGivenPoint(Coordinates my_point, double distance, int angle_degrees)
{
	/*

Notice, that the grid is as following:

315degrees     45 degrees
		  \	| /
		   \|/
	------------------->X
			|0,0
			|
			V
			Y
	*/
	double angle_radian = Angle::DegreeToRadian(angle_degrees);
	Coordinates result = { 0,0 };
	result.x = my_point.x + (sin(angle_radian) * distance);
	result.y = my_point.y + (cos(angle_radian) * distance * -1);
	return result;
}

bool Collisions::IsThisPointInsideRectangle(Coordinates point, PreciseRect rectangle)
{
    bool result = false;

	if (point.x >= rectangle.x && point.x <= rectangle.x + rectangle.w
		&& point.y >= rectangle.y && point.y <= rectangle.h + rectangle.y)
	{
		result = true;
	}

	return result;
}

bool Collisions::DoTheseRectanglesOverlap(PreciseRect a, PreciseRect b)
{
	bool result = true;

	if (a.x > b.x + b.w || a.x + a.w < b.x)
	{
		result = false;
	}
	else if (a.y > b.y + b.h || a.y + a.h < b.y)
	{
		result = false;
	}

	return result;
}

bool Collisions::DoesSegmentIntersectRectangle(Coordinates segment_start, Coordinates segment_end, PreciseRect rectangle)
{

	bool result = false;

	Coordinates rectangle_diagonals[2][2] = 
	{
		{{0,0},{0,0}},
		{{0,0},{0,0}},
	};

	/*a ______________ b
	   |              |
	   |              |
	 c ---------------- d

	*/
	
	//Simplification - only diagonals are taken into account

	//Diagonal a - d
	rectangle_diagonals[0][0] = { rectangle.x, rectangle.y };
	rectangle_diagonals[0][1] = { rectangle.x + rectangle.w, rectangle.y + rectangle.h };
	//Diagonal c - b
	rectangle_diagonals[1][0] = { rectangle.x, rectangle.y + rectangle.h };
	rectangle_diagonals[1][1] = { rectangle.x + rectangle.w, rectangle.y};


	for (int i = 0; i < 2; i++)
	{
		if (Segment::DoSegmentsIntersect(rectangle_diagonals[i][0], rectangle_diagonals[i][1],
			segment_start, segment_end))
		{
			result = true;
			return result;
		}
	}


	return result;

}

double Segment::CalculateDetFromMatrix(Coordinates a, Coordinates b, Coordinates c)
{
	double c_a_x = c.x - a.x;
	double c_a_y = c.y - a.y;
	double b_a_x = b.x - a.x;
	double b_a_y = b.y - a.y;
	return c_a_x * b_a_y - b_a_x * c_a_y;
}

bool Segment::CheckIfEndOfOneSegmentIsPlacedOnAnotherSegment(Coordinates a, Coordinates b, Coordinates c)
{
	//a and b are ends of first segment
	//c is one of the second segment ends
	bool result = std::min(a.x, b.x) <= c.x &&
		c.x <= std::max(a.x,b.x) &&
		std::min(a.y, b.y) <= c.y &&
		c.y <= std::max(a.y, b.y);
	return result;
}

bool Segment::DoSegmentsIntersect(Coordinates a, Coordinates b, Coordinates c, Coordinates d)
{
	bool result = false;

	double v1 = CalculateDetFromMatrix(a,b,c);
	double v2 = CalculateDetFromMatrix(a,b,d);
	double v3 = CalculateDetFromMatrix(c,d,a);
	double v4 = CalculateDetFromMatrix(c,d,b);

	//printf("v1: %f, v2: %f, v3: %f, v4: %f, result: %d.\n", v1, v2, v3, v4, result);


	//Do they intersect?
	if ( ((v1 < 0 && v2>0) || (v1 > 0 && v2 < 0)) && ((v3 < 0 && v4>0) || (v3 > 0 && v4 < 0)) )
	{
		//printf("They intersect.\n");
		result = true;
	}

	//Is end of one segment placed on another segment?
	if (v1 == 0 && CheckIfEndOfOneSegmentIsPlacedOnAnotherSegment(c, d, a))
	{
		result = true;
	}
	else if (v2 == 0 && CheckIfEndOfOneSegmentIsPlacedOnAnotherSegment(c, d, b))
	{
		result = true;
	}
	else if (v3 == 0 && CheckIfEndOfOneSegmentIsPlacedOnAnotherSegment(a, b, c))
	{
		result = true;
	}
	else if (v4 == 0 && CheckIfEndOfOneSegmentIsPlacedOnAnotherSegment(a, b, d))
	{
		result = true;
	}

	return result;
}

double  MathLine::CalculateSlope(Coordinates a, Coordinates b)
{
	double result = (b.y - a.y) / (b.x - a.x);
	return result;
}

double MathLine::CalculateIntercept(Coordinates a, Coordinates b)
{
	double intercept = CalculateSlope(a, b);
	return (a.y - intercept * a.x);
}

MathLineParams MathLine::CalculateLineParams(Coordinates point_a, Coordinates point_b)
{
	MathLineParams result = { 0,0 };
	result.slope = CalculateSlope(point_a, point_b);
	result.intercept = CalculateIntercept(point_a, point_b);
	return result;
}

MathLineParams MathLine::CalculateParamsForOrthogonalLineCrossingGivenPoint(MathLineParams base_line, Coordinates point)
{
	MathLineParams result = { 0 , 0 };
	double slope = 1 / (base_line.slope*-1);
	double intercept = point.y - slope*point.x;
	result = { slope, intercept };
	return result;
}

std::pair<double, double> MathLine::CalculateXesForPointLyingOnGivenLineInGivenDistanceFromGivenX(MathLineParams line, double distance, double starting_x)
{
	double first_value = sqrt(pow(distance, 2) / (pow(line.slope, 2) + 1)) + starting_x;
	double second_value = -sqrt(pow(distance, 2) / (pow(line.slope, 2) + 1)) + starting_x;
	return { first_value, second_value };
}

double MathLine::CalculateYForGivenX(MathLineParams my_params, double my_x)
{
	return (my_params.slope*my_x + my_params.intercept);
}