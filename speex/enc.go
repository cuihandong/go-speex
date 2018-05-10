package speex

/*
#cgo CFLAGS: -I${SRCDIR}/../speex-lib/include
#include <stdlib.h>
#include "speex/speex.h"
#include "enc_wrapper.h"
*/
import "C"

import (
	"unsafe"
)

type SpeexEncoder struct {
	m C.speex_encoder
}

func NewSpeexEncoder() *SpeexEncoder {
	encoder := &SpeexEncoder{}
	encoder.m = C.speex_enc_init()
	return encoder
}

//#cgo linux LDFLAGS: -static-libstdc++ -static-libgcc ${SRCDIR}/../speex-lib/lib/linux/libspeex.a -lm
func (x *SpeexEncoder) EnableCachePackage(enable bool){
	ienable := C.int(0);
	if enable {
		ienable = C.int(1);
	}
	
	C.speex_enc_enable_cache_package(x.m, ienable)
}

func (x *SpeexEncoder) ProcessIn(buf []byte){
	cBuf := (*C.uchar)(unsafe.Pointer(&buf[0]))
	cLen := C.uint(len(buf))
	C.speex_enc_process_in(x.m, cBuf, cLen)
}

func (x *SpeexEncoder) ProcessOut(end bool) []byte {
	iend := C.int(0);
	if end {
		iend = C.int(1);
	}
	var charPtr *C.uchar = nil
	cLen := C.speex_enc_process_out(x.m, iend, &charPtr)
	if cLen > 0 {
		buf := C.GoBytes(unsafe.Pointer(charPtr),cLen)
		C.free(unsafe.Pointer(charPtr))
		return buf
	}

	return nil
}

func (x *SpeexEncoder) Destroy(){
	C.speex_enc_free(x.m)
}
