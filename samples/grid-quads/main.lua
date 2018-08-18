----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 512, 512 )

viewport = MOAIViewport.new ()
viewport:setSize ( 512, 512 )
viewport:setScale ( 512, 512 )

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass ()

grid = MOAIGrid.new ()
grid:initDiamondGrid ( 4, 4, 128, 128 )
grid:setRepeat ( true )

grid:setRow ( 1, 	0x01, 0x02, 0x01, 0x02 )
grid:setRow ( 2,	0x03, 0x04, 0x03, 0x04 )
grid:setRow ( 3,	0x01, 0x02, 0x01, 0x02 )
grid:setRow ( 4,	0x03, 0x04, 0x03, 0x04 )

tileDeck = MOAITileDeck2D.new ()
tileDeck:setTexture ( "diamond-tiles.png" )
tileDeck:setSize ( 4, 4 )
tileDeck:setQuad ( 0, 0.5, 0.5, 0, 0, -0.5, -0.5, 0 )
tileDeck:setUVQuad ( 0, 0.5, 0.5, 0, 0, -0.5, -0.5, 0 )

prop = MOAIProp.new ()
prop:setDeck ( tileDeck )
prop:setGrid ( grid )
prop:forceUpdate ()
prop:setPartition ( layer )
