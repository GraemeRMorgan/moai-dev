----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------


image = MOAIImage.new ()
image:load ( "moai.png" )

image:fillRect ( 32, 32, 96, 96, 1, 1, 0, 1 )

MOAISim.openWindow ( "test", 128, 128 )

viewport = MOAIViewport.new ()
viewport:setSize ( 128, 128 )
viewport:setScale ( 128, -128 )

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass ()


gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( image )
gfxQuad:setRect ( -64, -64, 64, 64 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp.new ()
prop:setDeck ( gfxQuad )
prop:setPartition ( layer )
