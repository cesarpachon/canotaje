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


#include "river.h"
#include "obplineset.h"


using namespace gameplay;

#define TRACKPOINT_RADIUS 5.0f
#define MAX_RIVER_FORCE 100.0f
#define REORIENTATION_SPEED 100.0f


#include "Principal.h"

#include "obpstringutil.h"

River::River()
{
  
};
//-------------------


void River::initialize(Principal * context)
{
  
  _context = context;
  
  Scene * _scene = context->_scene;
  
     _flowEnabled = true;
     
     _riverCollisionNode = _scene->findNode("rio");
     
     //find trackpoints..
    obpLineSet auxlineset;
    int numtrackpoints = _scene->findNodes("trackpoint", _trackpoints, true, false);
    GP_WARN("found %d trackpoints", numtrackpoints);
    std::vector<Node *>::iterator iter;
    for(iter = _trackpoints.begin(); iter != _trackpoints.end(); iter++)
    {
      //create an axis for each trackpoint
      auxlineset.createAxis(3.0f);
      Node *node = *iter;
      node->setModel(auxlineset.getModel());
      GP_WARN("added trackpoint %s", node->getId());
    }
    
    
    //initialize the two first trackpoints..
    _trackpointIter= _trackpoints.begin();
    _lastTrackpoint = *_trackpointIter;
    GP_WARN("first trackpoint is %s", _lastTrackpoint->getId());
    _trackpointIter++;
    _currTrackpoint = *_trackpointIter;
    GP_WARN("second trackpoint is %s", _currTrackpoint->getId());
    
    
    
        //find obstacles and attach rigid bodies to them..
    
   /**
    * obstacles are objects with suffix "obstacle". we must use a visitor 
    * to find them, because _scene->findNodes will only check prefix. 
    */
   _scene->visit(this, &River::visitObstacle);
  
}
//---------------------

bool River::visitObstacle(Node *node)
{
  if(obpStringUtil::stringEndsWith(std::string(node->getId()), std::string("obstacle")))
  {
    PhysicsRigidBody::Parameters params;
    params.mass = 0.0f; //innamovible
    float size = node->getScaleX();
     node->setCollisionObject(PhysicsCollisionObject::RIGID_BODY,
                         PhysicsCollisionShape::sphere(size, Vector3::zero(), true), &params);
  }
  return true;
}
//---------------------


void River::update()
{
   if(_trackpointIter == _trackpoints.end()) return; 
    
    
    /* EVALUATE ONLY THE CHANGE OF TRACKPOINT! 
     * we are going to performe TWO test: the simple dist2 test, 
     * and a dot product in the case that the boat pass the trackpoint far away..
     */
    //test 1: 2d circle colision
   
   float dist2 = _currTrackpoint->getTranslation().distanceSquared(_context->_balsa.getNode()->getTranslation());
    bool test1 = dist2 < TRACKPOINT_RADIUS*TRACKPOINT_RADIUS;

    bool test2 = false;
    //if(!test1) //optimization trick: given that it is used in a OR, evaluate only if test1 is false. 
    //{
      //test 2: dot product 
      Vector3 vPointBoat = _context->_balsa.getNode()->getTranslation() - _currTrackpoint->getTranslation();
      vPointBoat.normalize();
      
      //Vector3 vPointNormal = _currTrackpoint->getForwardVector();
      Vector3 vPointNormal = _currTrackpoint->getRightVector();
      //vPointNormal.negate();
      
      float dot = Vector3::dot(vPointBoat, vPointNormal);
      test2 = dot > 0; 
   //}

    if(test1 || test2)
    {
      //CHANGING TRACKPOINT!
      _lastTrackpoint = _currTrackpoint;
      _trackpointIter++;
      if(_trackpointIter != _trackpoints.end())
      {
	_currTrackpoint = *(_trackpointIter);
	_context->setDebugString(1, _currTrackpoint->getId());
      GP_WARN("change trackpoint to %s", _currTrackpoint->getId());

      }
      else
      {
	GP_WARN("currtrackpoint is null");
	_currTrackpoint = NULL;
	
      }
    }
    
    
    //APPLY FORCE.. BUT YOU MUST EVALUATE THE CASE: INSIDE THE TRACKPOINT AREA, OR OUSIDE OF THE TRACKPOINT AREA
    if(_currTrackpoint && _flowEnabled)
    {
      //apply force!
      Vector3 dist = _currTrackpoint->getTranslation() - _context->_balsa.getNode()->getTranslation();
      
      //but ONLY if dist is greater than the minimum! 
      
      
      float factor = 1.0f; 
      if(test1) //boat is INSIDE trackpoint area.. apply decay function 
      {
	factor = dist2 * (1.0f / (TRACKPOINT_RADIUS*TRACKPOINT_RADIUS)); 
      }
      
      dist.normalize();
      Vector3 force = dist * (factor * MAX_RIVER_FORCE * _currTrackpoint->getScaleX());
      
      _context->_balsa.getRigidBody()->applyForce(force);
      
      

	
	//balsaMeshNode reorientation.. by now, just match the  active trackpoint
	//
	/*
	 * we have last and current trackpoint.. match special cases (last is null, curr is null)
	 */
	//if(!_lastTrackpoint)
	//{
	  _context->_balsa.matchOrientation(_currTrackpoint);
	
      
       //}
	//else
	//{
	  /*INTERPOLATE between the two trackpoints. 
	   * first: find the line that passes over the two trackpoints
	   * second: project the position of the boat over the line
	   * 	http://stackoverflow.com/questions/7023240/projection-of-point-on-line-in-3d
	   * third: normalize the position
	   * fourth: interpolate using SLERP.
	   */
	  
	Vector3 line(_lastTrackpoint->getTranslation(), _currTrackpoint->getTranslation()); 
	  float len = line.length();
	  line.normalize();
	  //be aware that we must work in line local space: 
	   Vector3 point = _context->_balsa.getNode()->getTranslation() - _lastTrackpoint->getTranslation();
	   float proj = Vector3::dot(point, line) / len;
	   	  
	  // _context->setDebugString(0, proj);
	   
	   Quaternion q0 = _lastTrackpoint->getRotation();
	   Quaternion q1 = _currTrackpoint->getRotation();
	   Quaternion::slerp(q0, q1, proj, &_targetOrientation);
	  // _context->_balsa.matchOrientation(q2);
	   
	
	
    }
}
//-----------------------

