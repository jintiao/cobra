#include <cmath>
#include <fstream>
#include <functional>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

const int WIDTH = 1024, HEIGHT = 768;

struct Vector3 {
	float x, y, z;
	Vector3 operator- (const Vector3 &rhs) const { return { x - rhs.x, y - rhs.y, z - rhs.z }; }
	Vector3 Cross (const Vector3 &rhs) const { return { y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x }; }
	Vector3 Normalize () const { float invlen = 1.0f / sqrtf (x * x + y * y + z * z); return { x * invlen, y * invlen, z * invlen }; };
};

struct Matrix4 {
	float m[4][4];
    Matrix4 operator* (const Matrix4 &rhs) const {
        Matrix4 res;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                res.m[i][j] = m[i][0] * rhs.m[0][j] + m[i][1] * rhs.m[1][j] + m[i][2] * rhs.m[2][j] + m[i][3] * rhs.m[3][j];
            }
        }
        return res;
    }
    Vector3 operator* (const Vector3 &b) const {
		Vector3 v;
		float w = m[0][3] * b.x + m[1][3] * b.y + m[2][3] * b.z + m[3][3];
		v.x = (m[0][0] * b.x + m[1][0] * b.y + m[2][0] * b.z + m[3][0]) / w;
		v.y = (m[0][1] * b.x + m[1][1] * b.y + m[2][1] * b.z + m[3][1]) / w;
		v.z = (m[0][2] * b.x + m[1][2] * b.y + m[2][2] * b.z + m[3][2]) / w;
		return v;
	}
	void Invert () {
		Matrix4	temm = *this;
		float tmp[12];
		tmp[0] = temm.m[2][2] * temm.m[3][3];
		tmp[1] = temm.m[3][2] * temm.m[2][3];
		tmp[2] = temm.m[1][2] * temm.m[3][3];
		tmp[3] = temm.m[3][2] * temm.m[1][3];
		tmp[4] = temm.m[1][2] * temm.m[2][3];
		tmp[5] = temm.m[2][2] * temm.m[1][3];
		tmp[6] = temm.m[0][2] * temm.m[3][3];
		tmp[7] = temm.m[3][2] * temm.m[0][3];
		tmp[8] = temm.m[0][2] * temm.m[2][3];
		tmp[9] = temm.m[2][2] * temm.m[0][3];
		tmp[10] = temm.m[0][2] * temm.m[1][3];
		tmp[11] = temm.m[1][2] * temm.m[0][3];
		m[0][0] = tmp[0] * temm.m[1][1] + tmp[3] * temm.m[2][1] + tmp[4] * temm.m[3][1];
		m[0][0] -= tmp[1] * temm.m[1][1] + tmp[2] * temm.m[2][1] + tmp[5] * temm.m[3][1];
		m[0][1] = tmp[1] * temm.m[0][1] + tmp[6] * temm.m[2][1] + tmp[9] * temm.m[3][1];
		m[0][1] -= tmp[0] * temm.m[0][1] + tmp[7] * temm.m[2][1] + tmp[8] * temm.m[3][1];
		m[0][2] = tmp[2] * temm.m[0][1] + tmp[7] * temm.m[1][1] + tmp[10] * temm.m[3][1];
		m[0][2] -= tmp[3] * temm.m[0][1] + tmp[6] * temm.m[1][1] + tmp[11] * temm.m[3][1];
		m[0][3] = tmp[5] * temm.m[0][1] + tmp[8] * temm.m[1][1] + tmp[11] * temm.m[2][1];
		m[0][3] -= tmp[4] * temm.m[0][1] + tmp[9] * temm.m[1][1] + tmp[10] * temm.m[2][1];
		m[1][0] = tmp[1] * temm.m[1][0] + tmp[2] * temm.m[2][0] + tmp[5] * temm.m[3][0];
		m[1][0] -= tmp[0] * temm.m[1][0] + tmp[3] * temm.m[2][0] + tmp[4] * temm.m[3][0];
		m[1][1] = tmp[0] * temm.m[0][0] + tmp[7] * temm.m[2][0] + tmp[8] * temm.m[3][0];
		m[1][1] -= tmp[1] * temm.m[0][0] + tmp[6] * temm.m[2][0] + tmp[9] * temm.m[3][0];
		m[1][2] = tmp[3] * temm.m[0][0] + tmp[6] * temm.m[1][0] + tmp[11] * temm.m[3][0];
		m[1][2] -= tmp[2] * temm.m[0][0] + tmp[7] * temm.m[1][0] + tmp[10] * temm.m[3][0];
		m[1][3] = tmp[4] * temm.m[0][0] + tmp[9] * temm.m[1][0] + tmp[10] * temm.m[2][0];
		m[1][3] -= tmp[5] * temm.m[0][0] + tmp[8] * temm.m[1][0] + tmp[11] * temm.m[2][0];
		tmp[0] = temm.m[2][0]*temm.m[3][1];
		tmp[1] = temm.m[3][0]*temm.m[2][1];
		tmp[2] = temm.m[1][0]*temm.m[3][1];
		tmp[3] = temm.m[3][0]*temm.m[1][1];
		tmp[4] = temm.m[1][0]*temm.m[2][1];
		tmp[5] = temm.m[2][0]*temm.m[1][1];
		tmp[6] = temm.m[0][0]*temm.m[3][1];
		tmp[7] = temm.m[3][0]*temm.m[0][1];
		tmp[8] = temm.m[0][0]*temm.m[2][1];
		tmp[9] = temm.m[2][0]*temm.m[0][1];
		tmp[10] = temm.m[0][0]*temm.m[1][1];
		tmp[11] = temm.m[1][0]*temm.m[0][1];
		m[2][0] = tmp[0] * temm.m[1][3] + tmp[3] * temm.m[2][3] + tmp[4] * temm.m[3][3];
		m[2][0] -= tmp[1] * temm.m[1][3] + tmp[2] * temm.m[2][3] + tmp[5] * temm.m[3][3];
		m[2][1] = tmp[1] * temm.m[0][3] + tmp[6] * temm.m[2][3] + tmp[9] * temm.m[3][3];
		m[2][1] -= tmp[0] * temm.m[0][3] + tmp[7] * temm.m[2][3] + tmp[8] * temm.m[3][3];
		m[2][2] = tmp[2] * temm.m[0][3] + tmp[7] * temm.m[1][3] + tmp[10] * temm.m[3][3];
		m[2][2] -= tmp[3] * temm.m[0][3] + tmp[6] * temm.m[1][3] + tmp[11] * temm.m[3][3];
		m[2][3] = tmp[5] * temm.m[0][3] + tmp[8] * temm.m[1][3] + tmp[11] * temm.m[2][3];
		m[2][3] -= tmp[4] * temm.m[0][3] + tmp[9] * temm.m[1][3] + tmp[10] * temm.m[2][3];
		m[3][0] = tmp[2] * temm.m[2][2] + tmp[5] * temm.m[3][2] + tmp[1] * temm.m[1][2];
		m[3][0] -= tmp[4] * temm.m[3][2] + tmp[0] * temm.m[1][2] + tmp[3] * temm.m[2][2];
		m[3][1] = tmp[8] * temm.m[3][2] + tmp[0] * temm.m[0][2] + tmp[7] * temm.m[2][2];
		m[3][1] -= tmp[6] * temm.m[2][2] + tmp[9] * temm.m[3][2] + tmp[1] * temm.m[0][2];
		m[3][2] = tmp[6] * temm.m[1][2] + tmp[11] * temm.m[3][2] + tmp[3] * temm.m[0][2];
		m[3][2] -= tmp[10] * temm.m[3][2] + tmp[2] * temm.m[0][2] + tmp[7] * temm.m[1][2];
		m[3][3] = tmp[10] * temm.m[2][2] + tmp[4] * temm.m[0][2] + tmp[9] * temm.m[1][2];
		m[3][3] -= tmp[8] * temm.m[1][2] + tmp[11] * temm.m[2][2] + tmp[5] * temm.m[0][2];
		float idet = 1.0f / (temm.m[0][0] * m[0][0] + temm.m[1][0] * m[0][1] + temm.m[2][0] * m[0][2] + temm.m[3][0] * m[0][3]);
		m[0][0] *= idet; m[0][1] *= idet; m[0][2] *= idet; m[0][3] *= idet;
		m[1][0] *= idet; m[1][1] *= idet; m[1][2] *= idet; m[1][3] *= idet;
		m[2][0] *= idet; m[2][1] *= idet; m[2][2] *= idet; m[2][3] *= idet;
		m[3][0] *= idet; m[3][1] *= idet; m[3][2] *= idet; m[3][3] *= idet;
	}
};

struct Color { unsigned char b, g, r, a; };
static Color WHITE = { 255, 255, 255, 0 };
struct Index { int pos[3], uv[3], normal[3]; };
struct Vertex { Vector3 pos, uv, normal; };

Matrix4 CreateProjectionMatrix (float fov, float ratio, float n, float f) {
	float r = n * tan (fov * 0.5f), l = -r, b = -r / ratio, t = r / ratio;
    Matrix4 mat;
    mat.m[0][0] = 2 * n / (r - l); mat.m[0][1] = 0.0f; mat.m[0][2] = 0.0f; mat.m[0][3] = 0.0f;
    mat.m[1][0] = 0.0f; mat.m[1][1] = 2 * n / (t - b); mat.m[1][2] = 0.0f; mat.m[1][3] = 0.0f;
    mat.m[2][0] = (r + l) / (r - l); mat.m[2][1] = (t + b) / (t - b); mat.m[2][2] = -(f + n) / (f - n); mat.m[2][3] = -1.0f;
    mat.m[3][0] = 0.0f; mat.m[3][1] = 0.0f; mat.m[3][2] = (-2.0f * f * n) / (f - n); mat.m[3][3] = 0.0f;
    return mat;
}

Matrix4 CreateViewMatrix (const Vector3 &look, const Vector3 &at, const Vector3 &up) {
    Vector3 dir = (look - at).Normalize (), left = up.Cross (dir).Normalize (), newUp = dir.Cross (left);
    Matrix4 mat;
    mat.m[0][0] = left.x; mat.m[0][1] = left.y; mat.m[0][2] = left.z; mat.m[0][3] = 0.0f;
    mat.m[1][0] = newUp.x; mat.m[1][1] = newUp.y; mat.m[1][2] = newUp.z; mat.m[1][3] = 0.0f;
    mat.m[2][0] = dir.x; mat.m[2][1] = dir.y; mat.m[2][2] = dir.z; mat.m[2][3] = 0.0f;
    mat.m[3][0] = look.x; mat.m[3][1] = look.y; mat.m[3][2] = look.z; mat.m[3][3] = 1.0f;
    mat.Invert ();
    return mat;
}

int CalcOctant (int x0, int y0, int x1, int y1)
{
	int x = x1 - x0, y = y1 - y0;
	if (x >= 0)
	{
		if (y >= 0)
		{
			if (x > y)
				return 0;
			else
				return 1;
		}
		else
		{
			if (x > -y)
				return 7;
			else
				return 6;
		}
	}
	else
	{
		if (y >= 0)
		{
			if (-x > y)
				return 3;
			else
				return 2;
		}
		else
		{
			if (x > y)
				return 5;
			else
				return 4;
		}
	}
}
void SwitchToOctantZeroFrom (int octant, int &x, int &y)
{
	switch (octant)
	{
	case 1:
		std::swap (x, y);
		break;
	case 2:
		x = -x;
		std::swap (x, y);
		break;
	case 3:
		x = -x;
		break;
	case 4:
		x = -x;
		y = -y;
		break;
	case 5:
		x = -x;
		y = -y;
		std::swap (x, y);
		break;
	case 6:
		y = -y;
		std::swap (x, y);
		break;
	case 7:
		y = -y;
		break;
	}
}
void SwitchFromOctantZeroTo (int octant, int &x, int &y)
{
	switch (octant)
	{
	case 1:
		std::swap (x, y);
		break;
	case 2:
		y = -y;
		std::swap (x, y);
		break;
	case 3:
		x = -x;
		break;
	case 4:
		x = -x;
		y = -y;
		break;
	case 5:
		x = -x;
		y = -y;
		std::swap (x, y);
		break;
	case 6:
		x = -x;
		std::swap (x, y);
		break;
	case 7:
		y = -y;
		break;
	}
}

void SaveBmp (std::vector<Color> &frameBuffer, int width, int height, std::string file) {
#define INT2CHAR_BIT(num, bit) (char)(((num) >> (bit)) & 0xff)
#define INT2CHAR(num) INT2CHAR_BIT((num),0), INT2CHAR_BIT((num),8), INT2CHAR_BIT((num),16), INT2CHAR_BIT((num),24)
	const char header[54] = { 'B', 'M', INT2CHAR (54 + width*height * sizeof (Color)), INT2CHAR (0), INT2CHAR (54), INT2CHAR (40), INT2CHAR (width), INT2CHAR (height), 1, 0, 32, 0 };
	std::ofstream ofs (file, std::ios_base::out | std::ios_base::binary);
	ofs.write (header, sizeof (header));
	ofs.write (static_cast<const char *> (static_cast<const void *> (frameBuffer.data ())), width * height * sizeof (Color));
}

struct Texture {
	int width, height;
	std::vector<Color> data;
	Texture (std::string file) {
		std::ifstream is (file, std::ios_base::binary);
		unsigned char buf[54];
		is.read ((char *)buf, sizeof (buf));
		width = *(int *)&buf[18], height = *(int *)&buf[22];
		data.resize (width * height);
		for (auto &color : data) {
			is.read ((char *)buf, 3);
			color = { buf[0], buf[1], buf[2], 0 };
		}
	}
};

struct Model {
	std::vector<Vector3> posBuffer, normalBuffer, uvBuffer;
	std::vector<Index> indexBuffer;
	Texture diffTexture;
	Model (std::string str) : diffTexture (str + ".bmp") {
		float x, y, z;
		char dummy;
		std::ifstream is (str + ".obj");
		while (std::getline (is, str)) {
			if (str.length () < 2) continue;
			std::istringstream iss (str);
			std::string token;
			if (str[1] == 't' && str[0] == 'v') {
				iss >> token >> x >> y;
				uvBuffer.push_back ({ x, y });
			} else if (str[1] == 'n' && str[0] == 'v') {
				iss >> token >> x >> y >> z;
				normalBuffer.push_back ({ x, y, z });
			} else if (str[0] == 'v') {
				iss >> token >> x >> y >> z;
				posBuffer.push_back ({ x, y, z });
			} else if (str[0] == 'f') {
				Index index;
				iss >> token >> index.pos[0] >> dummy >> index.uv[0] >> dummy >> index.normal[0] >>
					index.pos[1] >> dummy >> index.uv[1] >> dummy >> index.normal[1] >>
					index.pos[2] >> dummy >> index.uv[2] >> dummy >> index.normal[2];
				indexBuffer.push_back (index);
			}
		}
	}
};

struct Renderer {
	std::vector<Color> frameBuffer;
	std::vector<float> depthBuffer;
	Renderer (int width, int height) : frameBuffer (width * height, { 0, 0, 0, 0 }), depthBuffer (width * height, std::numeric_limits<float>::max ()) { }
	void DrawModel (Matrix4 projMat, Matrix4 viewMat, Model &model, bool wireframe = false) {
		Matrix4 mvp = viewMat * projMat;
		auto VertexShader = [&mvp] (const Vector3 &pos, const Vector3 &, const Vector3 &uv, Vertex &outVertex) {
			outVertex.pos = mvp * pos;
			outVertex.uv = uv;
		};
		for (auto &index : model.indexBuffer) {
			Vertex ov[3];
			for (int i = 0; i < 3; i++) {
				VertexShader (model.posBuffer[index.pos[i] - 1], model.normalBuffer[index.normal[i] - 1], model.uvBuffer[index.uv[i] - 1], ov[i]);
                ov[i].pos.x = (ov[i].pos.x + 1) * WIDTH * 0.5f;
                ov[i].pos.y = (1 - ov[i].pos.y) * HEIGHT * 0.5f;
			}
            wireframe ? DrawTriangle (ov[0], ov[1], ov[2]) : FillTriangle (ov[0], ov[1], ov[2]);
		}
	}
	void DrawTriangle (const Vertex &v0, const Vertex &v1, const Vertex &v2) {
		DrawLine (v0, v1);
		DrawLine (v1, v2);
		DrawLine (v0, v2);
	}
    void DrawLine (const Vertex &v0, const Vertex &v1) {
        int x0 = (int)round (v0.pos.x), y0 = (int)round (v0.pos.y), x1 = (int)round (v1.pos.x), y1 = (int)round (v1.pos.y), octant = CalcOctant (x0, y0, x1, y1);
        SwitchToOctantZeroFrom (octant, x0, y0);
        SwitchToOctantZeroFrom (octant, x1, y1);
        float dx = (float)(x1 - x0), dy = (float)(y1 - y0), y = (float)y0, delta = dy / dx;
        for (int x = x0 + 1; x <= x1; x++, y += delta) {
			int ix = x, iy = (int)round (y);
			SwitchFromOctantZeroTo (octant, ix, iy);
			if (ix >= 0 && iy >= 0 && (ix + iy * WIDTH) < frameBuffer.size ()) {
				frameBuffer[ix + iy * WIDTH] = WHITE;
			}
        }
    }
	void FillTriangle (const Vertex &, const Vertex &, const Vertex &) {
	}
	void DrawPoint (const Vertex &v) {
		auto PixelShader = [] (const Vector3 &, Color &outColor) {
			outColor = { 255, 255, 255, 0 };
		};
		Color c;
		int x = (int)v.pos.x, y = (int)v.pos.y, p = x + y * WIDTH;
		if (x >= 0 && y >= 0 && p < frameBuffer.size ()) {
			PixelShader (v.uv, c);
			frameBuffer[p] = c;
		}
	}
};

int main () {
	Renderer renderer (WIDTH, HEIGHT);
	Model model ("cube");
	renderer.DrawModel (CreateProjectionMatrix ((float)M_PI_2, (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f), CreateViewMatrix ({ 0.0f, 1.8f, 1.1f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }), model, true);
	SaveBmp (renderer.frameBuffer, WIDTH, HEIGHT, "output.bmp");
	return 0;
}
