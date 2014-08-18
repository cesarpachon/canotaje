#ifndef TEMPLATEGAME_H_
#define TEMPLATEGAME_H_

#include "gameplay.h"
#include "boat.h"
#include "oardevice.h"
#include "context.h"
#include "river.h"

using namespace gameplay;


typedef std::map<std::string, Node *> _NODE_MAP;
typedef std::map<std::string, Node *>::iterator _NODE_MAP_ITER;


/**
 * Main game class.
 */
class Principal: public Game, public Control::Listener
{
public:
  
  friend class River;
  friend class Boat;
  
  
  static Principal& getInstance();

    /**
     * Constructor.
     */
    Principal();

    /**
     * @see Game::keyEvent
     */
	void keyEvent(Keyboard::KeyEvent evt, int key);
	
    /**
     * @see Game::touchEvent
     */
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);
    
    void controlEvent(Control* control, EventType evt);

protected:

    /**
     * @see Game::initialize
     */
    void initialize();

    /**
     * @see Game::finalize
     */
    void finalize();

    /**
     * @see Game::update
     */
    void update(float elapsedTime);

    /**
     * @see Game::render
     */
    void render(float elapsedTime);
    
    void setDebugString(int id, const std::string & text);
    void setDebugString(int id, float value);
    
    //used to bind value to the material 
    const Vector2 &getCurrBumpUVCoords() const {return _currBumpUVCoords;} ;
    const Vector2 &getCurrUVOffset() const {return _currUVOffset;} ;

private:

    /**
     * Draws the scene each frame.
     */
    bool drawScene(Node* node);
    
    void onToggleCamera();
    
    Node* setDefaultMaterial(Node *node);
    Node* setWaterMaterial(Node *node);
    
    bool bindLights(Node* node);

     Model* createGridModel(unsigned int lineCount = 41);
    Model * createAxisModel(float size);
    
    void loadSceneObw();
    /**
     * check the masterNodeMesh cache, if not exist, try to load from a bundle (and register in cache!)
     */
    Model * getMasterModel(std::string &name);
    
    /**
     * temporal
     */
    void updateBumpFrame();

    
    //PRIVATE ATTRIBUTES
    
    Scene* _scene;
    Node * _lightNode;
    
    
    Boat _balsa;
    Node * _axis;
    obpLineSet _balsaAxis;

    OarDevice _oarLeft;
    OarDevice _oarRight;
    
    //GUI
    Font* _font;
    Form * _form;
    Slider * _rollSlider;
    Slider * _yawSlider;
    Slider * _pitchSlider;
    Button * _cameraButton;
    CheckBox * _flowCheckbox;
    CheckBox * _pauseCheckbox;
    Label * _debug0;
    Label * _debug1;
    
    Camera * _globalCamera;
    Camera * _fpsCamera; 
    Camera * _currentCamera;
    
    Node * _skydome;
    
     River _river;
     
     //Node * _waterTile;
     
     //bumpmapping test
     Model * _bumpModel;
     Node * _bumpNode;
     Node * _bumpLightNode;
     int _currBumpFrame;
     Vector2 _currBumpUVCoords;
     float _bumpSpeed;
     float _bumpPeriod;
     float _bumpElapsedTime;
     Vector2 _currUVOffset;
     
     
     //terrain
     gameplay::Terrain * _terrain;
     
     //parsing of scene.obw.. 
     /**
      *keep a temporal map of reusable meshes while parsing scene.obw. clean after parsing.  
      */
     _NODE_MAP _masterMeshNodes;
     
};

#endif
