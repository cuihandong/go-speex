package speex_test

import (
"testing"
"github.com/cuihandong/go-speex/speex"
	"io/ioutil"
	"bytes"
)

func TestSpeexEnc(t *testing.T) {
	fileBuf, err := ioutil.ReadFile("demo/audio/nideyisishi.wav")
	if err != nil {
		t.Errorf("read audio file failed")
		return
	}

	outBuf := bytes.Buffer{}
	enc := speex.NewSpeexEncoder()
	enc.EnableCachePackage(false)
	for i := 0; i<len(fileBuf); i+=128{
		e := i +128
		if i + 128 > len(fileBuf){
			e = len(fileBuf)
		}
		bLast := false
		if e == len(fileBuf) {
			bLast = true
		}
		enc.ProcessIn(fileBuf[i:e])
		buf := enc.ProcessOut(bLast)
		outBuf.Write(buf)
	}

	ioutil.WriteFile("demo/out/nideyisishi.spx",outBuf.Bytes(),0555)
}