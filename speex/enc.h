#pragma once
#include "../speex-lib/include/speex/speex.h"
#include <vector>
#include <cstdlib>
//#include <cstdint>
#include <deque>
class SpeexEncoder {
public:
    SpeexEncoder();
    ~SpeexEncoder();

public:
    void ProcessIn(const unsigned short * data, size_t len);
    std::vector<char> ProcessOut(bool force);

    void EnableCachePackage(bool enable);

private:
    SpeexBits				m_speex_bits;
    void*					m_enc_state;
    int                     m_framesize;

    std::deque<unsigned char>	m_in_remains;
    std::vector<char>		m_out_remains;

    bool                    m_enable_cache_package;  // default enable
};

