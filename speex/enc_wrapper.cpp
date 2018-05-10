#include "enc_wrapper.h"
#include "enc.h"
#include <cstring>

speex_encoder speex_enc_init()
{
    SpeexEncoder * enc = new SpeexEncoder();
    return (void*)enc;
}

void speex_enc_process_in(speex_encoder enc, const unsigned char * data, unsigned int len)
{
    SpeexEncoder* encoder = (SpeexEncoder*)enc;
    //assert(len%2==0);
    encoder->ProcessIn((const unsigned short *)data, len/2);
}

int speex_enc_process_out(speex_encoder enc, int end, unsigned char ** data)
{
    SpeexEncoder* encoder = (SpeexEncoder*)enc;
    std::vector<char> res = encoder->ProcessOut(end!=0);
    *data = new unsigned char[res.size()];
    memcpy(*data,res.data(),res.size());
    return res.size();
}

void speex_enc_enable_cache_package(speex_encoder enc, int enable)
{
    SpeexEncoder* encoder = (SpeexEncoder*)enc;
    encoder->EnableCachePackage(enable!=0);
}

void speex_enc_free(speex_encoder enc)
{
    SpeexEncoder* encoder = (SpeexEncoder*)enc;
    delete encoder;
}
