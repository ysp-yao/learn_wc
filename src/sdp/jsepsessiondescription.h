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