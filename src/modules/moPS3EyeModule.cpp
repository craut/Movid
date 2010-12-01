/***********************************************************************
 ** Copyright (C) 2010 Movid Authors.  All rights reserved.
 **
 ** This file is part of the Movid Software.
 **
 ** This file may be distributed under the terms of the Q Public License
 ** as defined by Trolltech AS of Norway and appearing in the file
 ** LICENSE included in the packaging of this file.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** Contact info@movid.org if any conditions of this licensing are
 ** not clear to you.
 **
 **********************************************************************/


#include <assert.h>

#include "../moLog.h"
#include "../moModule.h"
#include "../moDataStream.h"
#include "moPS3EyeModule.h"
#include "highgui.h"

MODULE_DECLARE(PS3Eye, "native", "Fetch PS3 camera stream");

void showSettings(moProperty *property, void *userdata)
{
	PS3EyeMulticamShowSettings();
}

moPS3EyeModule::moPS3EyeModule() : moModule(MO_MODULE_OUTPUT) {

	MODULE_INIT();

	this->stream = new moDataStream("IplImage8");	
	
	// declare outputs
	this->declareOutput(0, &this->stream, new moDataStreamInfo(
			"ps3camera", "IplImage8", "Image stream of the ps3 camera"));	

	// declare properties
	this->properties["index"] = new moProperty(0);
	this->properties["settings"] = new moProperty(false);
	this->properties["settings"]->addCallback(showSettings, this);
}

moPS3EyeModule::~moPS3EyeModule() {
}

void moPS3EyeModule::start() {
	LOGM(MO_TRACE, "start ps3 camera");

	printf("selecting format...\n");
	camWidth = 320;
	camHeight = 240;
	framerate = 60;
	camNum = PS3EyeMulticamGetCameraCount();
	PS3EyeMulticamOpen(camNum, VGA, framerate);
	PS3EyeMulticamLoadSettings(".\\data\\cameras.xml");
	// get stitched image width
	PS3EyeMulticamGetFrameDimensions(camWidth, camHeight);
	frame = cvCreateImage(cvSize(camWidth, camHeight), IPL_DEPTH_8U, 1);	
	printf("camWidth / camHeight: %d / %d\n", camWidth, camHeight);
	// Allocate image buffer for grayscale image
	pBuffer = new BYTE[camWidth*camHeight];
	// Start capturing
	PS3EyeMulticamStart();
	printf("ps3 cam started\n");

	moModule::start();	
}

void moPS3EyeModule::stop() {
	moModule::stop();
	if (frame != NULL) {
		LOGM(MO_TRACE, "release camera");
		// Stop capturing
		PS3EyeMulticamStop();
		Sleep(50);
		PS3EyeMulticamSaveSettings(".\\data\\cameras.xml");
		PS3EyeMulticamClose();	
		delete [] pBuffer;
		cvReleaseImage(&frame);
		frame = NULL;
		if(remove("settings.xml")==-1);	
	}
}

void moPS3EyeModule::update() {
	if (frame != NULL) {
		// push a new image on the stream
		LOGM(MO_TRACE, "push a new image on the stream");	
		if (PS3EyeMulticamGetFrame(pBuffer)) { // is frame new?
			this->setFromPixels(pBuffer, camWidth, camHeight);
			this->stream->push(frame);    
		}
		this->notifyUpdate();	
	}
}

void moPS3EyeModule::poll() {
	this->notifyUpdate();
	moModule::poll();
}

void moPS3EyeModule::setFromPixels(unsigned char* _pixels, int w, int h) {
    // copy pixels from _pixels, however many we have or will fit in frame
    for(int i = 0; i < h; i++) {
        memcpy(frame->imageData + (i * frame->widthStep), _pixels + (i * w), w);
    }
}

