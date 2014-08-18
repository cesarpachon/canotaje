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


#ifndef RIVER_H
#define RIVER_H

#include "gameplay.h"

class Principal; 

class River
{
private:
    Principal * _context;
  
    gameplay::Node * _currTrackpoint;
    gameplay::Node * _lastTrackpoint;
    
    /**
     * interpolated orientation between last and current trackpoint 
     * and the projected position of the boat between the given river line segment
     */
    gameplay::Quaternion _targetOrientation;
    
    bool _flowEnabled;
    std::vector<gameplay::Node *>::iterator _trackpointIter;
    std::vector<gameplay::Node *> _trackpoints;
    
    gameplay::Node * _riverCollisionNode;
public:
  River();
  void initialize(Principal * context);
  
  void update();
  
   inline bool isEnabled(){return _flowEnabled;};
   inline void enable(bool flag){_flowEnabled = flag;};
   
   inline gameplay::Node * getCollisionNode(){return _riverCollisionNode;};
   inline gameplay::Node * getCurrentTrackpoint(){return _currTrackpoint;};
   
   inline const gameplay::Quaternion &getTargetOrientation(){return _targetOrientation;};
   
   /**
    * callback method for visitor 
    * check if node is a obstacle and add the physic objects
    */
   bool visitObstacle(gameplay::Node *node);
  
};

#endif // RIVER_H
