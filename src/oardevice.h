/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2013  <copyright holder> <email>

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


#ifndef OARDEVICE_H
#define OARDEVICE_H


#include <iostream>
#include <vector>

#include "obplineset.h"
#include "obppulse.h"


/** handles reading of hardware signals (and keyboard emulated signals too!) 
 *  keeps the graphical node for the Monitor HUD
 */
class OarDevice
{
  /**
   * size of the ring buffer: number of pulse samples taked into account 
   */
static const int BUFFER_SIZE = 20; 
  
/**
 * timing step for monitoring the device (in milliseconds??)
 */
static const float TIME_STEP = 50.0f; 
  
public:
OarDevice();
virtual ~OarDevice();

void initialize(Scene *scene);

Node * getNode();

/**
 * notify the device that there is a new value to read. 
 * this is a temporal method, used for simulation from keyboard
 */
void notifyInput(float value);

void update(float elapsedTime);

float getOutput();

private: 
  
  /**
   * initialize graphic stuff
   */
  void initializeMonitor(Scene *scene);
  /** updates the lines in the lineset from the values of the ring buffer
   */
  void updateLines();
  
  /**
   * buffer of normalized pulse samples
   */
  std::vector<float> _buffer;
  
  //normalized last input readed from hardware. this is consumed in the update.
  float _input;
  
  //normalized output value of the device
  float _output;
  
  //graphics things of the monitor
  obpLineSet _lineset;
  //node to wich the model in the lineset is attached to
  Node * _node;
  
  //cursor in the buffer ring. 
  int _cursor; 
  
  //puse to advance buffer ring cursor
  obpPulse _pulse;
};

#endif // OARDEVICE_H 
