#include "enc.h"

#include <cassert>
#include <algorithm>
#include <iterator>
#include <iostream>

// 进行识别的最小数据块大小
#define SS_MIN_CHUNK_SIZE 1600

SpeexEncoder::SpeexEncoder() {

	m_enable_cache_package = true;

	speex_bits_init(&m_speex_bits);
	m_enc_state = speex_encoder_init(&speex_wb_mode);

	int quality = 7;
	int complexity = 3;

	speex_encoder_ctl(m_enc_state, SPEEX_SET_QUALITY, &quality);
	speex_encoder_ctl(m_enc_state, SPEEX_SET_COMPLEXITY, &complexity);

	speex_encoder_ctl(m_enc_state, SPEEX_GET_FRAME_SIZE, &m_framesize);
}

SpeexEncoder::~SpeexEncoder() {

	speex_encoder_destroy(m_enc_state);
	speex_bits_destroy(&m_speex_bits);

}

void SpeexEncoder::ProcessIn(const unsigned short * data, size_t len) {
    m_in_remains.insert(m_in_remains.end(), data, data + len);
}

std::vector<char> SpeexEncoder::ProcessOut(bool force) {

	std::vector<spx_int16_t> frame;
    frame.reserve(m_framesize);

    std::vector<char> ret = m_out_remains;
    m_out_remains.clear();

    while(force && (m_in_remains.size() % m_framesize) > 0) {
        m_in_remains.push_back(0);
    }

    for (; static_cast<int>(m_in_remains.size()) >= m_framesize; m_in_remains.erase(m_in_remains.begin(), m_in_remains.begin() + m_framesize)) {

        frame.assign(m_in_remains.begin(), m_in_remains.begin() + m_framesize);

        speex_bits_reset(&m_speex_bits);
        speex_encode_int(m_enc_state, frame.data(), &m_speex_bits);

        int need = speex_bits_nbytes(&m_speex_bits);

        int outseek = ret.size();
        ret.resize(outseek + need);

        int bytes = speex_bits_write(&m_speex_bits, ret.data() + outseek, need);
        assert(bytes == need);

    }

    if (m_enable_cache_package) {
        if (ret.size() < SS_MIN_CHUNK_SIZE && !force) {
            m_out_remains = ret;
            ret.clear();
        }
    }

	return ret;
}

void SpeexEncoder::EnableCachePackage(bool enable) {
	m_enable_cache_package = enable;
}