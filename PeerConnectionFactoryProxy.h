//
// 宏展开
//



// RTC_FROM_HERE === ::rtc::Location(__FUNCTION__, "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/peerconnectionfactoryproxy.h" ":" "26")

namespace webrtc {

// BEGIN_SIGNALING_PROXY_MAP(PeerConnectionFactory)
  template <class INTERNAL_CLASS>
  class PeerConnectionFactoryProxyWithInternal;

  typedef PeerConnectionFactoryProxyWithInternal<PeerConnectionFactoryInterface> PeerConnectionFactoryProxy;

  template <class INTERNAL_CLASS>
  class PeerConnectionFactoryProxyWithInternal : public PeerConnectionFactoryInterface {
  protected:
    typedef PeerConnectionFactoryInterface C;
  public:
    const INTERNAL_CLASS* internal() const { return c_; }
    INTERNAL_CLASS* internal() { return c_; }
  protected:
    PeerConnectionFactoryProxyWithInternal(rtc::Thread* signaling_thread, INTERNAL_CLASS* PeerConnectionFactory) :
      signaling_thread_(signaling_thread), c_(PeerConnectionFactory) {}
  private:
    mutable rtc::Thread* signaling_thread_;
  protected:
    ~PeerConnectionFactoryProxyWithInternal() {
      MethodCall0<PeerConnectionFactoryProxyWithInternal, void> call(this, &PeerConnectionFactoryProxyWithInternal::DestroyInternal);
      call.Marshal(::rtc::Location(__FUNCTION__, "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/peerconnectionfactoryproxy.h" ":" "26"), destructor_thread());
    }
  private:
    void DestroyInternal() { c_ = nullptr; }
    rtc::scoped_refptr<INTERNAL_CLASS> c_;
  public:
    static rtc::scoped_refptr<PeerConnectionFactoryProxyWithInternal> 
    Create(rtc::Thread* signaling_thread, INTERNAL_CLASS* PeerConnectionFactory) { 
        return new rtc::RefCountedObject<PeerConnectionFactoryProxyWithInternal>(signaling_thread, PeerConnectionFactory); 
    }
// PROXY_SIGNALING_THREAD_DESTRUCTOR()
  private:
    rtc::Thread* destructor_thread() const { return signaling_thread_; } 
  public:
    using PeerConnectionFactoryInterface::CreateVideoSource;
// PROXY_METHOD1
    void SetOptions(const Options& a1) override { 
        MethodCall1<C, void, const Options&> call(c_, &C::SetOptions, std::move(a1)); 
        return call.Marshal(::rtc::Location(__FUNCTION__, "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/peerconnectionfactoryproxy.h" ":" "33"), signaling_thread_); 
    }
// PROXY_METHOD5
    rtc::scoped_refptr<PeerConnectionInterface> 
    CreatePeerConnection(const PeerConnectionInterface::RTCConfiguration& a1, const MediaConstraintsInterface* a2, std::unique_ptr<cricket::PortAllocator> a3, std::unique_ptr<rtc::RTCCertificateGeneratorInterface> a4, PeerConnectionObserver* a5) override { 
        MethodCall5<C, rtc::scoped_refptr<PeerConnectionInterface>, const PeerConnectionInterface::RTCConfiguration&, const MediaConstraintsInterface*, std::unique_ptr<cricket::PortAllocator>, std::unique_ptr<rtc::RTCCertificateGeneratorInterface>, PeerConnectionObserver*> call(c_, &C::CreatePeerConnection, std::move(a1), std::move(a2), std::move(a3), std::move(a4), std::move(a5)); 
        return call.Marshal(::rtc::Location(__FUNCTION__, "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/peerconnectionfactoryproxy.h" ":" "40"), signaling_thread_); 
    };
// PROXY_METHOD4
    rtc::scoped_refptr<PeerConnectionInterface> CreatePeerConnection(const PeerConnectionInterface::RTCConfiguration& a1, std::unique_ptr<cricket::PortAllocator> a2, std::unique_ptr<rtc::RTCCertificateGeneratorInterface> a3, PeerConnectionObserver* a4) override { 
        MethodCall4<C, rtc::scoped_refptr<PeerConnectionInterface>, const PeerConnectionInterface::RTCConfiguration&, std::unique_ptr<cricket::PortAllocator>, std::unique_ptr<rtc::RTCCertificateGeneratorInterface>, PeerConnectionObserver*> call(c_, &C::CreatePeerConnection, std::move(a1), std::move(a2), std::move(a3), std::move(a4)); 
        return call.Marshal(::rtc::Location(__FUNCTION__, "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/peerconnectionfactoryproxy.h" ":" "46"), signaling_thread_); 
    };
// PROXY_METHOD1
    rtc::scoped_refptr<MediaStreamInterface> CreateLocalMediaStream(const std::string& a1) override { 
        MethodCall1<C, rtc::scoped_refptr<MediaStreamInterface>, const std::string&> call(c_, &C::CreateLocalMediaStream, std::move(a1)); 
        return call.Marshal(::rtc::Location(__FUNCTION__, "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/peerconnectionfactoryproxy.h" ":" "48"), signaling_thread_); 
    }
// PROXY_METHOD1
    rtc::scoped_refptr<AudioSourceInterface> CreateAudioSource(const MediaConstraintsInterface* a1) override { 
        MethodCall1<C, rtc::scoped_refptr<AudioSourceInterface>, const MediaConstraintsInterface*> call(c_, &C::CreateAudioSource, std::move(a1)); 
        return call.Marshal(::rtc::Location(__FUNCTION__, "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/peerconnectionfactoryproxy.h" ":" "50"), signaling_thread_); 
    }
// PROXY_METHOD1
    rtc::scoped_refptr<AudioSourceInterface> CreateAudioSource(const cricket::AudioOptions& a1) override { 
        MethodCall1<C, rtc::scoped_refptr<AudioSourceInterface>, const cricket::AudioOptions&> call(c_, &C::CreateAudioSource, std::move(a1)); 
        return call.Marshal(::rtc::Location(__FUNCTION__, "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/peerconnectionfactoryproxy.h" ":" "53"), signaling_thread_); 
    }
// PROXY_METHOD2
    rtc::scoped_refptr<VideoTrackSourceInterface> CreateVideoSource(std::unique_ptr<cricket::VideoCapturer> a1, const MediaConstraintsInterface* a2) override { 
        MethodCall2<C, rtc::scoped_refptr<VideoTrackSourceInterface>, std::unique_ptr<cricket::VideoCapturer>, const MediaConstraintsInterface*> call(c_, &C::CreateVideoSource, std::move(a1), std::move(a2)); 
        return call.Marshal(::rtc::Location(__FUNCTION__, "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/peerconnectionfactoryproxy.h" ":" "57"), signaling_thread_); 
    }
// PROXY_METHOD1
    rtc::scoped_refptr<VideoTrackSourceInterface> CreateVideoSource(std::unique_ptr<cricket::VideoCapturer> a1) override { 
        MethodCall1<C, rtc::scoped_refptr<VideoTrackSourceInterface>, std::unique_ptr<cricket::VideoCapturer>> call(c_, &C::CreateVideoSource, std::move(a1)); 
        return call.Marshal(::rtc::Location(__FUNCTION__, "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/peerconnectionfactoryproxy.h" ":" "60"), signaling_thread_); 
    }
// PROXY_METHOD2
    rtc::scoped_refptr<VideoTrackInterface> CreateVideoTrack(const std::string& a1, VideoTrackSourceInterface* a2) override { 
        MethodCall2<C, rtc::scoped_refptr<VideoTrackInterface>, const std::string&, VideoTrackSourceInterface*> call(c_, &C::CreateVideoTrack, std::move(a1), std::move(a2)); 
        return call.Marshal(::rtc::Location(__FUNCTION__, "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/peerconnectionfactoryproxy.h" ":" "64"), signaling_thread_); 
    }
// PROXY_METHOD2
    rtc::scoped_refptr<AudioTrackInterface> CreateAudioTrack(const std::string& a1, AudioSourceInterface* a2) override { 
        MethodCall2<C, rtc::scoped_refptr<AudioTrackInterface>, const std::string&, AudioSourceInterface*> call(c_, &C::CreateAudioTrack, std::move(a1), std::move(a2)); 
        return call.Marshal(::rtc::Location(__FUNCTION__, "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/peerconnectionfactoryproxy.h" ":" "66"), signaling_thread_); 
    }
// PROXY_METHOD2
    bool StartAecDump(rtc::PlatformFile a1, int64_t a2) override { 
        MethodCall2<C, bool, rtc::PlatformFile, int64_t> call(c_, &C::StartAecDump, std::move(a1), std::move(a2)); 
        return call.Marshal(::rtc::Location(__FUNCTION__, "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/peerconnectionfactoryproxy.h" ":" "67"), signaling_thread_); 
    }
// PROXY_METHOD0
    void StopAecDump() override { 
        MethodCall0<C, void> call(c_, &C::StopAecDump); 
        return call.Marshal(::rtc::Location(__FUNCTION__, "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/peerconnectionfactoryproxy.h" ":" "68"), signaling_thread_); 
    }
// PROXY_METHOD1
    bool StartRtcEventLog(rtc::PlatformFile a1) override { 
        MethodCall1<C, bool, rtc::PlatformFile> call(c_, &C::StartRtcEventLog, std::move(a1)); 
        return call.Marshal(::rtc::Location(__FUNCTION__, "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/peerconnectionfactoryproxy.h" ":" "71"), signaling_thread_); 
    }
// PROXY_METHOD2
    bool StartRtcEventLog(rtc::PlatformFile a1, int64_t a2) override { 
        MethodCall2<C, bool, rtc::PlatformFile, int64_t> call(c_, &C::StartRtcEventLog, std::move(a1), std::move(a2)); 
        return call.Marshal(::rtc::Location(__FUNCTION__, "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/peerconnectionfactoryproxy.h" ":" "72"), signaling_thread_); 
    }
// PROXY_METHOD0
    void StopRtcEventLog() override { 
        MethodCall0<C, void> call(c_, &C::StopRtcEventLog); 
        return call.Marshal(::rtc::Location(__FUNCTION__, "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/peerconnectionfactoryproxy.h" ":" "73"), signaling_thread_); 
    }
// END_PROXY_MAP
  };
}