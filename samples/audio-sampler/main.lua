----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass ()

gfxQuad = MOAISpriteDeck2D.new ()
gfxQuad:setTexture ( "../resources/moai.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp.new ()
prop:setDeck ( gfxQuad )
prop:setPartition ( layer )

prop:moveRot ( 0, 0, 360, 1.5 )

stream = MOAIMemStream.new ()

sampler = MOAIAudioSamplerCocoa.new ()
sampler:init ()
sampler:setStream ( stream )
sampler:start ()

function onClick ( down )
	
	if down and sampler then
		
		sampler:stop ()
		sampler = nil
		
		stream:seek ( 0 )

		length = stream:getLength ()

		MOAIUntzSystem.initialize ()

		buffer = MOAIUntzSampleBuffer.new ()
		buffer:prepareBuffer ( 1, length, 44100 )
		buffer:setRawData ( stream:read (), length, 1 )

		sound = MOAIUntzSound.new ()
		sound:load ( buffer )
		sound:setVolume ( 1 )
		sound:setLooping ( true )
		sound:play ()
	end
end

MOAIInputMgr.device.mouseLeft:setCallback ( onClick )
