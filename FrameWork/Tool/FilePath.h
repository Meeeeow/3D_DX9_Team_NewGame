#pragma once
#ifndef __FILE_PATH_H__
#define __FILE_PATH_H__

class CFilePath
{
public:
	//파일의 이름에서 확장자 제거하고 리턴
	static CString Get_FileNameWithoutExtension(CString wstrCurFilePath);
};

#endif // !__FILE_PATH_H__
