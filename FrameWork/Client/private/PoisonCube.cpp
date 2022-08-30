#include "stdafx.h"
#include "..\public\PoisonCube.h"
#include "PoisonCubeParticle.h"
#include "PoisonCubeAttack.h"


CPoisonCube::CPoisonCube(_pDevice pDevice)
	: CCubeObject(pDevice)
{
}

CPoisonCube::CPoisonCube(const CPoisonCube & rhs)
	: CCubeObject(rhs)
{
}

HRESULT CPoisonCube::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CPoisonCube NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CPoisonCube::NativeConstruct(void * pArg)
{
	DESC tDesc = *(DESC*)(pArg);
	if (FAILED(__super::NativeConstruct(&tDesc.tObjInfo)))
		MSG_FAIL("CPoisonCube NativeConstruct - __super");

	if (FAILED(SetUp_TransformDesc(tDesc.tTransformDesc)))
		MSG_FAIL("CPoisonCube::NativeConstruct - SetUp_TransformDesc");

	//포물선 그리게 할 때 필요한 준비물들
	m_vTargetPos = tDesc.vTargetPos;
	m_vStartPos = Get_TransformState(CTransform::STATE_POS);
	m_tTimeProjectile.fDelay = 1.f;
	m_vProjectilePower = Get_ProjectilePowerToTarget(m_vStartPos, m_vTargetPos, m_tTimeProjectile.fDelay, m_fProjectileDegree);

	m_tTimeWaitDead.fDelay = 2.f;

	return S_OK;
}

_int CPoisonCube::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CPoisonCube Tick - __super");

	if (m_bDead)
	{
		return OBJ_DEAD;

	}

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	Tick_Move(fTimeDelta, pGameInstance);
	Tick_Rotate(fTimeDelta, pGameInstance);
	Tick_Particle(fTimeDelta, pGameInstance);
	Tick_WaitDead(fTimeDelta, pGameInstance);

	RELEASE_INSTANCE(CGameInstance);

	return 0;
}

_int CPoisonCube::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CPoisonCube LateTick - __super");

	return _int();
}

HRESULT CPoisonCube::Render()
{
	if (!m_bRender)
		return S_OK;

	Set_RenderState();

	if (FAILED(__super::Render()))
		MSG_FAIL("CPoisonCube Render - __super");

	Release_RenderState();

	return S_OK;
}

HRESULT CPoisonCube::SetUp_TransformDesc(CTransform::TRANSFORM_DESC tTransformDesc)
{
	if (FAILED(Set_Speed(tTransformDesc.fSpeed)))
		MSG_FAIL("CPoisonCube::SetUp_TransformDesc - Set_Speed");

	if (FAILED(Set_RotateSpeed(tTransformDesc.fDegree)))
		MSG_FAIL("CPoisonCube::SetUp_TransformDesc - Set_RotateSpeed");

	return S_OK;
}

void CPoisonCube::Tick_Move(_float fTimeDelta, CGameInstance * pGameInstance)
{
	if (pGameInstance == nullptr)
		MSG_VOID("CPoisonCube::Tick_Move - pGameInstance == nullptr");

	if (m_bParticle || m_bWaitDead)
		return;

	m_tTimeProjectile.Tick_Time(fTimeDelta);
	Move_Projectile(m_vStartPos, m_vProjectilePower, m_tTimeProjectile.fTime, m_fProjectileDegree);

	//목표 지점 도달
	if (m_tTimeProjectile.Check_Time())
	{
		Dead_Event(fTimeDelta, pGameInstance);
		m_tTimeProjectile.fTime = 0.f;
	}
}

void CPoisonCube::Tick_Rotate(_float fTimeDelta, CGameInstance * pGameInstance)
{
	if (FAILED(Rotation_Axis(_vec3(1.f, 1.f, 0.f), 10.f)))
		MSG_VOID("CPoisonCube::Tick_Rotate - Rotation_Axis");
}

void CPoisonCube::Tick_Particle(_float fTimeDelta, CGameInstance * pGameInstance)
{
	if (!m_bParticle)
		return;

	m_tTimeParticle.Tick_Time(fTimeDelta);
	if (!m_tTimeParticle.Check_Time())
		return;

	m_tTimeParticle.fTime = 0.f;
	
	//12방향으로 10개씩 랜덤하게 작은 돌맹이 사출
	_float fBaseAngle;
	for (size_t i = 0; i < 12; ++i)
	{
		fBaseAngle = 30.f * i;

		//10개씩 사출 힘, 각도 랜덤
		for (size_t j = 0; j < 10; j++)
		{
			int iRandAngle = rand() % 30;
			int iRandPower = rand() % 4000;
			_float fAngle = fBaseAngle + iRandAngle;
			_float fPower = (_float)iRandPower / 1000.f;

			CPoisonCubeParticle::DESC tDesc;
			tDesc.fPower = fPower;
			lstrcpy(tDesc.tObjInfo.szTextureName, pTagPrototypeTexturePoisonCube);
			tDesc.tObjInfo.iTextureIndex = 1;
			D3DXMatrixIdentity(&tDesc.tObjInfo.matWorld);
			tDesc.tObjInfo.SetUp_Scale(0.1f, 0.1f, 0.1f);
			tDesc.tObjInfo.SetUp_State(OBJ_INFO::STATE_POS, Get_TransformState(CTransform::STATE_POS));
			tDesc.tObjInfo.SetUp_Rotation(_vec3(0.f, 1.f, 0.f), fAngle);

			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypePoisonCubeParticle, pTagLayerNonCollision, &tDesc)))
			MSG_VOID("CPoisonCube::Tick_Particle - pTagPrototypePoisonCubeParticle");
		}
	}

	m_iCount++;
	if (m_iCount >= m_iMaxCount)
	{
		m_bWaitDead = true;
		m_bParticle = false;
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::THIEF_SKILL_POISONCUBE);
	}
}

void CPoisonCube::Tick_WaitDead(_float fTimeDelta, CGameInstance * pGameInstance)
{
	if (!m_bWaitDead)
		return;

	m_tTimeWaitDead.Tick_Time(fTimeDelta);
	if (m_tTimeWaitDead.Check_Time())
		Set_Dead();
}

void CPoisonCube::Dead_Event(_float fTimeDelta, CGameInstance * pGameInstance)
{
	m_bRender = false;
	m_bParticle = true;
	m_tTimeParticle.fTime = 0.f;
	m_tTimeParticle.fDelay = 0.1f;

	CSoundMgr::Get_Instance()->PlayLoopSound(L"PoisonCubeSpary.wav", CSoundMgr::THIEF_SKILL_POISONCUBE);

	//중독 공격 소환
	CPoisonCubeAttack::DESC tDesc;
	tDesc.Set_Parent(this);
	tDesc.tSuperDesc.vPos = Get_TransformState(CTransform::STATE_POS);
	tDesc.tSuperDesc.vScale = _vec3(2.f, 2.f, 2.f);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypePoisonCubeAttack, pTagLayerPlayerAttack, &tDesc)))
		MSG_VOID("CPoisonCube::Dead_Event - pTagPrototypePoisonCubeAttack");
}

void CPoisonCube::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_VOID("CPoisonCube::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, TRUE)))
		MSG_VOID("CPoisonCube::Set_RenderState - D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAREF, 0)))
		MSG_VOID("CPoisonCube::Set_RenderState - D3DRENDERSTATETYPE::D3DRS_ALPHAREF, 0");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
		MSG_VOID("CPoisonCube::Set_RenderState - D3DRS_ALPHAFUNC, D3DCMP_GREATER");
}

void CPoisonCube::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_VOID("CPoisonCube::Release_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, FALSE)))
		MSG_VOID("CPoisonCube::Release_RenderState - D3DRS_ALPHATESTENABLE, FALSE");
}

CPoisonCube * CPoisonCube::Create(_pDevice pDevice)
{
	CPoisonCube* pInstance = new CPoisonCube(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CPoisonCube Create Failed");
	}

	return pInstance;
}

CGameObject * CPoisonCube::Clone(void * pArg)
{
	CPoisonCube* pInstance = new CPoisonCube(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CPoisonCube Clone Failed");
	}

	return pInstance;
}

void CPoisonCube::Free()
{
	__super::Free();
}
