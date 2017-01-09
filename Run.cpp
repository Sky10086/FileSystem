#include"FileSystemDefine.h"
#include<sstream>
using namespace std;
#define Correct 0 //����
#define Error 1 //�ڴ��������ϵͳ����
#define SameName 2 //��ǰĿ¼�Ѿ��д������ļ�����Ŀ¼
#define None 4//��ǰĿ¼Ϊ��

#define No 5 //��ǰĿ¼��Ϊ�յ��޴��ļ�����Ŀ¼
int main()
{
	FileSystem myFileSystem;
	SFloder *pCurFloder = myFileSystem.getRootFloder();
	cout<<"************************************************************"<<endl;
	cout<<"**"<<setw(16)<<" "<<"��ӭʹ�ü����ļ�ϵͳ!"<<setw(21)<<"**"<<endl;
	cout<<"**"<<setw(58)<<"**"<<endl;
	cout<<"**"<<"                      writen by ������      "<<setw(14)<<"**"<<endl;
	cout<<"**"<<"                      version 1.0           "<<setw(14)<<"**"<<endl;
	cout<<"************************************************************"<<endl;
	cout<<endl;
	myFileSystem.InputHelp();
	while(1)
	{
		cout<<">>";
		string line;
		stringstream input;
		vector<string> inputVec;

		input.clear();
		getline(cin,line);
		input.str(line);
		line.clear();
		while(input>>line)
		{
			inputVec.push_back(line);
		}
		int inputSize = inputVec.size();
		if(3 < inputSize)
		{
			cout<<"������������������࣬���ֻ�������������������䣡"<<endl;
			continue;
		}
		if(inputSize == 0)
		{
			cout<<"������������������٣�������һ�������������䣡"<<endl;
			continue;
		}
		//������1��~3��֮�䣬����������
		if("mkdir" == inputVec[0])
		{
			if(inputSize < 2 || inputSize > 2)
			{
				cout<<"����mkdir ��ֻ�ܸ�һ���������ò���Ϊ�½���Ŀ¼�����ο�����������mkdir ��"<<endl;
				continue;
			}
			if("?" == inputVec[1])
			{
				cout<<"mkdir [FloderName]"<<endl
					<<"mkdir ���Ŀ¼����FloderName����ʾ�ڵ�ǰĿ¼���½�һ����ΪFloderName��Ŀ¼"
					<<endl;
				continue;
			}
			int result = myFileSystem.CreateFloder(inputVec[1],pCurFloder);
			//��������ļ��д������
			myFileSystem.ReturnCodeProc(result);
			continue;
		}
		else if("ls" == inputVec[0])
		{
			if(inputSize > 1 && "?" != inputVec[1])
			{
				cout<<"����ls���治���κβ������鿴ʹ�ð���������ls ��"<<endl;
				continue;
			}
			if(inputSize > 1 && "?" == inputVec[1])
			{
				cout<<"ls"<<endl
					<<"ls �󲻸��κβ�������ʾ�г���ǰĿ¼��������Ŀ¼���ļ�����"
					<<endl;
				continue;
			}
			//�г���Ŀ¼
			int sonSize = pCurFloder->vpSonFloder.size();
			cout<<left<<setw(30)<<"FloderName"<<left<<setw(20)<<"FloderSize"<<endl;
			for(int i = 0;i < sonSize;i++)
			{
				cout<<setw(30)<<pCurFloder->vpSonFloder[i]->szFloderName
					<<setw(20)<<pCurFloder->vpSonFloder[i]->iFloderSize<<endl;
			}
			cout<<endl;
			cout<<setw(30)<<"FileName"<<setw(20)<<"FileSize"<<endl;
			SFile *pTempFile = pCurFloder->pFileList;
			while(NULL != pTempFile)
			{
				cout<<setw(30)<<pTempFile->szFileName
					<<setw(20)<<pTempFile->iFileSize<<endl;
				pTempFile = pTempFile->pNextFile;
			}
		}
		else if("cd" == inputVec[0])
		{
			if(inputSize < 2 || inputSize > 2)
			{
				cout<<"����cd��ֻ�ܸ�һ���������ò���ΪĿ¼�����ο�����������cd ��"<<endl;
				continue;
			}
			if("?" == inputVec[1])
			{
				cout<<"cd [FloderName]"<<endl
					<<"cd ���Ŀ¼����FloderName����..����ʾ���뵱ǰĿ¼����ΪFloderName��Ŀ¼,��֮���..ʱ����ʾ���븸Ŀ¼"
					<<endl;
				continue;
			}
			if(".." == inputVec[1])
			{
				if(NULL == pCurFloder->pFatherFloder)
				{
					cout<<"��ǰĿ¼Ϊ��Ŀ¼���޸�Ŀ¼��"<<endl;
					continue;
				}
				else
				{
					pCurFloder = pCurFloder->pFatherFloder;
					continue;
				}

			}
			int sonSize = pCurFloder->vpSonFloder.size();
			bool isFind = false;
			for(int i = 0;i < sonSize;i++)
			{
				if(inputVec[1] == pCurFloder->vpSonFloder[i]->szFloderName)
				{
					pCurFloder = pCurFloder->vpSonFloder[i];
					isFind = true;
					break;
				}
			}
			if(false == isFind)
			{
				cout<<"��ǰĿ¼���޴�Ŀ¼��������ٴ����룡"<<endl;
				continue;
			}
			continue;
		}
		else if("rmdir" == inputVec[0])
		{
			if(inputSize < 2 || inputSize > 2)
			{
				cout<<"����rmdir��ֻ�ܸ�һ���������ò���ΪĿ¼�����ο�����������rmdir ��"<<endl;
				continue;
			}
			if("?" == inputVec[1])
			{
				cout<<"rmdir [FloderName]"<<endl
					<<"rmdir ���Ŀ¼����FloderName����ʾɾ����ǰĿ¼����ΪFloderName��Ŀ¼"
					<<endl;
				continue;
			}
			int result = myFileSystem.DeleteFloder(inputVec[1],pCurFloder);
			//�жϽ��
			myFileSystem.ReturnCodeProc(result);
			continue;
		}
		else if("renamedir" == inputVec[0])
		{
			if(inputSize == 1 || inputSize == 2 && inputVec[1] != "?")
			{
				cout<<"����renamedir����Ҫ�������������ο�����������renamedir ��"<<endl;
				continue;
			}
			if(inputSize == 2 && inputVec[1] == "?")
			{
				cout<<"renamedir [oldFloderName] [newFloderName]"<<endl
					<<"renamedir �������Ŀ¼����oldFloderName��newFloderName����ʾ����ǰĿ¼����ΪoldFloderName��Ŀ¼������ΪnewFloderName"
					<<endl;
				continue;
			}
			int result = myFileSystem.RenameFloder(inputVec[1],inputVec[2],pCurFloder);
			//��������
			myFileSystem.ReturnCodeProc(result);
			continue;
		}
		else if("cpdir" == inputVec[0])
		{
			if(inputSize < 2 || inputSize > 2)
			{
				cout<<"����cpdir��ֻ�ܸ�һ���������ò���ΪĿ¼�����ο�����������cpdir ��"<<endl;
				continue;
			}
			if("?" == inputVec[1])
			{
				cout<<"cpdir [FloderName]"<<endl
					<<"cpdir ���Ŀ¼����FloderName����ʾ����ǰĿ¼����ΪFloderName��Ŀ¼���Ƶ����а�"
					<<endl;
				continue;
			}
			int result = myFileSystem.CopyFloder(inputVec[1],pCurFloder);
			//�жϽ��
			myFileSystem.ReturnCodeProc(result);
			continue;
		}
		else if("pastedir" == inputVec[0])
		{
			if(inputSize > 1 && "?" != inputVec[1])
			{
				cout<<"����pastedir���治���κβ������鿴ʹ�ð���������pastedir ��"<<endl;
				continue;
			}
			if(inputSize > 1 && "?" == inputVec[1])
			{
				cout<<"pastedir"<<endl
					<<"pastedir �󲻸��κβ�������ʾ���������Ŀ¼���Ƶ���ǰĿ¼ "
					<<endl;
				continue;
			}
			int result = myFileSystem.PasteFloder(myFileSystem.getCopyFloderBuffer()->szFloderName,pCurFloder);
			myFileSystem.ReturnCodeProc(result);
			continue;
		}
		else if("mk" == inputVec[0])
		{
			if(inputSize < 2 || inputSize > 2)
			{
				cout<<"����mk��ֻ�ܸ�һ���������ò���Ϊ�ļ������ο�����������mk ��"<<endl;
				continue;
			}
			if(inputVec[1] == "?")
			{
				cout<<"mk [FileName]"<<endl
					<<"mk ����ļ�����FileName����ʾ�ڵ�ǰĿ¼���½�һ����ΪFileName���ļ�"
					<<endl;
				continue;
			}
			int result = myFileSystem.CreateFile(inputVec[1],pCurFloder);
			//��������
			myFileSystem.ReturnCodeProc(result);
			continue;
		}
		else if("rm" == inputVec[0])
		{
			if(inputSize < 2 || inputSize > 2)
			{
				cout<<"����rm��ֻ�ܸ�һ���������ò���Ϊ�ļ������ο�����������rm ��"<<endl;
				continue;
			}
			if(inputVec[1] == "?")
			{
				cout<<"rm [FileName]"<<endl
					<<"rm ����ļ�����FileName����ʾ�ڵ�ǰĿ¼��ɾ��һ����ΪFileName���ļ�"
					<<endl;
				continue;
			}
			int result = myFileSystem.DeleteFile(inputVec[1],pCurFloder);
			//��������
			myFileSystem.ReturnCodeProc(result);
			continue;
		}
		else if("rename" == inputVec[0])
		{
			if(inputSize == 1 || inputSize == 2 && inputVec[1] != "?")
			{
				cout<<"����rename����Ҫ�������������ο�����������rename ��"<<endl;
				continue;
			}
			if(inputSize == 2 && inputVec[1] == "?")
			{
				cout<<"rename [oldFileName] [newFileName]"<<endl
					<<"rename �������Ŀ¼����oldFileName��newFileName����ʾ����ǰĿ¼����ΪoldFileName���ļ�������ΪnewFileName"
					<<endl;
				continue;
			}
			int result = myFileSystem.RenameFile(inputVec[1],inputVec[2],pCurFloder);
			//��������
			myFileSystem.ReturnCodeProc(result);
			continue;
		}
		else if("write" == inputVec[0])
		{
			if(inputSize < 2 || inputSize > 2)
			{
				cout<<"����write��ֻ�ܸ�һ���������ò���Ϊ�ļ������ο�����������write ��"<<endl;
				continue;
			}
			if(inputVec[1] == "?")
			{
				cout<<"write [FileName]"<<endl
					<<"write ����ļ�����FileName����ʾ�޸��ڵ�ǰĿ¼����ΪFileName���ļ�����"
					<<endl;
				continue;
			}
			int result = myFileSystem.WriteFile(inputVec[1],pCurFloder);
			//������������
			myFileSystem.ReturnCodeProc(result);
			continue;
		}
		else if("read" == inputVec[0])
		{
			if(inputSize < 2 || inputSize > 2)
			{
				cout<<"����read��ֻ�ܸ�һ���������ò���Ϊ�ļ������ο�����������read ��"<<endl;
				continue;
			}
			if(inputVec[1] == "?")
			{
				cout<<"read [FileName]"<<endl
					<<"read ����ļ�����FileName����ʾ�鿴�ڵ�ǰĿ¼����ΪFileName���ļ�����"
					<<endl;
				continue;
			}
			int result = myFileSystem.ReadFile(inputVec[1],pCurFloder);
			//������������
			myFileSystem.ReturnCodeProc(result);
			continue;
		}
		else if("cp" == inputVec[0])
		{
			if(inputSize < 2 || inputSize > 2)
			{
				cout<<"����cp���һ����������ʾҪ���Ƶ��ļ���,�鿴����������cp ?"<<endl;
				continue;
			}
			if(inputVec[1] == "?")
			{
				cout<<"cp [FileName]"<<endl
					<<"cp ����ļ�����cp����ʾ�����ڵ�ǰĿ¼����ΪFileName���ļ���������"
					<<endl;
				continue;
			}
			int result = myFileSystem.CopyFile(inputVec[1],pCurFloder);
			myFileSystem.ReturnCodeProc(result);
			continue;
		}
		else if("paste" == inputVec[0])
		{
			if(inputSize > 1 && "?" != inputVec[1])
			{
				cout<<"����paste���治���κβ������鿴ʹ�ð���������paste ��"<<endl;
				continue;
			}
			if(inputSize > 1 && "?" == inputVec[1])
			{
				cout<<"paste"<<endl
					<<"paste �󲻸��κβ�������ʾ����������ļ����Ƶ���ǰĿ¼��"
					<<endl;
				continue;
			}
			int result = myFileSystem.PasteFile(myFileSystem.getCopyFileBuffer()->szFileName,pCurFloder);
			myFileSystem.ReturnCodeProc(result);
			continue;
		}
		else if("clear" == inputVec[0])
		{
			if(inputSize > 1 && "?" != inputVec[1])
			{
				cout<<"����clear���治���κβ������鿴ʹ�ð���������clear ��"<<endl;
				continue;
			}
			if(inputSize > 1 && "?" == inputVec[1])
			{
				cout<<"clear"<<endl
					<<"clear �󲻸��κβ�������ʾ����"
					<<endl;
				continue;
			}
			system("cls");
		}
		else if("exit" == inputVec[0])
		{
			if(inputSize > 1 && "?" != inputVec[1])
			{
				cout<<"����exit���治���κβ������鿴ʹ�ð���������exit ��"<<endl;
				continue;
			}
			if(inputSize > 1 && "?" == inputVec[1])
			{
				cout<<"exit"<<endl
					<<"exit �󲻸��κβ�������ʾ�˳����ļ�ϵͳ"
					<<endl;
				continue;
			}
			break;
		}
		else if("help" == inputVec[0])
		{
			myFileSystem.InputHelp();
			continue;
		}
		else if ("pwd" == inputVec[0])
		{
			cout << "��ǰĿ¼Ϊ��"<<endl << pCurFloder->szFloderName << endl;
			continue;
		}
		else
		{
			cout<<"����������Ч��������help�鿴����˵����"<<endl;
			continue;
		}
	}
	return 0;
}
