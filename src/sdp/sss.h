// See: https://www.w3.org/TR/webrtc/#idl-def-rtcofferansweroptions
struct RTCOfferAnswerOptions {
static const int kUndefined = -1;
static const int kMaxOfferToReceiveMedia = 1;

// The default value for constraint offerToReceiveX:true.
static const int kOfferToReceiveMediaTrue = 1;

// These options are left as backwards compatibility for clients who need
// "Plan B" semantics. Clients who have switched to "Unified Plan" semantics
// should use the RtpTransceiver API (AddTransceiver) instead.
//
// offer_to_receive_X set to 1 will cause a media description to be
// generated in the offer, even if no tracks of that type have been added.
// Values greater than 1 are treated the same.
//
// If set to 0, the generated directional attribute will not include the
// "recv" direction (meaning it will be "sendonly" or "inactive".
int offer_to_receive_video = kUndefined;
int offer_to_receive_audio = kUndefined;

bool voice_activity_detection = true;
bool ice_restart = false;

// If true, will offer to BUNDLE audio/video/data together. Not to be
// confused with RTCP mux (multiplexing RTP and RTCP together).
bool use_rtp_mux = true;
};


// Provides a mechanism for describing how m= sections should be generated.
// The m= section with index X will use media_description_options[X]. There
// must be an option for each existing section if creating an answer, or a
// subsequent offer.
struct MediaSessionOptions {
  DataChannelType data_channel_type = DCT_NONE;
  bool vad_enabled = true;  // When disabled, removes all CN codecs from SDP.
  bool rtcp_mux_enabled = true;
  bool bundle_enabled = false;
  bool is_unified_plan = false;
  std::string rtcp_cname = kDefaultRtcpCname;
  rtc::CryptoOptions crypto_options;
  // List of media description options in the same order that the media
  // descriptions will be generated.
  std::vector<MediaDescriptionOptions> media_description_options;
};


struct CreateSessionDescriptionRequest {
  enum Type {
    kOffer,
    kAnswer,
  };

  Type type;
  rtc::scoped_refptr<CreateSessionDescriptionObserver> observer;
  cricket::MediaSessionOptions options;
};

// Describes a collection of contents, each with its own name and
// type.  Analogous to a <jingle> or <session> stanza.  Assumes that
// contents are unique be name, but doesn't enforce that.
class SessionDescription {
 private:
  ContentInfos contents_;
  TransportInfos transport_infos_;
  ContentGroups content_groups_;
  bool msid_supported_ = true;
  // Default to what Plan B would do.
  // TODO(bugs.webrtc.org/8530): Change default to kMsidSignalingMediaSection.
  int msid_signaling_ = kMsidSignalingSsrcAttribute;
};

// Implementation of SessionDescriptionInterface.
class JsepSessionDescription : public SessionDescriptionInterface {
 public:
  static const int kDefaultVideoCodecId;
  static const char kDefaultVideoCodecName[];

 private:
  std::unique_ptr<cricket::SessionDescription> description_;
  std::string session_id_;
  std::string session_version_;
  SdpType type_;
  std::vector<JsepCandidateCollection> candidate_collection_;

  RTC_DISALLOW_COPY_AND_ASSIGN(JsepSessionDescription);
};