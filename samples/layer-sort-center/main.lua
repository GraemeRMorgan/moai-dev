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
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )

function addProp ( x, y )
	local prop = MOAIProp.new ()
	prop:setDeck ( gfxQuad )
	prop:setLoc ( x, y )
	prop:setPartition ( layer )
end

addProp ( -32, -32 )
addProp ( -40, 40 )
addProp ( 48, -48 )
addProp ( 56, 56 )
addProp ( 0, 0 )

-- sort using prop location in camera space
layer:setSortMode ( MOAIPartitionViewLayer2D.SORT_DIST_SQUARED_DESCENDING, true )
layer:setSortScale ( 0, 0, 0, 0 )

