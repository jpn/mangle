#ifndef MANGLE_SOUND_SOURCE_H
#define MANGLE_SOUND_SOURCE_H

#include <string>
#include <stdint.h>
#include <assert.h>

#include "../stream/stream.h"

namespace Mangle {
namespace Sound {

/// A stream containing raw sound data and information about the format
class SampleSource : public Stream::Stream
{
 protected:
  bool isEof;

 public:
  SampleSource()
    {
      // These are usually not needed for sound data
      isSeekable = false;
      hasPosition = false;
      hasSize = false;

      isEof = false;
    }

  /// Get the sample rate, number of channels, and bits per
  /// sample. NULL parameters are ignored.
  virtual void getInfo(int32_t *rate, int32_t *channels, int32_t *bits) const = 0;

  bool eof() const { return isEof; }

  // Disabled functions
  void seek(size_t pos) const { assert(0); }
  size_t tell() const { assert(0); }
  size_t size() const { assert(0); }
};

/// A factory interface for loading SampleSources from file or stream
class SampleSourceLoader
{
 public:
  /// If true, the stream version of load() works
  bool canLoadStream;

  /// If true, the file version of load() works
  bool canLoadFile;

  /// Load a sound input source from file (if canLoadFile is true)
  virtual SampleSource *load(const std::string &file) = 0;

  /// Load a sound input source from stream (if canLoadStream is true)
  virtual SampleSource *load(Stream::Stream *input) = 0;

  /// Virtual destructor
  virtual ~SampleSourceLoader() {}
};

}} // namespaces
#endif