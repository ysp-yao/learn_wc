

#include "webrtc.h"

#include <iostream>

int main() {

  rtc::scoped_refptr<Webrtc> webrtc(new rtc::RefCountedObject<Webrtc>());
  
  //webrtc->CreateOffer();

  webrtc->CreateDataOffer();

  while (1);

  return 0;
}