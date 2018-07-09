// A WebRtcSession manages general session state. This includes negotiation
// of both the application-level and network-level protocols:  the former
// defines what will be sent and the latter defines how it will be sent.  Each
// network-level protocol is represented by a Transport object.  Each Transport
// participates in the network-level negotiation.  The individual streams of
// packets are represented by TransportChannels.  The application-level protocol
// is represented by SessionDecription objects.
class WebRtcSession :
    public DataChannelProviderInterface,
    public sigslot::has_slots<> {
 public:
  enum State {
    STATE_INIT = 0,
    STATE_SENTOFFER,         // Sent offer, waiting for answer.
    STATE_RECEIVEDOFFER,     // Received an offer. Need to send answer.
    STATE_SENTPRANSWER,      // Sent provisional answer. Need to send answer.
    STATE_RECEIVEDPRANSWER,  // Received provisional answer, waiting for answer.
    STATE_INPROGRESS,        // Offer/answer exchange completed.
    STATE_CLOSED,            // Close() was called.
  };

  enum Error {
    ERROR_NONE = 0,       // no error
    ERROR_CONTENT = 1,    // channel errors in SetLocalContent/SetRemoteContent
    ERROR_TRANSPORT = 2,  // transport error of some kind
  };


  // Called when voice_channel_, video_channel_ and
  // rtp_data_channel_/sctp_transport_ are created and destroyed. As a result
  // of, for example, setting a new description.
  sigslot::signal0<> SignalVoiceChannelCreated;
  sigslot::signal0<> SignalVoiceChannelDestroyed;
  sigslot::signal0<> SignalVideoChannelCreated;
  sigslot::signal0<> SignalVideoChannelDestroyed;
  sigslot::signal0<> SignalDataChannelCreated;
  sigslot::signal0<> SignalDataChannelDestroyed;

  // Called when a valid data channel OPEN message is received.
  // std::string represents the data channel label.
  sigslot::signal2<const std::string&, const InternalDataChannelInit&> SignalDataChannelOpenMessage;

 private:
  // Indicates the type of SessionDescription in a call to SetLocalDescription
  // and SetRemoteDescription.
  enum Action {
    kOffer,
    kPrAnswer,
    kAnswer,
  };
  
  rtc::Thread* const network_thread_;
  rtc::Thread* const worker_thread_;
  rtc::Thread* const signaling_thread_;

  State state_ = STATE_INIT;
  Error error_ = ERROR_NONE;
  std::string error_desc_;

  const std::string sid_;
  bool initial_offerer_ = false;

  const std::unique_ptr<cricket::TransportController> transport_controller_;
  const std::unique_ptr<cricket::SctpTransportInternalFactory> sctp_factory_;
  const cricket::MediaConfig media_config_;
  RtcEventLog* event_log_;
  Call* call_;
  std::unique_ptr<cricket::VoiceChannel> voice_channel_;
  std::unique_ptr<cricket::VideoChannel> video_channel_;
  // |rtp_data_channel_| is used if in RTP data channel mode, |sctp_transport_|
  // when using SCTP.
  std::unique_ptr<cricket::RtpDataChannel> rtp_data_channel_;

  std::unique_ptr<cricket::SctpTransportInternal> sctp_transport_;
  // |sctp_transport_name_| keeps track of what DTLS transport the SCTP
  // transport is using (which can change due to bundling).
  rtc::Optional<std::string> sctp_transport_name_;
  // |sctp_content_name_| is the content name (MID) in SDP.
  rtc::Optional<std::string> sctp_content_name_;
  // Value cached on signaling thread. Only updated when SctpReadyToSendData
  // fires on the signaling thread.
  bool sctp_ready_to_send_data_ = false;
  // Same as signals provided by SctpTransport, but these are guaranteed to
  // fire on the signaling thread, whereas SctpTransport fires on the networking
  // thread.
  // |sctp_invoker_| is used so that any signals queued on the signaling thread
  // from the network thread are immediately discarded if the SctpTransport is
  // destroyed (due to m= section being rejected).
  // TODO(deadbeef): Use a proxy object to ensure that method calls/signals
  // are marshalled to the right thread. Could almost use proxy.h for this,
  // but it doesn't have a mechanism for marshalling sigslot::signals
  std::unique_ptr<rtc::AsyncInvoker> sctp_invoker_;
  sigslot::signal1<bool> SignalSctpReadyToSendData;
  sigslot::signal2<const cricket::ReceiveDataParams&,
                   const rtc::CopyOnWriteBuffer&>
      SignalSctpDataReceived;
  sigslot::signal1<int> SignalSctpStreamClosedRemotely;

  cricket::ChannelManager* channel_manager_;
  IceObserver* ice_observer_;
  PeerConnectionInterface::IceConnectionState ice_connection_state_;
  bool ice_connection_receiving_;
  std::unique_ptr<SessionDescriptionInterface> current_local_description_;
  std::unique_ptr<SessionDescriptionInterface> pending_local_description_;
  std::unique_ptr<SessionDescriptionInterface> current_remote_description_;
  std::unique_ptr<SessionDescriptionInterface> pending_remote_description_;
  // If the remote peer is using a older version of implementation.
  bool older_version_remote_peer_;
  bool dtls_enabled_;
  // Specifies which kind of data channel is allowed. This is controlled
  // by the chrome command-line flag and constraints:
  // 1. If chrome command-line switch 'enable-sctp-data-channels' is enabled,
  // constraint kEnableDtlsSrtp is true, and constaint kEnableRtpDataChannels is
  // not set or false, SCTP is allowed (DCT_SCTP);
  // 2. If constraint kEnableRtpDataChannels is true, RTP is allowed (DCT_RTP);
  // 3. If both 1&2 are false, data channel is not allowed (DCT_NONE).
  // The data channel type could be DCT_QUIC if the QUIC data channel is
  // enabled.
  cricket::DataChannelType data_channel_type_;
  // List of content names for which the remote side triggered an ICE restart.
  std::set<std::string> pending_ice_restarts_;

  std::unique_ptr<WebRtcSessionDescriptionFactory> webrtc_session_desc_factory_;

  // Member variables for caching global options.
  cricket::AudioOptions audio_options_;
  cricket::VideoOptions video_options_;
  MetricsObserverInterface* metrics_observer_;

  // Declares the bundle policy for the WebRTCSession.
  PeerConnectionInterface::BundlePolicy bundle_policy_;

  // Declares the RTCP mux policy for the WebRTCSession.
  PeerConnectionInterface::RtcpMuxPolicy rtcp_mux_policy_;

  bool received_first_video_packet_ = false;
  bool received_first_audio_packet_ = false;

#ifdef HAVE_QUIC
  std::unique_ptr<QuicDataTransport> quic_data_transport_;
#endif  // HAVE_QUIC

  RTC_DISALLOW_COPY_AND_ASSIGN(WebRtcSession);
};