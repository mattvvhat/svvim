#include "cinder/app/AppBasic.h"

#include "cinder/qtime/QuickTime.h" 
#include "distortion.h"

// GL
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"

// Audio
#include "cinder/audio/Io.h"
#include "cinder/audio/Input.h"
#include "cinder/audio/FftProcessor.h"
#include "cinder/audio/PcmBuffer.h"

// Perlin noise
#include "cinder/Perlin.h"

// Resource definitions
#define RES_VERT_GLSL "pass-through.glsl"
#define RES_FRAG_GLSL "yea.glsl"
#define RES_SVVIM_LOGO_PNG "swim-bw-logo-3.png"

using namespace ci;
using namespace ci::app;
using namespace std;

class svvimApp : public AppBasic {
public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
  void prepareSettings (Settings *settings);
  void getAlpha ();

private:
  
  int mMovieIndex;
  
  audio::Input mInput;
  audio::PcmBuffer32fRef mPcmBuffer;
  
  gl::Texture mFrameTexture;
  gl::Texture mMaskTexture;
  gl::Texture mImageTexture;
  
  gl::Texture mCurrentBgTexture;
  gl::Texture * mBgTextureList;
  
  gl::Texture mSvvimLogoPng;
  
  gl::Fbo mFbo;
  gl::GlslProg mShader;
  
  Perlin mPerlinGenerator;
  
  qtime::MovieGl mCurrentMovie, mPoolWaterMovie;
  qtime::MovieGl * mMovieList;
  
  float mAlpha;

  std::shared_ptr <float> mFftDataRef;
  
  void incrementMovie();
  
  int mLastElapsedSecondsSwitch;
};

void svvimApp::setup() {
  mLastElapsedSecondsSwitch = 0;
  mPerlinGenerator = Perlin();
  
  mBgTextureList = new gl::Texture [3];
  
  
  
  mPoolWaterMovie = qtime::MovieGl(getResourcePath("water-over.m4v"));
  mPoolWaterMovie.setLoop();
  mPoolWaterMovie.play();
  
  mMovieList = new qtime::MovieGl [13];
  
  mMovieList[ 0] = qtime::MovieGl(getResourcePath("SVVIM4_VIS_04.mov"));
  mMovieList[ 1] = qtime::MovieGl(getResourcePath("SVVIM4_VIS_08.mov"));
  mMovieList[ 2] = qtime::MovieGl(getResourcePath("SVVIM4_VIS_10.mov"));
  mMovieList[ 3] = qtime::MovieGl(getResourcePath("SVVIM4_VIS_11.mov"));
  mMovieList[ 4] = qtime::MovieGl(getResourcePath("SVVIM4_VIS_07.mov"));
  mMovieList[ 5] = qtime::MovieGl(getResourcePath("movie-006.mov"));
  mMovieList[ 6] = qtime::MovieGl(getResourcePath("movie-002.mov"));
  mMovieList[ 7] = qtime::MovieGl(getResourcePath("movie-008.mov"));
  mMovieList[ 8] = qtime::MovieGl(getResourcePath("movie-009.mov"));
  mMovieList[ 9] = qtime::MovieGl(getResourcePath("movie-010.mov"));
  mMovieList[10] = qtime::MovieGl(getResourcePath("movie-011.mov"));
  mMovieList[11] = qtime::MovieGl(getResourcePath("movie-012.mov"));
  mMovieList[12] = qtime::MovieGl(getResourcePath("movie-013.mov"));
  
  // Load Svvim Logo from PNG
  
  app::console() << "setup\n";
  
  // Compile shader
  try {
    mShader = gl::GlslProg(loadResource(RES_VERT_GLSL), loadResource(RES_FRAG_GLSL));
  }
  catch (const std::exception & e) {
    mShader = gl::GlslProg();
    console() << "Compile error in shader: " << e.what() << "\n";
  }
  
  // Begin reading MIC
  mInput = audio::Input();
  mInput.start();
  
  //
  mMovieIndex = 4;
  
  mCurrentMovie = mMovieList[mMovieIndex];
  
  mCurrentMovie.play();
  mCurrentMovie.setVolume(0.);
  
  gl::Texture::Format format;
  format.setTargetRect();
  
  mImageTexture = gl::Texture(loadImage(loadResource("saturday-night.jpg")), format);
  mSvvimLogoPng = gl::Texture(loadImage(loadResource(RES_SVVIM_LOGO_PNG)), format);
  mCurrentBgTexture = gl::Texture(loadImage(loadResource("bgMarble.png")), format);
  
  mCurrentMovie.seekToTime(82.);
  
  gl::enableAlphaBlending();
}

void svvimApp::prepareSettings (Settings *settings) {
  app::console() << "prepareSettings (entering)\n";
  
  mCurrentMovie = qtime::MovieGl(getResourcePath("water-under.m4v"));
  mCurrentMovie.play();
  mCurrentMovie.setVolume(0.);
  
  mFrameTexture = mCurrentMovie.getTexture();
  
  //
  settings->setWindowSize(mCurrentMovie.getWidth(), mCurrentMovie.getHeight());
  settings->setFrameRate(mCurrentMovie.getFramerate()*2);
  settings->setFullScreen(true);
  
  // ...
  app::console() << "prepareSettings (leaving)\n";
}

void svvimApp::mouseDown(MouseEvent event) {
}

void svvimApp::update() {
  float noise = mPerlinGenerator.fBm(Vec2f(0., app::getElapsedSeconds()));
  
  app::console() << noise << "\n";
  // Update alpha values
  getAlpha();

  if (mPoolWaterMovie)
    mMaskTexture = mPoolWaterMovie.getTexture();
  
  if (mCurrentMovie.isDone()) {
    incrementMovie();
  }
  
  app::console() << 10 * app::getElapsedSeconds() << "\n";
  

  mImageTexture = mCurrentMovie.getTexture();
  
  //mImageTexture = mCurrentBgTexture;
  
  // ...
  
  

}

// Just create the latest thing
void svvimApp::draw() {
  
  // SvvimLogo
  if (mSvvimLogoPng)
    mSvvimLogoPng.bind(0);
  
  // Mask
  if (mMaskTexture)
    mMaskTexture.bind(1);
  
  //
  if (mImageTexture)
    mImageTexture.bind(2);
  
  // Clear to black
  gl::clear();

  /*
  if (mMaskTexture) {
    Area bounds = mMaskTexture.getBounds();
    
    Area displayArea = getWindowBounds();
    // displayArea.y1 = displayArea.y1 + displayArea.getHeight() * .05;
    // displayArea.y2 = displayArea.y2 - displayArea.getHeight() * .05;
    
    Rectf centeredRect = Rectf(mMaskTexture.getBounds()).getCenteredFit(displayArea, false);
    
    float coverAspectRatio = mMaskTexture.getAspectRatio();
    float windowAspectRatio = getWindowAspectRatio();
    
    if (coverAspectRatio > windowAspectRatio) {
      float scale = displayArea.getHeight()/centeredRect.getHeight();
      centeredRect.scaleCentered(scale);
    }
    else {
      float scale = displayArea.getWidth()/centeredRect.getWidth();
      centeredRect.scaleCentered(scale);
    }
    
    gl::draw(mMaskTexture, centeredRect);
  } */

  
  // Draw pool-water background cover
  if (mImageTexture) {
    Area bounds = mImageTexture.getBounds();

    Area displayArea = getWindowBounds();
    displayArea.y1 = displayArea.y1 + displayArea.getHeight() * .05;
    displayArea.y2 = displayArea.y2 - displayArea.getHeight() * .05;
    
    Rectf centeredRect = Rectf(mImageTexture.getBounds()).getCenteredFit(displayArea, true);

    float coverAspectRatio = mImageTexture.getAspectRatio();
    float windowAspectRatio = getWindowAspectRatio();
    
    if (coverAspectRatio > windowAspectRatio) {
      float scale = displayArea.getHeight()/centeredRect.getHeight();
      centeredRect.scaleCentered(scale);
    }
    else {
      float scale = displayArea.getWidth()/centeredRect.getWidth();
      centeredRect.scaleCentered(scale);
    }
    
    gl::draw(mCurrentBgTexture, centeredRect);
  }

  if (mShader) {
    float displacement = 10. - min(mAlpha * 300.0, 10.0);
    mShader.bind();
    mShader.uniform("displacement", displacement);
    mShader.uniform("maskTexture", 0);
    mShader.uniform("innerTexture", 2);
    mShader.uniform("fillColor", Vec3f(0.3, 0.3, 0.3));
    mShader.uniform("bounds", Vec2f(getWindowWidth(), getWindowHeight()));
    mShader.uniform("alpha", 6 * mAlpha);
  }
  
  if (mSvvimLogoPng) {
    Rectf centeredRect = Rectf(mSvvimLogoPng.getBounds()).getCenteredFit(getWindowBounds(), true);
    Rectf topRect = centeredRect;
    gl::draw(mSvvimLogoPng, centeredRect);
    centeredRect.y1 -= 200;
    centeredRect.y2 -= 200;
    //gl::draw(mSvvimLogoPng, centeredRect);
    centeredRect.y1 += 400;
    centeredRect.y2 += 400;
    //gl::draw(mSvvimLogoPng, centeredRect);
  }
  
  if (mShader)
    mShader.unbind();
  
  if (mMaskTexture)
    mMaskTexture.unbind();
  
  if (mImageTexture)
    mImageTexture.unbind();
  
  if (mSvvimLogoPng)
    mSvvimLogoPng.unbind();

}

void svvimApp::getAlpha () {
  
  // Sound buffer for FFT
  mPcmBuffer = mInput.getPcmBuffer();
  
  //
  if (mPcmBuffer) {
    mFftDataRef = audio::calculateFft(mPcmBuffer->getChannelData( audio::CHANNEL_FRONT_LEFT ), 512);
  }
  
	uint16_t bandCount = 64;
	
	if( ! mPcmBuffer )
    return;
	
	//use the most recent Pcm data to calculate t`he Fft
	std::shared_ptr <float> fftRef = audio::calculateFft( mPcmBuffer->getChannelData( audio::CHANNEL_FRONT_LEFT ), bandCount );
  
	if( ! fftRef ) {
		return;
	}
	
	float * fftBuffer = fftRef.get();
	 
  //mAlpha = max(2, mAlpha);
	//draw the bands
	for( int i = 0; i < ( bandCount/32 ); i++ ) {
		 mAlpha += fftBuffer[i] / bandCount / 4;
	}
  
  mAlpha /= 6.5;
}

/**
 * Increment Movie
 * Restarts and stpos current movie, then begins playing next movie in mMovieList without volumne.
 */
void svvimApp::incrementMovie () {
  mCurrentMovie.setLoop(true);
  /*
  // If movie is on, seek to start and stop
  if (mCurrentMovie) {
    mCurrentMovie.seekToStart();
    mCurrentMovie.setLoop(false);
    mCurrentMovie.stop();
  }
  
  // Increment movie index
  mMovieIndex = (mMovieIndex + 1) % 6;
  
  // Get the next movie from the list, start it from the beginning, and turn the volume off
  mCurrentMovie = mMovieList[mMovieIndex];
  mCurrentMovie.setVolume(0.);
  mCurrentMovie.play();
  mCurrentMovie.seekToStart();
   */
}

// I guess this registers it as a cinder app.
CINDER_APP_BASIC(svvimApp, RendererGl)