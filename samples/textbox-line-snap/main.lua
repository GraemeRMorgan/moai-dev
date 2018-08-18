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

charcodes = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-'
text = 'Lorem ipsum dolor\nsit amet, consectetur\nadipiscing elit.\nMaecenas diam augue, laoreet\nvitae\ndapibus in, sodales in ligula.'

font = MOAIFont.new ()
font:loadFromTTF ( 'arial-rounded.TTF', charcodes, 7.5, 163 )

local textbox = MOAITextLabel.new ()
textbox:setString ( text )
textbox:setFont ( font )
textbox:setTextSize ( 7.5, 163 )
textbox:setRect ( -150, -230, 150, 230 )
textbox:setYFlip ( true )
textbox:setColor ( 1, 0, 0, 1 )
textbox:setPartition ( layer )

local textbox = MOAITextLabel.new ()
textbox:setString ( text )
textbox:setFont ( font )
textbox:setTextSize ( 7.5, 163 )
textbox:setRect ( -150, -230, 150, 230 )
textbox:setYFlip ( true )
textbox:setColor ( 1, 1, 1, 1 )
textbox:setLineSnap ( 4, 8 )
textbox:setPartition ( layer )

