Script.radius=0
Script.backgroundcolor = Vec4(0.2,0.2,0.2,1)
Script.bordercolor = Vec4(0,0,0,1)
Script.imagecolor = Vec4(1,1,1,1)
Script.bordertype = 0
Script.drawboarder = false
Script.drawbackground = true

function Script:SetColor(r,g,b,a,index)
	self.backgroundcolor.r=r
	self.backgroundcolor.g=g
	self.backgroundcolor.b=b
	self.backgroundcolor.a=a
end

function Script:SetStyle(style)
	if self:bitand(style,Style.Panel.Group) then
		self.widget:SetPadding(8,8,8,8)	
	elseif self:bitand(style,Style.Panel.Raised) or self:bitand(style,Style.Panel.Border) then
		self.widget:SetPadding(1,1,1,1)
	else
		self.widget:SetPadding(0,0,0,0)		
	end
end

function Script:MouseDown()
	self.widget:Redraw()
end

function Script:Adjust()
	if self.border then
		self.widget:SetPadding(1,1,1,1)
	end
end

function Script:Draw(x,y,width,height)	
	local gui = self.widget:GetGUI()
	local pos = self.widget:GetPosition(true)
	local sz = self.widget:GetSize(true)
	local scale = self.widget:GetGUI():GetScale()
	
	if self.widget:GetParent()==nil then
		gui:SetColor(0,0,0,0)
		gui:Clear()
	end

	if self.drawbackground then
		gui:SetColor(self.backgroundcolor.r,self.backgroundcolor.g,self.backgroundcolor.b,self.backgroundcolor.a)
		gui:DrawRect(pos.x,pos.y,sz.width,sz.height,0,self.radius)
	end

	--Draw image if present
	local image = self.widget:GetImage()
	if image~=nil then
		local imgsz = image:GetSize()
		imgsz.x = imgsz.x * scale
		imgsz.y = imgsz.y * scale

		gui:SetColor(self.imagecolor.r,self.imagecolor.g,self.imagecolor.b,self.imagecolor.a)
		gui:DrawImage(image,pos.x,pos.y,sz.x,sz.y)
	end

	--Draw border
	if self.drawboarder then
		if self.bordertype > 0 then
			gui:SetColor(self.bordercolor.r,selfbordercolor.g,self.bordercolor.b,self.bordercolor.a)
			local thickness = 2 * scale
			gui:DrawRect(pos.x, pos.y, sz.width, thickness, 0, 0)
			gui:DrawRect(pos.x, pos.y + sz.height - thickness, sz.width, thickness, 0, 0)
		else 
			gui:SetColor(self.bordercolor.r,self.bordercolor.g,selfbordercolor.b,self.bordercolor.a)
			gui:DrawRect(pos.x,pos.y,sz.width,sz.height,1,self.radius)
		end
	end
end