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


#ifndef MO_PS3EYE_MODULE_H
#define MO_PS3EYE_MODULE_H

#include "../moModule.h"
#include "../ofxPS3/src/PS3EyeMulticam.h"
#include "cv.h"

class moDataStream;

class moPS3EyeModule : public moModule {
public:
	moPS3EyeModule(); 
	virtual ~moPS3EyeModule();

	virtual void start();
	virtual void stop();
	virtual void update();
	virtual void poll();

private:	
	moDataStream *stream;
	IplImage* frame;

	int camNum;
	int camWidth, camHeight;
	int framerate;
	PBYTE pBuffer;	

	void setFromPixels(unsigned char* _pixels, int w, int h);

	MODULE_INTERNALS();
};

#endif

