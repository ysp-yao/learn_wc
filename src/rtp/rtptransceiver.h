//
// cflags = -Qunused-arguments /E
//

template <class INTERNAL_CLASS> 
class RtpTransceiverProxyWithInternal; 

typedef RtpTransceiverProxyWithInternal<RtpTransceiverInterface> RtpTransceiverProxy; 

template <class INTERNAL_CLASS> 
class RtpTransceiverProxyWithInternal : public RtpTransceiverInterface { 
protected: 
  typedef RtpTransceiverInterface C; 
public: 
  const INTERNAL_CLASS* internal() const { return c_; } 
  INTERNAL_CLASS* internal() { return c_; } 
protected: 
  RtpTransceiverProxyWithInternal(rtc::Thread* signaling_thread, INTERNAL_CLASS* RtpTransceiver) : signaling_thread_(signaling_thread), c_(RtpTransceiver) {} 
private: 
  mutable rtc::Thread* signaling_thread_; 
protected: 
  ~RtpTransceiverProxyWithInternal() { 
    MethodCall0<RtpTransceiverProxyWithInternal, void> call( this, &RtpTransceiverProxyWithInternal::DestroyInternal); 
    call.Marshal(::rtc::Location(__FUNCTION__, "g:\\wc\\src\\pc\\rtptransceiver.h" ":" "195"), destructor_thread()); 
  } 
private: 
  void DestroyInternal() { c_ = nullptr; } 
  rtc::scoped_refptr<INTERNAL_CLASS> c_; 
public: 
  static rtc::scoped_refptr<RtpTransceiverProxyWithInternal> Create( rtc::Thread* signaling_thread, INTERNAL_CLASS* RtpTransceiver) { 
    return new rtc::RefCountedObject<RtpTransceiverProxyWithInternal>(signaling_thread, RtpTransceiver); 
  }
private: 
  rtc::Thread* destructor_thread() const { return signaling_thread_; } 
public:
  cricket::MediaType media_type() const override { 
    ConstMethodCall0<C, cricket::MediaType> call(c_, &C::media_type); 
    return call.Marshal(::rtc::Location(__FUNCTION__, "g:\\wc\\src\\pc\\rtptransceiver.h" ":" "197"), signaling_thread_); 
  };
  absl::optional<std::string> mid() const override { 
    ConstMethodCall0<C, absl::optional<std::string> > call(c_, &C::mid); 
    return call.Marshal(::rtc::Location(__FUNCTION__, "g:\\wc\\src\\pc\\rtptransceiver.h" ":" "198"), signaling_thread_); 
  };
  rtc::scoped_refptr<RtpSenderInterface> sender() const override { 
    ConstMethodCall0<C, rtc::scoped_refptr<RtpSenderInterface> > call(c_, &C::sender); 
    return call.Marshal(::rtc::Location(__FUNCTION__, "g:\\wc\\src\\pc\\rtptransceiver.h" ":" "199"), signaling_thread_); 
  };
  rtc::scoped_refptr<RtpReceiverInterface> receiver() const override { 
    ConstMethodCall0<C, rtc::scoped_refptr<RtpReceiverInterface> > call(c_, &C::receiver); 
    return call.Marshal(::rtc::Location(__FUNCTION__, "g:\\wc\\src\\pc\\rtptransceiver.h" ":" "200"), signaling_thread_); 
  };
  bool stopped() const override { 
    ConstMethodCall0<C, bool> call(c_, &C::stopped); 
    return call.Marshal(::rtc::Location(__FUNCTION__, "g:\\wc\\src\\pc\\rtptransceiver.h" ":" "201"), signaling_thread_); 
  };
  RtpTransceiverDirection direction() const override { 
    ConstMethodCall0<C, RtpTransceiverDirection> call(c_, &C::direction); 
    return call.Marshal(::rtc::Location(__FUNCTION__, "g:\\wc\\src\\pc\\rtptransceiver.h" ":" "202"), signaling_thread_); 
  };
  void SetDirection(RtpTransceiverDirection a1) override { 
    MethodCall1<C, void, RtpTransceiverDirection> call(c_, &C::SetDirection, std::move(a1)); 
    return call.Marshal(::rtc::Location(__FUNCTION__, "g:\\wc\\src\\pc\\rtptransceiver.h" ":" "203"), signaling_thread_); 
  };
  absl::optional<RtpTransceiverDirection> current_direction() const override { 
    ConstMethodCall0<C, absl::optional<RtpTransceiverDirection> > call(c_, &C::current_direction); 
    return call.Marshal(::rtc::Location(__FUNCTION__, "g:\\wc\\src\\pc\\rtptransceiver.h" ":" "204"), signaling_thread_); 
  };
  void Stop() override { 
    MethodCall0<C, void> call(c_, &C::Stop); 
    return call.Marshal(::rtc::Location(__FUNCTION__, "g:\\wc\\src\\pc\\rtptransceiver.h" ":" "205"), signaling_thread_); 
  };
  void SetCodecPreferences(rtc::ArrayView<RtpCodecCapability> a1) override { 
    MethodCall1<C, void, rtc::ArrayView<RtpCodecCapability> > call(c_, &C::SetCodecPreferences, std::move(a1)); 
    return call.Marshal(::rtc::Location(__FUNCTION__, "g:\\wc\\src\\pc\\rtptransceiver.h" ":" "206"), signaling_thread_); 
  };
};;