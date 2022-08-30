#pragma once
#ifndef __CLIENT_LOADING_DAT_FILE_H__
#define __CLIENT_LOADING_DAT_FILE_H__

BEGIN(Engine)
class CGameObject;
END
#include "Base.h"
BEGIN(Client)

class CLoadingDatFile final:
	public CBase
{
	DECLARE_SINGLETON(CLoadingDatFile)

private:
	explicit CLoadingDatFile();
	virtual ~CLoadingDatFile() = default;

public:
	//레디 매니저 - 메인앱에서 불러주면 됨
	HRESULT Ready_LoadingDatFile();
	//해당 씬의 벡터 사이즈 받아가기
	size_t Get_VectorSize(wstring wstrSceneName);
	//오브젝트 인포 받아가기
	const OBJ_INFO* Get_ObjInfo(wstring wstrSceneName, _uint iIndex) const;


private:
	map<wstring, vector<OBJ_INFO*>>			m_mapScene;
	typedef map<wstring, vector<OBJ_INFO*>>	SCENES;

public:
	virtual void Free() override;
};

END
#endif // !__CLIENT_LOADING_DAT_FILE_H__
