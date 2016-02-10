/* 
    This file is part of WARG's computer-vision

    Copyright (c) 2015, Waterloo Aerial Robotics Group (WARG)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3. Usage of this code MUST be explicitly referenced to WARG and this code
       cannot be used in any competition against WARG.
    4. Neither the name of the WARG nor the names of its contributors may be used
       to endorse or promote products derived from this software without specific
       prior written permission.

    THIS SOFTWARE IS PROVIDED BY WARG ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL WARG BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include "../../core/include/frame.h"
#include "../include/imgimport.h"
#include <vector>
#include <string>
#include <dirent.h>
#include <boost/log/trivial.hpp>

using namespace cv;
using namespace std;

ImageImport::ImageImport(std::string telemetry_path, std::string filePath, std::vector<int> videoDeviceNums) {
    this->videoDeviceNums=videoDeviceNums;
    mdls=readcsv(telemetry_path.c_str());
    dr=opendir(filePath.c_str());
}

ImageImport::~ImageImport(){
    BOOST_LOG_TRIVIAL(trace)<<"image import ends."<<endl;
}

Frame * ImageImport::next_frame(){
    drnt=readdir(dr);
    if(drnt==NULL){
        BOOST_LOG_TRIVIAL(trace)<<"no more images"<<endl;
        return NULL;
    }
    while(strcmp(drnt->d_name,"..")==0||strcmp(drnt->d_name,".")==0){
        drnt=readdir(dr);
	if(drnt==NULL){
            BOOST_LOG_TRIVIAL(trace)<<"no more images"<<endl;
            return NULL;
        }
    }
    Mat img;
    img=imread(drnt->d_name,-1);
    Frame newframe(&img,drnt->d_name,mdls->a);
    frame_buffer=&newframe;
    mdls=mdls->next;
    return frame_buffer;
}
