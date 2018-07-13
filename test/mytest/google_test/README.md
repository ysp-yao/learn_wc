### 前言
* 借助webrtc中提供的单元测试来学习webrtc源码

### 结构，以p2p相关的单元测试为例
```
|-src
  |-BUILD.gn // rtc_test("rtc_unittests")
  |-p2p
    |-BUILD.gn // rtc_source_set("rtc_p2p_unittests")
```