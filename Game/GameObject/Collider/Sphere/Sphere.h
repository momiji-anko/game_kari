#pragma once
#include"pch.h"
#include<SimpleMath.h>

//球の構造体
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
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="sphere1">一つ目の球</param>
	/// <param name="sphere2">二つ目の球</param>
	/// <returns>true= 当たった、false=当っていない</returns>
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