#include <cmath>
#include "CORE/config.h"
#include "io.h"

using namespace std;

static const float Pi = 3.141592f;
static const float C1 = 0.866025f; /* sqrt(3)/2 */

ostringstream IO::ioerr;

CoordinatesF OutputProvider::edgeTextPosition(const LineF &line, float size,
  const CoordinatesF &textBox)
{
	float angle = atan2(abs(line.dy()), abs(line.dx()));
	float h = textBox.y() / 2;
	float w = textBox.x() / 2;
	float hyp = sqrt(h*h + w*w);
	float dist = hyp * sin(angle + asin(h / hyp));

	LineF l(line);
	l.setP1(l.pointAt(0.5));
	l = l.normalVector();
	l.setLength((size / 2) + dist);

	return l.p2() - CoordinatesF(w, -h);
}

CoordinatesF OutputProvider::vertexTextPosition(const CoordinatesF &point,
  float size)
{
	return point + CoordinatesF(size / 2, size);
}

OutputProvider::Arrow OutputProvider::arrow(LineF &line, float size)
{
	Arrow arrow;
	float angle;

	angle = atan2(-line.dy(), line.dx());
	if (angle < 0)
		angle = 2 * Pi + angle;

	arrow.p[0] = line.pointAt(1 - ((size / 2) / line.length()));
	arrow.p[1] = CoordinatesF(sin(angle - Pi / 3) * size,
	  cos(angle - Pi / 3) * size) + arrow.p[0];
	arrow.p[2] = CoordinatesF(sin(angle - Pi + Pi / 3) * size,
	  cos(angle - Pi + Pi / 3) * size) + arrow.p[0];

	line.setP2(
	  line.pointAt(1 - ((size * C1 * 1.5f) / line.length())));

	return arrow;
}

void IO::stringReplace(wstring &source, const wstring &find,
  const wstring &replace)
{
	size_t j = 0;

	while ((j = source.find(find, j)) != wstring::npos) {
		source.replace(j, find.length(), replace);
		j += replace.length();
	}
}

bool IO::stringCaseCmp(const wstring &s1, const wstring &s2)
{
	if (s1.length() != s2.length())
		return false;

	for (size_t i = 0; i < s1.length(); i++)
		if (tolower(s1[i]) != tolower(s2[i]))
			return false;

	return true;
}
