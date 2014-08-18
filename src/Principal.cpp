#include "Principal.h"
#include "mathutil.h"
#include "obpmathutil.h"
#include "obpstringutil.h"

#include <fstream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

using namespace std;

// Declare our game instance
Principal game;


void setupLine(std::vector<float> &vertices, int lineid,  const Vector3 &ori, const Vector3 & des, const Vector3 & color)
{
  int index = lineid*12;
  Logger::log(Logger::LEVEL_INFO, "adding line %d at index %d from %f %f %f to %f %f %f color %f %f %f\n", lineid, index, ori.x, ori.y, ori.z, des.x, des.y, des.z, color.x, color.y, color.z );
  vertices[index++] = ori.x;
  vertices[index++] = ori.y;
  vertices[index++] = ori.z;
  vertices[index++] = color.x;
  vertices[index++] = color.y;
  vertices[index++] = color.z;
  vertices[index++] = des.x;
  vertices[index++] = des.y;
  vertices[index++] = des.z;
  vertices[index++] = color.x;
  vertices[index++] = color.y;
  vertices[index++] = color.z;
}
//----------------------------------------------

Principal& Principal::getInstance()
{
    return game;
}
//----------------------------------------------


Principal::Principal()
    : _scene(NULL)
{
}

void Principal::initialize()
{
  
     _font = Font::create("res/arial18.gpb");

  
    // Load game scene from file
   // _scene = Scene::load("res/test_scene.gpb");
     //eventually test_scene.gpb only will have light and camera  
    _scene = Scene::load("res/test_scene.scene");

    Light* directionalLight = Light::createDirectional(Vector3::one());
   _bumpLightNode = _scene->addNode("directionalLight");
   _bumpLightNode->setTranslation(Vector3(-3, 2, 7));
  _bumpLightNode->setLight(directionalLight);

    // Set the aspect ratio for the scene's camera to match the current resolution
    _scene->getActiveCamera()->setAspectRatio(getAspectRatio());
    

    
    getPhysicsController()->setGravity(Vector3(0, -5.0, 0));
    
  
   _balsa.initialize(this, _scene->findNode("balsa"));

   _balsaAxis.createAxis(5.0f);
   _axis = _scene->addNode("axis0");
  // _axis->setTranslation(_balsa.getNode()->getTranslation());
   _axis->setModel(_balsaAxis.getModel());
   
   Model* model ;
    model = createGridModel();
    _scene->addNode("grid")->setModel(model);
    model->release();

    //axis for the world..
   /* model = createAxisModel(5.0f);
    _axis = _scene->addNode("axis0");
    _axis->setModel(model);
   //axis->setTranslation(0.5, 0.5, 0.5);
    model->release();
    */
    //_balsa.getNode()->addChild(axis);
    
    //axis for the HUD
    model = createAxisModel(1.0f);
    Node * axis = _scene->addNode("axis");
    axis->setModel(model);
    model->release();

    Camera * _cam = _scene->getActiveCamera();
    Node * camnode = _cam->getNode();
    camnode->addChild(axis);
    axis->setTranslation(-0.8, -0.45, -1);
    axis->setScaleY(0.9);
    axis->setScaleX(1.55);
    _cam->setFarPlane(10000);
    //axis->setRotation(Vector3(1, 1, 1), M_DEG_TO_RAD*45.0f);
    
        //this is needed for the bindLights method..
    _lightNode = _scene->findNode("Lamp");
    
    _form = Form::create("res/formsetup.form");
    
     _cameraButton = static_cast<Button*>(_form->getControl("cameraButton"));
    _cameraButton->addListener(this, Control::Listener::RELEASE);
    
    _rollSlider = static_cast<Slider *> (_form->getControl("rollSlider"));
   _yawSlider = static_cast<Slider *> (_form->getControl("yawSlider"));
   _pitchSlider = static_cast<Slider *> (_form->getControl("pitchSlider"));
   _flowCheckbox = static_cast<CheckBox *> (_form->getControl("flowCheckbox"));
   _flowCheckbox->addListener(this, Control::Listener::VALUE_CHANGED);
   _pauseCheckbox = static_cast<CheckBox *> (_form->getControl("pauseCheckbox"));
   _pauseCheckbox->addListener(this, Control::Listener::VALUE_CHANGED);
   _debug0 = static_cast<Label *> (_form->getControl("debug0"));
   _debug1 = static_cast<Label *> (_form->getControl("debug1"));
    
    //configure cameras
    _globalCamera = _scene->getActiveCamera();
    _fpsCamera = Camera::createPerspective(_globalCamera->getFieldOfView(), _globalCamera->getAspectRatio(), _globalCamera->getNearPlane(), _globalCamera->getFarPlane());
    _balsa.getCameraNode()->setCamera(_fpsCamera);
    _currentCamera = _globalCamera;
    
    
    //configure oar devices
    _oarLeft.initialize(_scene);
    
    Node * oarNode = _oarLeft.getNode();
    oarNode->setScale(0.1f);
    oarNode->setTranslation(0.9, 0.8, 0.0);
    axis->addChild(oarNode);
    
    
    _oarRight.initialize(_scene);
    oarNode = _oarRight.getNode();
    oarNode->setScale(0.1f);
    oarNode->setTranslation(0.9, 0.6, 0.0);
    axis->addChild(oarNode);
 
    loadSceneObw();
    
    
    //assign materials..
    _scene->visit(this, &Principal::bindLights);

    
    /*skydome:
     * we will remove the skydome from the node hierarchy, because we dont want it be affected 
     * by far occlusion. we will also try to render it manually, before the visitor to the nodes.
    */
    Bundle * bundle = Bundle::create("res/skydome.gpb");
    _skydome = bundle->loadNode("skydome");
    Material * mat = _skydome->getModel()->setMaterial("res/skydome.material");
    mat->setParameterAutoBinding("u_worldViewProjectionMatrix", "WORLD_VIEW_PROJECTION_MATRIX");
   Texture::Sampler* sampler = mat->getParameter("u_diffuseTexture")->setValue("res/skydome.png", false);
    _scene->addNode(_skydome);
    SAFE_RELEASE(bundle);
    
    /**
     * water tile:
     */
    
   /* bundle = Bundle::create("res/water.gpb");
    _waterTile = bundle->loadNode("waterTile");
    mat = _waterTile->getModel()->setMaterial("res/water.material");
    mat->setParameterAutoBinding("u_worldViewProjectionMatrix", "WORLD_VIEW_PROJECTION_MATRIX");
   sampler = mat->getParameter("u_diffuseTexture")->setValue("res/water512_prekf_seamless.png", false);
    _scene->addNode(_waterTile);
    SAFE_RELEASE(bundle);
    */
   
   /**
    * bump mapping test
    */
   bundle = Bundle::create("res/lightBrickWall.gpb");
   _bumpNode = bundle->loadNode("wall"); 
   _scene->addNode(_bumpNode);
   _bumpNode->setScale(1.5f);
   _bumpModel = _bumpNode->getModel();
   _bumpNode->rotateX(MATH_DEG_TO_RAD(90));
   SAFE_RELEASE(bundle);

  
    setWaterMaterial(_bumpNode);
  _currBumpFrame = 0; 
  _bumpSpeed = 10.0f/1000.0f; //30 frames per millisecond
  _bumpPeriod = 100.0f / _bumpSpeed; //total time of a loop of animation
  _bumpElapsedTime = 0.0f;
 
      _river.initialize(this);
      
      
      //terrain
      /*_terrain = _scene->findNode("terrain")->getTerrain();
      _terrain->setFlag(Terrain::FRUSTUM_CULLING, true);
      _terrain->setFlag(Terrain::DEBUG_PATCHES, false);
      _terrain->getNode()->setTranslation(Vector3(0, -15, -100));
      */
      GP_ASSERT(_terrain);

}
//---------------------------------

void Principal::loadSceneObw()
{
    //experiment: load a mesh from a bundle and try to create nodes that share it. 
    /*
    Bundle * _bundle = Bundle::create("res/tile0.gpb");
    Node * _node = _bundle->loadNode("tile0");
    SAFE_RELEASE(_bundle);
    
    Node * _node2 = _scene->addNode("tile0.1");
    _node2->setTranslation(_node->getTranslation());
    _node2->setRotation(_node->getRotation());
    _node2->setScale(_node->getScale());
    _node2->setTranslationZ(_node->getTranslationX()+20.0f);
    _node2->setModel(Model::create(_node->getModel()->getMesh()));

    _node2 = _scene->addNode("tile0.2");
    _node2->setTranslation(_node->getTranslation());
    _node2->setRotation(_node->getRotation());
    _node2->setScale(_node->getScale());
    _node2->setTranslationZ(_node->getTranslationX()-20.0f);
    _node2->setModel(Model::create(_node->getModel()->getMesh()));

    SAFE_RELEASE(_node);
    */
    
    //PARSE FILE SCENE.OBW
    //std::map<std::string, Node *> 
    _masterMeshNodes.clear();
    std::ifstream is;
    stringstream ls;
    is.open("res/scene.obw"); 
    GP_ASSERT(fin.good()) 
    
    std::string line;
    
     std::string type;
     std::string objname;
     std::string meshname;
     float pos[3], rot[3], esc[3];
     while(std::getline (is,line)){
      ls.clear();
      ls.str(line);
      ls >> type;
      if(type == "obj"){
	ls >> objname;
      }
      else if(type == "mesh"){
	ls >> meshname;
      }
      else if(type == "matrix"){

	float f[16];
	ls >> f[0] >> f[1] >> f[2] >> f[3] >> f[4] >> f[5] >> f[6] >> f[7] >> f[8] >> f[9] >> f[10] >> f[11] >> f[12] >> f[13] >> f[14] >> f[15];
	Matrix _m(f);
	//_m.set(f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7], f[8], f[9], f[10], f[11], f[12], f[13], f[14], f[15]);
	
	std::cout << "found obj "<< objname << " with mesh " << meshname << std::endl; 
	Node * _node = _scene->addNode(objname.c_str());
        _node->setIdentity();
	Vector3 scale;
	Quaternion rotation;
	Vector3 translation;
	_m.decompose(&scale, &rotation, &translation);
	_node->setTranslation(translation);
	_node->setRotation(rotation);
	_node->setScale(scale);
	Model * model = getMasterModel(meshname);
	_node->setModel(Model::create(model->getMesh()));
	
      }
     }
    is.close();  
    for(_NODE_MAP_ITER iter = _masterMeshNodes.begin(); iter != _masterMeshNodes.end(); iter++)
    {
      SAFE_RELEASE(iter->second);
    }
    _masterMeshNodes.clear();
}
//-------------------------------------


Model* Principal::getMasterModel(std::string &name)
{
    _NODE_MAP_ITER iter = _masterMeshNodes.find(name);
    if(iter != _masterMeshNodes.end()){
      return  iter->second->getModel();
    }
    std::string bundle_path = "res/"+name+".gpb";
     Bundle * _bundle = Bundle::create(bundle_path.c_str());
    Node * _node = _bundle->loadNode(name.c_str());
    _masterMeshNodes.insert(std::pair<std::string, Node *>(name, _node));
    SAFE_RELEASE(_bundle);
    return _node->getModel();
}
//-------------------------------------

Node * Principal::setWaterMaterial(Node *node)
{
  GP_ASSERT(node);
  GP_ASSERT(_bumpLightNode);
  
     Material * bumpMaterial = Material::create("res/light.material");
     Model * model = node->getModel();
     GP_ASSERT(model);
  model->setMaterial(bumpMaterial);

  Technique * technique = bumpMaterial->getTechnique("directionalSpecularBumped");
  bumpMaterial->setTechnique("directionalSpecularBumped");
    technique->getParameter("u_lightDirection")->bindValue(_bumpLightNode, &Node::getForwardVectorWorld); 
  technique->getParameter("u_lightColor")->setValue(Vector3(1.0, 0.8, 0.6));
  technique->getParameter("u_ambientColor")->setValue(Vector3(0.0f, 0.0f, 0.0f));
  technique->getParameter("u_normalmapOffset")->bindValue(this,&Principal::getCurrBumpUVCoords);
  technique->getParameter("u_textureOffset")->bindValue(this,&Principal::getCurrUVOffset);

}
//-----------------------------------




Node* Principal::setDefaultMaterial(Node *node)
{
    // Get light node
    Light* light = _lightNode->getLight();
    const Vector3 _light_color = light->getColor();   
    const Vector3 _ambient_color = _scene->getAmbientColor();
    GP_ASSERT(node);

  Model* boxModel = node->getModel();
    
    Material* boxMaterial = boxModel->setMaterial("res/box.material");
    boxMaterial->getParameter("u_ambientColor")->setValue(_light_color);
    boxMaterial->getParameter("u_lightColor")->setValue(_light_color);
    boxMaterial->getParameter("u_lightDirection")->setValue(_lightNode->getForwardVectorView());
    return node;
}
//---------------------------------

bool Principal::bindLights(Node* node)
{
    Model* model = node->getModel();
    if (model)
    {
        Material* material = model->getMaterial();
	
	if(!material)
	{
	  //assign material according to prefix..
	  if(obpStringUtil::stringStartsWith(std::string(node->getId()), std::string("rock3")))
	  {
	       material = model->setMaterial("res/rock3.material"); 
	  }
	  else if(obpStringUtil::stringStartsWith(std::string(node->getId()), std::string("rock2")))
	  {
	         material = model->setMaterial("res/rock2.material");
	  }
	  else if(obpStringUtil::stringStartsWith(std::string(node->getId()), std::string("rock1")))
	  {
	         material = model->setMaterial("res/rock1.material");
	  }
	  else if(obpStringUtil::stringStartsWith(std::string(node->getId()), std::string("trunk1")))
	  {
	         material = model->setMaterial("res/trunk1.material");
	  }
	  else if(obpStringUtil::stringStartsWith(std::string(node->getId()), std::string("leaves1")))
	  {
	         material = model->setMaterial("res/leaves1.material");
	  }
	  else if(obpStringUtil::stringStartsWith(std::string(node->getId()), std::string("tile0")))
	  {
	         material = model->setMaterial("res/tile0.material");
	  }
	  else if(obpStringUtil::stringStartsWith(std::string(node->getId()), std::string("tile1")))
	  {
	         material = model->setMaterial("res/tile1.material");
	  }
	  else if(obpStringUtil::stringStartsWith(std::string(node->getId()), std::string("tile2")))
	  {
	         material = model->setMaterial("res/tile2.material");
	  }
	  else if(obpStringUtil::stringStartsWith(std::string(node->getId()), std::string("treeline")))
	  {
	         material = model->setMaterial("res/treeline.material");
	  }
	  else if(obpStringUtil::stringStartsWith(std::string(node->getId()), std::string("fern")))
	  {
	         material = model->setMaterial("res/fern.material");
	  }
	  else if(obpStringUtil::stringStartsWith(std::string(node->getId()), std::string("water")))
	  {
	    setWaterMaterial(node);
	    return true;
	  }
	}
	
	
        if (material)
        {
            MaterialParameter* ambientColorParam = material->getParameter("u_ambientColor");
            if (ambientColorParam)
            {
                ambientColorParam->setValue(_scene->getAmbientColor());
            }
            if (_lightNode && _lightNode->getLight())
            {
                MaterialParameter* lightDirectionParam = material->getParameter("u_lightDirection");
                MaterialParameter* lightColorParam = material->getParameter("u_lightColor");
                if (lightDirectionParam)
               {
                    lightDirectionParam->bindValue(_lightNode, &Node::getForwardVectorView);
                }
                if (lightColorParam)
               {
                    lightColorParam->setValue(_lightNode->getLight()->getColor());
                }
            }
        }
    }
    return true;
}
//------------------------------------



void Principal::finalize()
{
    SAFE_RELEASE(_scene);
}

void Principal::update(float elapsedTime)
{
  
  _skydome->setTranslation(_balsa.getCameraNode()->getTranslationWorld());

    // Rotate model
    //_bumpNode->rotateY(MATH_DEG_TO_RAD((float)elapsedTime / 1000.0f * 45.0f));
    
    //make axis node match position of balsa..
    _balsaAxis.updateAxis(_balsa.getCameraNode(), 5.0f);
    //_balsaAxis.updateAxis(_bumpLightNode, 5.0f);
    
    //_terrain->getNode()->setTranslation(_balsa.getNode()->getTranslationWorld());
   
   /*Node * trackpoint = _river.getCurrentTrackpoint();
   if(trackpoint)
    _balsaAxis.updateAxis(trackpoint, 5.0f);
    */
   
   
  _bumpElapsedTime+= elapsedTime;
  while(_bumpElapsedTime>_bumpPeriod)
  {
    _bumpElapsedTime -= _bumpPeriod;
  }
  float t = (_bumpElapsedTime/_bumpPeriod);
  _currBumpFrame = 100 * t;
  updateBumpFrame();
  
  //use this interpolation to update uvoffset too..
  _currUVOffset.x = t; 
  _currUVOffset.y = t; 
   
    
    //make global camera looks at the boat
    Node * camnode = _globalCamera->getNode();
    
    Vector3 balsaPos = _balsa.getNode()->getTranslation();
    camnode->setTranslationX(balsaPos.x - 45.0f);
    camnode->setTranslationZ(balsaPos.z - 45.0f);
    camnode->setTranslationY(balsaPos.y + 120.0f);
    
    obpMathUtil::lookAt(camnode, camnode->getTranslation(), balsaPos, Vector3::unitY());
    
    if(getState() == RUNNING)
    {
      _river.update();
      _oarLeft.update(elapsedTime);
      _oarRight.update(elapsedTime);
      _balsa.update(elapsedTime, _oarRight.getOutput(), _oarLeft.getOutput());
    }
    
    
    _form->update(elapsedTime);
    
   _yawSlider->setValue(_balsa.getYaw()*M_RAD_TO_DEG);
   _yawSlider->setEnabled(false);_yawSlider->setEnabled(true);
   _rollSlider->setValue(_balsa.getRoll()*M_RAD_TO_DEG);
   _rollSlider->setEnabled(false);_rollSlider->setEnabled(true);
   _pitchSlider->setValue(_balsa.getPitch()*M_RAD_TO_DEG);
   _pitchSlider->setEnabled(false);_pitchSlider->setEnabled(true);
}
//--------------------------------------------------

void Principal::render(float elapsedTime)
{
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, /*Vector4::zero()*/Vector4(0.0, 0.5, 0.0, 1.0), 1.0f, 0);
    
    //humm.. we need to setup world and projection matrix from _scene..
    _skydome->getModel()->draw(false);

    // Visit all the nodes in the scene for drawing
    _scene->visit(this, &Principal::drawScene);
    
    //getPhysicsController()->drawDebug(_scene->getActiveCamera()->getViewProjectionMatrix());
    
    _font->start();
    _font->drawText("Remo Izquierdo:", 1100, 50, Vector4(0, 0, 1, 1), 0, false);
    _font->drawText("Remo Derecho:", 1100, 180, Vector4(0, 0, 1, 1), 0, false);
    _font->finish();
    
    _form->draw();
   // _batch->draw();
}
//-------------------------------------------

bool Principal::drawScene(Node* node)
{
  if(node == _skydome) return true;
  
  if(node == _river.getCollisionNode())
  {
      _river.getCollisionNode()->getModel()->draw(true);
      return true;
  }
  
    // If the node visited contains a model, draw it
    Model* model = node->getModel(); 
    if (model)
    {
        model->draw(false);
    }
    else if (node->getTerrain())
	{
		node->getTerrain()->draw(false);
	}
    return true;
}
//-------------------------------

void Principal::keyEvent(Keyboard::KeyEvent evt, int key)
{
    if (evt == Keyboard::KEY_PRESS)
    {
        switch (key)
        {
	  case Keyboard::KEY_X:
	    //_balsaRigidBody->applyImpulse(Vector3(1.0f, 0.0f, 0.0 ));
	    _oarRight.notifyInput(1.0f);
	    break;
	  case Keyboard::KEY_Y:
	   // _balsaRigidBody->applyImpulse(Vector3(0.0f, 1.0f, 0.0 ));
	    break;
	  case Keyboard::KEY_Z:
	   // _balsaRigidBody->applyImpulse(Vector3(0.0f, 0.0f, 1.0 ));
	   _oarLeft.notifyInput(1.0f);
	    break;
	  case Keyboard::KEY_T:
	    _balsa.test();
	    break;
        case Keyboard::KEY_ESCAPE:
            exit();
            break;
	case Keyboard::KEY_U:
	//  _currBumpFrame++;
	//  updateBumpFrame();
	  break;
	case Keyboard::KEY_V:
	 // _currBumpFrame--;
	 // updateBumpFrame();
	  break;
        }
    }
}
//--------------------------------------


void Principal::updateBumpFrame()
{
    if(_currBumpFrame>99)_currBumpFrame = 0; 
    if(_currBumpFrame<0) _currBumpFrame = 99;
    int row, col;
    row = _currBumpFrame/ 10;
    col = _currBumpFrame % 10;
   //GP_WARN("row col : %d %d", row, col);
    _currBumpUVCoords.x = 0.1f * col;
    _currBumpUVCoords.y = 0.1f * row;
}
//--------------------------------------


void Principal::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    switch (evt)
    {
    case Touch::TOUCH_PRESS:
        break;
    case Touch::TOUCH_RELEASE:
	break;
    case Touch::TOUCH_MOVE:
        break;
    };
}
//-------------------------------


void Principal::controlEvent(Control* control, EventType evt)
{
    switch(evt)
    {
      case RELEASE:
	if(_cameraButton == control)
	{
	  onToggleCamera();
	}
	break;
      case VALUE_CHANGED:
	if(_flowCheckbox == control)
	{
	  GP_WARN("changed flowEnabled to %d", !_flowCheckbox->isChecked());
	  _river.enable(!_flowCheckbox->isChecked());
	}
	if(_pauseCheckbox == control)
	{
	  if(_pauseCheckbox->isChecked())
	  {
	    pause();
	  }
	  else
	  {
	    resume();
	  }
	  
	}
	break;
    }
}
//-------------------------------

void Principal::onToggleCamera()
{
    Logger::log(Logger::LEVEL_INFO, "onToggleCamera\n");
    if(_currentCamera == _globalCamera)
    {
      _currentCamera = _fpsCamera;
    }
    else
    {
      _currentCamera = _globalCamera;
      
    }
    _scene->setActiveCamera(_currentCamera);
}
//-------------------------------


Model* Principal::createGridModel(unsigned int lineCount)
{
    // There needs to be an odd number of lines
    lineCount |= 1;
    const unsigned int pointCount = lineCount * 4;
    const unsigned int verticesSize = pointCount * (3 + 3);  // (3 (position(xyz) + 3 color(rgb))

    std::vector<float> vertices;
    vertices.resize(verticesSize);

    const float gridLength = (float)(lineCount / 2);
    float value = -gridLength;
    for (unsigned int i = 0; i < verticesSize; ++i)
    {
        // Default line color is dark grey
        Vector4 color(0.3f, 0.3f, 0.3f, 1.0f);

        // Very 10th line is brighter grey
        if (((int)value) % 10 == 0)
        {
            color.set(0.45f, 0.45f, 0.45f, 1.0f);
        }

        // The Z axis is blue
        if (value == 0.0f)
        {
            color.set(0.15f, 0.15f, 0.7f, 1.0f);
        }

        // Build the lines
        vertices[i] = value;
        vertices[++i] = 0.0f;
        vertices[++i] = -gridLength;
        vertices[++i] = color.x;
        vertices[++i] = color.y;
        vertices[++i] = color.z;

        vertices[++i] = value;
        vertices[++i] = 0.0f;
        vertices[++i] = gridLength;
        vertices[++i] = color.x;
        vertices[++i] = color.y;
        vertices[++i] = color.z;

        // The X axis is red
        if (value == 0.0f)
        {
            color.set(0.7f, 0.15f, 0.15f, 1.0f);
        }
        vertices[++i] = -gridLength;
        vertices[++i] = 0.0f;
        vertices[++i] = value;
        vertices[++i] = color.x;
        vertices[++i] = color.y;
        vertices[++i] = color.z;

        vertices[++i] = gridLength;
        vertices[++i] = 0.0f;
        vertices[++i] = value;
        vertices[++i] = color.x;
        vertices[++i] = color.y;
        vertices[++i] = color.z;

        value += 1.0f;
    }
    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::COLOR, 3)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 2), pointCount, false);
    if (mesh == NULL)
    {
        return NULL;
    }
    mesh->setPrimitiveType(Mesh::LINES);
    mesh->setVertexData(&vertices[0], 0, pointCount);
    
    Model* model = Model::create(mesh);
    model->setMaterial("res/unlit.material");
    SAFE_RELEASE(mesh);
    
    return model;
}
//-----------------------------------------------

Model * Principal::createAxisModel(float size)
{
  
    const unsigned int verticesSize =3 * (6 + 6);  // (3 (position(xyz) + 3 color(rgb))

    std::vector<float> vertices;
    vertices.resize(verticesSize);

    setupLine(vertices, 0,  Vector3(0,0,0), Vector3(size, 0, 0), Vector3(1, 0, 0));
    setupLine(vertices, 1,  Vector3(0,0,0), Vector3(0, size, 0), Vector3(0, 1, 0));
    setupLine(vertices, 2,  Vector3(0,0,0), Vector3(0, 0, size), Vector3(0, 0, 1));
       
    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::COLOR, 3)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 2), 6, false);
    if (mesh == NULL)
    {
        return NULL;
    }
    mesh->setPrimitiveType(Mesh::LINES);
    mesh->setVertexData(&vertices[0], 0, 6);
    
    Model* model = Model::create(mesh);
    model->setMaterial("res/unlit.material");
    SAFE_RELEASE(mesh);
    
    return model;

}
//------------------------------------------------

void Principal::setDebugString(int id, const std::string & text)
{
  switch(id)
  {
    case 0: _debug0->setText(text.c_str()); break;
    case 1: _debug1->setText(text.c_str()); break;
  }
}
//------------------------------------------------

void  Principal::setDebugString(int id, float value)
{
  char buffer[80];
  sprintf(buffer, "%f", value);
  
  switch(id)
  {
    case 0: _debug0->setText(buffer);break;
    case 1: _debug1->setText(buffer);break;
  }
   
}
//------------------------------------------------

