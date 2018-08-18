----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------


image = MOAIImage.new ()
image:load ( "moai.png" )

w, h = image:getSize ()

image:copyRect ( image,
	0, 0, w * 0.5, h * 0.5,
	w * 0.5, 0, w, h,
	MOAIImage.FILTER_LINEAR
)

image:copyRect ( image,
	0, 0, w * 0.5, h,
	0, h * 0.5, w * 0.5, h,
	MOAIImage.FILTER_LINEAR
)

MOAISim.openWindow ( "test", w, h )

viewport = MOAIViewport.new ()
viewport:setSize ( w, h )
viewport:setScale ( w, -h )

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass ()

hw = w * 0.5
hh = h * 0.5

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( image )
gfxQuad:setRect ( -hw, -hh, hw, hh )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp.new ()
prop:setDeck ( gfxQuad )
prop:setPartition ( layer )

