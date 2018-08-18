----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass ()

texture = MOAIImageTexture.new ()
texture:init ( 16, 16 )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( texture )
gfxQuad:setRect ( 0, 0, 16, 16 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp.new ()
prop:setDeck ( gfxQuad )
prop:setPiv ( 8, 8 )
prop:setScl ( 8, 8 )
prop:setPartition ( layer )

prop:moveRot ( 360, 1.5 )

function getPointerLocInProp ()

	local x, y = prop:worldToModel ( layer:wndToWorld ( MOAIInputMgr.device.pointer:getLoc ()))
	--return math.floor ( x + 0.5 ), math.floor ( y - 0.5 )
	return x, y
end

function input ()

	while true do

		local x, y = getPointerLocInProp ()

		if MOAIInputMgr.device.mouseLeft:isDown () then
			texture:setRGBA ( x, y, 1, 1, 1, 1 )
			texture:invalidate ( x, y, x + 1, y + 1 )
		end

		if MOAIInputMgr.device.mouseRight:isDown () then
			texture:setRGBA ( x, y, 0, 0, 1, 1 )
			texture:invalidate ( x, y, x + 1, y + 1 )
		end

		coroutine.yield ()
	end
end

thread = MOAICoroutine.new ()
thread:run ( input )
