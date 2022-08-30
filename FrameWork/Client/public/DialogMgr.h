#pragma once
#ifndef __CLIENT_DIALOG_MGR_H__
#define __CLIENT_DIALOG_MGR_H__

#include "Base.h"
#include "Dialog.h"
BEGIN(Engine)
END

BEGIN(Client)
class CDialogMgr final:
	public CBase
{
	DECLARE_SINGLETON(CDialogMgr)

private:
	explicit CDialogMgr();
	virtual ~CDialogMgr();

public:
	HRESULT SetUp_Dialog(_bool bNotTick, CDialog::DESC tDesc);
	HRESULT Draw_Font(const _tchar * pText, D3DXCOLOR FontColor, _vec3* vPos = nullptr, _vec3 vScale = _vec3(0.5f, 1.f, 1.f));
	_int	Tick(_float fTimeDelta);

private:
	HRESULT Add_Dialog(CDialog* pDialog);

private:
	list<class CDialog*> m_listDialog;

public:
	virtual void Free() override;
};
END
#endif // !__CLIENT_DIALOG_MGR_H__
