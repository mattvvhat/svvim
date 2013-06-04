//
//  distortion.cpp
//  xxxgeometry
//
//  Created by Matthew Owen on 12/21/12.
//
//

#include "distortion.h"
#include <ctime>
#include "cinder/app/App.h"

using namespace cinder;

/**
 * Construct an empty distortion object
 */
distortion::distortion () {}
/**
 * Construct a distortion from a surface.
 * @note This copies the surface to a private variable.
 */
distortion::distortion (const Surface & surface)
: mSurface(surface), mOriginalSurface(surface) {
}
/**
 * Construct a distortion from a texture
 * @note This constructs a new surface from the texture.
 * @param texture Texture the texture to convert into a surface
 */
distortion::distortion (const gl::Texture & texture)
: mSurface(Surface(texture)), mOriginalSurface(Surface(texture)) {
}
/**
 * Apply Rainbow-Crush Distortion
 * @param intensity a double-precision number between 0 and 1
 */
void distortion::applyRainbowCrush(const double intensity) {
  
  Surface::Iter iter = mSurface.getIter();
  
  // int a = time(0);
  int t = 0;
  
  while (iter.line()) {
    while (iter.pixel()) {
      t++;
      iter.rClamped(0, 0) = iter.gClamped(7, 3);
      iter.gClamped(0, 0) = iter.rClamped(7, 1);
      iter.bClamped(0, 0) = iter.bClamped(7, 2);
      
    }
  }
  app::console() << t << "\n";
}
/**
 * Get Surface Object
 * @
 */
Surface distortion::getSurface() {
  return mSurface;
}