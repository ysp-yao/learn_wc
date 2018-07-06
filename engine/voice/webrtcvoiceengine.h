// WebRtcVoiceEngine is a class to be used with CompositeMediaEngine.
// It uses the WebRtc VoiceEngine library for audio handling.
class WebRtcVoiceEngine final : public webrtc::TraceCallback  {
  friend class WebRtcVoiceMediaChannel;

 private:

  std::unique_ptr<rtc::TaskQueue> low_priority_worker_queue_;


  rtc::ThreadChecker signal_thread_checker_;
  rtc::ThreadChecker worker_thread_checker_;

  // The audio device manager.
  rtc::scoped_refptr<webrtc::AudioDeviceModule> adm_;
  rtc::scoped_refptr<webrtc::AudioEncoderFactory> encoder_factory_;
  rtc::scoped_refptr<webrtc::AudioDecoderFactory> decoder_factory_;
  rtc::scoped_refptr<webrtc::AudioMixer> audio_mixer_;
  // Reference to the APM, owned by VoE.
  rtc::scoped_refptr<webrtc::AudioProcessing> apm_;
  // Reference to the TransmitMixer, owned by VoE.
  webrtc::voe::TransmitMixer* transmit_mixer_ = nullptr;
  // The primary instance of WebRtc VoiceEngine.
  std::unique_ptr<VoEWrapper> voe_wrapper_;
  rtc::scoped_refptr<webrtc::AudioState> audio_state_;
  std::vector<AudioCodec> send_codecs_;
  std::vector<AudioCodec> recv_codecs_;
  std::vector<WebRtcVoiceMediaChannel*> channels_;
  webrtc::VoEBase::ChannelConfig channel_config_;
  bool is_dumping_aec_ = false;
  bool initialized_ = false;

  webrtc::AgcConfig default_agc_config_;
  // Cache received extended_filter_aec, delay_agnostic_aec, experimental_ns
  // level controller, and intelligibility_enhancer values, and apply them
  // in case they are missing in the audio options. We need to do this because
  // SetExtraOptions() will revert to defaults for options which are not
  // provided.
  rtc::Optional<bool> extended_filter_aec_;
  rtc::Optional<bool> delay_agnostic_aec_;
  rtc::Optional<bool> experimental_ns_;
  rtc::Optional<bool> intelligibility_enhancer_;
  rtc::Optional<bool> level_control_;

  webrtc::AudioProcessing::Config apm_config_;
};