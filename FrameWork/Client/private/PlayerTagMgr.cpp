#include "stdafx.h"
#include "..\public\PlayerTagMgr.h"
#include "GameInstance.h"
#include "CameraTargetBack.h"
#include "Camera2D.h"

IMPLEMENT_SINGLETON(CPlayerTagMgr)

CPlayerTagMgr::CPlayerTagMgr()
{
	m_eCurType = CPlayer::TYPE_WARRIOR;
	ZeroMemory(m_pPlayerArr, sizeof(m_pPlayerArr));

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	m_pSubjectTagPlayer = dynamic_cast<CSubject*>(pGameInstance->Clone_Component(LEVEL_STATIC, pTagPrototypeSubject));
	if (m_pSubjectTagPlayer == nullptr)
		MSGBOX("CPlayerTagMgr::CPlayerTagMgr - m_pSubjectTagPlayer == nullptr");
	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CPlayerTagMgr::Set_Player(CPlayer::TYPE ePlayerType, CPlayer * pPlayer)
{
	if (ePlayerType == CPlayer::TYPE_END)
		MSG_FAIL("CPlayerTagMgr::Set_Player - ePlayerType == CPlayer::TYPE_END");

	if (pPlayer == nullptr)
		MSG_FAIL("CPlayerTagMgr::Set_Player - pPlayer == nullptr");

	Safe_AddRef(pPlayer);
	m_pPlayerArr[ePlayerType] = pPlayer;

	return S_OK;
}

void CPlayerTagMgr::Set_Changing(_bool bChanging)
{
	m_bChanging = bChanging;

	//카메라 현재 위치로 고정
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (pGameInstance->Get_CurrentCameraID() == CAMERA_BACK)
	{
		CCameraTargetBack* pCamera = dynamic_cast<CCameraTargetBack*>(pGameInstance->Get_CurrentCamera());
		if (pCamera == nullptr)
			MSG_VOID("CPlayerTagMgr::Change_Player - pCamera == nullptr");
		pCamera->Set_FixCam(m_bChanging);
	}
	if (pGameInstance->Get_CurrentCameraID() == CAMERA_2D)
	{
		CCamera2D* pCamera = dynamic_cast<CCamera2D*>(pGameInstance->Get_CurrentCamera());
		if (pCamera == nullptr)
			MSG_VOID("CPlayerTagMgr::Change_Player 2D- pCamera == nullptr");
		pCamera->Set_FixCam(m_bChanging);
	}
	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CPlayerTagMgr::Subscribe_ChangePlayer(CObserver * pObserver)
{
	if (m_pSubjectTagPlayer == nullptr)
		MSG_FAIL("CPlayerTagMgr::SubScribe_ChangePlayer - m_pSubjectTagPlayer == nullptr");

	return m_pSubjectTagPlayer->Subscribe(pObserver);
}

HRESULT CPlayerTagMgr::UnSubscribe_ChangePlayer(CObserver * pObserver)
{
	if (m_pSubjectTagPlayer == nullptr)
		MSG_FAIL("CPlayerTagMgr::UnSubScribe_ChangePlayer - m_pSubjectTagPlayer == nullptr");

	return m_pSubjectTagPlayer->UnSubscribe(pObserver);
}

CTransform * CPlayerTagMgr::Get_CurPlayerTransform()
{
	if (m_eCurType == CPlayer::TYPE_END)
		MSG_NULL("CPlayerTagMgr::Get_CurPlayerTransform - m_eCurType == CPlayer::TYPE_END");

	if (m_pPlayerArr[m_eCurType] == nullptr)
		MSG_NULL("CPlayerTagMgr::Get_CurPlayerTransform - m_pPlayerArr[m_eCurType] == nullptr");

	return dynamic_cast<CTransform*>(m_pPlayerArr[m_eCurType]->Get_Component(pTagComTransform));
}

HRESULT CPlayerTagMgr::Change_Player(CPlayer::TYPE ePlayerType)
{
	//진행 중
	if (m_bChanging)
		return S_OK;
	//쿨타임 아직 안돔
	if (CPlayer::m_fTagCoolTime < m_fMaxTagCooltime)
		return S_OK;

	if (ePlayerType == CPlayer::TYPE_END)
		MSG_FAIL("CPlayerTagMgr::Change_Player - ePlayerType == CPlayer::TYPE_END");

	if (m_pPlayerArr[ePlayerType] == nullptr)
		MSG_FAIL("CPlayerTagMgr::Change_Player - m_pPlayerArr[ePlayerType] == nullptr");

	//현재 타입과 같은 타입이 들어오는 경우(플레이 중에 잘못 누를 수도 있기 때문에 그냥 메시지 리턴은 안하겠음)
	if (m_eCurType == ePlayerType)
		return S_OK;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_eCurType = ePlayerType;

	for (size_t i = 0; i < CPlayer::TYPE_END; ++i)
	{
		if (m_pPlayerArr[i] != nullptr)
			m_pPlayerArr[i]->Set_Control(false);
	}
	m_pPlayerArr[m_eCurType]->Set_Control(true);

	m_pSubjectTagPlayer->Notify(&ePlayerType);

	Set_Changing(true);
	
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CPlayerTagMgr::Free()
{
	for (auto& pPlayer : m_pPlayerArr)
		Safe_Release(pPlayer);

	Safe_Release(m_pSubjectTagPlayer);
}
