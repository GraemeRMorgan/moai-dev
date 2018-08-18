----------------------------------------------------------------
-- Demonstrate tracking a prop with the camera fitter.
-- With MOAICameraFitter2D.FITTING_MODE_APPLY_BOUNDS mode set,
-- it is visible in the end how the fitter stops at the boundaries
-- while the prop keeps moving off the screen center.
----------------------------------------------------------------

function drawBackground()
	MOAIGfxDevice.setPenColor(1,1,1)
	MOAIDraw.drawLine(-1000, -1000, 1000, 1000)
	MOAIDraw.drawLine(1000, -1000, -1000, 1000)
end

MOAIDebugLines.setStyle(MOAIDebugLines.PROP_WORLD_BOUNDS)

MOAISim.openWindow ( "test", 640, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 640, 480 )
viewport:setScale ( 640, 480 )

camera = MOAICamera2D.new ()

layer = MOAILayer.new ()
layer:setViewport ( viewport )
layer:setCamera ( camera )
MOAISim.pushRenderPass ( layer )

layer:showDebugLines ( true )

-- add a background to make the tracking more visible
backgroundDeck = MOAIScriptDeck.new ()
backgroundDeck:setRect ( 0, 0, 1000, 1000 )
backgroundDeck:setDrawCallback ( drawBackground )

backgroundProp = MOAIProp.new ()
backgroundProp:setDeck ( backgroundDeck )
backgroundProp:setPartition ( layer )

fitter = MOAICameraFitter2D.new ()
fitter:setViewport ( viewport )
fitter:setCamera ( camera )
fitter:setBounds ( -500, -128, 500, 1000 )
fitter:setMin ( 256 )
fitter:start ()

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )

-- this prop will be tracked while it is going to the right
prop = MOAIProp.new ()
prop:setDeck ( gfxQuad )
prop:moveLoc ( 256, 0, 3 )
prop:setPartition ( layer )

fitter:setFitMode(MOAICameraFitter2D.FITTING_MODE_APPLY_BOUNDS)
fitter:startTrackingNode(prop)

prop = MOAIProp.new ()
prop:setDeck ( gfxQuad )
prop:moveLoc ( -256, 0, 3 )
prop:setPartition ( layer )
