#include"FileSystemDefine.h"
#include<sstream>
using namespace std;
#define Correct 0 //正常
#define Error 1 //内存分配错误等系统错误
#define SameName 2 //当前目录已经有此命名文件或者目录
#define None 4//当前目录为空

#define No 5 //当前目录不为空但无此文件或者目录
int main()
{
	FileSystem myFileSystem;
	SFloder *pCurFloder = myFileSystem.getRootFloder();
	cout<<"************************************************************"<<endl;
	cout<<"**"<<setw(16)<<" "<<"欢迎使用简易文件系统!"<<setw(21)<<"**"<<endl;
	cout<<"**"<<setw(58)<<"**"<<endl;
	cout<<"**"<<"                      writen by 王炳琦      "<<setw(14)<<"**"<<endl;
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
			cout<<"错误，输入命令参数过多，最多只能有三个参数！请重输！"<<endl;
			continue;
		}
		if(inputSize == 0)
		{
			cout<<"错误，输入命令参数过少，至少有一个参数！请重输！"<<endl;
			continue;
		}
		//参数在1个~3个之间，正常，继续
		if("mkdir" == inputVec[0])
		{
			if(inputSize < 2 || inputSize > 2)
			{
				cout<<"错误，mkdir 后只能跟一个参数，该参数为新建的目录名，参看帮助请输入mkdir ？"<<endl;
				continue;
			}
			if("?" == inputVec[1])
			{
				cout<<"mkdir [FloderName]"<<endl
					<<"mkdir 后跟目录名称FloderName，表示在当前目录下新建一个名为FloderName的目录"
					<<endl;
				continue;
			}
			int result = myFileSystem.CreateFloder(inputVec[1],pCurFloder);
			//后面加上文件夹创建结果
			myFileSystem.ReturnCodeProc(result);
			continue;
		}
		else if("ls" == inputVec[0])
		{
			if(inputSize > 1 && "?" != inputVec[1])
			{
				cout<<"错误，ls后面不带任何参数，查看使用帮助请输入ls ？"<<endl;
				continue;
			}
			if(inputSize > 1 && "?" == inputVec[1])
			{
				cout<<"ls"<<endl
					<<"ls 后不跟任何参数，表示列出当前目录下所有子目录和文件详情"
					<<endl;
				continue;
			}
			//列出子目录
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
				cout<<"错误，cd后只能跟一个参数，该参数为目录名，参看帮助请输入cd ？"<<endl;
				continue;
			}
			if("?" == inputVec[1])
			{
				cout<<"cd [FloderName]"<<endl
					<<"cd 后跟目录名称FloderName或者..，表示进入当前目录下名为FloderName的目录,当之后跟..时，表示进入父目录"
					<<endl;
				continue;
			}
			if(".." == inputVec[1])
			{
				if(NULL == pCurFloder->pFatherFloder)
				{
					cout<<"当前目录为根目录，无父目录！"<<endl;
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
				cout<<"当前目录下无此目录，请检查后再次输入！"<<endl;
				continue;
			}
			continue;
		}
		else if("rmdir" == inputVec[0])
		{
			if(inputSize < 2 || inputSize > 2)
			{
				cout<<"错误，rmdir后只能跟一个参数，该参数为目录名，参看帮助请输入rmdir ？"<<endl;
				continue;
			}
			if("?" == inputVec[1])
			{
				cout<<"rmdir [FloderName]"<<endl
					<<"rmdir 后跟目录名称FloderName，表示删除当前目录下名为FloderName的目录"
					<<endl;
				continue;
			}
			int result = myFileSystem.DeleteFloder(inputVec[1],pCurFloder);
			//判断结果
			myFileSystem.ReturnCodeProc(result);
			continue;
		}
		else if("renamedir" == inputVec[0])
		{
			if(inputSize == 1 || inputSize == 2 && inputVec[1] != "?")
			{
				cout<<"错误，renamedir后需要跟两个参数，参看帮助请输入renamedir ？"<<endl;
				continue;
			}
			if(inputSize == 2 && inputVec[1] == "?")
			{
				cout<<"renamedir [oldFloderName] [newFloderName]"<<endl
					<<"renamedir 后跟两个目录名称oldFloderName和newFloderName，表示将当前目录下名为oldFloderName的目录重命名为newFloderName"
					<<endl;
				continue;
			}
			int result = myFileSystem.RenameFloder(inputVec[1],inputVec[2],pCurFloder);
			//处理结果码
			myFileSystem.ReturnCodeProc(result);
			continue;
		}
		else if("cpdir" == inputVec[0])
		{
			if(inputSize < 2 || inputSize > 2)
			{
				cout<<"错误，cpdir后只能跟一个参数，该参数为目录名，参看帮助请输入cpdir ？"<<endl;
				continue;
			}
			if("?" == inputVec[1])
			{
				cout<<"cpdir [FloderName]"<<endl
					<<"cpdir 后跟目录名称FloderName，表示将当前目录下名为FloderName的目录复制到剪切板"
					<<endl;
				continue;
			}
			int result = myFileSystem.CopyFloder(inputVec[1],pCurFloder);
			//判断结果
			myFileSystem.ReturnCodeProc(result);
			continue;
		}
		else if("pastedir" == inputVec[0])
		{
			if(inputSize > 1 && "?" != inputVec[1])
			{
				cout<<"错误，pastedir后面不带任何参数，查看使用帮助请输入pastedir ？"<<endl;
				continue;
			}
			if(inputSize > 1 && "?" == inputVec[1])
			{
				cout<<"pastedir"<<endl
					<<"pastedir 后不跟任何参数，表示将剪贴板的目录复制到当前目录 "
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
				cout<<"错误，mk后只能跟一个参数，该参数为文件名，参看帮助请输入mk ？"<<endl;
				continue;
			}
			if(inputVec[1] == "?")
			{
				cout<<"mk [FileName]"<<endl
					<<"mk 后跟文件名称FileName，表示在当前目录下新建一个名为FileName的文件"
					<<endl;
				continue;
			}
			int result = myFileSystem.CreateFile(inputVec[1],pCurFloder);
			//处理结果码
			myFileSystem.ReturnCodeProc(result);
			continue;
		}
		else if("rm" == inputVec[0])
		{
			if(inputSize < 2 || inputSize > 2)
			{
				cout<<"错误，rm后只能跟一个参数，该参数为文件名，参看帮助请输入rm ？"<<endl;
				continue;
			}
			if(inputVec[1] == "?")
			{
				cout<<"rm [FileName]"<<endl
					<<"rm 后跟文件名称FileName，表示在当前目录下删除一个名为FileName的文件"
					<<endl;
				continue;
			}
			int result = myFileSystem.DeleteFile(inputVec[1],pCurFloder);
			//处理返回码
			myFileSystem.ReturnCodeProc(result);
			continue;
		}
		else if("rename" == inputVec[0])
		{
			if(inputSize == 1 || inputSize == 2 && inputVec[1] != "?")
			{
				cout<<"错误，rename后需要跟两个参数，参看帮助请输入rename ？"<<endl;
				continue;
			}
			if(inputSize == 2 && inputVec[1] == "?")
			{
				cout<<"rename [oldFileName] [newFileName]"<<endl
					<<"rename 后跟两个目录名称oldFileName和newFileName，表示将当前目录下名为oldFileName的文件重命名为newFileName"
					<<endl;
				continue;
			}
			int result = myFileSystem.RenameFile(inputVec[1],inputVec[2],pCurFloder);
			//处理结果码
			myFileSystem.ReturnCodeProc(result);
			continue;
		}
		else if("write" == inputVec[0])
		{
			if(inputSize < 2 || inputSize > 2)
			{
				cout<<"错误，write后只能跟一个参数，该参数为文件名，参看帮助请输入write ？"<<endl;
				continue;
			}
			if(inputVec[1] == "?")
			{
				cout<<"write [FileName]"<<endl
					<<"write 后跟文件名称FileName，表示修改在当前目录下名为FileName的文件内容"
					<<endl;
				continue;
			}
			int result = myFileSystem.WriteFile(inputVec[1],pCurFloder);
			//处理结果返回码
			myFileSystem.ReturnCodeProc(result);
			continue;
		}
		else if("read" == inputVec[0])
		{
			if(inputSize < 2 || inputSize > 2)
			{
				cout<<"错误，read后只能跟一个参数，该参数为文件名，参看帮助请输入read ？"<<endl;
				continue;
			}
			if(inputVec[1] == "?")
			{
				cout<<"read [FileName]"<<endl
					<<"read 后跟文件名称FileName，表示查看在当前目录下名为FileName的文件内容"
					<<endl;
				continue;
			}
			int result = myFileSystem.ReadFile(inputVec[1],pCurFloder);
			//处理结果返回码
			myFileSystem.ReturnCodeProc(result);
			continue;
		}
		else if("cp" == inputVec[0])
		{
			if(inputSize < 2 || inputSize > 2)
			{
				cout<<"错误，cp后跟一个参数，表示要复制的文件名,查看帮助请输入cp ?"<<endl;
				continue;
			}
			if(inputVec[1] == "?")
			{
				cout<<"cp [FileName]"<<endl
					<<"cp 后跟文件名称cp，表示复制在当前目录下名为FileName的文件到剪贴板"
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
				cout<<"错误，paste后面不带任何参数，查看使用帮助请输入paste ？"<<endl;
				continue;
			}
			if(inputSize > 1 && "?" == inputVec[1])
			{
				cout<<"paste"<<endl
					<<"paste 后不跟任何参数，表示将剪贴板的文件复制到当前目录下"
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
				cout<<"错误，clear后面不带任何参数，查看使用帮助请输入clear ？"<<endl;
				continue;
			}
			if(inputSize > 1 && "?" == inputVec[1])
			{
				cout<<"clear"<<endl
					<<"clear 后不跟任何参数，表示清屏"
					<<endl;
				continue;
			}
			system("cls");
		}
		else if("exit" == inputVec[0])
		{
			if(inputSize > 1 && "?" != inputVec[1])
			{
				cout<<"错误，exit后面不带任何参数，查看使用帮助请输入exit ？"<<endl;
				continue;
			}
			if(inputSize > 1 && "?" == inputVec[1])
			{
				cout<<"exit"<<endl
					<<"exit 后不跟任何参数，表示退出此文件系统"
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
			cout << "当前目录为："<<endl << pCurFloder->szFloderName << endl;
			continue;
		}
		else
		{
			cout<<"输入命令无效，请输入help查看命令说明。"<<endl;
			continue;
		}
	}
	return 0;
}
