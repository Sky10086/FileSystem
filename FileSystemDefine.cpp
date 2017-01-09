#include"FileSystemDefine.h"
using namespace std;
#define Correct 0 //����
#define Error 1 //�ڴ��������ϵͳ����
#define SameName 2 //��ǰĿ¼�Ѿ��д������ļ�����Ŀ¼
#define None 4//��ǰĿ¼Ϊ��
#define No 5 //��ǰĿ¼��Ϊ�յ��޴��ļ�����Ŀ¼
#define LogPrint(S) (printf("%s\n",S))
SFile::SimpleFile(string name)
{
	szFileName = name;
	iFileSize = 0;
	szFileContent = "";
	pNextFile = NULL;
}
SFile::SimpleFile(const SimpleFile &File)
{
	szFileName = File.szFileName;
	iFileSize = File.iFileSize;
	szFileContent = File.szFileContent;
	pNextFile = NULL;
}

SFloder::SimpleFloder(string name, SimpleFloder *cur)//��ͨ���캯��
{
	szFloderName = name;
	iFloderSize = 0;
	pFatherFloder = cur;//ָ����Ŀ¼
	vector<SimpleFloder*> initV;//��ʼ������
	vpSonFloder = initV;
	pFileList = NULL;
}
SFloder::SimpleFloder(const SimpleFloder &Floder)//���ƹ��캯��
{
	szFloderName = Floder.szFloderName;
	iFloderSize = Floder.iFloderSize;
	SFloder *father = new SFloder(Floder.pFatherFloder->szFloderName, Floder.pFatherFloder->pFatherFloder);
	pFatherFloder = father;
	vector<SimpleFloder*> NewvpSonFloder;
	for (unsigned i = 0; i < Floder.vpSonFloder.size(); i++)
	{
		SFloder *NewFloder = new SimpleFloder(*Floder.vpSonFloder[i]);
		NewvpSonFloder.push_back(NewFloder);
	}
	vpSonFloder = NewvpSonFloder;
	SFile *File = new SFile(Floder.pFileList->szFileName);
	SFile *pHelp = File;
	SFile *pTempFile = Floder.pFileList->pNextFile;
	while (NULL != pTempFile)
	{
		SFile *NewFile = new SFile(pTempFile->szFileName);
		File->pNextFile = NewFile;
		File = File->pNextFile;
		pTempFile = pTempFile->pNextFile;
	}
	pFileList = File;
}
int FileSystem::CreateFloder(string FloderName,SFloder *CurFloder)
{
	//��ǰĿ¼����û��Ŀ¼
	if(CurFloder->vpSonFloder.size() == 0)
	{
		SFloder *NewNode = new SFloder(FloderName,CurFloder);
		if(NULL == NewNode)
			return Error;
		CurFloder->vpSonFloder.push_back(NewNode);
		return Correct;
	}
	else
	{
		int size = CurFloder->vpSonFloder.size();
		//�������
		for(int i= 0;i < size;i++)
		{
			if(CurFloder->vpSonFloder[i]->szFloderName == FloderName)
			{
				return SameName;
			}
		}
		SFloder *NewNode = new SFloder(FloderName,CurFloder);
		if(NULL == NewNode)
			return Error;
		CurFloder->vpSonFloder.push_back(NewNode);
		return Correct;
	}
}

int FileSystem::DeleteFloder(string FloderName,SFloder *CurFloder)
{
	//��ǰĿ¼������Ŀ¼
	if(CurFloder->vpSonFloder.size() == 0)
	{
		return None;
	}
	else
	{
		int flag = false;
		//Ѱ��Ŀ¼
		for(unsigned int i= 0;i < CurFloder->vpSonFloder.size();i++)
		{
			if(CurFloder->vpSonFloder[i]->szFloderName == FloderName)
			{
				SFloder *AdjSize = CurFloder;
				while (AdjSize != NULL)
				{
					AdjSize->iFloderSize -= CurFloder->vpSonFloder[i]->iFloderSize;
					AdjSize = AdjSize->pFatherFloder;
				}
				delete CurFloder->vpSonFloder[i];
				CurFloder->vpSonFloder.erase(CurFloder->vpSonFloder.begin()+i);
				flag = true;
			}
		}
		if(false == flag)
		{
			return No;
		}
		return Correct;
	}
}

int FileSystem::RenameFloder(string oldFloderName,string newFloderName,SFloder *CurFloder)
{
	//��ǰĿ¼������Ŀ¼
	if(CurFloder->vpSonFloder.size() == 0)
	{
		return None;
	}
	else
	{
		int size = CurFloder->vpSonFloder.size();
		bool isFind = false;
		bool isNamed = false;
		//Ѱ�ҵ�ǰĿ¼���Ƿ���ڴ�Ŀ¼
		for(int i= 0;i < size;i++)
		{
			if(CurFloder->vpSonFloder[i]->szFloderName == oldFloderName)
			{
				isFind = true;
				for(int j = 0;j < size;j++)
				{
					//��鵱ǰĿ¼��ʱ���Ѵ��ں���������ͬ��Ŀ¼
					if(CurFloder->vpSonFloder[j]->szFloderName == newFloderName)
					{
						isNamed = true;
					}
				}
				//������
				if(false == isNamed)
				CurFloder->vpSonFloder[i]->szFloderName = newFloderName;
			}
		}
		if(false == isFind)
		{
			return No;
		}
		if(true == isNamed)
		{
			return SameName;
		}
		return Correct;
	}
}

int FileSystem::CopyFloder(string FloderName,SFloder *CurFloder)
{
	//��ǰĿ¼������Ŀ¼
	if(CurFloder->vpSonFloder.size() == 0)
	{
		return None;
	}
	else
	{
		int isFind = false;
		//Ѱ��Ŀ¼
		for(unsigned int i= 0;i < CurFloder->vpSonFloder.size();i++)
		{
			if(CurFloder->vpSonFloder[i]->szFloderName == FloderName)
			{
				this->pCopyFloderBuffer= new SFloder(*CurFloder->vpSonFloder[i]);
				isFind = true;
			}
		}
		if(false == isFind)
		{
			return No;
		}
		return Correct;
	}
}

int FileSystem::PasteFloder(string FloderName,SFloder *CurFloder)
{
	//��ǰĿ¼����û��Ŀ¼
	if(CurFloder->vpSonFloder.size() == 0)
	{
		if(this->pCopyFloderBuffer == NULL)
		{
			LogPrint("ճ���������󣬸��ƻ�����Ϊ�գ�����ѡ������Ŀ¼!");
			return Error;
		}
		CurFloder->vpSonFloder.push_back(this->pCopyFloderBuffer);
		SFloder *AdjSize = CurFloder;
		while (AdjSize != NULL)
		{
			AdjSize->iFloderSize += this->pCopyFloderBuffer->iFloderSize;
			AdjSize = AdjSize->pFatherFloder;
		}
		return Correct;
	}
	else
	{
		int size = CurFloder->vpSonFloder.size();
		//�������
		for(int i= 0;i < size;i++)
		{
			if(CurFloder->vpSonFloder[i]->szFloderName == FloderName)
			{
				return SameName;
			}
		}
		if(this->pCopyFloderBuffer == NULL)
		{
			LogPrint("ճ���������󣬸��ƻ�����Ϊ�գ�����ѡ������Ŀ¼!");
			return Error;
		}
		CurFloder->vpSonFloder.push_back(this->pCopyFloderBuffer);
		SFloder *AdjSize = CurFloder->pFatherFloder;
		while (AdjSize != NULL)
		{
			AdjSize->iFloderSize += this->pCopyFloderBuffer->iFloderSize;
			AdjSize = AdjSize->pFatherFloder;
		}
		return Correct;
	}
}
int FileSystem::CreateFile(string name,SFloder *CurFloder)
{
	if(NULL == CurFloder->pFileList)
	{
		SFile *FileHead = new SFile(name);
		CurFloder->pFileList = FileHead;
	}
	else
	{
		SFile *pTempFile = CurFloder->pFileList;
		while(NULL != pTempFile)
		{
			if(name == pTempFile->szFileName)
				return SameName;
			pTempFile = pTempFile->pNextFile;
		}
		//ͷ�巨��β�巨��
		SFile *pNewHead = new SFile(name);
		pNewHead->pNextFile = CurFloder->pFileList;
		CurFloder->pFileList = pNewHead;
	}
	return Correct;
}

int FileSystem::DeleteFile(string name,SFloder *CurFloder)
{
	if(NULL == CurFloder->pFileList)
	{
		return None;
	}
	else
	{
		SFile *pTempFile = CurFloder->pFileList;
		bool isFind = false;
		while(NULL != pTempFile)
		{
			if(pTempFile->szFileName == name)
			{
				isFind = true;
				SFile *pHelp = CurFloder->pFileList;
				while(NULL != pHelp)
				{
					if(pHelp->pNextFile == pTempFile)
						break;
					pHelp = pHelp->pNextFile;
				}
				if (NULL == pTempFile->pNextFile)
				{
					CurFloder->pFileList = NULL;
				}
				else
				{ 
					pHelp->pNextFile = pTempFile->pNextFile;
				}
			
				SFloder *AdjSize = CurFloder;
				while (AdjSize != NULL)
				{
					AdjSize->iFloderSize -= pTempFile->iFileSize;
					AdjSize = AdjSize->pFatherFloder;
				}
				delete pTempFile;
				break;
			}
			pTempFile = pTempFile->pNextFile;
		}
		if(false == isFind)
			return No;
		return Correct;
	}
}

int FileSystem::RenameFile(string oldname,string newname,SFloder *CurFloder)
{
	if(NULL == CurFloder->pFileList)
	{
		return None;
	}
	else
	{
		SFile *pTempFile = CurFloder->pFileList;
		bool isFind = false;
		bool isSameName = false;
		while(NULL != pTempFile)
		{
			if(pTempFile->szFileName == oldname)
			{
				isFind = true;
				SFile *pHelp = CurFloder->pFileList;
				while(NULL != pHelp)
				{
					if(pHelp->szFileName == newname)
					{
						isSameName = true;
						break;
					}
					pHelp = pHelp->pNextFile;
				}
				if(false == isSameName)
				{
					pTempFile->szFileName = newname;
				}
				break;
			}
			pTempFile = pTempFile->pNextFile;
		}
		if(false == isFind)
			return No;
		if(true == isSameName)
			return SameName;
		return Correct;
	}
}

int FileSystem::WriteFile(string FileName,SFloder *CurFloder)
{
	if(NULL == CurFloder->pFileList)
	{
		return None;
	}
	else
	{
		SFile *pTempFile = CurFloder->pFileList;
		bool isFind = false;
		while(NULL != pTempFile)
		{
			if(pTempFile->szFileName == FileName)
			{
				isFind = true;
				cout<<"��ǰ�ļ�����Ϊ:"<<endl;
				if("" == pTempFile->szFileContent)
				{
					cout<<"��ǰ�ļ�������!"<<endl;
				}
				else
				{
					cout<<pTempFile->szFileContent<<endl;
				}
				cout<<"��������Ҫд�������(�Ḳ�Ǿ�����)"<<endl;
				int oldsize = pTempFile->iFileSize;
				string content;
				getline(cin,content);
				pTempFile->szFileContent = content;
				pTempFile->iFileSize = sizeof(content);
				SFloder *AdjSize = CurFloder;
				while (AdjSize != NULL)
				{
					AdjSize->iFloderSize += (pTempFile->iFileSize - oldsize);
					AdjSize = AdjSize->pFatherFloder;
				}
				break;
			}
			pTempFile = pTempFile->pNextFile;
		}
		if(false == isFind)
			return No;
		return Correct;
	}
}

int FileSystem::ReadFile(string FileName,SFloder *CurFloder)
{
	if(NULL == CurFloder->pFileList)
	{
		return None;
	}
	else
	{
		SFile *pTempFile = CurFloder->pFileList;
		bool isFind = false;
		while(NULL != pTempFile)
		{
			if(pTempFile->szFileName == FileName)
			{
				isFind = true;
				cout<<"��ǰ�ļ�����Ϊ:"<<endl;
				if("" == pTempFile->szFileContent)
				{
					cout<<"��ǰ�ļ�������!"<<endl;
				}
				else
				{
					cout<<pTempFile->szFileContent<<endl;
				}
				break;
			}
			pTempFile = pTempFile->pNextFile;
		}
		if(false == isFind)
			return No;
		return Correct;
	}
}
int FileSystem::CopyFile(string FileName,SFloder *CurFloder)
{
	if(NULL == CurFloder->pFileList)
	{
		return None;
	}
	else
	{
		SFile *pTempFile = CurFloder->pFileList;
		bool isFind = false;
		while(NULL != pTempFile)
		{
			if(pTempFile->szFileName == FileName)
			{
				isFind = true;
				this->pCopyFileBuffer = new SFile(*pTempFile);
				break;
			}
			pTempFile = pTempFile->pNextFile;
		}
		if(false == isFind)
			return No;
		return Correct;
	}
}
int FileSystem::PasteFile(string FileName,SFloder *CurFloder)
{
	if(NULL == CurFloder->pFileList)
	{
		if(this->pCopyFileBuffer == NULL)
		{
			LogPrint("ճ���������󣬸��ƻ�����Ϊ�գ�����ѡ�������ļ�!");
			return Error;
		}
		CurFloder->pFileList = this->pCopyFileBuffer;
		SFloder *AdjSize = CurFloder;
		while (AdjSize != NULL)
		{
			AdjSize->iFloderSize += this->pCopyFileBuffer->iFileSize;
			AdjSize = AdjSize->pFatherFloder;
		}
	}
	else
	{
		SFile *pTempFile = CurFloder->pFileList;
		while(NULL != pTempFile)
		{
			if(FileName == pTempFile->szFileName)
				return SameName;
			pTempFile = pTempFile->pNextFile;
		}
		//ͷ�巨��β�巨��
		if(this->pCopyFileBuffer == NULL)
		{
			LogPrint("ճ���������󣬸��ƻ�����Ϊ�գ�����ѡ�������ļ�!");
			return Error;
		}
		this->pCopyFileBuffer->pNextFile = CurFloder->pFileList;
		CurFloder->pFileList = this->pCopyFileBuffer;
		SFloder *AdjSize = CurFloder;
		while (AdjSize != NULL)
		{
			AdjSize->iFloderSize += this->pCopyFileBuffer->iFileSize;
			AdjSize = AdjSize->pFatherFloder;
		}
	}
	return Correct;
}
void FileSystem::ReturnCodeProc(int res)
{
	switch(res)
	{
		case Correct:
			LogPrint("�����ɹ���");
			break;
		case Error:
			LogPrint("�����ڴ����ʧ��!");
			break;
		case SameName:
			LogPrint("���󣬸��ļ��������и������ļ�(��Ŀ¼)��������ȡ��");
			break;
		case None:
			LogPrint("���󣬸��ļ��������ļ�(��Ŀ¼),���ȴ����ٲ���!");
			break;
		case No:
			LogPrint("���󣬸��ļ������޴������ļ�(��Ŀ¼),���������������!");
			break;
		default:
			break;
	}
}
 void FileSystem::InputHelp()
 {
	cout<<"Ŀ¼��������:"<<endl;
	cout<<"mkdir [FloderName]"<<endl
		<<"  "<<"�½�һ����ΪFloderName��Ŀ¼"
		<<endl;
	cout<<"rmdir [FloderName]"<<endl
		<<"  "<<"ɾ����ǰĿ¼����ΪFloderName��Ŀ¼"
		<<endl;
	cout<<"ls"<<endl
		<<"  "<<"�г���ǰĿ¼��������Ŀ¼���ļ�����"
		<<endl;
	cout<<"cd [FloderName]"<<endl
		<<"  "<<"���뵱ǰĿ¼����ΪFloderName��Ŀ¼,��֮���..ʱ����ʾ���븸Ŀ¼"
		<<endl;
	cout<<"renamedir [oldFloderName] [newFloderName]"<<endl
		<<"  "<<"����ǰĿ¼����ΪoldFloderName��Ŀ¼������ΪnewFloderName"
		<<endl;
	cout<<"cpdir [FloderName]"<<endl
		<<"  "<<"���Ƶ�ǰĿ¼����ΪFloderName��Ŀ¼��������"
		<<endl;
	cout<<"pastedir"<<endl
		<<"  "<<"���������Ŀ¼���Ƶ���ǰĿ¼"
		<<endl;
	cout << "pwd" << endl
		<< "  " << "��ʾ��ǰĿ¼����"
		<< endl;
	cout<<endl;
	cout<<"�ļ���������"<<endl;
	cout<<"mk [FileName]"<<endl
		<<"  "<<"�½�һ����ΪFileName���ļ�"
		<<endl;
	cout<<"rm [FileName]"<<endl
		<<"  "<<"ɾ��һ����ΪFileName���ļ�"
		<<endl;
	cout<<"rename [oldFileName] [newFileName]"<<endl
		<<"  "<<"����ǰĿ¼����ΪoldFileName���ļ�������ΪnewFileName"
		<<endl;
	cout<<"write [FileName]"<<endl
		<<"  "<<"�޸��ڵ�ǰĿ¼����ΪFileName���ļ�����"
		<<endl;
	cout<<"read [FileName]"<<endl
		<<"  "<<"�鿴�ڵ�ǰĿ¼����ΪFileName���ļ�����"
		<<endl;
	cout<<"cp [FileName]"<<endl
		<<"  "<<"���Ƶ�ǰĿ¼����ΪFileName���ļ���������"
		<<endl;
	cout<<"paste"<<endl
		<<"  "<<"����������ļ����Ƶ���ǰĿ¼"
		<<endl;
	cout<<endl;
	cout<<"ϵͳ����"<<endl;
	cout<<"help"<<endl
		<<"  "<<"help ���������Ϣ"<<endl;
	cout<<"clear"<<endl
		<<"  "<<"clear �󲻸��κβ�������ʾ����"
		<<endl;
	cout<<"exit"<<endl
		<<"  "<<"exit �󲻸��κβ�������ʾ�˳����ļ�ϵͳ"
		<<endl;

 }