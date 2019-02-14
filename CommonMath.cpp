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

double Distance::CalculateDistanceBetweenPoints(Coordinates a, Coordinates b)
{
	double distance_x = std::abs(a.x - b.x);
	double distance_y = std::abs(a.y - b.y);
	double distance = sqrt(pow(distance_x,2) + pow(distance_y,2));
	/*printf("Calculated distance between x1: %f y1: %f and x2: %f y2: %f is %f.\n",
		a.x, a.y, b.x, b.y, distance);*/
	return distance;
}

bool Collisions::IsThisPointInsideRectangle(Coordinates point, PreciseRect rectangle)
{
    bool result = false;

	if (point.x > rectangle.x && point.x < rectangle.x + rectangle.w
		&& point.y > rectangle.h && point.y < rectangle.h + rectangle.y)
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

	if (result == true)
	{
		//printf("Intersection detected!\nFirst segment: x1: %f y1: %f x2: %f y2: %f.\n", a.x, a.y, b.x, b.y );
		//printf("Second segment: x1: %f y1: %f x2: %f y2: %f.\n", c.x, c.y, d.x, d.y);
	}
	else
	{
		//printf("NO intersection detected!\nFirst segment: x1: %f y1: %f x2: %f y2: %f.\n", a.x, a.y, b.x, b.y);
		//printf("Second segment: x1: %f y1: %f x2: %f y2: %f.\n", c.x, c.y, d.x, d.y);
	}

	return result;
}