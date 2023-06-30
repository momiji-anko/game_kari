//
//MyRandom.cpp
//
#include"pch.h"
#include"MyRandom.h"

std::random_device MyRandom::s_rd;
std::mt19937 MyRandom::s_mt;

//===================================
//������
//===================================
void MyRandom::Initialize()
{
	MyRandom::Seed(s_rd());
}

//===================================
//�V�[�h�l���X�V
//===================================
void MyRandom::Seed(unsigned int seed)
{
	s_mt.seed(seed);
}

//===================================
//float�̒l���擾����
//===================================
float MyRandom::GetFloat()
{
	return GetFloatRange(0.0f, 1.0f);
}

//===================================
//�͈͂��w�肵�āAfloat�l���擾����
//===================================
float MyRandom::GetFloatRange(float min, float max)
{
	assert(min < max);
	std::uniform_real_distribution<float> urd(min, max);
	return urd(s_mt);
}

//===================================
//�͈͂��w�肵�āAint�l���擾����
//===================================
int MyRandom::GetIntRange(int min, int max)
{
	assert(min <= max);

	std::uniform_int_distribution<int> urd(min, max);
	return urd(s_mt);
}

//===================================
//�͈͂��w�肵�āAVector2�l���擾����
//===================================
DirectX::SimpleMath::Vector2 MyRandom::GetVector(const DirectX::SimpleMath::Vector2& min, const DirectX::SimpleMath::Vector2& max)
{
	DirectX::SimpleMath::Vector2 r = DirectX::SimpleMath::Vector2(GetFloat(), GetFloat());
	return min + (max - min) * r;
}
