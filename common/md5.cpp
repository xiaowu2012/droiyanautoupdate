
#include "stdafx.h"
#include "md5.h"
#include "md5sum.h"

bool getFileMd5Value(const char *fileName,std::string &md5Ret)
{
	///读取文件内容到buffer中
	FILE *fp = fopen(fileName, "rb");
	if (!fp) 
		return false;
	fseek(fp, 0, SEEK_END);
	size_t Size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char *buffer = NULL;
	if (Size != 0)
	{
		buffer = new char[Size];
		fread(buffer, sizeof(char), Size, fp);
	}
	fclose(fp);

	MD5Sum md5;
	md5.put(buffer,Size);
	md5Ret = md5.toString();
	if(buffer != NULL)
		delete []buffer;
	return true;
}
void getStringMD5Value(const char *str,std::string &md5Ret)
{
	MD5Sum md5;
	md5.put(str,strlen(str));
	md5Ret = md5.toString();
}

CString GetPwdMd5(const char *str)
{
	MD5Sum md5;
	md5.put(str,strlen(str));
	CString temp = _T("");
	temp.Format(_T("%s"),md5.toString().c_str());
	return temp.Mid(6,10);
}
