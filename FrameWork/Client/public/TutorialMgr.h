#pragma once
#ifndef __CLIENT_TUTORIAL_MGR_H__
#define __CLIENT_TUTORIAL_MGR_H__

#include "Base.h"
#include "Dialog.h"

BEGIN(Engine)
END

BEGIN(Client)
class CTutorialMgr final:
	public CBase
{
	DECLARE_SINGLETON(CTutorialMgr)

private:
	explicit CTutorialMgr();
	virtual ~CTutorialMgr();

public:
	enum TUTORIAL { TUTORIAL_WET, TUTORIAL_MAGMA, TUTORIAL_BOSS_SKELETON, TUTOTRIAL_BREAK
		,TUTORIAL_CLWON_DODGE, TUTORIAL_CIRCUS_CAMERA, TUTORIAL_CLOWN_TELEKINESIS, TUTORIAL_FALL,TUTORIAL_END};

	typedef struct tagTutorialMgrDesc {
		list<CDialog::DESC>	listDialogDesc;
		_bool				bEnd = false;
	} DESC;
	
public:
	HRESULT SetUp_Mgr();
	void	Set_Tutorial(TUTORIAL eTuto);
	void	Set_TutorialToolTip(TUTORIAL eTuto);

private:
	void	Set_Dialog(TUTORIAL eTuto);

	//텍스트, 이미지 둘 다 월드좌표계로 좌표 입력(직교투영 신경쓰지말고 안에서 처리해줌, y기본으로 위에서 떨어지는걸로 생각해서 짬)
	void	Create_TipObject(_bool bBlend, LEVEL eTextureLevel, const _tchar* pTagPrototypeVIBuffer, _uint iTextureIndex, const _tchar* pTextureName,
		_vec3 vPos, _vec3 vScale, TUTORIAL eTuto, _bool bText = false, D3DXCOLOR dwColor = 0xffffffff, const _tchar* pText = nullptr);
	
	void	Create_TutorialWet();
	void	Create_TutorialMagma();
	void	Create_TutorialBreak();
	void	Create_TutorialBossSkeleton();

	void	Create_TutorialClown();
	void	Create_TutorialCamera();
	void	Create_TutorialTelekinesis();
	void	Create_TutorialFall();
private:
	DESC	m_tTutoArr[TUTORIAL_END];

	list<class CTipObject*> m_listTipObjArr[TUTORIAL_END];

public:
	virtual void Free() override;
};
END

#endif // !__CLIENT_TUTORIAL_MGR_H__
