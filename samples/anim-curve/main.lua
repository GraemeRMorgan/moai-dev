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

prop = MOAIProp.new ()
prop:setDeck ( '../resources/moai.png' )
prop:setPartition ( layer )

curve = MOAIAnimCurve.new ()
curve:reserveKeys ( 2 )
curve:setKey ( 1, 0, 0 )
curve:setKey ( 2, 1.5, 360 )

prop:setAttrLink ( MOAIProp.ATTR_Z_ROT, curve, MOAIAnimCurve.ATTR_VALUE )

timer = MOAITimer.new ()
timer:setSpan ( 0, curve:getLength ())

curve:setAttrLink ( MOAIAnimCurve.ATTR_TIME, timer, MOAITimer.ATTR_TIME )

timer:start ()
