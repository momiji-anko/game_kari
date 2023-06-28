/*
* 2023/05/24
* FileLoadManager.cpp
* �t�@�C���̓ǂݍ��݂���N���X
* �����@��
*/
#include"pch.h"
#include"FileLoadManager.h"
#include<windows.h>

/// <summary>
/// �t�@�C���ǂݍ���
/// </summary>
/// <param name="fileName">�ǂݍ��ރt�@�C���p�X</param>
/// <returns>�ǂݍ��񂾃t�@�C��</returns>
std::vector<std::wstring> FileLoadManager::LoadFile(std::wstring fileName)
{
	if (m_fileNames[fileName].size() == 0)
	{
		//�t�@�C�����
		WIN32_FIND_DATA fileFindData;
		//�A�X�^���X�N
		std::wstring asterisk = L"*";

		//�t�@�C������
		const HANDLE hFindFile = FindFirstFile((fileName + asterisk).c_str(), &fileFindData);

		//�t�@�C���������ł�����
		if (hFindFile != INVALID_HANDLE_VALUE)
		{

			do
			{
				//�t�@�C�����f�B���N�g���ł͂Ȃ��ꍇ
				if (!(fileFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					//���������t�@�C���̖��O
					std::wstring findFileName = fileFindData.cFileName;
					//�t�@�C�p�X�ƌ��������t�@�C���𑫂�
					std::wstring jsonFilePath = fileName + findFileName;
					//�X�e�[�WJson�ɒǉ�
					m_fileNames[fileName].push_back(jsonFilePath);
				}

				//���̃t�@�C�����������烋�[�v
			} while (FindNextFile(hFindFile, &fileFindData));

		}
	}

	//�ǂݍ��񂾃t�@�C����Ԃ�
	return m_fileNames[fileName];
}