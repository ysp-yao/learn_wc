class AudioDeviceModule : public rtc::RefCountInterface {
 public:
  // Deprecated.
  // TODO(henrika): to be removed.
  enum ErrorCode { kAdmErrNone = 0, kAdmErrArgument = 1 };

  enum AudioLayer {
    kPlatformDefaultAudio = 0,
    kWindowsCoreAudio,
    kWindowsCoreAudio2,  // experimental
    kLinuxAlsaAudio,
    kLinuxPulseAudio,
    kAndroidJavaAudio,
    kAndroidOpenSLESAudio,
    kAndroidJavaInputAndOpenSLESOutputAudio,
    kAndroidAAudioAudio,
    kAndroidJavaInputAndAAudioOutputAudio,
    kDummyAudio,
  };

  enum WindowsDeviceType {
    kDefaultCommunicationDevice = -1,
    kDefaultDevice = -2
  };

  // TODO(bugs.webrtc.org/7306): deprecated.
  enum ChannelType { kChannelLeft = 0, kChannelRight = 1, kChannelBoth = 2 };
};

class AudioDeviceModuleImpl : public AudioDeviceModule {
 public:
  enum PlatformType {
    kPlatformNotSupported = 0,
    kPlatformWin32 = 1,
    kPlatformWinCe = 2,
    kPlatformLinux = 3,
    kPlatformMac = 4,
    kPlatformAndroid = 5,
    kPlatformIOS = 6
  };

 private:

  AudioLayer audio_layer_;
  PlatformType platform_type_ = kPlatformNotSupported;
  bool initialized_ = false;
#if defined(WEBRTC_ANDROID)
  // Should be declared first to ensure that it outlives other resources.
  std::unique_ptr<AudioManager> audio_manager_android_;
#endif
  AudioDeviceBuffer audio_device_buffer_;
  std::unique_ptr<AudioDeviceGeneric> audio_device_;
};
