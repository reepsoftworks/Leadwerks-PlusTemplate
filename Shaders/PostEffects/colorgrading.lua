--------------------------------------------------------
--Color grading
--------------------------------------------------------

--Called once at start
function Script:Start()
	
	--Load this script's shaders
	self.shader = Shader:Load("Shaders/PostEffects/colorgrading.shader")
	
	--Change this to another lookup table if you want
	self.lookuptable = Texture:Load("Shaders/PostEffects/Textures/lut_exposurecorrection.tex")
	
end

--Called each time the camera is rendered
function Script:Render(camera,context,buffer,depth,diffuse,normals,emission)
	
	--Bind images
	buffer:Enable()
	diffuse:Bind(0)
	if self.lookuptable~=nil then
		self.lookuptable:Bind(4)
	end
	
	--Enable the shader and draw the diffuse image onscreen
	if self.shader then self.shader:Enable() end
	context:DrawRect(0,0,buffer:GetWidth(),buffer:GetHeight())
	
end

--Called when the effect is detached or the camera is deleted
function Script:Detach()
	
	if self.shader~=nil then
		self.shader:Release()
		self.shader = nil	
	end
	
	if self.lookuptable~=nil then
		self.lookuptable:Release()
		self.lookuptable = nil	
	end
	
end