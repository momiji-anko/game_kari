#pragma once
#include"pch.h"
#include<SimpleMath.h>

//‹…‚Ì\‘¢‘Ì
struct Sphere
{
	DirectX::SimpleMath::Vector3 centerPosition;
	float radius;
};

class SphereCollision
{
public:
	SphereCollision() = default;
	~SphereCollision() = default;

	/// <summary>
	/// ‹…‚Æ‹…‚Ì“–‚½‚è”»’è
	/// </summary>
	/// <param name="sphere1">ˆê‚Â–Ú‚Ì‹…</param>
	/// <param name="sphere2">“ñ‚Â–Ú‚Ì‹…</param>
	/// <returns>true= “–‚½‚Á‚½Afalse=“–‚Á‚Ä‚¢‚È‚¢</returns>
	static bool DetectCollition(Sphere sphere1, Sphere sphere2)
	{
		float lengthSquared = (sphere1.centerPosition - sphere2.centerPosition).LengthSquared();

		float radiusSquared = (sphere1.radius * sphere1.radius) + (sphere2.radius * sphere2.radius);

		if (lengthSquared < radiusSquared)
		{
			return true;
		}

		return false;
	}
};