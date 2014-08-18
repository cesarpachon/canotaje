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


#include "boat.h"
#include "mathutil.h"
#include "Principal.h"

const float Boat::_WATER_DEPTH = 2.0f; 
const float Boat::_BOUYANCE_ATTENUATION_FACTOR = 0.5f;
const float Boat::_OAR_MAX_FORCE = 150.0f;
const float Boat::_ROOF_HEIGHT = 2.0f;
const float Boat::_MAX_LINEAR_SPEED = 15.0f;
const float Boat::_MAX_ANGULAR_SPEED = 40.0f*M_DEG_TO_RAD;





#define COS_30_DEGREES 0.866025404f

Boat::Boat()
{
  
}
//----------------------------

Boat::~Boat()
{

}
//--------------------------------

void Boat::initialize(Principal * context, Node * node)
{
  GP_ASSERT(node);
  
  _context = context;
  PhysicsController * _controller  = _context->getPhysicsController();
  Scene *_scene = _context->_scene; 
  //root node! geometry node will be a child because it comes wit a blender transformation
  _balsa = _scene->addNode("boat");
  
  _balsa->setTranslation(node->getTranslation());
  
 
  node->setTranslation(0, 0, 0);
  
  //INSTEAD OF MAKING NODE (THE MESH) CHILD OF BALSA (THE ROOT) WE KEEP THEM IN THE SAME LOCATION IN THE UPDATE METHOD!
  _balsaMeshNode = node;
  _balsa->addChild(node);
  
  /*
   * we wont use the phisic body of the geometry node.. we will attach it to the new root node
   * _balsaRigidBody = _balsa->getCollisionObject()->asRigidBody();
   */
  PhysicsCollisionObject * phyobj = _balsa->setCollisionObject("res/test_scene.physics#balsa");
  GP_ASSERT(phyobj);
  _balsaRigidBody = phyobj->asRigidBody();
  _balsaRigidBody->setDamping(0.1f, 0.95f);
  
  
  GP_ASSERT(_balsaRigidBody);
            
    addCorner(0);
    addCorner(1);
    addCorner(2);
    addCorner(3);
        
    _ray.setDirection(Vector3(0, -1, 0));
    
    _output.set(0, 0);
    
    
    //create a constraint to avoid sinking
    /*
    PhysicsGenericConstraint * constraint = controller->createGenericConstraint(_balsaRigidBody);
    //constraint->setDirection(Vector3(0, 1, 0));
    constraint->setLinearLowerLimit(Vector3(-10000, -10000, -10000));
    constraint->setLinearUpperLimit(Vector3(10000, 10000, 10000));
    
    constraint->setAngularLowerLimit(Vector3(0, -180, -15));
    constraint->setAngularUpperLimit(Vector3(0, 180, 15));
    
   // constraint->setAngularLowerLimit(Vector3(0, -180, 0));
   // constraint->setAngularUpperLimit(Vector3(0, 180, 0));
    constraint->setBreakingImpulse(999999.9f);
    */
       
    
    _balsaTopNode = _scene->addNode();

    Vector3 pos = _balsa->getTranslation();
    pos.y += _ROOF_HEIGHT; 
    _balsaTopNode->setTranslation(pos);
    
     PhysicsRigidBody::Parameters params;
    params.mass = 1.0f;
    params.kinematic = true;
    _balsaTopNode->setCollisionObject(PhysicsCollisionObject::RIGID_BODY,
                         PhysicsCollisionShape::box(Vector3(4, 1, 4)), &params);
    
    GP_WARN("adding top node at %f %f %f", pos.x, pos.y, pos.z);
    
    
    //configure camera node
    _cameraNode = _context->_scene->addNode("fpsCamera");
    _cameraNode->setTranslation(-6, 3, 0);
    //_cameraNode->rotateX(M_DEG_TO_RAD*90);
    _cameraNode->rotateY(-M_DEG_TO_RAD*90);
    
    _balsa->addChild(_cameraNode);
    
    
    //initialize helper
    
    _helper.begin();
    
    _helper.addLine(Vector3::zero(), Vector3(10, 0, 10), Vector3(255, 228, 0)); //0 yellow: min angle
    _helper.addLine(Vector3::zero(), Vector3(10, 0, -10), Vector3(255, 100, 0)); //1 orange: max angle
    _helper.addLine(Vector3::zero(), Vector3(10, 0, 0), Vector3(0, 0, 0)); //2 black: boat angle
    _helper.addLine(Vector3::zero(), Vector3(10, 0, 0), Vector3(255,255, 255)); //3 withe: river angle
    
    _helper.end();
    
    _helperNode = _scene->addNode("helpernode");
    _helperNode->setModel(_helper.getModel());
    
    m_serial.run(this);
}
//----------------------


void Boat::addCorner(int id)
{
    Node * _corner = _context->_scene->addNode();
    
    Vector3 pos;
    switch(id)
    {
      case 0: pos.x = 2; pos.y=0; pos.z=1; break;
      case 1: pos.x = 2; pos.y=0; pos.z=-1; break;
      case 2: pos.x = -2; pos.y=0; pos.z=1.5; break;
      case 3: pos.x = -2; pos.y=0; pos.z=-1.5; 
	
    };
    
    _balsa->addChild(_corner);

    PhysicsRigidBody::Parameters params;
    params.mass = 1.0f;
    params.restitution = 0.0f;
    
    _corner->setCollisionObject(PhysicsCollisionObject::RIGID_BODY,
                         PhysicsCollisionShape::sphere(0.5f, pos, true), &params);
    
    _context->getPhysicsController()->createFixedConstraint(_balsaRigidBody, _corner->getCollisionObject()->asRigidBody());
  
    _balsaCornerNode[id] = _corner;
    
    //now, create the top sphere
    
    
   /* Node * _top = _scene->addNode();
    pos.y += _ROOF_HEIGHT; 
    params.mass = 1.0f;
    params.kinematic = true;
    _top->setCollisionObject(PhysicsCollisionObject::RIGID_BODY,
                         PhysicsCollisionShape::sphere(0.5f, pos, true), &params);
    _balsaTopNode[id] = _top; 
    GP_WARN("adding top node %d at %f %f %f", id, pos.x, pos.y, pos.z);
*/
}
//---------------------------------

void Boat::update(float elapsedTime, float rightOarValue, float leftOarValue)
{
  
   //make visual node match physic node..
  Vector3 pos = _balsa->getTranslation();
 // _balsaMeshNode->setTranslation(pos);
  
  //make topNode moves always over the boat..
  pos.y += _ROOF_HEIGHT;
  _balsaTopNode->setTranslation(pos);
  
  //make the helper node the same position that top node..
  _helperNode->setTranslation(pos);
  
  //calculate boatdir y riverdir..
   Matrix rot;
  Vector3 arrow(1, 0, 0);
  float angle;
  
  //2: current boat angle
  //const Quaternion &q1 = 
  _balsa->getRotation(&rot);
  //Matrix::createRotation(q1, &rot);
  _boatdir = rot * arrow; 
 
 //3: desired river angle
  const Quaternion &q3 = _context->_river.getTargetOrientation();
  Matrix::createRotation(q3, &rot);
  arrow.set(1, 0, 0);
  _riverdir = rot * arrow; 
  
  
 //compute the angle between them.. 
  float cosangle = Vector3::dot(_boatdir, _riverdir);
  
  if(cosangle < COS_30_DEGREES)
  {
    //left or right?
    Vector3 up;
    Vector3::cross(_boatdir, _riverdir, &up);
    if(up.y > 0.0)
    {
      _context->setDebugString(0, "turn to the left!");
       _balsaRigidBody->applyTorque(Vector3(0,  _OAR_MAX_FORCE*(1.0-cosangle), 0));
    }
    else
    {
      _context->setDebugString(0, "turn to the right!");
       _balsaRigidBody->applyTorque(Vector3(0,  -_OAR_MAX_FORCE*(1.0-cosangle), 0));
    }
    
  }
  else
  {
   _context->setDebugString(0, "ok");
    
  }
    
  
  //EVALUATE PITCH, ROLL, YAW
  Vector3 axis;
  angle = _balsa->getRotation(&axis);
  
  _rotationAngles.x = axis.x * angle;
  _rotationAngles.y = axis.y * angle; 
  _rotationAngles.z = axis.z * angle; 
  
  //EVALUATE INPUTS
  _output.set(rightOarValue, leftOarValue);
  /*evaluate the _output array, to decide if apply left turn, right turn or forward force.. 
   * given that _output is a 2d normalized array, we can asume output.x is the cos of the angle. 
   * use it to determine one of three areas: 
   * angle < 30 degrees: turn to the left
   * ange > 60 degrees: turn to the right
   * angle between 30 and 60 degrees: linear force
  */
  Vector2 outputNorm;
  _output.normalize(&outputNorm);
  
 // GP_WARN("output: right: %f left: %f normalized: %f %f", rightOarValue, leftOarValue, outputNorm.x, outputNorm.y);
  
    //_balsaRigidBody->applyTorque(Vector3(0, 0, 10.0f));
  
  if(outputNorm.x > cos(M_DEG_TO_RAD*30))
  {
    //zone 1: turn to the left   
    _balsaRigidBody->applyTorque(Vector3(0,  _OAR_MAX_FORCE*_output.x, 0));
   // GP_WARN("applytorque to the left %f", _OAR_MAX_FORCE*_output.x);
  //    _balsaRigidBody->applyTorque(Vector3(0, 0, 10.0f));

  }
  else if(outputNorm.y > sin(M_DEG_TO_RAD*60))
  {
    //zone 2: turn to the right
    _balsaRigidBody->applyTorque(Vector3(0, -_OAR_MAX_FORCE*_output.y, 0));
    //GP_WARN("applytorque to the right %f", _OAR_MAX_FORCE*_output.y);
    
  }
  /*else if(_output.lengthSquared() > 0.1f)
  {
    //moves forward, use the length of _output, NOT _outputNorm because the last one is always 1.0
    Vector3 forward = _balsa->getRightVectorWorld();//it must be x.. but.. blender things!
    float magnitude = _output.length();
    float forcex,forcey;
    forcex = _OAR_MAX_FORCE*magnitude*forward.x;
    forcey = _OAR_MAX_FORCE*magnitude*forward.z;
    //GP_WARN("forward %f %f %f force %f %f", forward.x, forward.y, forward.z, forcex, forcey);
    _balsaRigidBody->applyForce(Vector3(_OAR_MAX_FORCE*magnitude*forward.x, 0, _OAR_MAX_FORCE*magnitude*forward.y));
  }*/

  //Vector3 translation = _balsa->getTranslation();
  //GP_WARN("boat pos %f %f %f", translation.x, translation.y, translation.z);

  
  //const Vector3 & up = _balsa->getUpVectorWorld();
  //Vector3 down = up * -1.0f;
  
  for(int i=0; i<4; i++)
  {
    updateCorner(i);
  }
  
  
  //FORWARD is the right vector.. left/right inclination over this axis
  /*
   Vector3 currdir = _balsa->getRightVector();
   Vector3 refdir = Vector3(1.0f, 0.0f, 0.0f);
  float dotdir = Vector3::dot(currdir, refdir);
  if(dotdir < cos(M_DEG_TO_RAD*45.0f))
  {
    float torque = _OAR_MAX_FORCE*(dotdir);
    GP_WARN("torque %f", torque);
     //_balsaRigidBody->applyTorque(Vector3(0, 0, -torque));
     
  }*/
  
  //SIDE.. 
 /* currdir = _balsa->getForwardVector();
  Vector3 refdir = Vector3(0.0f, 0.0f, 1.0f);
  float dotdir = Vector3::dot(currdir, refdir);
  if(dotdir < cos(M_DEG_TO_RAD*45.0f))
  {
    float torque = _OAR_MAX_FORCE*(1.0f-dotdir);
     _balsaRigidBody->applyTorque(Vector3(0, 0, torque));
  }*/
 
 Vector3 angularSpeedV = _balsaRigidBody->getAngularVelocity();
 
 if(angularSpeedV.x > _MAX_ANGULAR_SPEED) angularSpeedV.x = _MAX_ANGULAR_SPEED;
 else if(angularSpeedV.x < -_MAX_ANGULAR_SPEED) angularSpeedV.x = -_MAX_ANGULAR_SPEED;

  if(angularSpeedV.y > _MAX_ANGULAR_SPEED) angularSpeedV.y = _MAX_ANGULAR_SPEED;
 else if(angularSpeedV.y < -_MAX_ANGULAR_SPEED) angularSpeedV.y = -_MAX_ANGULAR_SPEED;

  if(angularSpeedV.z > _MAX_ANGULAR_SPEED) angularSpeedV.z = _MAX_ANGULAR_SPEED;
 else if(angularSpeedV.z < -_MAX_ANGULAR_SPEED) angularSpeedV.z = -_MAX_ANGULAR_SPEED;

 _balsaRigidBody->setAngularVelocity(angularSpeedV);
 
 Vector3 linearSpeedV = _balsaRigidBody->getLinearVelocity();
 if(linearSpeedV.lengthSquared() > _MAX_LINEAR_SPEED*_MAX_LINEAR_SPEED)
 {
   linearSpeedV.normalize();
   
    _balsaRigidBody->setLinearVelocity(linearSpeedV*_MAX_LINEAR_SPEED);  
 }
 
 updateHelper();
 
}
//---------------------------------


void Boat::updateHelper()
{
  
  Matrix rot;
  Vector3 arrow(1, 0, 0);
  float angle;
  
  const Quaternion &q3 = _context->_river.getTargetOrientation();
  Matrix::createRotation(q3, &rot);

  //2: current boat angle
  /*const Quaternion &q1 = _balsa->getRotation();
  Matrix::createRotation(q1, &rot);
  _boatdir = rot * arrow; 
  */
  _helper.setLineEnd(2, 10*_boatdir.x, 10*_boatdir.y, 10* _boatdir.z);
  
 
 //3: desired river angle
  /*
  const Quaternion &q3 = _context->_river.getTargetOrientation();
  Matrix::createRotation(q3, &rot);
  arrow.set(1, 0, 0);
  _riverdir = rot * arrow; 
  */
  _helper.setLineEnd(3, 10*_riverdir.x, 10*_riverdir.y, 10* _riverdir.z);
  
  //0: MIN allowed angle
  angle = -M_DEG_TO_RAD*30.0f;
  Matrix roty;
  Matrix::createRotationY(angle, &roty);
  
  arrow.set(1, 0, 0);
  arrow = roty * rot * arrow; 
  _helper.setLineEnd(0, 10*arrow.x, 10*arrow.y, 10* arrow.z);

  
  //1: MAX allowed angle
  angle = M_DEG_TO_RAD*30.0f;
  Matrix::createRotationY(angle, &roty);
  arrow.set(1, 0, 0);
  arrow = roty * rot * arrow; 
  _helper.setLineEnd(1, 10*arrow.x, 10*arrow.y, 10* arrow.z);
  
  _helper.refreshBuffer();
}
//------------------------------------

void Boat::updateCorner(int id)
{
  
  //humm.. try linear drag? 
  
  
  
    Vector3 pos = _balsaCornerNode[id]->getTranslationWorld();
    //Logger::log(Logger::LEVEL_INFO, "balsa corner %d: %f %f %f\n", id, pos.x, pos.y, pos.z);
    //lanzar un rayo desde el objeto corner..
    
    /*const Vector3 &forward = _balsaCornerNode[id]->getForwardVectorWorld();
    const Vector3 &up = _balsaCornerNode[id]->getUpVectorWorld();
    Logger::log(Logger::LEVEL_INFO, "forward: % f %f %f up %f %f %f\n", forward.x, forward.y, forward.z, up.x, up.y, up.z);
    */
    _ray.setOrigin(pos + Vector3(0, -0.51f, 0));
    //move downs the origin to avoid collisions with the corner itself..
    
    
    if(_context->getPhysicsController()->rayTest(_ray, _WATER_DEPTH, &_result/*, &_filter*/))
    {
      float collisionDepth  = _result.fraction*_WATER_DEPTH;
     // Logger::log(Logger::LEVEL_INFO, "collision corner %d depth %f\n", id, collisionDepth);
      if(_result.fraction < 1.0f)
      {
	//apply a opposite impulse..
	float impulse = (_result.fraction *_result.fraction) - (_result.fraction * 2.0f) + 1.0;
	Vector3 vimpulse(0, impulse*90.0f, 0);
	//_balsaCornerNode[id]->getCollisionObject()->asRigidBody()->applyImpulse(vimpulse);
	
	//IMPULSO 
	_balsaCornerNode[id]->getCollisionObject()->asRigidBody()->applyForce(vimpulse);
      }
    }
    
    //update top..
    // pos.y = _balsa->getTranslation().y + _ROOF_HEIGHT; 
    //_balsaTopNode[id]->setTranslation(pos); 
    //  GP_WARN("update top node %d at %f %f %f", id, pos.x, pos.y, pos.z);
 }
//---------------------------------
    
Node * Boat::getNode()
{
  return _balsa;  
}
//-----------------------------------

Node * Boat::getCameraNode()
{
  return _cameraNode;  
}
//-----------------------------------

void Boat::test()
{
  Vector3 dir = _balsa->getRightVector();
  _balsaRigidBody->applyForce(Vector3(dir.x*100,dir.y*0.0f,dir.z*0.0f));
    //_balsaRigidBody->applyForce(Vector3(100,0,0));
}
//-----------------------------------

PhysicsRigidBody * Boat::getRigidBody()
{
    return _balsaRigidBody;
}
//-----------------------------------

float Boat::getPitch()
{
    return _rotationAngles.z;//z
}
//-----------------------------------

float Boat::getRoll()
{
    return _rotationAngles.x;//x
}
//-----------------------------------

float Boat::getYaw()
{
    return _rotationAngles.y;//y
}
//-----------------------------------

void Boat::matchOrientation(/*const Quaternion &qtarget*/Node * target)
{
 
  Vector3 axis;
  float angle; 
  angle = _balsa->getRotation(&axis);
  
  Vector3 taxis;
  float tangle;
  tangle = target->getRotation(&taxis);
  
 
  Vector3 cross;
  Vector3::cross(axis, taxis, &cross);
  
  float torque = 0.0f;
 /* 
  if(Vector3::dot(axis, taxis)< 0.8f)
  {
    torque = 0.5f*_OAR_MAX_FORCE;
    if(cross.y > 0.0)
      _balsaRigidBody->applyTorque(Vector3(0, torque, 0));
    else
      _balsaRigidBody->applyTorque(Vector3(0, -torque, 0));    
  }
  */
  
  
  
  //float torque = ( tangle*taxis.y ) - ( angle*axis.y);
  //_balsaRigidBody->applyTorque(Vector3(0, 0.5f*_OAR_MAX_FORCE*torque, 0));
  
  //_context->setDebugString(0, torque);
 
 
 /*
  Quaternion qbalsa = _balsa->getRotation();
  Quaternion qtarget = target->getRotation();
  Quaternion q2;
  Quaternion::slerp(qbalsa, qtarget, 1.0f, &q2);
 Vector3 axis;
 float angle = q2.toAxisAngle(&axis);
 _balsaRigidBody->applyTorque(Vector3(axis.x*angle, _OAR_MAX_FORCE*axis.y*angle, axis.z*angle));
  */
}
//--------------------------------------



