#pragma once
#ifndef __CLIENT_PLAYER_TAG_MGR_H__
#define __CLIENT_PLAYER_TAG_MGR_H__

#include "Player.h"

BEGIN(Engine)
class CSubject;
END
BEGIN(Client)
class CPlayerTagMgr :
	public CBase
{
	DECLARE_SINGLETON(CPlayerTagMgr)

private:
	explicit CPlayerTagMgr();
	virtual ~CPlayerTagMgr() = default;


public: //SET
	HRESULT		Set_Player(CPlayer::TYPE ePlayerType, CPlayer* pPlayer);
	void		Set_MaxTagCoolTime(_float fMaxTagCoolTime) { m_fMaxTagCooltime = fMaxTagCoolTime; }
	void		Set_Changing(_bool bChanging);

	HRESULT		Subscribe_ChangePlayer(class CObserver* pObserver);
	HRESULT		UnSubscribe_ChangePlayer(class CObserver* pObserver);

public: //GET
	CTransform* Get_CurPlayerTransform();
	CPlayer*	Get_Player(CPlayer::TYPE eType) { return m_pPlayerArr[eType]; }
	_float		Get_TagCooltimeRatio() { return CPlayer::m_fTagCoolTime / m_fMaxTagCooltime; }

public:
	//플레이어 탭(변경)
	HRESULT Change_Player(CPlayer::TYPE ePlayerType);

private:
	CPlayer*		m_pPlayerArr[CPlayer::TYPE_END];
	CPlayer::TYPE	m_eCurType = CPlayer::TYPE_WARRIOR;
	_float			m_fMaxTagCooltime = 0.f;
	_bool			m_bChanging = false;

	CSubject*		m_pSubjectTagPlayer = nullptr;

public:
	virtual void Free() override;
};
END
#endif // !__CLIENT_PLAYER_TAG_MGR_H__
