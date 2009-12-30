#ifndef MANGLE_SOUND_OPENAL_OUT_H
#define MANGLE_SOUND_OPENAL_OUT_H

#include "../output.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <list>

namespace Mangle {
namespace Sound {

/// OpenAL sound output
class OpenAL_Sound : public Sound
{
 protected:
  ALuint inst;

 public:
  OpenAL_Sound(SampleSource *input);
  ~OpenAL_Sound();

  /// Play or resume the sound
  void play();

  /// Stop the sound
  void stop();

  /// Pause the sound, may be resumed later
  void pause();

  /// Check if the sound is still playing
  bool isPlaying();

  /// Set the volume. The parameter must be between 0.0 and 1.0.
  void setVolume(float);

  /// Set the 3D position.
  void setPos(float x, float y, float z);
};

class OpenALFactory : public SoundFactory
{
  ALCdevice  *Device;
  ALCcontext *Context;
  bool didSetup;

 public:
  /// Initialize object. Pass true (default) if you want the
  /// constructor to set up the current ALCdevice and ALCcontext for
  /// you.
  OpenALFactory(bool doSetup = true)
   : didSetup(doSetup)
    {
      needsUpdate = false;
      has3D = true;
      canRepeatStream = false;
      canLoadFile = false;
      canLoadStream = false;
      canLoadSource = true;

      if(doSetup)
        {
          // Set up sound system
          Device = alcOpenDevice(NULL);
          Context = alcCreateContext(Device, NULL);

          if(!Device || !Context)
            fail("Failed to initialize context or device");

          alcMakeContextCurrent(Context);
        }
    }

  ~OpenALFactory()
    {
      // Deinitialize sound system
      if(didSetup)
        {
          alcMakeContextCurrent(NULL);
          if(Context) alcDestroyContext(Context);
          if(Device) alcCloseDevice(Device);
        }
    }

  Sound *load(const std::string &file, bool stream=false) { assert(0); }
  Sound *load(Stream::Stream *input, bool stream=false) { assert(0); }
  Sound *load(SampleSource* input, bool stream=false)
  { return new OpenAL_Sound(input); }

  void update() {}
  setListenerPos(float x, float y, float z,
                 float fx, float fy, float fz,
                 float ux, float uy, float uz)
    {
      ALfloat orient[6];
      orient[0] = fx;
      orient[1] = fy;
      orient[2] = fz;
      orient[3] = ux;
      orient[4] = uy;
      orient[5] = uz;
      alListener3f(AL_POSITION, x, y, z);
      alListenerfv(AL_ORIENTATION, orient);
    }
};

}} // namespaces
#endif