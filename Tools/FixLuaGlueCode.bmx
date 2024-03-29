Local file:String="lua-gluecode.cpp"
Local in:TStream=ReadFile(file)
Local out:TStream=WriteFile(file+".temp")
While Not in.Eof()
	Local s:String=in.ReadLine()
	s=s.Replace("Window::","Leadwerks::Window::")
	s=s.Replace("Font::","Leadwerks::Font::")
	s=s.Replace("Time::","Leadwerks::Time::")
	s=s.Replace("Color::","Leadwerks::Color::")
	s=s.Replace("Brush::","Leadwerks::Brush::")
	s=s.Replace("Window*","Leadwerks::Window*")
	s=s.Replace("Font*","Leadwerks::Font*")
	s=s.Replace("Time*","Leadwerks::Time*")
	s=s.Replace("Component*","Leadwerks::Component*")
	s=s.Replace("Color*","Leadwerks::Color*")
	s=s.Replace("Brush*","Leadwerks::Brush*")
	out.WriteLine s
Wend
in.close()
out.close()
DeleteFile file
If Not RenameFile(file+".temp",file) Print "Failed to rename file."