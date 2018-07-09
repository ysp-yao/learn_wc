namespace webrtc {

// BEGIN_SIGNALING_PROXY_MAP
  template <class INTERNAL_CLASS> 
  class MediaStreamProxyWithInternal; 
  
  typedef MediaStreamProxyWithInternal<MediaStreamInterface> MediaStreamProxy; 
  
  template <class INTERNAL_CLASS> 
  class MediaStreamProxyWithInternal : public MediaStreamInterface {
    protected: 
      typedef MediaStreamInterface C; 
    public: 
      const INTERNAL_CLASS* internal() const { return c_; } 
      INTERNAL_CLASS* internal() { return c_; } 
    protected: 
      MediaStreamProxyWithInternal(rtc::Thread* signaling_thread, INTERNAL_CLASS* MediaStream) : signaling_thread_(signaling_thread), c_(MediaStream) {} 
    private: 
      mutable rtc::Thread* signaling_thread_; 
    protected: 
      ~MediaStreamProxyWithInternal() { 
        MethodCall0<MediaStreamProxyWithInternal, void> call(this, &MediaStreamProxyWithInternal::DestroyInternal); 
        call.Marshal(::rtc::Location(__FUNCTION__, "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/mediastreamproxy.h" ":" "23"), destructor_thread()); 
      } 
    private: 
      void DestroyInternal() { c_ = nullptr; } 
      rtc::scoped_refptr<INTERNAL_CLASS> c_; 
    public: 
      static rtc::scoped_refptr<MediaStreamProxyWithInternal> 
      Create(rtc::Thread* signaling_thread, INTERNAL_CLASS* MediaStream) { 
        return new rtc::RefCountedObject<MediaStreamProxyWithInternal>(signaling_thread, MediaStream); 
      }
// PROXY_SIGNALING_THREAD_DESTRUCTOR
    private: 
      rtc::Thread* destructor_thread() const { return signaling_thread_; } 
    public:
// PROXY_CONSTMETHOD0
      std::string label() const override { 
        ConstMethodCall0<C, std::string> call(c_, &C::label); 
        return call.Marshal(::rtc::Location(__FUNCTION__, "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/mediastreamproxy.h" ":" "25"), signaling_thread_); 
      }
// PROXY_METHOD0
      AudioTrackVector GetAudioTracks() override { 
        MethodCall0<C, AudioTrackVector> call(c_, &C::GetAudioTracks); 
        return call.Marshal(::rtc::Location(__FUNCTION__, "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/mediastreamproxy.h" ":" "26"), signaling_thread_); 
      }
// PROXY_METHOD0
      VideoTrackVector GetVideoTracks() override { 
        MethodCall0<C, VideoTrackVector> call(c_, &C::GetVideoTracks); 
        return call.Marshal(::rtc::Location(__FUNCTION__, "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/mediastreamproxy.h" ":" "27"), signaling_thread_); 
      }
// PROXY_METHOD1
      rtc::scoped_refptr<AudioTrackInterface> FindAudioTrack(const std::string& a1) override { 
        MethodCall1<C, rtc::scoped_refptr<AudioTrackInterface>, const std::string&> call(c_, &C::FindAudioTrack, std::move(a1)); 
        return call.Marshal(::rtc::Location(__FUNCTION__, "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/mediastreamproxy.h" ":" "30"), signaling_thread_); 
      }
// PROXY_METHOD1
      rtc::scoped_refptr<VideoTrackInterface> FindVideoTrack(const std::string& a1) override { 
        MethodCall1<C, rtc::scoped_refptr<VideoTrackInterface>, const std::string&> call(c_, &C::FindVideoTrack, std::move(a1)); 
        return call.Marshal(::rtc::Location(__FUNCTION__, "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/mediastreamproxy.h" ":" "33"), signaling_thread_); 
      }
// PROXY_METHOD1
      bool AddTrack(AudioTrackInterface* a1) override { 
        MethodCall1<C, bool, AudioTrackInterface*> call(c_, &C::AddTrack, std::move(a1)); 
        return call.Marshal(::rtc::Location(__FUNCTION__, "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/mediastreamproxy.h" ":" "34"), signaling_thread_); 
      }
// PROXY_METHOD1
      bool AddTrack(VideoTrackInterface* a1) override { 
        MethodCall1<C, bool, VideoTrackInterface*> call(c_, &C::AddTrack, std::move(a1)); 
        return call.Marshal(::rtc::Location(__FUNCTION__, "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/mediastreamproxy.h" ":" "35"), signaling_thread_); 
      }
// PROXY_METHOD1
      bool RemoveTrack(AudioTrackInterface* a1) override { 
        MethodCall1<C, bool, AudioTrackInterface*> call(c_, &C::RemoveTrack, std::move(a1)); 
        return call.Marshal(::rtc::Location(__FUNCTION__, "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/mediastreamproxy.h" ":" "36"), signaling_thread_); 
      }
// PROXY_METHOD1
      bool RemoveTrack(VideoTrackInterface* a1) override { 
        MethodCall1<C, bool, VideoTrackInterface*> call(c_, &C::RemoveTrack, std::move(a1)); 
        return call.Marshal(::rtc::Location(__FUNCTION__, "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/mediastreamproxy.h" ":" "37"), signaling_thread_); 
      }
// PROXY_METHOD1
      void RegisterObserver(ObserverInterface* a1) override { 
        MethodCall1<C, void, ObserverInterface*> call(c_, &C::RegisterObserver, std::move(a1)); 
        return call.Marshal(::rtc::Location(__FUNCTION__, "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/mediastreamproxy.h" ":" "38"), signaling_thread_); 
      }
// PROXY_METHOD1
      void UnregisterObserver(ObserverInterface* a1) override { 
        MethodCall1<C, void, ObserverInterface*> call(c_, &C::UnregisterObserver, std::move(a1)); 
        return call.Marshal(::rtc::Location(__FUNCTION__, "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/mediastreamproxy.h" ":" "39"), signaling_thread_); 
      }
// END_PROXY_MAP
  };
}
