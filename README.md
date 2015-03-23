# LMS imaging

Support for the following image formats:
- [GREY](http://linuxtv.org/downloads/v4l-dvb-apis/V4L2-PIX-FMT-GREY.html)
- [YUYV](http://linuxtv.org/downloads/v4l-dvb-apis/V4L2-PIX-FMT-YUYV.html)
- BGRA

Support for the following image files:
- [PGM (Protable Pixel Map)](http://en.wikipedia.org/wiki/Netpbm_format) for GREY images (using P5)

Converting image formats:
- YUYV -> GREY
- YUYV -> BGRA
- GREY -> BGRA

TODO:
- Bitmaps (*.bmp)

## Dependencies
- [cereal](https://github.com/syxolk/cereal)
