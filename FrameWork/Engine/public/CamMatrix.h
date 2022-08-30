#pragma once
#ifndef __ENGINE_CAM_MATRIX_H__
#define __ENGINE_CAM_MATRIX_H__

#include "Base.h"
BEGIN(Engine)
class CCamMatrix :
	public CBase
{
	DECLARE_SINGLETON(CCamMatrix)
private:
	explicit CCamMatrix();
	virtual ~CCamMatrix() = default;

public:
	_int Save_CamMatrix(_float fTimeDelte);

public: //SET
	HRESULT Set_Device(_pDevice pDevice);

public: //GET
	const _mat& Get_CamWorld() const { return m_matCamWorld; }
	const _mat& Get_CamView() const { return m_matCamView; }
	const _mat& Get_CamProj() const { return m_matCamProj; }
	
private:
	_pDevice	m_pDevice = nullptr;
	_mat		m_matCamWorld;
	_mat		m_matCamView;
	_mat		m_matCamProj;

public:
	virtual void Free() override;
};
END
#endif // !__ENGINE_CAM_MATRIX_H__
