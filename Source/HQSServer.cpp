/*
 * ============================================================================
 *
 * HQSServer.cpp
 * InterprocessConnectionServer implementation class
 * 
 * From HQStreamer - Stream lossless audio from one DAW to another across the web
 * Copyright (C) 2016 Sauraen
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * ============================================================================
*/

#include "HQSServer.h"
#include "HQSConnection.h"

HQSServer::HQSServer(HqstreamerAudioProcessor& p) : parent(p){
    
}
HQSServer::~HQSServer(){
    
}
    
InterprocessConnection* HQSServer::createConnectionObject(){
    if(parent.conn == nullptr){
        parent.conn = new HQSConnection(parent);
        return parent.conn;
    }else{
        return nullptr;
    }
}
