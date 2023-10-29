#include"pch.h"

#include"ObstacleSpawner.h"
#include"ObstacleManager.h"
#include"Obstacle.h"
#include"DeviceResources.h"

#include"Game/GameContext/GameContext.h"
#include"Libraries/MyLibraries/ModelManager.h"
#include"Libraries/MyLibraries/Camera.h"

//��Q�������N�[���^�C��
const float ObstacleSpawner::OBSTACLE_SPAWN_COOL_TIME_S=5;


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="position">���W</param>
/// <param name="obstacleManager">��Q���}�l�[�W���[</param>
/// <param name="area">�͈�</param>
/// <param name="obstacleSpeed">��Q���̑��x</param>
/// <param name="obstacleScale">��Q���̊g�嗦</param>
/// <param name="obstacleRotation">��Q���̊p�x</param>
/// <param name="model">���f��</param>
ObstacleSpawner::ObstacleSpawner(
	DirectX::SimpleMath::Vector3 position,
	ObstacleManager* obstacleManager,
	float area,
	float obstacleSpeed,
	DirectX::SimpleMath::Vector3 obstacleScale, 
	DirectX::SimpleMath::Vector3 obstacleRotation,
	DirectX::Model* model
	)
	:
	Actor{
		position,
		DirectX::SimpleMath::Vector3::Zero,
		DirectX::SimpleMath::Vector3::One,
		DirectX::SimpleMath::Vector3::Zero,
		model,
		true
	},
	m_area{ area },
	m_obstacleManager{ obstacleManager },
	m_obstacleSpawnCoolTime_s{ OBSTACLE_SPAWN_COOL_TIME_S },
	m_sphere{ position,area }, 
	m_obstacleSpeed{obstacleSpeed},
	m_obstaceleScale{obstacleScale},
	m_obstacleRotation{obstacleRotation}
{
	
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ObstacleSpawner::~ObstacleSpawner()
{

}

/// <summary>
/// ������	
/// </summary>
void ObstacleSpawner::Initialize()
{
	//�G�t�F�N�g�쐬
	m_fireShaderManager = std::make_unique<FireEffectManager>();
	//�V�F�[�_�[����
	m_fireShaderManager->Create();
	//������
	m_fireShaderManager->Initialize(2.0f, DirectX::SimpleMath::Vector3::Zero);
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�^�C�}�[</param>
void ObstacleSpawner::Update(const DX::StepTimer& timer)
{
	//�A�N�e�B�u��Ԃł͂Ȃ�OR�v���C���[������ł��Ȃ���Ԃł���΍X�V���Ȃ�
	if (!IsActive() || GameContext::GetInstance().IsPlayerDeath())
		return;

	//�p�x
	DirectX::SimpleMath::Quaternion rotation = DirectX::SimpleMath::Quaternion(0,0,0,0);

	//�v���C���[���͈͓��ɂ��邩
	if (GameContext::GetInstance().GetCollisionManager()->DetectCollisionPlayerSphere2EnemySphere(&m_sphere))
	{
		//�v���C���[�ւ̃x�N�g��
		DirectX::SimpleMath::Vector3 playerVel = GetPosition() - GameContext::GetInstance().GetPlayerPosition();
		//��]
		rotation = DirectX::SimpleMath::Quaternion::FromToRotation(DirectX::SimpleMath::Vector3::Forward, DirectX::SimpleMath::Vector3(playerVel));
		//�N�[���^�C�������炷
		m_obstacleSpawnCoolTime_s -= timer.GetElapsedSeconds();
		//�N�[���^�C�����O�ɂȂ������Q���̂𐶐�����
		if (m_obstacleSpawnCoolTime_s < 0)
		{
			//��Q������
			CreateObstacle();
			//�N�[���^�C����
			m_obstacleSpawnCoolTime_s = OBSTACLE_SPAWN_COOL_TIME_S;
		}
	}
	//��]
	SetRotation(rotation);
	//�V�F�[�_�[�X�V
	FireUpdate(timer);
}

/// <summary>
/// �`��
/// </summary>
/// <param name="camera">�J�����̐��|�C���^</param>
void ObstacleSpawner::Render(const Camera* camera)
{
	//�f�o�C�X���\�[�X�̎擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//�f�o�C�X�R���e�L�X�g�̎擾
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//���[���h�s��v�Z
	CalculateWorldMatrix();
	//���f���`��
	GetModel()->Draw(context, *GameContext::GetInstance().GetCommonState(), GetWorldMatrix(), camera->GetViewMatrix(), camera->GetProjectionMatrix());

}

/// <summary>
/// �I������
/// </summary>
void ObstacleSpawner::Finalize()
{
	m_obstacleManager = nullptr;
}

/// <summary>
/// ���Z�b�g
/// </summary>
void ObstacleSpawner::Reset()
{
}

/// <summary>
/// ��Q���쐬
/// </summary>
void ObstacleSpawner::CreateObstacle()
{
	//��Q��
	std::unique_ptr<Actor> obstacle;
	//�v���C���[���W
	DirectX::SimpleMath::Vector3 playerPosition = GameContext::GetInstance().GetPlayerPosition();
	//�ړ�����
	DirectX::SimpleMath::Vector3 velocity = playerPosition - GetPosition();
	//���K��
	velocity.Normalize();
	//�X�s�[�h���|����
	velocity *= m_obstacleSpeed;

	//����
	obstacle = std::make_unique<Obstacle>(
		GetPosition(),
		velocity,
		m_obstaceleScale,
		m_obstacleRotation,
		ModelManager::GetInstance().LoadCmoModel(L"dice.cmo"),
		true,
		m_fireShaderManager.get()
		);

	//������
	obstacle->Initialize();

	//��Q���ǉ�
	m_obstacleManager->AddObstacle(std::move(obstacle));
}

/// <summary>
/// �G�t�F�N�g�̍X�V
/// </summary>
/// <param name="timer">�^�C�}�[</param>
void ObstacleSpawner::FireUpdate(const DX::StepTimer& timer)
{
	//�t�@�C���[�X�V��
	int fireUpdateNumber = 5;

	//�V�F�[�_�[�X�V
	//�X�V�񐔂𑝂₵�t�@�C���[���ۂ����Ă���
	for (int i = 0; i < fireUpdateNumber; i++)
	{
		m_fireShaderManager->Update(timer);
	}
}
