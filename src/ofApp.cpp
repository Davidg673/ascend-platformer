#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup()
{
	map.loadLevel(0, 0);
	player.loadPlayerData(map.playerPos);
}

//--------------------------------------------------------------
void ofApp::update()
{
	player.update(ofGetLastFrameTime(), map.tiles);
}

//--------------------------------------------------------------
void ofApp::draw()
{
	map.renderLevel();
	player.drawPlayer();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == 'a' || key == 'A') player.leftPressed = true;
	if (key == 'd' || key =='D') player.rightPressed = true;
	if (key == 'w' || key == 'W') player.upPressed = true;
	if (key == 's' || key == 'S') player.downPressed = true;
	if (key == ' ') player.jumpPressed = true;
	if (key == OF_KEY_SHIFT) player.dashPressed = true;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
	if (key == 'a' || key == 'A') player.leftPressed = false;
	if (key == 'd' || key == 'D') player.rightPressed = false;
	if (key == 'w' || key == 'W') player.upPressed = false;
	if (key == 's' || key =='S') player.downPressed = false;
	if (key == ' ') player.jumpPressed = false;
	if (key == OF_KEY_SHIFT) player.dashPressed = false;

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
