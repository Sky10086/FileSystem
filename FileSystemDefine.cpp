#include"FileSystemDefine.h"
using namespace std;
#define Correct 0 //正常
#define Error 1 //内存分配错误等系统错误
#define SameName 2 //当前目录已经有此命名文件或者目录
#define None 4//当前目录为空
#define No 5 //当前目录不为空但无此文件或者目录
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

SFloder::SimpleFloder(string name, SimpleFloder *cur)//普通构造函数
{
	szFloderName = name;
	iFloderSize = 0;
	pFatherFloder = cur;//指定父目录
	vector<SimpleFloder*> initV;//初始化向量
	vpSonFloder = initV;
	pFileList = NULL;
}
SFloder::SimpleFloder(const SimpleFloder &Floder)//复制构造函数
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
	//当前目录下有没有目录
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
		//重名检测
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
	//当前目录下无子目录
	if(CurFloder->vpSonFloder.size() == 0)
	{
		return None;
	}
	else
	{
		int flag = false;
		//寻找目录
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
	//当前目录下无子目录
	if(CurFloder->vpSonFloder.size() == 0)
	{
		return None;
	}
	else
	{
		int size = CurFloder->vpSonFloder.size();
		bool isFind = false;
		bool isNamed = false;
		//寻找当前目录下是否存在此目录
		for(int i= 0;i < size;i++)
		{
			if(CurFloder->vpSonFloder[i]->szFloderName == oldFloderName)
			{
				isFind = true;
				for(int j = 0;j < size;j++)
				{
					//检查当前目录下时候已存在和新名字相同的目录
					if(CurFloder->vpSonFloder[j]->szFloderName == newFloderName)
					{
						isNamed = true;
					}
				}
				//重名民
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
	//当前目录下无子目录
	if(CurFloder->vpSonFloder.size() == 0)
	{
		return None;
	}
	else
	{
		int isFind = false;
		//寻找目录
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
	//当前目录下有没有目录
	if(CurFloder->vpSonFloder.size() == 0)
	{
		if(this->pCopyFloderBuffer == NULL)
		{
			LogPrint("粘贴产生错误，复制缓冲区为空，请先选定复制目录!");
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
		//重名检测
		for(int i= 0;i < size;i++)
		{
			if(CurFloder->vpSonFloder[i]->szFloderName == FloderName)
			{
				return SameName;
			}
		}
		if(this->pCopyFloderBuffer == NULL)
		{
			LogPrint("粘贴产生错误，复制缓冲区为空，请先选定复制目录!");
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
		//头插法比尾插法好
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
				cout<<"当前文件内容为:"<<endl;
				if("" == pTempFile->szFileContent)
				{
					cout<<"当前文件无内容!"<<endl;
				}
				else
				{
					cout<<pTempFile->szFileContent<<endl;
				}
				cout<<"请输入需要写入的内容(会覆盖旧内容)"<<endl;
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
				cout<<"当前文件内容为:"<<endl;
				if("" == pTempFile->szFileContent)
				{
					cout<<"当前文件无内容!"<<endl;
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
			LogPrint("粘贴产生错误，复制缓冲区为空，请先选定复制文件!");
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
		//头插法比尾插法好
		if(this->pCopyFileBuffer == NULL)
		{
			LogPrint("粘贴产生错误，复制缓冲区为空，请先选定复制文件!");
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
			LogPrint("操作成功！");
			break;
		case Error:
			LogPrint("错误，内存分配失败!");
			break;
		case SameName:
			LogPrint("错误，该文件夹下已有该命名文件(或目录)，请重新取名");
			break;
		case None:
			LogPrint("错误，该文件夹下无文件(或目录),请先创建再操作!");
			break;
		case No:
			LogPrint("错误，该文件夹下无此名称文件(或目录),请检查后再重新输入!");
			break;
		default:
			break;
	}
}
 void FileSystem::InputHelp()
 {
	cout<<"目录操作命令:"<<endl;
	cout<<"mkdir [FloderName]"<<endl
		<<"  "<<"新建一个名为FloderName的目录"
		<<endl;
	cout<<"rmdir [FloderName]"<<endl
		<<"  "<<"删除当前目录下名为FloderName的目录"
		<<endl;
	cout<<"ls"<<endl
		<<"  "<<"列出当前目录下所有子目录和文件详情"
		<<endl;
	cout<<"cd [FloderName]"<<endl
		<<"  "<<"进入当前目录下名为FloderName的目录,当之后跟..时，表示进入父目录"
		<<endl;
	cout<<"renamedir [oldFloderName] [newFloderName]"<<endl
		<<"  "<<"将当前目录下名为oldFloderName的目录重命名为newFloderName"
		<<endl;
	cout<<"cpdir [FloderName]"<<endl
		<<"  "<<"复制当前目录下名为FloderName的目录到剪贴板"
		<<endl;
	cout<<"pastedir"<<endl
		<<"  "<<"将剪贴板的目录复制到当前目录"
		<<endl;
	cout << "pwd" << endl
		<< "  " << "显示当前目录名称"
		<< endl;
	cout<<endl;
	cout<<"文件操作命令"<<endl;
	cout<<"mk [FileName]"<<endl
		<<"  "<<"新建一个名为FileName的文件"
		<<endl;
	cout<<"rm [FileName]"<<endl
		<<"  "<<"删除一个名为FileName的文件"
		<<endl;
	cout<<"rename [oldFileName] [newFileName]"<<endl
		<<"  "<<"将当前目录下名为oldFileName的文件重命名为newFileName"
		<<endl;
	cout<<"write [FileName]"<<endl
		<<"  "<<"修改在当前目录下名为FileName的文件内容"
		<<endl;
	cout<<"read [FileName]"<<endl
		<<"  "<<"查看在当前目录下名为FileName的文件内容"
		<<endl;
	cout<<"cp [FileName]"<<endl
		<<"  "<<"复制当前目录下名为FileName的文件到剪贴板"
		<<endl;
	cout<<"paste"<<endl
		<<"  "<<"将剪贴板的文件复制到当前目录"
		<<endl;
	cout<<endl;
	cout<<"系统命令"<<endl;
	cout<<"help"<<endl
		<<"  "<<"help 输出帮助信息"<<endl;
	cout<<"clear"<<endl
		<<"  "<<"clear 后不跟任何参数，表示清屏"
		<<endl;
	cout<<"exit"<<endl
		<<"  "<<"exit 后不跟任何参数，表示退出此文件系统"
		<<endl;

 }