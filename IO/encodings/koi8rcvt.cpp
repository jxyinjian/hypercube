#include <cstdlib>
#include "koi8rcvt.h"


using namespace std;

struct cw {
	char c;
	wchar_t w;
};

static wchar_t koi2ucs[] = {
	0x2500, 0x2502, 0x250C, 0x2510, 0x2514, 0x2518, 0x251C, 0x2524,
	0x252C, 0x2534, 0x253C, 0x2580, 0x2584, 0x2588, 0x258C, 0x2590,
	0x2591, 0x2592, 0x2593, 0x2320, 0x25A0, 0x2219, 0x221A, 0x2248,
	0x2264, 0x2265, 0x00A0, 0x2321, 0x00B0, 0x00B2, 0x00B7, 0x00F7,
	0x2550, 0x2551, 0x2552, 0x0451, 0x2553, 0x2554, 0x2555, 0x2556,
	0x2557, 0x2558, 0x2559, 0x255A, 0x255B, 0x255C, 0x255D, 0x255E,
	0x255F, 0x2560, 0x2561, 0x0401, 0x2562, 0x2563, 0x2564, 0x2565,
	0x2566, 0x2567, 0x2568, 0x2569, 0x256A, 0x256B, 0x256C, 0x00A9,
	0x044E, 0x0430, 0x0431, 0x0446, 0x0434, 0x0435, 0x0444, 0x0433,
	0x0445, 0x0438, 0x0439, 0x043A, 0x043B, 0x043C, 0x043D, 0x043E,
	0x043F, 0x044F, 0x0440, 0x0441, 0x0442, 0x0443, 0x0436, 0x0432,
	0x044C, 0x044B, 0x0437, 0x0448, 0x044D, 0x0449, 0x0447, 0x044A,
	0x042E, 0x0410, 0x0411, 0x0426, 0x0414, 0x0415, 0x0424, 0x0413,
	0x0425, 0x0418, 0x0419, 0x041A, 0x041B, 0x041C, 0x041D, 0x041E,
	0x041F, 0x042F, 0x0420, 0x0421, 0x0422, 0x0423, 0x0416, 0x0412,
	0x042C, 0x042B, 0x0417, 0x0428, 0x042D, 0x0429, 0x0427, 0x042A
};

static struct cw ucs2koi[] = {
	{0x9A, 0x00A0}, {0xBF, 0x00A9}, {0x9C, 0x00B0}, {0x9D, 0x00B2},
	{0x9E, 0x00B7}, {0x9F, 0x00F7}, {0xB3, 0x0401}, {0xE1, 0x0410},
	{0xE2, 0x0411}, {0xF7, 0x0412}, {0xE7, 0x0413}, {0xE4, 0x0414},
	{0xE5, 0x0415}, {0xF6, 0x0416}, {0xFA, 0x0417}, {0xE9, 0x0418},
	{0xEA, 0x0419}, {0xEB, 0x041A}, {0xEC, 0x041B}, {0xED, 0x041C},
	{0xEE, 0x041D}, {0xEF, 0x041E}, {0xF0, 0x041F}, {0xF2, 0x0420},
	{0xF3, 0x0421}, {0xF4, 0x0422}, {0xF5, 0x0423}, {0xE6, 0x0424},
	{0xE8, 0x0425}, {0xE3, 0x0426}, {0xFE, 0x0427}, {0xFB, 0x0428},
	{0xFD, 0x0429}, {0xFF, 0x042A}, {0xF9, 0x042B}, {0xF8, 0x042C},
	{0xFC, 0x042D}, {0xE0, 0x042E}, {0xF1, 0x042F}, {0xC1, 0x0430},
	{0xC2, 0x0431}, {0xD7, 0x0432}, {0xC7, 0x0433}, {0xC4, 0x0434},
	{0xC5, 0x0435}, {0xD6, 0x0436}, {0xDA, 0x0437}, {0xC9, 0x0438},
	{0xCA, 0x0439}, {0xCB, 0x043A}, {0xCC, 0x043B}, {0xCD, 0x043C},
	{0xCE, 0x043D}, {0xCF, 0x043E}, {0xD0, 0x043F}, {0xD2, 0x0440},
	{0xD3, 0x0441}, {0xD4, 0x0442}, {0xD5, 0x0443}, {0xC6, 0x0444},
	{0xC8, 0x0445}, {0xC3, 0x0446}, {0xDE, 0x0447}, {0xDB, 0x0448},
	{0xDD, 0x0449}, {0xDF, 0x044A}, {0xD9, 0x044B}, {0xD8, 0x044C},
	{0xDC, 0x044D}, {0xC0, 0x044E}, {0xD1, 0x044F}, {0xA3, 0x0451},
	{0x95, 0x2219}, {0x96, 0x221A}, {0x97, 0x2248}, {0x98, 0x2264},
	{0x99, 0x2265}, {0x93, 0x2320}, {0x9B, 0x2321}, {0x80, 0x2500},
	{0x81, 0x2502}, {0x82, 0x250C}, {0x83, 0x2510}, {0x84, 0x2514},
	{0x85, 0x2518}, {0x86, 0x251C}, {0x87, 0x2524}, {0x88, 0x252C},
	{0x89, 0x2534}, {0x8A, 0x253C}, {0xA0, 0x2550}, {0xA1, 0x2551},
	{0xA2, 0x2552}, {0xA4, 0x2553}, {0xA5, 0x2554}, {0xA6, 0x2555},
	{0xA7, 0x2556}, {0xA8, 0x2557}, {0xA9, 0x2558}, {0xAA, 0x2559},
	{0xAB, 0x255A}, {0xAC, 0x255B}, {0xAD, 0x255C}, {0xAE, 0x255D},
	{0xAF, 0x255E}, {0xB0, 0x255F}, {0xB1, 0x2560}, {0xB2, 0x2561},
	{0xB4, 0x2562}, {0xB5, 0x2563}, {0xB6, 0x2564}, {0xB7, 0x2565},
	{0xB8, 0x2566}, {0xB9, 0x2567}, {0xBA, 0x2568}, {0xBB, 0x2569},
	{0xBC, 0x256A}, {0xBD, 0x256B}, {0xBE, 0x256C}, {0x8B, 0x2580},
	{0x8C, 0x2584}, {0x8D, 0x2588}, {0x8E, 0x258C}, {0x8F, 0x2590},
	{0x90, 0x2591}, {0x91, 0x2592}, {0x92, 0x2593}, {0x94, 0x25A0}
};

static int cmp(const void *p1, const void *p2)
{
	struct cw *cw1 = (struct cw*) p1;
	struct cw *cw2 = (struct cw*) p2;

	return (int)cw1->w - (int)cw2->w;
}


codecvt_base::result koi8rcvt::do_in(mbstate_t&, const char* from,
  const char* from_end, const char*& from_next, wchar_t* to,
  wchar_t* to_limit, wchar_t*& to_next) const
{
	for (from_next = from, to_next = to; from_next < from_end
	  && to_next < to_limit; from_next++, to_next++) {
		if ((unsigned char)*from_next < 0x80)
			*to_next = (unsigned char)*from_next;
		else
			*to_next = koi2ucs[(unsigned char)*from_next - 0x80];
	}

	return codecvt_base::ok;
}

codecvt_base::result koi8rcvt::do_out(mbstate_t&, const wchar_t* from,
  const wchar_t* from_end, const wchar_t*& from_next, char* to,
  char* to_limit, char*& to_next) const
{
	cw key, *res;

	for (from_next = from, to_next = to; from_next < from_end
	  && to_next < to_limit; from_next++, to_next++) {
		if (*from_next < 0x80)
			*to_next = *from_next;
		else {
			key.w = *from_next;
			if ((res = (struct cw*) bsearch(&key, ucs2koi,
			  sizeof(ucs2koi) / sizeof(struct cw),
			  sizeof(struct cw), cmp)) == NULL)
				return codecvt_base::error;

			*to_next = res->c;
		}
	}

	return codecvt_base::ok;
}

codecvt_base::result koi8rcvt::do_unshift(mbstate_t&, char* to, char*,
  char*& to_next) const
{
	to_next = to;

	return codecvt_base::noconv;
}
