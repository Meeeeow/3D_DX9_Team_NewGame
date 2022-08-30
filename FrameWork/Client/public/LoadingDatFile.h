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
	//���� �Ŵ��� - ���ξۿ��� �ҷ��ָ� ��
	HRESULT Ready_LoadingDatFile();
	//�ش� ���� ���� ������ �޾ư���
	size_t Get_VectorSize(wstring wstrSceneName);
	//������Ʈ ���� �޾ư���
	const OBJ_INFO* Get_ObjInfo(wstring wstrSceneName, _uint iIndex) const;


private:
	map<wstring, vector<OBJ_INFO*>>			m_mapScene;
	typedef map<wstring, vector<OBJ_INFO*>>	SCENES;

public:
	virtual void Free() override;
};

END
#endif // !__CLIENT_LOADING_DAT_FILE_H__
