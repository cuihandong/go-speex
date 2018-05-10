#pragma once
#ifdef __cplusplus
extern "C" {
#endif
typedef void* speex_encoder;
speex_encoder speex_enc_init();
void speex_enc_process_in(speex_encoder encoder, const unsigned char * data, unsigned int len);
int speex_enc_process_out(speex_encoder encoder, int end, unsigned char ** data);
void speex_enc_enable_cache_package(speex_encoder encoder, int enable);
void speex_enc_free(speex_encoder encoder);
#ifdef __cplusplus
}
#endif
