-- This file is generated by the pre-processor on 2024-04-07 14:44:53
-- Please do not do not modify this file!
Script.actor="Spinner"

-- Start Enabled
Script.enabled=true --bool "Start Enabled"

-- Spin Speed
Script.spinspeed=Vec3(0,0,0) --vec3 "Spin Speed"

--[[ Inputs ]]--
function Script:Enable() --in
end

function Script:Disable() --in
end

--[[ Outputs ]]--
function Script:Outputs()
	self.component:CallOutputs("OnTurn")
end
