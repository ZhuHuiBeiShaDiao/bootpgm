/* The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 * 
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 * License for the specific language governing rights and limitations
 * under the License.
 * 
 * The Initial Developer of the Original Code is Johannes Rudolph.
 * Portions created by the Initial Developer are Copyright (C) 2006
 * the Initial Developer. All Rights Reserved.
 * 
 * Contributor(s): 
 *    Johannes Rudolph <johannes_rudolph@gmx.de>
 */ 

#pragma once
#include "io.h"

typedef void (*invokeFunc)(IO &io,char *args);

struct command{
	invokeFunc func;
	void *pObject;
	char *name;
	char *description;
	char *help;
	void invoke(IO &io,char *args);
};

struct deleg
{
	void *func;
	void *object;
};

// template to create a command delegate
template<class T> deleg make_dg(T *t,void(T::*func)(IO&,char*))
{
	deleg dg;
	dg.func = *(void**)&func;
	dg.object = t;
	return dg;
}

class Main
{
	IO &io;
	
	int funcc;
	static const int maxFuncs=50;
	command commands[maxFuncs];

	char** argv;
	int argc;

	void help(IO &io,char *args);
	void showCmds(IO &io,char *args);	
	void showArgs(IO &io,char *args);

	command *findCommand(char *name);
public:
	Main(IO &io,int argc,char** argv);
	void run();
	~Main(void);
	void rpl();
	void addCommand(char *name,invokeFunc func,char *desc=0,char *help=0,void *pObject=0);
	void addCommand(char *name,deleg d,char *desc=0,char *help=0);

	void showSplashScreen();
	IO &get_io(){return io;}
private:
	friend void showCmds(IO &io,char *args);
	friend void showArgs(IO &io,char *args);

public:
	int getArgc()
	{
		return argc;
	}
	char **getArgs()
	{
		return argv;
	}
};

extern Main *mainSingleton;
char **split_args(IO &io,wchar_t* cmdLine,int length,int *pargc);