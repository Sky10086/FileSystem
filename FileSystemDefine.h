#include<iostream>
#include<string>
#include<vector>
#include<iomanip>
using namespace std;
#ifndef _FILESYSTEMDEFINE_H_
#define _FILESYSTEMDEFINE_H_
typedef class SimpleFile
{
public:
	string szFileName;//文件名
	int iFileSize;//文件大小
	string szFileContent;//文件内容
	SimpleFile *pNextFile;//文件链表指针
	SimpleFile(string name);//普通构造函数
	SimpleFile(const SimpleFile &File);//复制构造函数
}SFile;

typedef class SimpleFloder
{
public:
	string szFloderName;//名字
	int iFloderSize;//总大小
	SimpleFloder *pFatherFloder;//父目录指针
	vector<SimpleFloder*> vpSonFloder;//子目录数组
	SFile *pFileList;//指向文件链表的指针
	SimpleFloder(string name, SimpleFloder *cur);//普通构造函数
	SimpleFloder(const SimpleFloder &Floder);//复制构造函数
}SFloder;

class FileSystem
{
private :
	SFloder *pRootFloder;//根目录
	SFloder *pCopyFloderBuffer;//文件夹剪贴板
	SFile *pCopyFileBuffer;//文件剪贴板
public:
	FileSystem()//初始化文件系统
	{
		pRootFloder = new SFloder("root",NULL);
		pCopyFloderBuffer = NULL;
		pCopyFileBuffer = NULL;
	}
	~FileSystem()//析构函数
	{
		delete pRootFloder;
	}
	SFloder *getRootFloder()//获取根目录
	{
		return pRootFloder;
	}
	SFloder *getCopyFloderBuffer()//获取文件夹剪贴板
	{
		return pCopyFloderBuffer;
	}
	int CreateFloder(string FloderName,SFloder *CurFloder);//创建文件夹
	int DeleteFloder(string FloderName,SFloder *CurFloder);//删除文件夹
	int RenameFloder(string oldFloderName,string newFloderName,SFloder *CurFloder);//重命名文件夹
	int CopyFloder(string FloderName,SFloder *CurFloder);//复制文件夹
	int PasteFloder(string FloderName,SFloder *CurFloder);//粘贴文件夹

	int CreateFile(string FileName,SFloder *CurFloder);//创建文件
	int DeleteFile(string FileName,SFloder *CurFloder);//删除文件
	int RenameFile(string oldFileName,string newFileName,SFloder *CurFloder);//重命名文件
	int WriteFile(string FileName,SFloder *CurFloder);//进行写操作
	int ReadFile(string FileName,SFloder *CurFloder);//进行读操作
	int CopyFile(string FileName,SFloder *CurFloder);//复制文件
	int PasteFile(string FileName,SFloder *CurFloder);//粘贴文件
	SFile *getCopyFileBuffer()//获取文件剪切板的内容
	{
		return pCopyFileBuffer;
	}

	void ReturnCodeProc(int res);//返回码处理
	void InputHelp();

};
#endif
