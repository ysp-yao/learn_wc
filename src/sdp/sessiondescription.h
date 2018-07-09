/////////////////////////////////////////////////////////////////////////////////////////
// Describes a session content. Individual content types inherit from
// this class.  Analagous to a <jingle><content><description> or
// <session><description>.
class ContentDescription {
 public:
  virtual ~ContentDescription() {}
  virtual ContentDescription* Copy() const = 0;
};
// Analagous to a <jingle><content> or <session><description>.
// name = name of <content name="...">
// type = xmlns of <content>
struct ContentInfo {
  std::string name;
  std::string type;
  bool rejected = false;
  bool bundle_only = false;
  ContentDescription* description = nullptr;
};
////////////////////////////////////////////////////////////////////////////////////////
typedef std::vector<std::string> ContentNames;

// This class provides a mechanism to aggregate different media contents into a
// group. This group can also be shared with the peers in a pre-defined format.
// GroupInfo should be populated only with the |content_name| of the
// MediaDescription.
class ContentGroup {
 private:
  std::string semantics_;
  ContentNames content_names_;
};
////////////////////////////////////////////////////////////////////////////////////////

// ICE RFC 5245 implementation type.
enum IceMode {
  ICEMODE_FULL,  // As defined in http://tools.ietf.org/html/rfc5245#section-4.1
  ICEMODE_LITE   // As defined in http://tools.ietf.org/html/rfc5245#section-4.2
};

// RFC 4145 - http://tools.ietf.org/html/rfc4145#section-4
// 'active':  The endpoint will initiate an outgoing connection.
// 'passive': The endpoint will accept an incoming connection.
// 'actpass': The endpoint is willing to accept an incoming
//            connection or to initiate an outgoing connection.
enum ConnectionRole {
  CONNECTIONROLE_NONE = 0,
  CONNECTIONROLE_ACTIVE,
  CONNECTIONROLE_PASSIVE,
  CONNECTIONROLE_ACTPASS,
  CONNECTIONROLE_HOLDCONN,
};

class SSLCertificate;

struct SSLFingerprint {
  std::string algorithm;
  rtc::CopyOnWriteBuffer digest;
};

struct TransportDescription {
  // These are actually ICE options (appearing in the ice-options attribute in
  // SDP).
  // TODO(deadbeef): Rename to ice_options.
  std::vector<std::string> transport_options;
  std::string ice_ufrag;
  std::string ice_pwd;
  IceMode ice_mode;
  ConnectionRole connection_role;

  std::unique_ptr<rtc::SSLFingerprint> identity_fingerprint;
};

// A TransportInfo is NOT a transport-info message.  It is comparable
// to a "ContentInfo". A transport-infos message is basically just a
// collection of TransportInfos.
struct TransportInfo {
  std::string content_name;
  TransportDescription description;
};
////////////////////////////////////////////////////////////////////////////////////////////


typedef std::vector<ContentInfo> ContentInfos;
typedef std::vector<ContentGroup> ContentGroups;
typedef std::vector<TransportInfo> TransportInfos;


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