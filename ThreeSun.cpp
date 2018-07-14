#include "ThreeSun.cpp"

//----------------------------------------------------------------------------------------------------
vectors::vectors()
{
	x = 0;
	y = 0;
	z = 0;
}

vectors::vectors(const vectors &other)
{
	x = other.x;
	y = other.y;
	z = other.z;
}

vectors::vectors(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}


vectors operator+(const vectors & one, const vectors & two)
{
	double x = one.x + two.x;
	double y = one.y + two.y;
	double z = one.z + two.z;
	return vectors(x, y, z);
}

vectors operator-(const vectors &one, const vectors &two)
{
	double x = one.x - two.x;
	double y = one.y - two.y;
	double z = one.z - two.z;
	return vectors(x, y, z);
}

vectors operator*(int a, const vectors& v)
{
	double x = a*v.x;
	double y = a*v.y;
	double z = a*v.z;
	return vectors(x, y, z);
}

vectors operator*(const vectors &v, double a)
{
	double x = a*v.x;
	double y = a*v.y;
	double z = a*v.z;
	return vectors(x, y, z);
}

ostream  &operator<<(ostream &output, sun &s)
{
	output << "位置为: " << s.location.x << "," << s.location.y << "," << s.location.z << endl;
	return output;
}

void vectors::single()
{
	double model = sqrt(x*x + y*y + z*z);
	x = x / model;
	y = y / model;
	z = z / model;
}

//-------------------------------------------------------------------------------------------------------
sun::sun(double m, vectors x, vectors v)
	:location(x),velocity(v)
{
	this->m = m;
}

sun::sun(double m)
{
	vectors x(0, 0, 0);
	location = x;
	velocity = x;
	this->m = m;
}

sun::sun(const sun &other)
{
	location = other.location;
	velocity = other.velocity;
	accelerrate = other.accelerrate;
	m = other.m;
}

sun::sun()
{
	m = 1;
}

double sun::light()
{
	double R2 = location.x*location.x + location.y*location.y + location.z*location.z;
	return 1/R2;
}

//------------------------------------------------------------------------------------------------------
imitate::imitate(sun sun1, sun sun2,sun sun3)
	:prevSunA(sun1),prevSunB(sun2),prevSunC(sun3),
	sunA(sun1.m),sunB(sun2.m),sunC(sun3.m),
	nextSunA(sun1.m), nextSunB(sun2.m), nextSunC(sun3.m)
{
	G = 1;
	intervalTime = 0.1;
	totalTime = 1000;
}

imitate::imitate()
{
	G = 1;
	intervalTime = 0.01;
	totalTime = 100;
}

double imitate::calculateR(const sun &sun1,const sun &sun2)
{
	double R = sqrt(pow((sun1.location.x - sun2.location.x), 2) + pow((sun1.location.y - sun2.location.y), 2)
		+ pow((sun1.location.z - sun2.location.z), 2));
	return R;
}

vectors imitate::calculateAcc(const sun &sun1, const sun &sun2)
{
	double R = calculateR(sun1, sun2);
	double a = G*sun2.m / R / R;
	vectors direction = sun2.location - sun1.location;
	direction.single();

	return vectors(direction.x*a, direction.y*a , direction.z*a);
}

void imitate::getAllAcc()
{
	vectors ACCab = calculateAcc(sunA, sunB);
	vectors ACCac = calculateAcc(sunA, sunC);
	vectors ACCbc = calculateAcc(sunB, sunC);
	vectors ACCba = calculateAcc(sunB, sunA);
	vectors ACCca = calculateAcc(sunC, sunA);
	vectors ACCcb = calculateAcc(sunC, sunB);
	sunA.accelerrate = ACCab + ACCac;
	sunB.accelerrate = ACCba + ACCbc;
	sunC.accelerrate = ACCca + ACCcb;
}

bool imitate::ifCrash()
{
	double Rab = calculateR(sunA, sunB);
	double Rac = calculateR(sunA, sunC);
	double Rbc = calculateR(sunB, sunC);
	if (Rab <= (sunA.m + sunB.m)/10 || Rac <= (sunA.m + sunC.m)/10 || Rbc <= (sunB.m + sunC.m)/10)return true;
	else return false;
}


void imitate::totalImitate()
{
	sunA.location = prevSunA.velocity*intervalTime + prevSunA.location;
	sunB.location = prevSunB.velocity*intervalTime + prevSunB.location;
	sunC.location = prevSunC.velocity*intervalTime + prevSunC.location;
	int times = totalTime / intervalTime;
	for (size_t i = 1; i <= times; i++)
	{
		getAllAcc();
		nextSunA.location = 2 * sunA.location - prevSunA.location + sunA.accelerrate*(intervalTime*intervalTime);
		nextSunB.location = 2 * sunB.location - prevSunB.location + sunB.accelerrate*(intervalTime*intervalTime);
		nextSunC.location = 2 * sunC.location - prevSunC.location + sunC.accelerrate*(intervalTime*intervalTime);
		prevSunA = sunA;
		prevSunB = sunB;
		prevSunC = sunC;
		sunA = nextSunA;
		sunB = nextSunB;
		sunC = nextSunC;
		if (ifCrash())
		{
			cout << "crash!!!" << endl;
			break;
		}
		Sleep(100);
		cout << sunA << sunB << sunC;
	}
}
