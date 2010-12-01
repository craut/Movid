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
#include "moGrayToColorModule.h"
#include "../moLog.h"
#include "cv.h"

MODULE_DECLARE(GrayToColor, "native", "Converts input image to a three channel image");

moGrayToColorModule::moGrayToColorModule() : moImageFilterModule(){
	MODULE_INIT();
	this->setInputType(0, "IplImage8");
	this->setOutputType(0, "IplImage");
}

moGrayToColorModule::~moGrayToColorModule() {
}

void moGrayToColorModule::allocateBuffers() {
	IplImage* src = static_cast<IplImage*>(this->input->getData());
	if ( src == NULL )
		return;
	this->output_buffer = cvCreateImage(cvGetSize(src),src->depth, 3);	// three channels
	LOG(MO_DEBUG, "allocated output buffer for GrayToColor module.");
}

void moGrayToColorModule::applyFilter(IplImage *src) {
	cvCvtColor(src, this->output_buffer, CV_GRAY2RGB);
}

