----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

if MOAIEnvironment.osBrand ~= MOAIEnvironment.OS_BRAND_IOS then
	print ( 'iOS only sample - please run on an iOS device or simulator' )
	os.exit ()
end

print ( "hello, iPhone!" )

print ( 'screen: ', MOAIEnvironment.screenWidth, MOAIEnvironment.screenHeight )

viewWidth, viewHeight = MOAIGfxMgr.getViewSize ()
print ( 'view: ', viewWidth, viewHeight )

if MOAIEnvironment.iosRetinaDisplay then
	print ( 'retina display' )
end

----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass ()

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp.new ()
prop:setDeck ( gfxQuad )
prop:setPartition ( layer )

prop:moveRot ( 360, 1.5 )
