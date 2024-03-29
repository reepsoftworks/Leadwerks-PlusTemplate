

'Open header file
Global cleanheaderfile = WriteFile("luacommands.pkg")
If Not cleanheaderfile RuntimeError "could not open file luacommands.pkg"

'Write necessary header functions
WriteLine cleanheaderfile,"$#include ~qLeadwerks.h~q" 
WriteLine cleanheaderfile,"$using namespace Leadwerks;"
WriteLine cleanheaderfile,""

'Traverse directory and write all marked functions
TraverseFiles(CurrentDir())

'Assign classes
For Local class:TClass=EachIn TClass.list
	If class.parentname
		class.parent = TClass.Find(class.parentname)
		'If class.parent Print class.parent.name
	EndIf
Next

'Sort classes
'TClass.list.sort()

'Write the data	
For class=EachIn TClass.list
	class.Write(cleanheaderfile)
Next

'WriteLine cleanheaderfile,"bool dofile(const std::string& path);"
'WriteLine cleanheaderfile,"bool require(const std::string& path);"
WriteLine cleanheaderfile,"bool import(const std::string& path);"

CloseFile cleanheaderfile

Type TClass
	Global map:TMap=New TMap
	Global list:TList=New TList
	
	Field written:Int
	Field name:String
	Field parent:TClass
	Field parentname:String
	Field members:TList=New TList
		
	Function Create:TClass(name:String)
		Local class:TClass=New TClass
		class.name=name
		list.AddLast(class)
		map.Insert name.Trim(),class
		Return class
	EndFunction
	
	Function Find:TClass(name:String)
		Return TClass(map.valueforkey(name.Trim()))
	EndFunction
	
	Rem
	Method ContainsParent:Int(class:TClass)
		If parent
			If parent=class
				Return True
			EndIf
			Return parent.ContainsParent(class)
		EndIf
		Return False
	EndMethod
	
	Method Compare:Int(o:Object)
		Local class:TClass=TClass(o)
		If class.ContainsParent(Self) Return -1
		If Self.ContainsParent(class) Return 1
		If class.name>Self.name Return 1
		If class.name<Self.name Return -1
		Return 0
	EndMethod
	EndRem
	
	Method Write(stream:TStream)
		If Not written
			Local baseclass:TClass=parent
			While baseclass
				baseclass.Write(stream)
				baseclass=baseclass.parent
			Wend
			If parentname
				stream.WriteLine "class "+name+" : public "+parentname	
			Else
				stream.WriteLine "class "+name		
			EndIf
			stream.WriteLine "{"
			For Local s:String=EachIn members
				's = s.Replace("const uint64_t","const unsigned long long")
				's = s.Replace("uint64_t","unsigned long long")
				s = s.Replace("GetEntryUserID_","GetEntryUserID_ @ GetEntryUserID")
				s = s.Replace("GetUserID_","GetUserID_ @ GetUserID")
				s = s.Replace("const uint64_t","const unsigned long long")
				s = s.Replace("uint64_t","unsigned long long")
				stream.WriteLine "	"+s
			Next
			stream.WriteLine "};"
			stream.WriteLine ""
			written=True
		EndIf
	EndMethod

EndType

'Helper Functions 
Function FindLua(inputFile:String)
	'Print "Analyzing ~q"+inputfile+"~q"

	file= ReadFile(inputFile)
	Local closureneeded = False
	Local class:TClass
	
	If Not file RuntimeError "could not open file"
	
	While Not Eof(file)
		line$ = ReadLine(file)
		OriginalLine$ = line
		'line = Lower(line)
		
		Local sarr:String[]
		line=line.Trim()
		sarr=line.Split("//")
		If sarr.length>1
			Local tag$ = sarr[sarr.length-1]
			If tag.ToLower()="lua"
				line=sarr[0].Trim()
				'If "class is in the string print the line then an open bracket else print the line tabbed twice
				'DebugStop
				If Instr(line, "class",1) <> 0
					Local sarr2:String[]=line.split("")
					class=TClass.Create(sarr2[1])					
					'class.name=sarr2[1]
					If sarr2.length>4
						class.parentname=sarr2[4].Trim()
					EndIf
					'Print class.name
					
					'WriteLine cleanheaderfile,sarr[sarr.length-2].Trim()
					'WriteLine cleanheaderfile,"{"
					'closureneeded = True
				Else
					If class'Assert class
						class.members.addlast(sarr[sarr.length-2].Trim())
					Else
						Print "Possible error in file ~q"+inputfile+"~q.  No class found."
					EndIf
					'Print sarr[sarr.length-2].Trim()
					'If closureneeded WriteLine cleanheaderfile,"~t" + sarr[sarr.length-2].Trim()
				EndIf
			EndIf
		EndIf
	Wend
	'If closureneeded
		'WriteLine cleanheaderfile,"};"
		'WriteLine cleanheaderfile,""
	'EndIf 
	
	'Extra global functions
	CloseStream file
EndFunction

Function TraverseFiles(root:String)
	
	dir = ReadDir(root)
	If Not dir RuntimeError "failed to read current directory"
	
	Repeat
		currentFile$ = NextFile(dir)
		If currentFile = "" Exit
		If currentFile = "." Or currentFile = ".." Continue
		If FileType(root + "\" +currentFile) = 2 
			'DebugStop
			TraverseFiles(root + "\" + currentFile)
		EndIf
		If ExtractExt(currentFile) = "h"
			'Print(root + "\" + currentFile)
			FindLua(root + "\" +currentFile)
		EndIf
	Forever
	
	CloseDir dir
	
EndFunction 

