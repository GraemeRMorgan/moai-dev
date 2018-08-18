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

function addTextbox ( top, height, alignment )

	local textbox = MOAITextBox.new ()
	textbox:setString ( text )
	textbox:setFont ( font )
	textbox:setTextSize ( 7.5, 163 )
	textbox:setRect ( -150, top - height, 150, top )
	textbox:setAlignment ( alignment )
	textbox:setYFlip ( true )
	textbox:setPartition ( layer )
end

local top = 230
local height = 160
local step = 160

addTextbox ( top, height, MOAITextBox.LEFT_JUSTIFY )
top = top - step

addTextbox ( top, height, MOAITextBox.CENTER_JUSTIFY )
top = top - step

addTextbox ( top, height, MOAITextBox.RIGHT_JUSTIFY )
top = top - step

