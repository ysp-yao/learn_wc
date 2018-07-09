

#include "api/mediastreaminterface.h"
#include "api/video/video_frame.h"
#include "media/base/mediachannel.h"
#include "media/base/videocommon.h"
#include "rtc_base/nethelpers.h"
#include "rtc_base/physicalsocketserver.h"
#include "rtc_base/signalthread.h"
#include "rtc_base/sigslot.h"

#include "api/mediastreaminterface.h"
#include "api/peerconnectioninterface.h"

#include "api/audio_codecs/builtin_audio_decoder_factory.h"
#include "api/audio_codecs/builtin_audio_encoder_factory.h"
#include "api/test/fakeconstraints.h"
#include "api/video_codecs/builtin_video_decoder_factory.h"
#include "api/video_codecs/builtin_video_encoder_factory.h"
#include "media/engine/webrtcvideocapturerfactory.h"
#include "modules/audio_device/include/audio_device.h"
#include "modules/audio_processing/include/audio_processing.h"
#include "modules/video_capture/video_capture_factory.h"
#include "rtc_base/checks.h"
#include "rtc_base/json.h"
#include "rtc_base/logging.h"

#include <iostream>

class CallBack : public webrtc::PeerConnectionObserver,
                  public webrtc::CreateSessionDescriptionObserver {
 public:
  //
  // PeerConnectionObserver implementation.
  //

  void OnSignalingChange(
      webrtc::PeerConnectionInterface::SignalingState new_state) override{};
  void OnAddTrack(
      rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver,
      const std::vector<rtc::scoped_refptr<webrtc::MediaStreamInterface>>&
          streams) override{};
  void OnRemoveTrack(
      rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver) override{};
  void OnDataChannel(
      rtc::scoped_refptr<webrtc::DataChannelInterface> channel) override {}
  void OnRenegotiationNeeded() override {}
  void OnIceConnectionChange(
      webrtc::PeerConnectionInterface::IceConnectionState new_state) override{};
  void OnIceGatheringChange(
      webrtc::PeerConnectionInterface::IceGatheringState new_state) override{};
  void OnIceCandidate(
      const webrtc::IceCandidateInterface* candidate) override{};
  void OnIceConnectionReceivingChange(bool receiving) override {}

  // CreateSessionDescriptionObserver implementation.
  void OnSuccess(webrtc::SessionDescriptionInterface* desc) override{
    std::string sdp;
    desc->ToString(&sdp);

    //peer_connection_->SetLocalDescription(DummySetSessionDescriptionObserver::Create(), desc);
  };
  void OnFailure(webrtc::RTCError error) override{};
};

int main() {
  rtc::Thread *worker_thread_ptr_, *signal_thread_ptr_;
  worker_thread_ptr_ = new rtc::Thread;
  signal_thread_ptr_ = new rtc::Thread;
  worker_thread_ptr_->Start();
  signal_thread_ptr_->Start();
  
  
  rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface>
      peer_connection_factory;
  rtc::scoped_refptr<webrtc::PeerConnectionInterface> peer_connection;
  rtc::scoped_refptr<webrtc::MediaStreamInterface> stream;
  rtc::scoped_refptr<CallBack> callback = new rtc::RefCountedObject<CallBack>();

  peer_connection_factory = webrtc::CreatePeerConnectionFactory(
      nullptr /* network_thread */, worker_thread_ptr_ /* worker_thread */,
    signal_thread_ptr_ /* signaling_thread */, nullptr /* default_adm */,
      webrtc::CreateBuiltinAudioEncoderFactory(),
      webrtc::CreateBuiltinAudioDecoderFactory(),
      webrtc::CreateBuiltinVideoEncoderFactory(),
      webrtc::CreateBuiltinVideoDecoderFactory(), nullptr /* audio_mixer */,
      nullptr /* audio_processing */);

  stream = peer_connection_factory->CreateLocalMediaStream("stream_label");

  rtc::scoped_refptr<webrtc::AudioTrackInterface> audio_track(
      peer_connection_factory->CreateAudioTrack(
          "audio_label",
          peer_connection_factory->CreateAudioSource(cricket::AudioOptions())));
  if (!stream->AddTrack(audio_track)) {
    return -1;
  }

  webrtc::PeerConnectionInterface::RTCConfiguration config;
  webrtc::PeerConnectionInterface::IceServer server_google;
  server_google.uri = "stun:stun.l.google.com:19302";
  config.servers.push_back(server_google);
  peer_connection = peer_connection_factory->CreatePeerConnection(
      config, nullptr, nullptr, callback);

  peer_connection->AddStream(stream);

  webrtc::FakeConstraints constraints;
  peer_connection->CreateOffer(callback, &constraints);

  while (1)
    ;

  return 0;
}