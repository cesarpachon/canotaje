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


#include "obplineset.h"

obpLineSet::obpLineSet()
{

}
//------------------------------


obpLineSet::~obpLineSet()
{
  SAFE_RELEASE(_model);
}
//-------------------------------

void obpLineSet::begin()
{
  _linecount = 0; 
  _vertices.clear();
   _dirty = true;
}
//-------------------------------


 
void obpLineSet::end()
{
  VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::COLOR, 3)
    };
    
    
   Mesh *  _mesh = Mesh::createMesh(VertexFormat(elements, 2), _linecount*2, false);
    GP_ASSERT(_mesh);
    
    _mesh->setPrimitiveType(Mesh::LINES);
    _mesh->setVertexData(&_vertices[0], 0, _linecount*2);
    
    _model = Model::create(_mesh);
    _model->setMaterial("res/unlit.material");
    SAFE_RELEASE(_mesh); 
    
    _dirty = false;
}
//-------------------------------

 
void obpLineSet::addLine(const Vector3 &ori, const Vector3 & des, const Vector3 & color)
{
  _vertices.resize((_linecount+1)*12);

  int index = _linecount*12;
 // Logger::log(Logger::LEVEL_INFO, "adding line %d at index %d from %f %f %f to %f %f %f color %f %f %f\n", _linecount, index, ori.x, ori.y, ori.z, des.x, des.y, des.z, color.x, color.y, color.z );
  _vertices[index++] = ori.x;
  _vertices[index++] = ori.y;
  _vertices[index++] = ori.z;
  _vertices[index++] = color.x;
  _vertices[index++] = color.y;
  _vertices[index++] = color.z;
  _vertices[index++] = des.x;
  _vertices[index++] = des.y;
  _vertices[index++] = des.z;
  _vertices[index++] = color.x;
  _vertices[index++] = color.y;
  _vertices[index++] = color.z;
  _linecount++;
}
//-------------------------------

void obpLineSet::setLineEndY(int lineid, float y)
{
   int index = ((lineid*12) + 7);
   _vertices[index] = y; 
   //GP_WARN("updating %d to %f", index, y);
   _dirty = true;
}
//-------------------------------

void obpLineSet::setLineOriY(int lineid, float y)
{
   int index = ((lineid*12) + 1);
   _vertices[index] = y; 
   //GP_WARN("updating %d to %f", index, y);
   _dirty = true;
}
//-------------------------------


void obpLineSet::setLineEnd(int lineid, const Vector3 &pos)
{
  setLineEnd(lineid, pos.x, pos.y, pos.z);
}
//---------------------------------


void obpLineSet::setLineEnd(int lineid, float x, float y, float z)
{
    int index = (lineid*12)+6;
    _vertices[index++] = x;
    _vertices[index++] = y;
    _vertices[index++] = z;
    _dirty = true;
}
//----------------------------------



void obpLineSet::setLineOri(int lineid, const Vector3 &pos)
{
   int index = (lineid*12);
    _vertices[index++] = pos.x;
    _vertices[index++] = pos.y;
    _vertices[index++] = pos.z;
    _dirty = true;
}
//---------------------------------


void obpLineSet::refreshBuffer()
{
  if(_dirty)
  {
    
   //this calls glBufferSubData
   _model->getMesh()->setVertexData(&_vertices[0], 0, _linecount*2);
   //forces a call to glBufferData.. instead of glBufferSubData
   //_model->getMesh()->setVertexData(&_vertices[0]);
    _dirty = false;
  }
}
//-------------------------------


void obpLineSet::createAxis(float size)
{
  begin();
   addLine(Vector3(0,0,0), Vector3(size, 0, 0), Vector3(1, 0, 0));
   addLine(Vector3(0,0,0), Vector3(0, size, 0), Vector3(0, 1, 0));
   addLine(Vector3(0,0,0), Vector3(0, 0, size), Vector3(0, 0, 1));
  end();
}
//-------------------------------

void obpLineSet::updateAxis(Node * node, float size)
{
    Vector3 pos = node->getTranslationWorld();
    Vector3 forward = node->getForwardVectorWorld();
    Vector3 up  = node->getUpVectorWorld();
    Vector3 side;
    Vector3::cross(forward, up, &side);  //node->getRightVector();
    
    setLineOri(0, pos);
    setLineOri(1, pos);
    setLineOri(2, pos);
    
    setLineEnd(0, pos + forward*size);
    setLineEnd(1, pos + up*size);
    setLineEnd(2, pos + side*size);
    refreshBuffer();
}
//-------------------------------



Model * obpLineSet::getModel()
{
  GP_ASSERT(_model);
  return _model; 
}
//-------------------------------


