----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass ()

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "../resources/moai.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )

prop = MOAIProp.new ()
prop:setDeck ( gfxQuad )
prop:setPartition ( layer )

function threadFunc ()
 
    local action
 
    action = prop:moveRot ( 180, 3 )
    MOAIThread.blockOnAction ( action )
 
    action = prop:moveLoc ( 64, 0, 2 ) 
    MOAIThread.blockOnAction ( action )
 
    action = prop:moveScl ( -0.5, -0.5, 1 ) 
    MOAIThread.blockOnAction ( action )

	action = prop:moveRot ( -180, 3 )
    MOAIThread.blockOnAction ( action )
	
end

thread = MOAIThread.new ()
thread:run ( threadFunc )

