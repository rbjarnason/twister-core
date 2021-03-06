/*

Copyright (c) 2003-2012, Arvid Norberg
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the distribution.
    * Neither the name of the author nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef TORRENT_HASHER_HPP_INCLUDED
#define TORRENT_HASHER_HPP_INCLUDED

#include <boost/cstdint.hpp>

#include "libtorrent/peer_id.hpp"
#include "libtorrent/config.hpp"
#include "libtorrent/assert.hpp"

#ifdef TORRENT_USE_GCRYPT
#include <gcrypt.h>

#elif TORRENT_USE_COMMONCRYPTO

#include <CommonCrypto/CommonDigest.h>

#elif defined TORRENT_USE_OPENSSL

extern "C"
{
#include <openssl/sha.h>
}

#else
// from sha1.cpp
namespace libtorrent
{

	struct TORRENT_EXTRA_EXPORT sha_ctx
	{
		boost::uint32_t state[5];
		boost::uint32_t count[2];
		boost::uint8_t buffer[64];
	};

	TORRENT_EXTRA_EXPORT void SHA1_init(sha_ctx* context);
	TORRENT_EXTRA_EXPORT void SHA1_update(sha_ctx* context, boost::uint8_t const* data, boost::uint32_t len);
	TORRENT_EXTRA_EXPORT void SHA1_final(boost::uint8_t* digest, sha_ctx* context);
} // namespace libtorrent

#endif

namespace libtorrent
{
	class TORRENT_EXTRA_EXPORT hasher
	{
	public:

		hasher();
		hasher(const char* data, int len);

#ifdef TORRENT_USE_GCRYPT
		hasher(hasher const& h);
		hasher& operator=(hasher const& h);
#endif

		void update(std::string const& data) { update(data.c_str(), data.size()); }
		void update(const char* data, int len);
		sha1_hash final();

		void reset();

#ifdef TORRENT_USE_GCRYPT
		~hasher();
#endif

	private:

#ifdef TORRENT_USE_GCRYPT
		gcry_md_hd_t m_context;
#elif TORRENT_USE_COMMONCRYPTO
		CC_SHA1_CTX m_context;
#elif defined TORRENT_USE_OPENSSL
		SHA_CTX m_context;
#else
		sha_ctx m_context;
#endif
	};
}

#endif // TORRENT_HASHER_HPP_INCLUDED

