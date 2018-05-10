package speex

/*
#cgo CFLAGS: -I${SRCDIR}/../speex-lib/include
#cgo linux LDFLAGS: -static-libstdc++ -static-libgcc ${SRCDIR}/../speex-lib/lib/linux/libspeex.a -lm
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
	return &SpeexEncoder{}
}

func (x *SpeexEncoder) EnableCachePackage(enable bool){
	C.speex_enc_enable_cache_package(x.m, enable)
}

func (x *SpeexEncoder) ProcessIn(buf []byte){
	cBuf := (*C.char)(unsafe.Pointer(&buf[0]))
	cLen := C.int(len(buf))
	C.speex_enc_process_in(x.m, cBuf, cLen)
}

func (x *SpeexEncoder) ProcessOut(end bool) []byte {
	var charPtr *C.char = nil
	cLen := C.speex_enc_process_out(x.m, end, &charPtr)
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