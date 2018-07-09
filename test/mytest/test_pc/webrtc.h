#ifndef WEBRTC_H_
#define WEBRTC_H_

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


class Webrtc : public webrtc::PeerConnectionObserver,
               public webrtc::CreateSessionDescriptionObserver {
 public:
  Webrtc();
  ~Webrtc();

  void CreateOffer();

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
      webrtc::PeerConnectionInterface::IceConnectionState new_state) override;

  void OnIceGatheringChange(
      webrtc::PeerConnectionInterface::IceGatheringState new_state) override;

  void OnIceCandidate(
      const webrtc::IceCandidateInterface* candidate) override;

  void OnIceConnectionReceivingChange(bool receiving) override{};

  //
  // CreateSessionDescriptionObserver implementation.
  //
  void OnSuccess(webrtc::SessionDescriptionInterface* desc) override;

  void OnFailure(webrtc::RTCError error) override;

private:
  rtc::Thread *worker_thread_ptr_, *signal_thread_ptr_;

  rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> peer_connection_factory_;
  
  rtc::scoped_refptr<webrtc::MediaStreamInterface> stream_;

  rtc::scoped_refptr<webrtc::PeerConnectionInterface> peer_connection_;
};


#endif // WEBRTC_H_