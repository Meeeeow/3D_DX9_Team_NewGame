#pragma once
#ifndef __FILE_PATH_H__
#define __FILE_PATH_H__

class CFilePath
{
public:
	//������ �̸����� Ȯ���� �����ϰ� ����
	static CString Get_FileNameWithoutExtension(CString wstrCurFilePath);
};

#endif // !__FILE_PATH_H__
