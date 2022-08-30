#pragma once
#ifndef __ENGINE_FUNCTOR_H__
#define __ENGINE_FUNCTOR_H__

BEGIN(Engine)

class CTagFinder
{
public:
	CTagFinder() {};
	CTagFinder(const wchar_t* pChar) { m_pChar = pChar; }

public:
	template <typename T>
	bool operator()(T& pair)
	{
		return !lstrcmp(pair.first, m_pChar);
	}

private:
	const wchar_t* m_pChar = nullptr;
};

END

#endif // !__ENGINE_FUNCTOR_H__
