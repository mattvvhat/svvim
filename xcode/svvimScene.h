//
//  svvimScene.h
//  svvim
//
//  Created by Matthew Owen on 4/18/13.
//
//

#ifndef __svvim__svvimScene__
#define __svvim__svvimScene__

// GL
#include "cinder/gl/Texture.h"
#include "cinder/Surface.h"


class svvimScene {

public:
  svvimScene () : mTicker(0), mPosition(0.) { };
  
  /**
   * Play Scene
   * Begin playing a scene.
   */
  virtual void play () = 0;
  /**
   * Pause Scene
   * Prevent scene from actively being updated.
   */
  virtual void pause () = 0;
  /**
   * Stop a Scene
   * Stop and a scene from rendering and return it to its initial state.
   */
  virtual void stop () = 0;
  /**
   * Move Scene to a Position between its Start and End
   * Move scene to a position between its start and end. This changes the frame of the scene.
   */
  virtual void seek (float) = 0;
  /**
   * Update Current Texture of the Scene
   * Updates the texture of a scene based on its time and external factors.
   */
  virtual void update () = 0;
  /**
   * Whether the Scene has Completed
   * Returns a boolean specifying whether the scene has finished playing.
   * @return whether the scene is over
   */
  virtual bool isOver () const = 0;
  /**
   * Whether the Scene is Currently *playing* and Not Over
   * @return whether the scene is playing and not over
   */
  virtual bool isPlaying () const = 0;
  /**
   * Return the Current Frame as a Texture
   * Returns a gl::Texture object representing the current frame of the scene
   */
  virtual cinder::gl::Texture getTexture () const = 0;
  /**
   * Return the Current Frame as a Surface
   * Returns a cinder::Surface object representing the current frame of the scene
   */
  cinder::Surface getSurface () {
    return cinder::Surface(getTexture());
  };
  
private:
  // Number of ticks or updates since creation
  int mTicker, mNumUpdates;
  // Float inside [0, 1] representing how complete the scene is
  float mPosition;
  //
  cinder::gl::Texture mCurrentTexture;
  
};



#endif
