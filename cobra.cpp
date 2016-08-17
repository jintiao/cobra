#include <algorithm>
#include <cmath>
#include <fstream>
#include <functional>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

struct Vector4 {
	float x, y, z, w;
	Vector4 operator- () const { return{ -x, -y, -z, -w }; }
	Vector4 operator+ (const Vector4 &rhs) const { return{ x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w }; }
	Vector4 operator- (const Vector4 &rhs) const { return { x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w }; }
	Vector4 operator* (const Vector4 &rhs) const { return{ x* rhs.x, y * rhs.y, z * rhs.z, w * rhs.w }; }
	Vector4 operator* (float f) const { return{ x * f, y * f, z * f, w * f }; }
	Vector4 Cross (const Vector4 &rhs) const { return { y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x }; }
	float Dot (const Vector4 &rhs) const { return (x * rhs.x + y * rhs.y + z * rhs.z); }
	Vector4 Normalize () const { float invlen = 1.0f / sqrtf (x * x + y * y + z * z); return { x * invlen, y * invlen, z * invlen }; };
};
static inline void Clamp (Vector4 &v, float lower, float upper) {
	v.x = std::min (upper, std::max (lower, v.x));
	v.y = std::min (upper, std::max (lower, v.y));
	v.z = std::min (upper, std::max (lower, v.z));
	v.w = std::min (upper, std::max (lower, v.w));
}

struct Matrix4 {
	float m[4][4];
	Matrix4 () { memset (m, 0, sizeof (m)); m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f; }
    Matrix4 operator* (const Matrix4 &rhs) const {
        Matrix4 t;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                t.m[i][j] = m[i][0] * rhs.m[0][j] + m[i][1] * rhs.m[1][j] + m[i][2] * rhs.m[2][j] + m[i][3] * rhs.m[3][j];
            }
        }
        return t;
    }
	void Translate (const Vector4 &v) { m[3][0] = v.x; m[3][1] = v.y; m[3][2] = v.z; }
	Matrix4 Transpose () const {
		Matrix4 t;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				t.m[i][j] = m[j][i];
			}
		}
		return t;
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
Vector4 TransformPoint (const Vector4 &b, const Matrix4 &mat) {
	Vector4 v;
	v.w = b.x * mat.m[0][3] + b.y * mat.m[1][3] + b.z * mat.m[2][3] + mat.m[3][3];
	v.x = (b.x * mat.m[0][0] + b.y * mat.m[1][0] + b.z * mat.m[2][0] + mat.m[3][0]) / v.w;
	v.y = (b.x * mat.m[0][1] + b.y * mat.m[1][1] + b.z * mat.m[2][1] + mat.m[3][1]) / v.w;
	v.z = (b.x * mat.m[0][2] + b.y * mat.m[1][2] + b.z * mat.m[2][2] + mat.m[3][2]) / v.w;
	return v;
}
Vector4 TransformDir (const Vector4 &b, const Matrix4 &mat) {
	Vector4 v = { 0 };
	v.x = b.x * mat.m[0][0] * + b.y * mat.m[1][0] + b.z * mat.m[2][0];
	v.y = b.x * mat.m[0][1] * b.x + b.y * mat.m[1][1] + b.z * mat.m[2][1];
	v.z = b.x * mat.m[0][2] * b.x + b.y * mat.m[1][2] + b.z * mat.m[2][2];
	return v;
}
Matrix4 CreateProjectionMatrix (float fov, float ratio, float n, float f) {
	float r = n * tan (fov * 0.5f), l = -r, b = -r / ratio, t = r / ratio;
	Matrix4 mat;
	mat.m[0][0] = 2 * n / (r - l); mat.m[0][1] = 0.0f; mat.m[0][2] = 0.0f; mat.m[0][3] = 0.0f;
	mat.m[1][0] = 0.0f; mat.m[1][1] = 2 * n / (t - b); mat.m[1][2] = 0.0f; mat.m[1][3] = 0.0f;
	mat.m[2][0] = (r + l) / (r - l); mat.m[2][1] = (t + b) / (t - b); mat.m[2][2] = -(f + n) / (f - n); mat.m[2][3] = -1.0f;
	mat.m[3][0] = 0.0f; mat.m[3][1] = 0.0f; mat.m[3][2] = (-2.0f * f * n) / (f - n); mat.m[3][3] = 0.0f;
	return mat;
}
Matrix4 CreateViewMatrix (const Vector4 &look, const Vector4 &at, const Vector4 &up) {
	Vector4 zaxis = (look - at).Normalize (), xzaxis = up.Cross (zaxis).Normalize (), yzaxis = zaxis.Cross (xzaxis);
	Matrix4 mat;
	mat.m[0][0] = xzaxis.x; mat.m[0][1] = xzaxis.y; mat.m[0][2] = xzaxis.z; mat.m[0][3] = 0.0f;
	mat.m[1][0] = yzaxis.x; mat.m[1][1] = yzaxis.y; mat.m[1][2] = yzaxis.z; mat.m[1][3] = 0.0f;
	mat.m[2][0] = zaxis.x; mat.m[2][1] = zaxis.y; mat.m[2][2] = zaxis.z; mat.m[2][3] = 0.0f;
	mat.m[3][0] = look.x; mat.m[3][1] = look.y; mat.m[3][2] = look.z; mat.m[3][3] = 1.0f;
	mat.Invert ();
	return mat;
}

struct Vertex { Vector4 pos, uv, normal, viewPos, color; };
struct Index { int pos[3], uv[3], normal[3]; };
struct Texture { int width, height; std::vector<Vector4> data; };
struct Light { Vector4 dir, ambientColor, diffuseColor, specularColor; };

void SaveBmp (std::vector<Vector4> &frameBuffer, int width, int height, std::string file) {
#define INT2CHAR_BIT(num, bit) (unsigned char)(((num) >> (bit)) & 0xff)
#define INT2CHAR(num) INT2CHAR_BIT((num),0), INT2CHAR_BIT((num),8), INT2CHAR_BIT((num),16), INT2CHAR_BIT((num),24)
	unsigned char buf[54] = { 'B', 'M', INT2CHAR (54 + width*height * 32), INT2CHAR (0), INT2CHAR (54), INT2CHAR (40), INT2CHAR (width), INT2CHAR (height), 1, 0, 32, 0 };
	std::ofstream ofs (file, std::ios_base::out | std::ios_base::binary);
	ofs.write ((char *)buf, sizeof (buf));
	for (auto &color : frameBuffer) {
		buf[0] = (unsigned char)std::min (255, (int)(color.z * 255));
		buf[1] = (unsigned char)std::min (255, (int)(color.y * 255));
		buf[2] = (unsigned char)std::min (255, (int)(color.x * 255));
		buf[3] = (unsigned char)std::min (255, (int)(color.w * 255));
		ofs.write ((char *)buf, 4);
	}
}
bool LoadBmp (Texture &texture, std::string file) {
	std::ifstream is (file, std::ios_base::binary);
    if (!is) return false;
	unsigned char buf[54];
	is.read ((char *)buf, sizeof (buf));
	texture.width = *(int *)&buf[18], texture.height = *(int *)&buf[22];
	int bytes = buf[28] / 8, count = texture.width * texture.height * bytes;
	unsigned char *tmp = new unsigned char[count];
	is.read ((char *)tmp, count);
	texture.data.resize (texture.width * texture.height);
	count = 0;
	for (auto &color : texture.data) {
		color = { tmp[count + 2] / 255.0f, tmp[count + 1] / 255.0f, tmp[count + 0] / 255.0f, 0.0f };
		count += bytes;
	}
	delete []tmp;
    return true;
}

struct Model {
	std::vector<Vector4> posBuffer, normalBuffer, uvBuffer;
	std::vector<Index> indexBuffer;
	Texture diffTex;
	Vector4 upperBound = { std::numeric_limits<float>::min (), std::numeric_limits<float>::min (), std::numeric_limits<float>::min (), 0};
	Matrix4 worldMat;
	Model (std::string name, const Vector4 &pos) : posBuffer (1, { 0 }), normalBuffer (1, { 0 }), uvBuffer (1, { 0 }) {
		worldMat.Translate (pos);
		LoadObj (name + ".obj");
		if (uvBuffer.size () > 1) LoadBmp (diffTex, name + ".bmp");
    }
	void LoadObj (std::string str) {
		float x, y, z;
		char dummy;
		std::ifstream is (str);
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
				Index index = { { 0 } };
				if (str.find ("//") != std::string::npos) {
					iss >> token >> index.pos[0] >> dummy >> dummy >> index.normal[0] >>
						index.pos[1] >> dummy >> dummy >> index.normal[1] >>
						index.pos[2] >> dummy >> dummy >> index.normal[2];
				} else {
					size_t count = 0, pos = str.find ('/');
					while (pos != std::string::npos) { count++; pos = str.find ('/', pos + 1); }
					if (count == 6) {
						iss >> token >> index.pos[0] >> dummy >> index.uv[0] >> dummy >> index.normal[0] >>
							index.pos[1] >> dummy >> index.uv[1] >> dummy >> index.normal[1] >>
							index.pos[2] >> dummy >> index.uv[2] >> dummy >> index.normal[2];
					} else if (count == 3) {
						iss >> token >> index.pos[0] >> dummy >> index.uv[0] >> index.pos[1] >> dummy >> index.uv[1] >> index.pos[2] >> dummy >> index.uv[2];
					}
				}
				indexBuffer.push_back (index);
			}
		}
		for (auto &index : indexBuffer) {
			for (int i = 0; i < 3; i++) {
				if (index.pos[i] < 0) index.pos[i] += (int)posBuffer.size ();
				if (index.uv[i] < 0) index.uv[i] += (int)uvBuffer.size ();
				if (index.normal[i] < 0) index.normal[i] += (int)normalBuffer.size ();
			}
		}
		for (auto &uv : uvBuffer) Clamp (uv, 0.0f, 0.999999f);
		for (auto &pos : posBuffer) {
			upperBound.x = std::max (upperBound.x, pos.x);
			upperBound.y = std::max (upperBound.y, pos.y);
			upperBound.z = std::max (upperBound.z, pos.z);
		}
	}
};

struct Renderer {
	int width, height;
	std::vector<Vector4> frameBuffer;
	std::vector<float> depthBuffer;
	Matrix4 projMat, viewMat, mvMat, mvpMat, nmvMat;
    Light light;

	Renderer (int w, int h, const Matrix4 &pm) : width (w), height (h), frameBuffer (w * h, { 0, 0, 0, 0 }), depthBuffer (w * h, std::numeric_limits<float>::max ()), projMat (pm) { }
	void SetLight (const Vector4 &dir, const Vector4 &ambi, const Vector4 &diff, const Vector4 &spec) { light.dir = dir; light.ambientColor = ambi; light.diffuseColor = diff;
	light.specularColor = spec;
	}
    void SetCamera (const Vector4 &look, const Vector4 &at) { viewMat = CreateViewMatrix (look, at, { 0.0f, 1.0f, 0.0f }); }

	void DrawModel (Model &model, bool drawTex = true, bool drawWireFrame = false) {
		mvMat = model.worldMat * viewMat, mvpMat = mvMat * projMat;
		nmvMat = mvMat;  nmvMat.Invert (); nmvMat = nmvMat.Transpose ();
		auto VertexShader = [this] (const Vector4 &pos, const Vector4 &normal, const Vector4 &uv, Vertex &outVertex) {
			outVertex.pos = TransformPoint (pos, mvpMat);
			outVertex.viewPos = TransformPoint (pos, mvMat);
            outVertex.normal = TransformDir(normal, nmvMat);
			outVertex.uv = uv;
		};
		for (auto &index : model.indexBuffer) {
			Vertex ov[3];
			for (int i = 0; i < 3; i++) {
				VertexShader (model.posBuffer[index.pos[i]], model.normalBuffer[index.normal[i]], model.uvBuffer[index.uv[i]], ov[i]);
				Proj2Screen (ov[i].pos);
			}
			if (BackFaceCulling (ov[0].viewPos, ov[1].viewPos, ov[2].viewPos)) continue;
            if (drawTex) FillTriangle (model, ov[0], ov[1], ov[2]);
			if (drawWireFrame) DrawTriangle (ov[0], ov[1], ov[2], { 0, 1.0f, 0, 0 });
		}
	}
	inline void Proj2Screen (Vector4 &pos) { pos.x = (pos.x + 1)* 0.5f * width; pos.y = (pos.y + 1)* 0.5f * height; pos.z = pos.w; }
	static inline bool BackFaceCulling (const Vector4 &p0, const Vector4 &p1, const Vector4 &p2) { return (p0.Dot ((p1 - p0).Cross (p2 - p0)) >= 0); }

	void FillTriangle (Model &model, const Vertex &v0, const Vertex &v1, const Vertex &v2) {
		auto PixelShader = [&model, this] (Vertex &v) -> Vector4 {
			auto normal = v.normal.Normalize ();
			auto ldir = light.dir.Normalize ();
			auto lambertian = std::max (0.0f, ldir.Dot (normal));
			auto specular = 0.0f;
			if (lambertian > 0) {
				auto viewDir = (v.viewPos * -1).Normalize ();
				auto halfDir = (ldir + viewDir).Normalize ();
				auto specAngle = std::max (0.0f, std::min (1.0f, halfDir.Dot (normal)));
				specular = std::pow (specAngle, 16.0f);
			}

			Vector4 difC = { 0.87f, 0.87f, 0.87f, 0 };
			if (!model.diffTex.data.empty ()) 
				difC = model.diffTex.data[(int)std::floor (v.uv.x * model.diffTex.width) + (int)std::floor (v.uv.y * model.diffTex.height) * model.diffTex.width];
			Vector4 c = difC * 0.7f + (light.ambientColor + light.diffuseColor * lambertian + light.specularColor * specular) * 0.3f;
			c.x = std::pow (c.x, 1.0f / 2.2f);
			c.y = std::pow (c.y, 1.0f / 2.2f);
			c.z = std::pow (c.z, 1.0f / 2.2f);
            return c;
		};
		float area = EdgeFunc (v0.pos, v1.pos, v2.pos);
        int x0 = std::max (0, (int)std::floor (std::min (v0.pos.x, std::min (v1.pos.x, v2.pos.x))));
        int y0 = std::max (0, (int)std::floor (std::min (v0.pos.y, std::min (v1.pos.y, v2.pos.y))));
        int x1 = std::min (width - 1, (int)std::floor (std::max (v0.pos.x, std::max (v1.pos.x, v2.pos.x))));
        int y1 = std::min (height - 1, (int)std::floor (std::max (v0.pos.y, std::max (v1.pos.y, v2.pos.y))));
        for (int y = y0; y <= y1; y++) {
            for (int x = x0; x <= x1; x++) {
                Vertex v = { { x + 0.5f, y + 0.5f, 0 } };
                if (!Interpolate (v0, v1, v2, v, area)) continue;
                if (v.pos.z >= depthBuffer[x + y * width]) continue;
                DrawPoint (x, y, PixelShader (v), v.pos.z);
            }
        }
	}
    static bool Interpolate (const Vertex &v0, const Vertex &v1, const Vertex &v2, Vertex &v, float area) {
        float w0 = EdgeFunc (v1.pos, v2.pos, v.pos) / area;
        float w1 = EdgeFunc (v2.pos, v0.pos, v.pos) / area;
        float w2 = EdgeFunc (v0.pos, v1.pos, v.pos) / area;
        if (w0 < 0 || w1 < 0 || w2 < 0) return false;
        v.pos.z = 1.0f / (w0 / v0.pos.z + w1 / v1.pos.z + w2 / v2.pos.z);
		v.viewPos = ((v0.viewPos * (w0 / v0.pos.z) + v1.viewPos * (w1 / v1.pos.z) + v2.viewPos * (w2 / v2.pos.z))) * v.pos.z;
		v.normal = ((v0.normal * (w0 / v0.pos.z) + v1.normal * (w1 / v1.pos.z) + v2.normal * (w2 / v2.pos.z))) * v.pos.z;
		v.color = ((v0.color * (w0 / v0.pos.z) + v1.color * (w1 / v1.pos.z) + v2.color * (w2 / v2.pos.z))) * v.pos.z;
		v.uv = ((v0.uv * (w0 / v0.pos.z) + v1.uv * (w1 / v1.pos.z) + v2.uv * (w2 / v2.pos.z))) * v.pos.z;
		Clamp (v.uv, 0.0f, 0.999999f);
        return true;
    }
    static inline float EdgeFunc (const Vector4 &p0, const Vector4 &p1, const Vector4 &p2) { return ((p2.x - p0.x) * (p1.y - p0.y) - (p2.y - p0.y) * (p1.x - p0.x)); }

	void DrawTriangle (const Vertex &v0, const Vertex &v1, const Vertex &v2, const Vector4 &color) {
		DrawLine (v0.pos, v1.pos, color); DrawLine (v1.pos, v2.pos, color); DrawLine (v0.pos, v2.pos, color);
	}
    void DrawLine (const Vector4 &p0, const Vector4 &p1, const Vector4 &color) {
        int x0 = (int)std::floor(p0.x), x1 = (int)std::floor(p1.x), y0 = (int)std::floor(p0.y), y1 = (int)std::floor(p1.y);
        if (abs (x1 - x0) >= abs (y1 - y0)) {
            if (x0 > x1) { std::swap (x0, x1); std::swap (y0, y1); }
            DrawLineInternal (x0, y0, x1, y1, color, false);
        }
        else {
            if (y0 > y1) { std::swap (x0, x1); std::swap (y0, y1); }
            DrawLineInternal (y0, x0, y1, x1, color, true);
        }
    }
    void DrawLineInternal (int x0, int y0, int x1, int y1, const Vector4 &color, bool steep) {
        if (y0 == y1) {
            for (int x = x0, y = y0; x <= x1; x++)
                steep ? DrawPoint (y, x, color, 0) : DrawPoint (x, y, color, 0);
            return;
        }
        int dx = x1 - x0, dy = abs (y1 - y0), ystep = dy / (y1 - y0), delta = dy - dx, y = y0;
        for (int x = x0; x <= x1; x++, delta += dy) {
            steep ? DrawPoint (y, x, color, 0) : DrawPoint (x, y, color, 0);
            if (delta >= 0) {
                y += ystep;
                delta -= dx;
            }
        }
    }
    void DrawPoint (int x, int y, const Vector4 &color, float z) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            frameBuffer[x + y * width] = color;
            depthBuffer[x + y * width] = z;
        }
    }
};

int main () {
	const int WIDTH = 1024, HEIGHT = 768;
	Renderer renderer (WIDTH, HEIGHT, CreateProjectionMatrix ((float)M_PI_2, (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f));
	renderer.SetLight ({ 0.0f, 1.0f, 1.0f }, { 0.1f, 0.1f, 0.1f, 0 }, { 0.5f, 0.5, 0, 0 }, { 1.0f, 1.0f, 1.0f, 0 });
    renderer.SetCamera({ 10.0f, 10.0f, -12.0f }, { 10.0f, 0.0f, 10.0f });
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            Model model ("sphere", { i * 5.0f, 0.0f, j * 5.0f });
            renderer.DrawModel (model, true, false);
        }
    }
	SaveBmp (renderer.frameBuffer, WIDTH, HEIGHT, "screenshot.bmp");
	return 0;
}
