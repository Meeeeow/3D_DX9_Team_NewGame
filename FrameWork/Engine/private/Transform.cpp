#include "..\public\Transform.h"

CTransform::CTransform(_pDevice pGraphicDevice)
	:CComponent(pGraphicDevice)
{
	D3DXMatrixIdentity(&m_matWorld);
}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_matWorld(rhs.m_matWorld)
	, m_tTransformDesc(rhs.m_tTransformDesc)
{
}



HRESULT CTransform::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CTransform NativeConstruct_Prototype - __super");


	return S_OK;
}

HRESULT CTransform::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CTransform NativeConstruct - __super");

	if (pArg != nullptr)
		memcpy(&m_tTransformDesc, pArg, sizeof(TRANSFORM_DESC));

	return S_OK;
}

_float CTransform::Get_DistanceTarget(CTransform * pTransform)
{
	_vec3 vPos = Get_State(STATE_POS);
	_vec3 vTargetPos = pTransform->Get_State(STATE_POS);

	return D3DXVec3Length(&(vTargetPos - vPos));
}

void CTransform::SetUp_LookOrtho(const _vec3 & vLook)
{
	//같은 방향 바라보게 룩벡터 세팅
	SetUp_State(CTransform::STATE_LOOK, vLook);
	//직교할 수 있게 나머지 방향도 세팅 필요함
	_vec3 vRight;
	D3DXVec3Cross(&vRight, &_vec3(0.f, 1.f, 0.f), &vLook);
	SetUp_State(CTransform::STATE_RIGHT, vRight);

	_vec3 vUp;
	D3DXVec3Cross(&vUp, &vLook, &vRight);
	SetUp_State(CTransform::STATE_UP, vUp);
}

void CTransform::SetUp_UpOrtho(const _vec3 & vUp)
{
	//같은 방향 바라보게 업벡터 세팅
	SetUp_State(CTransform::STATE_UP, vUp);
	//직교할 수 있게 나머지 방향도 세팅 필요함
	_vec3 vRight;
	D3DXVec3Cross(&vRight, &vUp, &_vec3(0.f, -1.f, 0.f));
	SetUp_State(CTransform::STATE_RIGHT, vRight);

	_vec3 vLook;
	D3DXVec3Cross(&vLook, &vRight, &vUp);
	SetUp_State(CTransform::STATE_LOOK, vLook);
}

void CTransform::SetUp_LookOrthoWithoutY(const _vec3 & vLook)
{
	//스케일 값 날아가지 않게 보관
	_vec3 vScale = Get_Scale();
	//같은 방향 바라보게 룩벡터 세팅
	SetUp_State(CTransform::STATE_LOOK, vLook * vScale.z);
	//직교할 수 있게 나머지 방향도 세팅 필요함
	_vec3 vRight;
	D3DXVec3Cross(&vRight, &_vec3(0.f, 1.f, 0.f), &vLook);
	SetUp_State(CTransform::STATE_RIGHT, vRight * vScale.x);

	SetUp_State(CTransform::STATE_UP, _vec3(0.f, 1.f, 0.f) * vScale.y);
}

void CTransform::SetUp_Scale(_float fScaleX, _float fScaleY, _float fScaleZ)
{
	_vec3 vRight = Get_State(STATE_RIGHT);
	_vec3 vUp = Get_State(STATE_UP);
	_vec3 vLook = Get_State(STATE_LOOK);

	SetUp_State(STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * fScaleX);
	SetUp_State(STATE_UP, *D3DXVec3Normalize(&vUp, &vUp) * fScaleY);
	SetUp_State(STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * fScaleZ);
}

void CTransform::SetUp_Scale(_vec3 vScale)
{
	_vec3 vRight = Get_State(STATE_RIGHT);
	_vec3 vUp = Get_State(STATE_UP);
	_vec3 vLook = Get_State(STATE_LOOK);

	SetUp_State(STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * vScale.x);
	SetUp_State(STATE_UP, *D3DXVec3Normalize(&vUp, &vUp) * vScale.y);
	SetUp_State(STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * vScale.z);
}

void CTransform::SetUp_Rotation(const _vec3 & vAxis, _float fDegree)
{
	_mat mat;
	D3DXMatrixRotationAxis(&mat, &vAxis, D3DXToRadian(fDegree));

	_vec3 vRight(1.f, 0.f, 0.f);
	_vec3 vUp(0.f, 1.f, 0.f);
	_vec3 vLook(0.f, 0.f, 1.f);
	_vec3 vScale = Get_Scale();

	SetUp_State(STATE_RIGHT, (*D3DXVec3TransformNormal(&vRight, &vRight, &mat)) * vScale.x);
	SetUp_State(STATE_UP, (*D3DXVec3TransformNormal(&vUp, &vUp, &mat)) * vScale.y);
	SetUp_State(STATE_LOOK, (*D3DXVec3TransformNormal(&vLook, &vLook, &mat)) * vScale.z);
}

void CTransform::Add_Position(_vec3 vPos)
{
	_vec3 vWorldPos = Get_State(STATE_POS);

	vWorldPos += vPos;
	SetUp_State(STATE_POS, vWorldPos);
}

_float CTransform::Go_Straight(_float fTimeDelta)
{
	_vec3 vLook = Get_State(STATE_LOOK);
	_vec3 vPos = Get_State(STATE_POS);
	_float fDist = fTimeDelta * (m_tTransformDesc.fSpeed - m_fDebuffSpeed);
	vPos += *D3DXVec3Normalize(&vLook, &vLook) * fDist;
	SetUp_State(STATE_POS, vPos);

	return fDist;
}

_float CTransform::Go_Backward(_float fTimeDelta)
{
	_vec3 vLook = Get_State(STATE_LOOK);
	_vec3 vPos = Get_State(STATE_POS);
	_float fDist = fTimeDelta * (m_tTransformDesc.fSpeed - m_fDebuffSpeed);;
	vPos -= *D3DXVec3Normalize(&vLook, &vLook) * fDist;
	SetUp_State(STATE_POS, vPos);
	return fDist;
}

_float CTransform::Go_Right(_float fTimeDelta)
{
	_vec3 vDir = Get_State(STATE_RIGHT);
	_vec3 vPos = Get_State(STATE_POS);
	_float fDist = fTimeDelta * (m_tTransformDesc.fSpeed - m_fDebuffSpeed);;
	vPos += *D3DXVec3Normalize(&vDir, &vDir) * fDist;
	SetUp_State(STATE_POS, vPos);
	return fDist;
}

_float CTransform::Go_Left(_float fTimeDelta)
{
	_vec3 vDir = Get_State(STATE_RIGHT);
	_vec3 vPos = Get_State(STATE_POS);
	_float fDist = fTimeDelta * (m_tTransformDesc.fSpeed - m_fDebuffSpeed);;
	vPos -= *D3DXVec3Normalize(&vDir, &vDir) * fDist;
	SetUp_State(STATE_POS, vPos);
	return fDist;
}

_float CTransform::Go_StraightRight(_float fTimeDelta)
{
	_vec3 vDir = Get_State(STATE_LOOK) + Get_State(STATE_RIGHT);
	_vec3 vPos = Get_State(STATE_POS);
	_float fDist = fTimeDelta * (m_tTransformDesc.fSpeed - m_fDebuffSpeed);;
	vPos += *D3DXVec3Normalize(&vDir, &vDir) * fDist;
	SetUp_State(STATE_POS, vPos);
	return fDist;
}

_float CTransform::Go_StraightLeft(_float fTimeDelta)
{
	_vec3 vDir = Get_State(STATE_LOOK) - Get_State(STATE_RIGHT);
	_vec3 vPos = Get_State(STATE_POS);
	_float fDist = fTimeDelta * (m_tTransformDesc.fSpeed - m_fDebuffSpeed);;
	vPos += *D3DXVec3Normalize(&vDir, &vDir) * fDist;
	SetUp_State(STATE_POS, vPos);
	return fDist;
}

_float CTransform::Go_BackwardRight(_float fTimeDelta)
{
	_vec3 vDir = -Get_State(STATE_LOOK) + Get_State(STATE_RIGHT);
	_vec3 vPos = Get_State(STATE_POS);
	_float fDist = fTimeDelta * (m_tTransformDesc.fSpeed - m_fDebuffSpeed);;
	vPos += *D3DXVec3Normalize(&vDir, &vDir) * fDist;
	SetUp_State(STATE_POS, vPos);
	return fDist;
}

_float CTransform::Go_BackwardLeft(_float fTimeDelta)
{
	_vec3 vDir = -Get_State(STATE_LOOK) - Get_State(STATE_RIGHT);
	_vec3 vPos = Get_State(STATE_POS);
	_float fDist = fTimeDelta * (m_tTransformDesc.fSpeed - m_fDebuffSpeed);;
	vPos += *D3DXVec3Normalize(&vDir, &vDir) * fDist;
	SetUp_State(STATE_POS, vPos);
	return fDist;
}

_float CTransform::Go_Up(_float fTimeDelta)
{
	_vec3 vDir = Get_State(STATE_UP);
	_vec3 vPos = Get_State(STATE_POS);
	_float fDist = fTimeDelta * (m_tTransformDesc.fSpeed - m_fDebuffSpeed);;
	vPos += *D3DXVec3Normalize(&vDir, &vDir) * fDist;
	SetUp_State(STATE_POS, vPos);
	return fDist;
}

_float CTransform::Go_Down(_float fTimeDelta)
{
	_vec3 vDir = -Get_State(STATE_UP);
	_vec3 vPos = Get_State(STATE_POS);
	_float fDist = fTimeDelta * (m_tTransformDesc.fSpeed - m_fDebuffSpeed);;
	vPos += *D3DXVec3Normalize(&vDir, &vDir) * fDist;
	SetUp_State(STATE_POS, vPos);
	return fDist;
}

_float CTransform::Go_Dir(_float fTimeDelta, _vec3 vDir)
{
	_vec3 vPos = Get_State(STATE_POS);
	_float fDist = fTimeDelta * (m_tTransformDesc.fSpeed - m_fDebuffSpeed);;
	vPos += *D3DXVec3Normalize(&vDir, &vDir) * fDist;
	SetUp_State(STATE_POS, vPos);
	return fDist;
}

void CTransform::Invert_Rectangle()
{
	_vec3 vScale = Get_Scale();
	vScale.x *= -1.f;
	SetUp_Scale(vScale);
}

_float CTransform::Jump(_float fJumpPos, _float fJumpPower, _float fJumpTime)
{
	_float fY = fJumpPos + (fJumpPower * fJumpTime - 0.5f * GRAVITY * fJumpTime * fJumpTime);
	_vec3 vPos = Get_State(STATE_POS);
	vPos.y = fY;
	SetUp_State(CTransform::STATE_POS, vPos);

	return fY;
}

void CTransform::Projectile(_vec3 vStartPos, _vec3 vProjectilePower, _float fProjectileTime, _float fDegree)
{
	_vec3 vPos = Get_State(CTransform::STATE_POS);
	vPos.x = vStartPos.x + vProjectilePower.x * fProjectileTime * cosf(D3DXToRadian(fDegree));
	vPos.z = vStartPos.z + vProjectilePower.z * fProjectileTime * cosf(D3DXToRadian(fDegree));

	// 타겟y = 현재y + 힘 * 시간 * 사인 - 0.5f * 중력 * 시간 * 시간
	vPos.y = vStartPos.y + vProjectilePower.y * fProjectileTime * sinf(D3DXToRadian(fDegree)) - 0.5f * GRAVITY * fProjectileTime * fProjectileTime;

	SetUp_State(CTransform::STATE_POS, vPos);
}

_vec3 CTransform::Get_ProjectilePowerToTarget(_vec3 vStartPos, _vec3 vTargetPos, _float fProjectileMaxTime, _float fDegree)
{
	_vec3 vPower;
	// 타겟x = 현재x + 힘 * 시간 * 코사인
	// (타겟x - 현재x) / (시간 * 코사인 ) = 힘
	vPower.x = (vTargetPos.x - vStartPos.x) / (fProjectileMaxTime * cosf(D3DXToRadian(fDegree)));

	// 타겟z = 현재z + 힘 * 시간 * 코사인
	vPower.z = (vTargetPos.z - vStartPos.z) / (fProjectileMaxTime * cosf(D3DXToRadian(fDegree)));

	// 타겟y = 현재y + 힘 * 시간 * 사인 - 0.5f * 중력 * 시간 * 시간
	// (타겟y - 현재y + 0.5f * 중력 * 시간 * 시간) / (시간 * 사인) = 힘
	vPower.y = (vTargetPos.y - vStartPos.y + 0.5f * GRAVITY * fProjectileMaxTime * fProjectileMaxTime) 
		/ (fProjectileMaxTime * sinf(D3DXToRadian(fDegree)));

	return vPower;
}

_float CTransform::Fall(_float fFallPos, _float fFallTime)
{
	_float fY = fFallPos - (0.f * fFallTime + 0.5f * GRAVITY * fFallTime * fFallTime);
	_vec3 vPos = Get_State(STATE_POS);
	vPos.y = fY;
	SetUp_State(CTransform::STATE_POS, vPos);

	return fY;
}

void CTransform::Rotation_Axis(_vec3 vAxis, _float fTimeDelta)
{
	_mat mat;
	D3DXMatrixRotationAxis(&mat, &vAxis, D3DXToRadian(m_tTransformDesc.fDegree * fTimeDelta));

	_vec3 vRight = Get_State(STATE_RIGHT);
	_vec3 vUp = Get_State(STATE_UP);
	_vec3 vLook = Get_State(STATE_LOOK);

	SetUp_State(STATE_RIGHT, (*D3DXVec3TransformNormal(&vRight, &vRight, &mat)));
	SetUp_State(STATE_UP, (*D3DXVec3TransformNormal(&vUp, &vUp, &mat)));
	SetUp_State(STATE_LOOK, (*D3DXVec3TransformNormal(&vLook, &vLook, &mat)));
}

void CTransform::ReSet_Rotation()
{
	_vec3 vRight = _vec3(1.f, 0.f, 0.f);
	_vec3 vUp = _vec3(0.f, 1.f, 0.f);
	_vec3 vLook = _vec3(0.f, 0.f, 1.f);

	SetUp_State(STATE_RIGHT, vRight);
	SetUp_State(STATE_UP, vUp);
	SetUp_State(STATE_LOOK, vLook);
}

void CTransform::Prototype_Rotation(_int iValueX, _int iValueY, _int iValueZ)
{
	// 회전
	_mat matRot;
	D3DXMatrixRotationAxis(&matRot, &_vec3(1.f, 0.f, 0.f), D3DXToRadian(iValueX));

	_vec3 vRight = _vec3(1.f, 0.f, 0.f);
	_vec3 vUp = _vec3(0.f, 1.f, 0.f);
	_vec3 vLook = _vec3(0.f, 0.f, 1.f);

	SetUp_State(STATE_RIGHT, (*D3DXVec3TransformNormal(&vRight, &vRight, &matRot)));
	SetUp_State(STATE_UP, (*D3DXVec3TransformNormal(&vUp, &vUp, &matRot)));
	SetUp_State(STATE_LOOK, (*D3DXVec3TransformNormal(&vLook, &vLook, &matRot)));

	vUp = Get_State(STATE_UP);
	D3DXMatrixRotationAxis(&matRot, &vUp, D3DXToRadian(iValueY));

	vRight = Get_State(STATE_RIGHT);
	vLook = Get_State(STATE_LOOK);

	SetUp_State(STATE_RIGHT, (*D3DXVec3TransformNormal(&vRight, &vRight, &matRot)));
	SetUp_State(STATE_LOOK, (*D3DXVec3TransformNormal(&vLook, &vLook, &matRot)));

	vLook = Get_State(STATE_LOOK);
	D3DXMatrixRotationAxis(&matRot, &vLook, D3DXToRadian(iValueZ));

	vRight = Get_State(STATE_RIGHT);
	vUp = Get_State(STATE_UP);

	SetUp_State(STATE_RIGHT, (*D3DXVec3TransformNormal(&vRight, &vRight, &matRot)));
	SetUp_State(STATE_UP, (*D3DXVec3TransformNormal(&vUp, &vUp, &matRot)));
}

void CTransform::Tick_Debuff(_float fTimeDelta)
{
	m_fDebuffTime -= fTimeDelta;

	if (m_fDebuffTime <= 0.f)
	{
		m_fDebuffSpeed = 0.f;
		m_fDebuffTime = 0.f;
		return;
	}
}

void CTransform::Chase_Target(CTransform * pTargetTransform, _float fTimeDelta, _float fDistance)
{
	_vec3 vPos = Get_State(STATE_POS);
	_vec3 vTargetPos = pTargetTransform->Get_State(STATE_POS);
	_vec3 vDir = vTargetPos - vPos;

	//남은 거리가 더 클 때만 이동
	if (D3DXVec3Length(&vDir) >= fDistance)
		SetUp_State(STATE_POS, vPos + *D3DXVec3Normalize(&vDir, &(vTargetPos - vPos)) * fTimeDelta);
}

void CTransform::Chase_Vector(_vec3 vTargetPos, _float fTimeDelta, _float fDistance)
{
	_vec3 vPos = Get_State(STATE_POS);
	_vec3 vDir = vTargetPos - vPos;

	if (D3DXVec3Length(&vDir) >= fDistance)
		SetUp_State(STATE_POS, vPos + *D3DXVec3Normalize(&vDir, &(vTargetPos - vPos)) * fTimeDelta);
}

void CTransform::Look_Target(CTransform * pTargetTransform)
{
	_vec3 vPos = Get_State(STATE_POS);
	_vec3 vTargetPos = pTargetTransform->Get_State(STATE_POS);
	_vec3 vLook = vTargetPos - vPos;
	_vec3 vScale = Get_Scale();
	_vec3 vRight;
	_vec3 vUp = _vec3(0.f, 1.f, 0.f);

	SetUp_State(STATE_RIGHT, (*D3DXVec3Normalize(&vRight, D3DXVec3Cross(&vRight, &vUp, &vLook))) * vScale.x);
	SetUp_State(STATE_LOOK, (*D3DXVec3Normalize(&vLook, D3DXVec3Cross(&vLook, &vRight, &vUp))) * vScale.z);
	SetUp_State(STATE_UP, (*D3DXVec3Normalize(&vUp, D3DXVec3Cross(&vUp, &vLook, &vRight))) * vScale.y);
}

void CTransform::Look_TargetByLandObject(CTransform * pTargetTransform)
{
	_vec3 vPos = Get_State(STATE_POS);
	_vec3 vTargetPos = pTargetTransform->Get_State(STATE_POS);
	_vec3 vLook = vTargetPos - vPos;
	_vec3 vScale = Get_Scale();
	_vec3 vRight;
	_vec3 vUp = _vec3(0.f, 1.f, 0.f);

	SetUp_State(STATE_RIGHT, (*D3DXVec3Normalize(&vRight,D3DXVec3Cross(&vRight, &vUp, &vLook))) * vScale.x);
	SetUp_State(STATE_LOOK, (*D3DXVec3Normalize(&vLook,D3DXVec3Cross(&vLook, &vRight, &vUp))) * vScale.z);
}

HRESULT CTransform::Bind_OnDevice()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CTransform Bind_OnGrpahicDevice - m_pGraphicDevice is null");

	m_pGraphicDevice->SetTransform(D3DTS_WORLD, &m_matWorld);

	return S_OK;
}

void CTransform::Free()
{
	__super::Free();
}

CComponent * CTransform::Clone(void * pArg)
{
	CComponent* pInstance = new CTransform(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CVIBufferRect Clone Failed");
	}

	return pInstance;
}

CTransform * CTransform::Create(_pDevice pGraphicDevice)
{
	CTransform* pInstance = new CTransform(pGraphicDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CVIBufferRect Create Failed");
	}

	return pInstance;
}
