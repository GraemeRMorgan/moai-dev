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

stretchPatch = MOAIStretchPatch2D.new ()
stretchPatch:setTexture ( '../resources/numbers.png' )

stretchPatch:reserveRows ( 3 )
stretchPatch:setRow ( 1, 0.25, false )
stretchPatch:setRow ( 2, 0.50, true )
stretchPatch:setRow ( 3, 0.25, false )

stretchPatch:reserveColumns ( 3 )
stretchPatch:setColumn ( 1, 0.25, false )
stretchPatch:setColumn ( 2, 0.50, true )
stretchPatch:setColumn ( 3, 0.25, false )

stretchPatch:reserveUVRects ( 4 )
stretchPatch:setUVRect ( 1, 0, 0.125, 0.125, 0 )
stretchPatch:setUVRect ( 2, 0.125, 0.125, 0.25, 0 )
stretchPatch:setUVRect ( 3, 0.25, 0.125, 0.375, 0 )
stretchPatch:setUVRect ( 4, 0.375, 0.125, 0.5, 0 )

stretchPatch:setRect ( -64, -64, 64, 64 )

prop = MOAIGraphicsProp.new ()
prop:setDeck ( stretchPatch )
prop:setPartition ( layer )

curve = MOAIAnimCurveIndex.new ()

curve:reserveKeys ( 5 )
curve:setKey ( 1, 0.00, 1)
curve:setKey ( 2, 0.25, 2 )
curve:setKey ( 3, 0.50, 3 )
curve:setKey ( 4, 0.75, 4 )
curve:setKey ( 5, 1.00, 1 )

anim = MOAIAnim:new ()
anim:reserveLinks ( 1 )
anim:setLink ( 1, curve, prop, MOAIGraphicsProp.ATTR_INDEX )
anim:setMode ( MOAITimer.LOOP )
anim:start ()

prop:moveScl ( 1, 1, 0, 3 )
