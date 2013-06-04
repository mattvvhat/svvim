//
//  distortion.h
//  xxxgeometry
//
//  Created by Matthew Owen on 12/21/12.
//
//

#ifndef xxxgeometry_distortion_h
#define xxxgeometry_distortion_h

#include "cinder/Surface.h"
#include "cinder/gl/Fbo.h"

class distortion {
public:
  
  distortion();
  distortion(const cinder::Surface &);
  distortion(const cinder::gl::Texture &);
  
  void applyRainbowCrush (const double);
  distortion & applyBlur (const double);
  distortion & applyNoise (const double);
  
  void reset ();
  
  cinder::Surface getSurface ();
  
  enum DISTORTION_TYPE { RAINBOWCRUSH, BLUR, NOISE };
  
private:
  
  cinder::Surface mOriginalSurface;
  cinder::Surface mSurface;
  
};
/**
 * Helper class that will be used in optimized versions of the app.
 */
class distortionType {
public:
  
  distortionType();
  
  distortionType & setRainbowCrush (double);
  distortionType & setBlur (double);
  distortionType & setNoise (double);
  
private:
  
  float mRainbowCrush;
  float mBlur;
  float mRNoise;
};

#endif
