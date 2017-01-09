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
	string szFileName;//�ļ���
	int iFileSize;//�ļ���С
	string szFileContent;//�ļ�����
	SimpleFile *pNextFile;//�ļ�����ָ��
	SimpleFile(string name);//��ͨ���캯��
	SimpleFile(const SimpleFile &File);//���ƹ��캯��
}SFile;

typedef class SimpleFloder
{
public:
	string szFloderName;//����
	int iFloderSize;//�ܴ�С
	SimpleFloder *pFatherFloder;//��Ŀ¼ָ��
	vector<SimpleFloder*> vpSonFloder;//��Ŀ¼����
	SFile *pFileList;//ָ���ļ������ָ��
	SimpleFloder(string name, SimpleFloder *cur);//��ͨ���캯��
	SimpleFloder(const SimpleFloder &Floder);//���ƹ��캯��
}SFloder;

class FileSystem
{
private :
	SFloder *pRootFloder;//��Ŀ¼
	SFloder *pCopyFloderBuffer;//�ļ��м�����
	SFile *pCopyFileBuffer;//�ļ�������
public:
	FileSystem()//��ʼ���ļ�ϵͳ
	{
		pRootFloder = new SFloder("root",NULL);
		pCopyFloderBuffer = NULL;
		pCopyFileBuffer = NULL;
	}
	~FileSystem()//��������
	{
		delete pRootFloder;
	}
	SFloder *getRootFloder()//��ȡ��Ŀ¼
	{
		return pRootFloder;
	}
	SFloder *getCopyFloderBuffer()//��ȡ�ļ��м�����
	{
		return pCopyFloderBuffer;
	}
	int CreateFloder(string FloderName,SFloder *CurFloder);//�����ļ���
	int DeleteFloder(string FloderName,SFloder *CurFloder);//ɾ���ļ���
	int RenameFloder(string oldFloderName,string newFloderName,SFloder *CurFloder);//�������ļ���
	int CopyFloder(string FloderName,SFloder *CurFloder);//�����ļ���
	int PasteFloder(string FloderName,SFloder *CurFloder);//ճ���ļ���

	int CreateFile(string FileName,SFloder *CurFloder);//�����ļ�
	int DeleteFile(string FileName,SFloder *CurFloder);//ɾ���ļ�
	int RenameFile(string oldFileName,string newFileName,SFloder *CurFloder);//�������ļ�
	int WriteFile(string FileName,SFloder *CurFloder);//����д����
	int ReadFile(string FileName,SFloder *CurFloder);//���ж�����
	int CopyFile(string FileName,SFloder *CurFloder);//�����ļ�
	int PasteFile(string FileName,SFloder *CurFloder);//ճ���ļ�
	SFile *getCopyFileBuffer()//��ȡ�ļ����а������
	{
		return pCopyFileBuffer;
	}

	void ReturnCodeProc(int res);//�����봦��
	void InputHelp();

};
#endif
