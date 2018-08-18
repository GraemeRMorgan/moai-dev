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

texture = MOAITexture.new ()
texture:load ( "moai.png" )
texture:setWrap ( true )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( texture )
gfxQuad:setRect ( -64, -64, 64, 64 )

uvTransform = MOAITransform2D.new ()
uvTransform:moveLoc ( 1, 0, 3 )
uvTransform:moveRot ( 360, 3 )
uvTransform:moveScl ( 1, 1, 3 )

prop = MOAIProp.new ()
prop:setDeck ( gfxQuad )
prop:setUVTransform ( uvTransform )
prop:setPartition ( layer )

prop:moveRot ( -360, 2 )
prop:moveScl ( 1.5, 1.5, 3 )

