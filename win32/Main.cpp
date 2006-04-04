#include "StdAfx.h"
#include "Main.h"

Main *mainSingleton=NULL;

char **split_args(IO &io,wchar_t* cmdLine,int *pargc)
{
	int length=wcslen(cmdLine);

	int argCount=0;
	for (int i=0;i<length;i++)
		if (*(cmdLine+i)==L' ')
			argCount++;
	argCount++;

	char **argv=(char**)io.malloc(4*argCount);
	int last=0;
	int argNo=0;
	for (int i=0;i<=length;i++)
	{
		wchar_t ch=*(cmdLine+i);
		if (ch==L' '||ch==0)
		{
			int argLength=i-last;
			char *arg=(char*)io.malloc(argLength+1);
			wcstombs(arg,cmdLine+last,argLength);
			arg[argLength]=0;
			argv[argNo]=arg;

			argNo++;
			last=i+1;
		}
	}
	*pargc=argCount;
	return argv;
}

void showCmds(IO &io,char *args)
{
	if (mainSingleton!=NULL)
	{
		io.println("Available commands:");
		for (int i=0;i<mainSingleton->funcc;i++)
		{
			io.println(mainSingleton->commands[i].name);
		}
	}
}

void showArgs(IO &io,char *args)
{
	if (mainSingleton!=NULL)
	{
		io.println("Commandline arguments:");
		for (int i=0;i<mainSingleton->argc;i++)
		{
			io.println((char*)mainSingleton->argv[i]);
		}
	}
}

Main::Main(IO &io,int argc,char** argv):io(io),funcc(0),argv(argv),argc(argc)
{
	addCommand("showCmds",showCmds);
	addCommand("showArgs",showArgs);
	if (mainSingleton!=NULL)
	{
		io.println("Error: Main may be instantiated only once");
		return;
	}
	else
	{
		mainSingleton=this;
	}
}

Main::~Main(void)
{
	mainSingleton=NULL;
}

void Main::run()
{
	showSplashScreen();
	rpl();
}
void Main::rpl()
{
	io.println("Starting RPL (Read-Print-Loop) Type \"exit\" to stop.");

	char *buffer=(char*)io.malloc(100);
	buffer[0]=0;

	while (strcmp(buffer,"exit")!=0)
	{
		io.print("rpl> ");
		io.readln(buffer,100);
		io.debugout("reaching command checking");
		int i;
		for(i=0;i<funcc;i++)
		{
			char *cmd=commands[i].name;
			unsigned int length=strlen(cmd);
			
			if (strlen(buffer)>=length&&strstr(buffer,cmd)!=0&&(buffer[length]==0||buffer[length]==' '))
			{
				commands[i].func(io,buffer+strlen(commands[i].name));
				break;
			}
		}

		if (i>=funcc)
		{
			io.print("Befehl konnte nicht gefunden werden: ");
			io.println(buffer);
		}
	}

	io.println("Exiting RPL");
}
void Main::showSplashScreen()
{
	io.println("Boot-time Computername Changer by Johannes Rudolph");
	io.println("v0.1");
	io.print("IO: ");
	io.println(io.getVersion());
}

void Main::addCommand(char *name,invokeFunc func)
{
	if (funcc>=maxFuncs)
	{
		io.println("Es kann kein neues Kommando hinzugef�gt werden");
		return;
	}

	command c;
	c.func=func;
	c.name=name;

	commands[funcc]=c;

	funcc++;
}
