#pragma once
#include"pch.h"
#include<SimpleMath.h>

//���̍\����
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
	/// ���Ƌ��̓����蔻��
	/// </summary>
	/// <param name="sphere1">��ڂ̋�</param>
	/// <param name="sphere2">��ڂ̋�</param>
	/// <returns>true= ���������Afalse=�����Ă��Ȃ�</returns>
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