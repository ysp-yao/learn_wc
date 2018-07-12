#include "webrtc.h"

class DummySetSessionDescriptionObserver
    : public webrtc::SetSessionDescriptionObserver {
 public:
  static DummySetSessionDescriptionObserver* Create() {
    return new rtc::RefCountedObject<DummySetSessionDescriptionObserver>();
  }
  virtual void OnSuccess() { 
    RTC_LOG(INFO) << __FUNCTION__; 
  }
  virtual void OnFailure(webrtc::RTCError error) {
    RTC_LOG(INFO) << __FUNCTION__ << " " << ToString(error.type()) << ": "
                  << error.message();
  }
};


//Webrtc::Webrtc() {
//  worker_thread_ptr_ = new rtc::Thread;
//  signal_thread_ptr_ = new rtc::Thread;
//  worker_thread_ptr_->Start();
//  signal_thread_ptr_->Start();
//
//  peer_connection_factory_ = webrtc::CreatePeerConnectionFactory(
//    nullptr /* network_thread */, worker_thread_ptr_ /* worker_thread */,
//    signal_thread_ptr_ /* signaling_thread */, nullptr /* default_adm */,
//    webrtc::CreateBuiltinAudioEncoderFactory(),
//    webrtc::CreateBuiltinAudioDecoderFactory(),
//    webrtc::CreateBuiltinVideoEncoderFactory(),
//    webrtc::CreateBuiltinVideoDecoderFactory(), nullptr /* audio_mixer */,
//    nullptr /* audio_processing */);
//
//  stream_ = peer_connection_factory_->CreateLocalMediaStream("stream_label");
//
//  rtc::scoped_refptr<webrtc::AudioTrackInterface> audio_track(
//    peer_connection_factory_->CreateAudioTrack(
//        "audio_label",
//        peer_connection_factory_->CreateAudioSource(cricket::AudioOptions())));
//  stream_->AddTrack(audio_track);
//
//}

Webrtc::Webrtc() {
  worker_thread_ptr_ = new rtc::Thread;
  worker_thread_ptr_->SetName("pc_worker_thread", nullptr);
  signal_thread_ptr_ = new rtc::Thread;
  signal_thread_ptr_->SetName("pc_signal_thread", nullptr);
  network_thread_ptr_ = new rtc::Thread;
  network_thread_ptr_->SetName("pc_network_thread", nullptr);

  worker_thread_ptr_->Start();
  signal_thread_ptr_->Start();
  network_thread_ptr_->Start();

    peer_connection_factory_ = webrtc::CreatePeerConnectionFactory(
      network_thread_ptr_ /* network_thread */,
      worker_thread_ptr_ /* worker_thread */,
      signal_thread_ptr_ /* signaling_thread */, 
      nullptr /* default_adm */,
      webrtc::CreateBuiltinAudioEncoderFactory(),
      webrtc::CreateBuiltinAudioDecoderFactory(),
      webrtc::CreateBuiltinVideoEncoderFactory(),
      webrtc::CreateBuiltinVideoDecoderFactory(), 
      nullptr /* audio_mixer */,
      nullptr /* audio_processing */);

}



Webrtc::~Webrtc() {

}

void Webrtc::CreateOffer() {
  webrtc::PeerConnectionInterface::RTCConfiguration config;
  webrtc::PeerConnectionInterface::IceServer server_google;
  server_google.uri = "stun:stun.l.google.com:19302";
  config.servers.push_back(server_google);

  peer_connection_ = peer_connection_factory_->CreatePeerConnection(
      config, nullptr, nullptr, this);

  peer_connection_->AddStream(stream_);

  webrtc::FakeConstraints constraints;
  peer_connection_->CreateOffer(this, &constraints);
}

//
// PeerConnectionObserver implementation.
//
void Webrtc::OnIceConnectionChange(
    webrtc::PeerConnectionInterface::IceConnectionState new_state) {
  int a = 10;
  a++;
}

void Webrtc::OnIceGatheringChange(
    webrtc::PeerConnectionInterface::IceGatheringState new_state) {
  int a = 10;
  a++;
}

void Webrtc::OnIceCandidate(
    const webrtc::IceCandidateInterface* candidate) {
  std::string ice;
  candidate->ToString(&ice);
  ice = std::string("a=") + ice + "\r\n";
}

//
// CreateSessionDescriptionObserver implementation.
//
void Webrtc::OnSuccess(webrtc::SessionDescriptionInterface* desc) {
  std::string sdp;
  desc->ToString(&sdp);

  peer_connection_->SetLocalDescription(DummySetSessionDescriptionObserver::Create(), desc);
}

void Webrtc::OnFailure(webrtc::RTCError error) {

}

void Webrtc::CreateDataOffer() {
  webrtc::PeerConnectionInterface::RTCConfiguration config;
  //webrtc::PeerConnectionInterface::IceServer server_google;
  //server_google.uri = "stun:stun.l.google.com:19302";
  //config.servers.push_back(server_google);

  peer_connection_ = peer_connection_factory_->CreatePeerConnection(
    config, nullptr, nullptr, this);

  webrtc::DataChannelInit dc_config;
  data_channel_ = peer_connection_->CreateDataChannel("DataChannel", &dc_config);
  data_channel_->RegisterObserver(this);

  webrtc::FakeConstraints constraints;
  peer_connection_->CreateOffer(this, &constraints);
}


void Webrtc::OnStateChange() {
  int a = 10;
  a++;
}

void Webrtc::OnMessage(const webrtc::DataBuffer& buffer) {
  int a = 10;
  a++;
}