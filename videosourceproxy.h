namespace webrtc {

// BEGIN_PROXY_MAP
template <class INTERNAL_CLASS> 
class VideoTrackSourceProxyWithInternal; 

typedef VideoTrackSourceProxyWithInternal<VideoTrackSourceInterface> VideoTrackSourceProxy; 

template <class INTERNAL_CLASS> 
class VideoTrackSourceProxyWithInternal : public VideoTrackSourceInterface { 
  protected: 
    typedef VideoTrackSourceInterface C; 
  public: 
    const INTERNAL_CLASS* internal() const { return c_; } 
    INTERNAL_CLASS* internal() { return c_; } 
  protected: 
    VideoTrackSourceProxyWithInternal(rtc::Thread* signaling_thread, rtc::Thread* worker_thread, INTERNAL_CLASS* VideoTrackSource) : signaling_thread_(signaling_thread), worker_thread_(worker_thread), c_(VideoTrackSource) {} 
  private: 
    mutable rtc::Thread* signaling_thread_; 
    mutable rtc::Thread* worker_thread_; 
  protected: 
    ~VideoTrackSourceProxyWithInternal() { 
      MethodCall0<VideoTrackSourceProxyWithInternal, void> call( this, &VideoTrackSourceProxyWithInternal::DestroyInternal); 
      call.Marshal(::rtc::Location( __FUNCTION__  , "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/videosourceproxy.h" ":" "23"), destructor_thread()); 
    } 
  private: 
    void DestroyInternal() { c_ = nullptr; } 
    rtc::scoped_refptr<INTERNAL_CLASS> c_; 
  public: 
    static rtc::scoped_refptr<VideoTrackSourceProxyWithInternal> 
    Create( rtc::Thread* signaling_thread, rtc::Thread* worker_thread, INTERNAL_CLASS* VideoTrackSource) { 
      return new rtc::RefCountedObject<VideoTrackSourceProxyWithInternal>(signaling_thread, worker_thread, VideoTrackSource); 
    }
// PROXY_SIGNALING_THREAD_DESTRUCTOR
  private: 
    rtc::Thread* destructor_thread() const { return signaling_thread_; } 
  public:
// PROXY_CONSTMETHOD0
    SourceState state() const override { 
      ConstMethodCall0<C, SourceState> call(c_, &C::state); 
      return call.Marshal(::rtc::Location( __FUNCTION__  , "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/videosourceproxy.h" ":" "25"), signaling_thread_); 
    }
// PROXY_CONSTMETHOD0
    bool remote() const override { 
      ConstMethodCall0<C, bool> call(c_, &C::remote); 
      return call.Marshal(::rtc::Location( __FUNCTION__  , "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/videosourceproxy.h" ":" "26"), signaling_thread_); 
    }
// PROXY_CONSTMETHOD0
    bool is_screencast() const override { 
      ConstMethodCall0<C, bool> call(c_, &C::is_screencast); 
      return call.Marshal(::rtc::Location( __FUNCTION__  , "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/videosourceproxy.h" ":" "27"), signaling_thread_); 
    }
// PROXY_CONSTMETHOD0
    rtc::Optional<bool> needs_denoising() const override { 
      ConstMethodCall0<C, rtc::Optional<bool>> call(c_, &C::needs_denoising); 
      return call.Marshal(::rtc::Location( __FUNCTION__  , "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/videosourceproxy.h" ":" "28"), signaling_thread_); 
    }
// PROXY_METHOD1
    bool GetStats(Stats* a1) override { 
      MethodCall1<C, bool, Stats*> call(c_, &C::GetStats, std::move(a1)); 
      return call.Marshal(::rtc::Location( __FUNCTION__  , "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/videosourceproxy.h" ":" "29"), signaling_thread_); 
    }
// PROXY_WORKER_METHOD2
    void AddOrUpdateSink(rtc::VideoSinkInterface<VideoFrame>* a1, const rtc::VideoSinkWants& a2) override { 
      MethodCall2<C, void, rtc::VideoSinkInterface<VideoFrame>*, const rtc::VideoSinkWants&> call(c_, &C::AddOrUpdateSink, std::move(a1), std::move(a2)); 
      return call.Marshal(::rtc::Location( __FUNCTION__  , "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/videosourceproxy.h" ":" "33"), worker_thread_); 
    }
// PROXY_WORKER_METHOD1
    void RemoveSink(rtc::VideoSinkInterface<VideoFrame>* a1) override { 
      MethodCall1<C, void, rtc::VideoSinkInterface<VideoFrame>*> call(c_, &C::RemoveSink, std::move(a1)); 
      return call.Marshal(::rtc::Location( __FUNCTION__  , "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/videosourceproxy.h" ":" "34"), worker_thread_); 
    }
// PROXY_METHOD1
    void RegisterObserver(ObserverInterface* a1) override { 
      MethodCall1<C, void, ObserverInterface*> call(c_, &C::RegisterObserver, std::move(a1)); 
      return call.Marshal(::rtc::Location( __FUNCTION__  , "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/videosourceproxy.h" ":" "35"), signaling_thread_); 
    }
// PROXY_METHOD1
    void UnregisterObserver(ObserverInterface* a1) override { 
      MethodCall1<C, void, ObserverInterface*> call(c_, &C::UnregisterObserver, std::move(a1)); 
      return call.Marshal(::rtc::Location( __FUNCTION__  , "G:\\vrv_workspace\\rtcsdk_experiment\\webrtc_m61\\src\\webrtc/api/videosourceproxy.h" ":" "36"), signaling_thread_); 
    }
// END_PROXY_MAP
  };
}