#pragma once
#ifndef __ENGINE_MACRO_H__
#define __ENGINE_MACRO_H__

#define DECLARE_SINGLETON(ClassName)								\
public:															\
static ClassName* Get_Instance()								\
{																\
	if (!m_pInstance)											\
		m_pInstance = new ClassName;							\
	return m_pInstance;											\
}																\
static unsigned long Destroy_Instance()							\
{																\
	unsigned long		dwRefCnt = 0;							\
	if (nullptr != m_pInstance)									\
	{															\
		dwRefCnt = m_pInstance->Release();						\
		if(0 == dwRefCnt)										\
			m_pInstance = nullptr;								\
	}															\
	return dwRefCnt;											\
}																\
private:														\
	static ClassName* m_pInstance;								

#define IMPLEMENT_SINGLETON(ClassName)							\
ClassName* ClassName::m_pInstance = nullptr; 



#define GET_INSTANCE(CLASSNAME)	[](){											\
	CLASSNAME*	pInstance = CLASSNAME::Get_Instance();							\
	if(nullptr == pInstance) {													\
	char	szMessage[MAX_PATH] = "";											\
	strcpy_s(szMessage, typeid(CLASSNAME).name());								\
	strcat_s(szMessage, "is nullptr");											\
	MessageBoxA(0, szMessage, nullptr, MB_OK);}									\
	else {																		\
	pInstance->AddRef();}														\
	return pInstance;															\
	}();

#define RELEASE_INSTANCE(CLASSNAME)	[](){										\
	CLASSNAME*	pInstance = CLASSNAME::Get_Instance();							\
	if(nullptr == pInstance) {													\
	char	szMessage[MAX_PATH] = "";											\
	strcpy_s(szMessage, typeid(CLASSNAME).name());								\
	strcat_s(szMessage, "is nullptr");											\
	MessageBoxA(0, szMessage, nullptr, MB_OK);}									\
	else {																		\
	pInstance->Release();}														\
	}();




#define BEGIN(NAME) namespace NAME {
#define END }

#ifdef ENGINE_EXPORTS
#define ENGINE_DLL _declspec(dllexport)
#else
#define ENGINE_DLL _declspec(dllimport)
#endif

#define DEFAULT = default

#ifndef _AFX

#define MSGBOX(MESSAGE) MessageBox(0, TEXT(MESSAGE), TEXT("System Message"), MB_OK)
#define MSG_FAIL(MESSAGE) { MSGBOX(MESSAGE); return E_FAIL;}
#define MSG_NULL(MESSAGE) { MSGBOX(MESSAGE); return nullptr;}
#define MSG_MINUSONE(MESSAGE) { MSGBOX(MESSAGE); return -1;}
#define MSG_VOID(MESSAGE) { MSGBOX(MESSAGE); return;}
#define MSG_FALSE(MESSAGE){ MSGBOX(MESSAGE); return false;}
 
#else
#define MSGBOX(MESSAGE) AfxMessageBox(TEXT(MESSAGE))
#define MSG_FAIL(MESSAGE) { MSGBOX(MESSAGE); return E_FAIL;}
#define MSG_NULL(MESSAGE) { MSGBOX(MESSAGE); return nullptr;}
#define MSG_MINUSONE(MESSAGE) { MSGBOX(MESSAGE); return -1;}
#define MSG_VOID(MESSAGE) { MSGBOX(MESSAGE); return;}

#endif // !_AFX

#define DO do{
#define WHILE }while(false);

#endif // !__ENGINE_MACRO_H__
