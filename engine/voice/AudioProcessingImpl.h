class AudioProcessingImpl : public AudioProcessing {
 private:
  class RuntimeSettingEnqueuer {
   private:
    SwapQueue<RuntimeSetting>& runtime_settings_;
  };
  struct ApmPublicSubmodules;
  struct ApmPrivateSubmodules;

  std::unique_ptr<ApmDataDumper> data_dumper_;
  static int instance_count_;

  SwapQueue<RuntimeSetting> capture_runtime_settings_;
  SwapQueue<RuntimeSetting> render_runtime_settings_;

  RuntimeSettingEnqueuer capture_runtime_settings_enqueuer_;
  RuntimeSettingEnqueuer render_runtime_settings_enqueuer_;

  // Submodule interface implementations.
  std::unique_ptr<HighPassFilter> high_pass_filter_impl_;

  // EchoControl factory.
  std::unique_ptr<EchoControlFactory> echo_control_factory_;

  class ApmSubmoduleStates {
   private:
    const bool capture_post_processor_enabled_ = false;
    const bool render_pre_processor_enabled_ = false;
    bool low_cut_filter_enabled_ = false;
    bool echo_canceller_enabled_ = false;
    bool mobile_echo_controller_enabled_ = false;
    bool residual_echo_detector_enabled_ = false;
    bool noise_suppressor_enabled_ = false;
    bool intelligibility_enhancer_enabled_ = false;
    bool adaptive_gain_controller_enabled_ = false;
    bool gain_controller2_enabled_ = false;
    bool pre_amplifier_enabled_ = false;
    bool echo_controller_enabled_ = false;
    bool level_estimator_enabled_ = false;
    bool voice_activity_detector_enabled_ = false;
    bool transient_suppressor_enabled_ = false;
    bool first_update_ = true;
  };

  // AecDump instance used for optionally logging APM config, input
  // and output to file in the AEC-dump format defined in debug.proto.
  std::unique_ptr<AecDump> aec_dump_;

  // Hold the last config written with AecDump for avoiding writing
  // the same config twice.
  InternalAPMConfig apm_config_for_aec_dump_ RTC_GUARDED_BY(crit_capture_);

  // Critical sections.
  rtc::CriticalSection crit_render_ RTC_ACQUIRED_BEFORE(crit_capture_);
  rtc::CriticalSection crit_capture_;

  // Struct containing the Config specifying the behavior of APM.
  AudioProcessing::Config config_;

  // Class containing information about what submodules are active.
  ApmSubmoduleStates submodule_states_;

  // Structs containing the pointers to the submodules.
  std::unique_ptr<ApmPublicSubmodules> public_submodules_;
  std::unique_ptr<ApmPrivateSubmodules> private_submodules_;

  // State that is written to while holding both the render and capture locks
  // but can be read without any lock being held.
  // As this is only accessed internally of APM, and all internal methods in APM
  // either are holding the render or capture locks, this construct is safe as
  // it is not possible to read the variables while writing them.
  struct ApmFormatState {
    ApmFormatState()
        :  // Format of processing streams at input/output call sites.
          api_format({{{kSampleRate16kHz, 1, false},
                       {kSampleRate16kHz, 1, false},
                       {kSampleRate16kHz, 1, false},
                       {kSampleRate16kHz, 1, false}}}),
          render_processing_format(kSampleRate16kHz, 1) {}
    ProcessingConfig api_format;
    StreamConfig render_processing_format;
  } formats_;

  // APM constants.
  const struct ApmConstants {
    ApmConstants(int agc_startup_min_volume,
                 int agc_clipped_level_min,
                 bool use_experimental_agc,
                 bool use_experimental_agc_agc2_level_estimation,
                 bool use_experimental_agc_agc2_digital_adaptive)
        :  // Format of processing streams at input/output call sites.
          agc_startup_min_volume(agc_startup_min_volume),
          agc_clipped_level_min(agc_clipped_level_min),
          use_experimental_agc(use_experimental_agc),
          use_experimental_agc_agc2_level_estimation(
              use_experimental_agc_agc2_level_estimation),
          use_experimental_agc_agc2_digital_adaptive(
              use_experimental_agc_agc2_digital_adaptive) {}
    int agc_startup_min_volume;
    int agc_clipped_level_min;
    bool use_experimental_agc;
    bool use_experimental_agc_agc2_level_estimation;
    bool use_experimental_agc_agc2_digital_adaptive;

  } constants_;

  struct ApmCaptureState {
    ApmCaptureState(bool transient_suppressor_enabled);
    ~ApmCaptureState();
    int aec_system_delay_jumps;
    int delay_offset_ms;
    bool was_stream_delay_set;
    int last_stream_delay_ms;
    int last_aec_system_delay_ms;
    int stream_delay_jumps;
    bool output_will_be_muted;
    bool key_pressed;
    bool transient_suppressor_enabled;
    std::unique_ptr<AudioBuffer> capture_audio;
    // Only the rate and samples fields of capture_processing_format_ are used
    // because the capture processing number of channels is mutable and is
    // tracked by the capture_audio_.
    StreamConfig capture_processing_format;
    int split_rate;
    bool echo_path_gain_change;
  } capture_ RTC_GUARDED_BY(crit_capture_);

  struct ApmCaptureNonLockedState {
    ApmCaptureNonLockedState(bool intelligibility_enabled)
        : capture_processing_format(kSampleRate16kHz),
          split_rate(kSampleRate16kHz),
          stream_delay_ms(0),
          intelligibility_enabled(intelligibility_enabled) {}
    // Only the rate and samples fields of capture_processing_format_ are used
    // because the forward processing number of channels is mutable and is
    // tracked by the capture_audio_.
    StreamConfig capture_processing_format;
    int split_rate;
    int stream_delay_ms;
    bool intelligibility_enabled;
    bool echo_controller_enabled = false;
  } capture_nonlocked_;

  struct ApmRenderState {
    ApmRenderState();
    ~ApmRenderState();
    std::unique_ptr<AudioConverter> render_converter;
    std::unique_ptr<AudioBuffer> render_audio;
  } render_ RTC_GUARDED_BY(crit_render_);

  size_t aec_render_queue_element_max_size_ RTC_GUARDED_BY(crit_render_)
      RTC_GUARDED_BY(crit_capture_) = 0;
  std::vector<float> aec_render_queue_buffer_ RTC_GUARDED_BY(crit_render_);
  std::vector<float> aec_capture_queue_buffer_ RTC_GUARDED_BY(crit_capture_);

  size_t aecm_render_queue_element_max_size_ RTC_GUARDED_BY(crit_render_)
      RTC_GUARDED_BY(crit_capture_) = 0;
  std::vector<int16_t> aecm_render_queue_buffer_ RTC_GUARDED_BY(crit_render_);
  std::vector<int16_t> aecm_capture_queue_buffer_ RTC_GUARDED_BY(crit_capture_);

  size_t agc_render_queue_element_max_size_ RTC_GUARDED_BY(crit_render_)
      RTC_GUARDED_BY(crit_capture_) = 0;
  std::vector<int16_t> agc_render_queue_buffer_ RTC_GUARDED_BY(crit_render_);
  std::vector<int16_t> agc_capture_queue_buffer_ RTC_GUARDED_BY(crit_capture_);

  size_t red_render_queue_element_max_size_ RTC_GUARDED_BY(crit_render_)
      RTC_GUARDED_BY(crit_capture_) = 0;
  std::vector<float> red_render_queue_buffer_ RTC_GUARDED_BY(crit_render_);
  std::vector<float> red_capture_queue_buffer_ RTC_GUARDED_BY(crit_capture_);

  RmsLevel capture_input_rms_ RTC_GUARDED_BY(crit_capture_);
  RmsLevel capture_output_rms_ RTC_GUARDED_BY(crit_capture_);
  int capture_rms_interval_counter_ RTC_GUARDED_BY(crit_capture_) = 0;

  // Lock protection not needed.
  std::unique_ptr<SwapQueue<std::vector<float>, RenderQueueItemVerifier<float>>>
      aec_render_signal_queue_;
  std::unique_ptr<
      SwapQueue<std::vector<int16_t>, RenderQueueItemVerifier<int16_t>>>
      aecm_render_signal_queue_;
  std::unique_ptr<
      SwapQueue<std::vector<int16_t>, RenderQueueItemVerifier<int16_t>>>
      agc_render_signal_queue_;
  std::unique_ptr<SwapQueue<std::vector<float>, RenderQueueItemVerifier<float>>>
      red_render_signal_queue_;
};