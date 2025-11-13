#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup()
{

	map.loadLevel();
	map.setupRain();
	player.loadPlayerData(map.playerPos);
}

//--------------------------------------------------------------
void ofApp::update()
{
	if (running)
	{
		player.update(ofGetLastFrameTime(), map.tiles, map.traps, map.jumpPads);
		if (map.checkBorderCollision(player.playerRect, running)) {
			map.loadLevel();
			player.loadPlayerData(map.playerPos);
		}
		for (auto & trap : map.traps) {
			if (trap.isMovingX || trap.isMovingY) trap.move(25, 2, ofGetLastFrameTime());
		}

		map.updateRain();
	}
}

//--------------------------------------------------------------
void ofApp::draw()
{
	if (running)
	{
		map.moveBackground(player.playerVel);
		map.renderLevel();
		player.drawPlayer();
	} else
	{
		ofBackground(0);
		ofSetColor(255);

		ofDrawBitmapString("Level 1 finished! Press Enter to Continue", ofGetWidth()/2-200, ofGetHeight()/2-50);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (running)
	{
		if (key == 'a' || key == 'A') player.leftPressed = true;
		if (key == 'd' || key == 'D') player.rightPressed = true;
		if (key == 'w' || key == 'W') player.upPressed = true;
		if (key == 's' || key == 'S') player.downPressed = true;
		if (key == ' ') player.jumpPressed = true;
		if (key == OF_KEY_SHIFT) player.dashPressed = true;
	} else if (key ==OF_KEY_RETURN)
	{
		ofExit();
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
	if (running)
	{
		if (key == 'a' || key == 'A') player.leftPressed = false;
		if (key == 'd' || key == 'D') player.rightPressed = false;
		if (key == 'w' || key == 'W') player.upPressed = false;
		if (key == 's' || key == 'S') player.downPressed = false;
		if (key == ' ') player.jumpPressed = false;
		if (key == OF_KEY_SHIFT) player.dashPressed = false;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
