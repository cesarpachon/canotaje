/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2013  cesar pachón cesarpachon@gmail.com http://www.cesarpachon.com

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


#ifndef OBPLINESET_H
#define OBPLINESET_H

#include "gameplay.h"

using namespace gameplay;
/**
 * helper class to create and manipulate gameplay3d mesh class 
 * as a set of 3d lines with plain colors. 
 */
class obpLineSet
{

public:
obpLineSet();
virtual ~obpLineSet();

/**
 * begins the creation of the mesh
 **/
 void begin();

 
 /**
 * ends the creation of the mesh
 **/
 void end();
 
 /**
  * adds a line to the set. it must be called between begin end calls. 
  */
 void addLine(const Vector3 &ori, const Vector3 & des, const Vector3 & color);

 /**
  * helper method. wraps begin, addline and end calls to create a 3d axis line set. 
  */
 void createAxis(float len);
 
 /**
  * if this line set was created using createAxis, call this method
  * will make the axis match the forward, up, side vectors of the given node
  */
 void updateAxis(Node * node, float len);
 
 Model * getModel();

 void setLineEndY(int lineid, float y);
 void setLineOriY(int lineid, float y);

 void setLineEnd(int lineid, const Vector3 &pos);
 void setLineOri(int lineid, const Vector3 &pos);
 
 void setLineEnd(int lineid, float x, float y, float z);
 
/** call once after all the setLineXXXX methods to refresh graphic buffer*/
void refreshBuffer();

private:
   const static unsigned int VERTICE_FLOAT_SIZE =6;  // (3 (position(xyz) + 3 color(rgb))

  Model * _model;
  int _linecount;
  //used whitin begin/end calls.. cleared at end.
  std::vector<float> _vertices;
  
  
  bool _dirty;
  

};

#endif // OBPLINESET_H
