/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2013  cesar pachon cesarpachon@gmail.com http://www.cesarpachon.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "oardevice.h"

OarDevice::OarDevice()
{
}
//----------------------------

OarDevice::~OarDevice()
{

}
//-------------------------

void OarDevice::initialize(Scene *scene)
{
    _buffer.resize(BUFFER_SIZE);
  for(int i=0; i<BUFFER_SIZE; ++i)
  {
    //initialize to zeros! this is just a temporal hack to preview the monitor
   // _buffer[i] = (float)i/(float)BUFFER_SIZE;
   _buffer[i] = 0.0f;
  }

  _output = 0.0f; 
  
  initializeMonitor(scene);
  
  _pulse.init(TIME_STEP);
}
//-------------------------

void OarDevice::initializeMonitor(Scene *scene)
{
  _lineset.begin();
  
  //samples lines (red)
  float step = 1.0f / (float)BUFFER_SIZE;
  for(int i=0; i<BUFFER_SIZE; i++)
  {
    float x = step*i; 
    float y = _buffer[i];
    _lineset.addLine(Vector3(x, 0, 0), Vector3(x, y, 0), Vector3(1, 0, 0));      
  }
  
  //level line (green)
  
  _lineset.addLine(Vector3(0, 0, 0), Vector3(1, 0, 0), Vector3(0, 1, 0));
  
  //floor line (blue):
  _lineset.addLine(Vector3(0, 0, 0), Vector3(1, 0, 0), Vector3(0, 0, 1));
  
  //ceiling line (blue): 
  _lineset.addLine(Vector3(0, 1, 0), Vector3(1, 1, 0), Vector3(0, 0, 1));
  
  _lineset.end();
  
    _node = scene->addNode("oarNode");
  _node->setModel(_lineset.getModel());
}
//------------------------------

Node * OarDevice::getNode()
{
    return _node; 
}
//------------------------------


void OarDevice::notifyInput(float value)
{
  _input = value;
}
//----------------------------------



void OarDevice::update(float elapsedTime)
{
  
  //rotate ring buffer..
  if(_pulse.update(elapsedTime))
  {
      //consume last input (if any)
    _buffer[_cursor] = _input;
    _input = 0.0f;

    
    _cursor++;
    if(_cursor >=BUFFER_SIZE) _cursor = 0; 
    //recalculate lines in the lineSet..
    //GP_WARN("pulse! %d", _cursor);

    //update _output..
    float acum = 0.0f;
    for(int i=0; i<BUFFER_SIZE; i++)
    {
      acum+=_buffer[i];
    }
    _output = acum / (float)BUFFER_SIZE;
    
    updateLines();
  }
}
//------------------------------

float OarDevice::getOutput()
{
  return _output;
}
//------------------------------


void OarDevice::updateLines()
{
  //samples lines (red)
  float step = 1.0f / (float)BUFFER_SIZE;
  for(int i=0; i<BUFFER_SIZE; i++)
  {
    float x = step*i; 
    int cursorpos = _cursor + i;
    if(cursorpos >= BUFFER_SIZE) cursorpos -= BUFFER_SIZE; 
    float y = _buffer[cursorpos];
    _lineset.setLineEndY(i, y);      
  }
  
  //level line (green)
  _lineset.setLineEndY(BUFFER_SIZE, _output);      
  _lineset.setLineOriY(BUFFER_SIZE, _output);      
  
  _lineset.refreshBuffer();
}
//-------------------------------
