

#include "webrtc.h"

#include <iostream>

int main() {

  rtc::scoped_refptr<Webrtc> webrtc(new rtc::RefCountedObject<Webrtc>());
  
  webrtc->CreateOffer();

  while (1);

  return 0;
}