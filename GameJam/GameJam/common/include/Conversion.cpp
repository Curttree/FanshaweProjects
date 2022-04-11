#include "Conversion.h"

bool Compare(const glm::mat4& a, const glm::mat4& b)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (a[i][j] != b[i][j])
				return false;
		}
	}
	return true;
}

glm::mat4 Convert(const aiMatrix4x4& m)
{
	return {
		m.a1, m.b1, m.c1, m.d1,
		m.a2, m.b2, m.c2, m.d2,
		m.a3, m.b3, m.c3, m.d3,
		m.a4, m.b4, m.c4, m.d4,
	};
}

void Convert(const aiMatrix4x4& in, glm::mat4& out)
{
	out = Convert(in);
}

void Convert(const aiVector3D& in, glm::vec3& out)
{
	out.x = in.x;
	out.y = in.y;
	out.z = in.z;
}

void Convert(const aiQuaternion& in, glm::quat& out)
{
	out.x = in.x;
	out.y = in.y;
	out.z = in.z;
	out.w = in.w;
}
