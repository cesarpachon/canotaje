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


#ifndef BOAT_H
#define BOAT_H

#include "gameplay.h"
#include "obplineset.h"
#include "serial.h"

class Principal;

using namespace gameplay;

#define _LEFT 0
#define _RIGHT 1

class Boat
{
public:
     static const float _WATER_DEPTH; 
     static const float _BOUYANCE_ATTENUATION_FACTOR;
     static const float _OAR_MAX_FORCE;
     static const float _ROOF_HEIGHT;
     static const float _MAX_LINEAR_SPEED;
     static const float _MAX_ANGULAR_SPEED;

public:
  Boat();
  virtual ~Boat();
  
  /**
   * @param node the node that contains the main body of the boat
   */
  void initialize(Principal * context, Node * node);
  
  void update(float elapsedTime, float rightOarValue, float leftOarValue);
  
  /**
   * returns the main node of the boat
   */
  Node * getNode();
  /**
   * returns the attachment point for the fps camera
   */
  Node * getCameraNode();
  
  
  PhysicsRigidBody * getRigidBody();
  
  
  /**
   * in radians
   */
  float getPitch();
  /**
   * in radians
   */
  float getRoll();
  /**
   * in radians
   */
  float getYaw();
  
  void test();
  
  
  /**
   * fix the given orientation NOW
   */
  void matchOrientation(/*const Quaternion &target*/Node *target);
  
  
private: //private methods
    void addCorner(int id);
    void updateCorner(int id);
    
    void updateHelper();
  
private:
      Principal * _context;
      
  
    /**
     * the invisible node attached to the physic body
     */
    Node * _balsa;
    
    /**
     * the visible mesh node
     */
    Node *_balsaMeshNode;
    
    /**
     * attachment point for the FPS camera
     */
    Node * _cameraNode; 
    
    
    PhysicsRigidBody *_balsaRigidBody;
    
    /**
     * these ones are the nodes with the physic spheres
     * that will be used to cast bouyance rays
     */
    Node * _balsaCornerNode[4];
    
    /**
     * these ones are the nodes with kinematic spheres
     * that will act as vertical limit to the boat 
     */
   // Node * _balsaTopNode[4]; 
   Node * _balsaTopNode;
     
     
     PhysicsController::HitResult _result;
     PhysicsController::HitFilter _filter;
     Ray _ray;
     
     
     //remo model output force (what's the english word for remo??)
     Vector2 _output; 
     
     Vector3 _rotationAngles;
     
     Vector3 _riverdir;
     Vector3 _boatdir;
     
     
     obpLineSet _helper;
     Node * _helperNode;
     
     //class to handle serial communication with physical device
     Serial m_serial;
     
};

#endif // BOAT_H
