What it does:
Scans all header files, generates a ToLua++ pkg file, and generates Lua binding code.

How to use:
Extract all files to your source code directory and run tolua++.bat

To expose a class you must add //lua after the class and after any members or methods you want to expose.

Example:

class foo//lua
{
	public:
	int thing;//lua
	void bar();//lua
};