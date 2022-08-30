#pragma once
#ifndef __CLIENT_SOUND_MGR_H__
#define __CLIENT_SOUND_MGR_H__

#include "fmod.h"

class CSoundMgr
{
public:
	static CSoundMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CSoundMgr; 

		return m_pInstance; 
	}
	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance; 
			m_pInstance = nullptr; 
		}
	}
public:
	enum CHANNELID {BGM, EFFECT
		,NULL1
		,NULL2
		,MAGE_SKILL, MAGE_MAGIC_MISSILE
		,THIEF_SKILL, THIEF_SKILL_POISONCUBE, THIEF_SKILL_POISONCUBE_BREAK
		,RAIN
		,CLOWN_INTRO, CLOWN_NEWACTOR, CLOWN_DODGE, CLOWN_ATTACK, CLOWN_DEAD
		,BOSS_SKELETON, BOSS_SKELETON_STAGE_CUBE
		,MAXCHANNEL};

private:
	CSoundMgr();
	~CSoundMgr();

public:
	void Initialize(); 

	void Release(); 
public:
	void PlaySound(TCHAR* pSoundKey, CHANNELID eID);
	void PlayBGM(TCHAR* pSoundKey);
	void StopSound(CHANNELID eID);
	void StopAll();
	void StopPlaySound(TCHAR* pSoundKey, CHANNELID eID);
	void PlayLoopSound(TCHAR* pSoundKey, CHANNELID eID);
	void PauseBGM(bool _bPause);

private:
	void LoadSoundFile(); 

private:
	static CSoundMgr* m_pInstance; 
	// ���� ���ҽ� ������ ���� ��ü 
	map<TCHAR*, FMOD_SOUND*> m_mapSound; 
	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL]; 
	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM* m_pSystem; 

};

#endif // !__CLIENT_SOUND_MGR_H__
