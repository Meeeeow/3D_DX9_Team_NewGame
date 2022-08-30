#pragma once

#ifndef __SKILLICON_H__
#define __SKILLICON_H__

#include "Observer.h"

BEGIN(Engine)
class CRenderer;
class CVIBufferRect;
class CTransform;
class CTexture;
END

BEGIN(Client)
class CSkillIcon final : 
	public CObserver
{
private:
	explicit CSkillIcon(_pDevice pGraphicDevice);
	explicit CSkillIcon(const CSkillIcon& rhs);
	virtual ~CSkillIcon() = default;
		
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Notify(void* pMessage = nullptr);

public: //GET

private:
	HRESULT SetUp_Component();

private:
	CVIBufferRect*				m_pVIBufferCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	CTexture*					m_pWarriorSkillTextureCom = nullptr;
	CTexture*					m_pMageSkillTextureCom = nullptr;
	CTexture*					m_pThiefSkillTextureCom = nullptr;

private:
	_mat						m_matProject;

	_bool						m_bControl = true;
	_bool						m_bCoolDown = false;

	_uint						m_iClassType = 0;
	_uint						m_iSkillNum = 0;

	_float						m_fMaxCoolTime = 0.f;
	_float						m_fCoolTime = 0.f;
	_float						m_fX = 0.f; 
	_float						m_fY = 0.f; 
	_float						m_fSizeX = 0.f; 
	_float						m_fSizeY = 0.f;

public:
	static CSkillIcon* Create(_pDevice pGraphicDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

	virtual _vec3	Get_MinPos() override;
	virtual _vec3	Get_MaxPos() override;
	virtual _vec3	Get_WorldPos() override;
	virtual _vec3	Get_Length() override;

};

END

#endif // !__SKILLICON_H__
