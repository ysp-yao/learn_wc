
// WebRtcVoiceEngine is a class to be used with CompositeMediaEngine.
// It uses the WebRtc VoiceEngine library for audio handling.
class WebRtcVoiceEngine final {
  friend class WebRtcVoiceMediaChannel;

 private:

  std::unique_ptr<rtc::TaskQueue> low_priority_worker_queue_;
  
  rtc::ThreadChecker signal_thread_checker_;
  rtc::ThreadChecker worker_thread_checker_;

  // The audio device module.
  rtc::scoped_refptr<webrtc::AudioDeviceModule> adm_;
  rtc::scoped_refptr<webrtc::AudioEncoderFactory> encoder_factory_;
  rtc::scoped_refptr<webrtc::AudioDecoderFactory> decoder_factory_;
  rtc::scoped_refptr<webrtc::AudioMixer> audio_mixer_;
  // The audio processing module.
  rtc::scoped_refptr<webrtc::AudioProcessing> apm_;
  // The primary instance of WebRtc VoiceEngine.
  rtc::scoped_refptr<webrtc::AudioState> audio_state_;
  std::vector<AudioCodec> send_codecs_;
  std::vector<AudioCodec> recv_codecs_;
  std::vector<WebRtcVoiceMediaChannel*> channels_;
  bool is_dumping_aec_ = false;
  bool initialized_ = false;

  webrtc::AgcConfig default_agc_config_;
  // Cache received extended_filter_aec, delay_agnostic_aec, experimental_ns
  // and intelligibility_enhancer values, and apply them
  // in case they are missing in the audio options. We need to do this because
  // SetExtraOptions() will revert to defaults for options which are not
  // provided.
  absl::optional<bool> extended_filter_aec_;
  absl::optional<bool> delay_agnostic_aec_;
  absl::optional<bool> experimental_ns_;
  absl::optional<bool> intelligibility_enhancer_;
  // Jitter buffer settings for new streams.
  size_t audio_jitter_buffer_max_packets_ = 50;
  bool audio_jitter_buffer_fast_accelerate_ = false;

  RTC_DISALLOW_IMPLICIT_CONSTRUCTORS(WebRtcVoiceEngine);
};