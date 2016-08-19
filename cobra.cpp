#include <algorithm>
#include <cmath>
#include <fstream>
#include <functional>
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
}; // we use vecter4 representing position/direction/uv/color etc.

struct Matrix4 {
	float m[4][4];
	Matrix4 () { memset (m, 0, sizeof (m)); m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f; }
    Matrix4 operator* (const Matrix4 &rhs) const {
        Matrix4 t;
        for (int i = 0; i < 4; i++) { for (int j = 0; j < 4; j++) { t.m[i][j] = m[i][0] * rhs.m[0][j] + m[i][1] * rhs.m[1][j] + m[i][2] * rhs.m[2][j] + m[i][3] * rhs.m[3][j]; } }
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
	} // copy from somewhere, no idea what it is.

	Matrix4 InvertTranspose () const {
		Matrix4 t, o = *this;
		o.Invert ();
		for (int i = 0; i < 4; i++) { for (int j = 0; j < 4; j++) {	t.m[i][j] = o.m[j][i]; } }
		return t;
	} // (M-1)T
}; // our matrix is in row-major order.

Vector4 TransformPoint (const Vector4 &b, const Matrix4 &mat) {
	Vector4 v;
	v.w = b.x * mat.m[0][3] + b.y * mat.m[1][3] + b.z * mat.m[2][3] + mat.m[3][3];
	v.x = (b.x * mat.m[0][0] + b.y * mat.m[1][0] + b.z * mat.m[2][0] + mat.m[3][0]) / v.w;
	v.y = (b.x * mat.m[0][1] + b.y * mat.m[1][1] + b.z * mat.m[2][1] + mat.m[3][1]) / v.w;
	v.z = (b.x * mat.m[0][2] + b.y * mat.m[1][2] + b.z * mat.m[2][2] + mat.m[3][2]) / v.w;
	return v;
} // using matrix to transform a point.

Vector4 TransformDir (const Vector4 &b, const Matrix4 &mat) {
	Vector4 v = { 0 };
	v.x = b.x * mat.m[0][0] * + b.y * mat.m[1][0] + b.z * mat.m[2][0];
	v.y = b.x * mat.m[0][1] * b.x + b.y * mat.m[1][1] + b.z * mat.m[2][1];
	v.z = b.x * mat.m[0][2] * b.x + b.y * mat.m[1][2] + b.z * mat.m[2][2];
	return v;
} // using matrix to transform a direction.

Matrix4 CreateProjectionMatrix (float hfov, float ratio, float n, float f) {
	float r = n * tan (hfov * 0.5f), l = -r, b = -r / ratio, t = r / ratio;
	Matrix4 mat;
	mat.m[0][0] = 2 * n / (r - l); mat.m[0][1] = 0.0f; mat.m[0][2] = 0.0f; mat.m[0][3] = 0.0f;
	mat.m[1][0] = 0.0f; mat.m[1][1] = 2 * n / (t - b); mat.m[1][2] = 0.0f; mat.m[1][3] = 0.0f;
	mat.m[2][0] = (r + l) / (r - l); mat.m[2][1] = (t + b) / (t - b); mat.m[2][2] = -(f + n) / (f - n); mat.m[2][3] = -1.0f;
	mat.m[3][0] = 0.0f; mat.m[3][1] = 0.0f; mat.m[3][2] = (-2.0f * f * n) / (f - n); mat.m[3][3] = 0.0f;
	return mat;
} // same as opengl's projection matrix, but remember we are using row-major order, so it looks a little different.

Matrix4 CreateViewMatrix (const Vector4 &look, const Vector4 &at, const Vector4 &up) {
	Vector4 zaxis = (look - at).Normalize (), xzaxis = up.Cross (zaxis).Normalize (), yzaxis = zaxis.Cross (xzaxis);
	Matrix4 mat;
	mat.m[0][0] = xzaxis.x; mat.m[0][1] = xzaxis.y; mat.m[0][2] = xzaxis.z; mat.m[0][3] = 0.0f;
	mat.m[1][0] = yzaxis.x; mat.m[1][1] = yzaxis.y; mat.m[1][2] = yzaxis.z; mat.m[1][3] = 0.0f;
	mat.m[2][0] = zaxis.x; mat.m[2][1] = zaxis.y; mat.m[2][2] = zaxis.z; mat.m[2][3] = 0.0f;
	mat.m[3][0] = look.x; mat.m[3][1] = look.y; mat.m[3][2] = look.z; mat.m[3][3] = 1.0f;
	mat.Invert ();
	return mat;
} // view matrix, camera matrix, whatever you call it.

Matrix4 CreateModelMatrix (const Vector4 &translate) {
	Matrix4 mat;
	mat.m[3][0] = translate.x; mat.m[3][1] = translate.y; mat.m[3][2] = translate.z;
	return mat;
} // model(world) matrix, now we only support translate. rotate/scale is NOT supported.

struct Vertex { Vector4 pos, uv, normal, viewPos, color; };
struct Index { int pos[3], uv[3], normal[3]; };
struct Texture { int width, height; float smax, tmax; std::vector<Vector4> data; };
struct Material { float ka, kd, ks; Texture texture; };
struct Light { Vector4 pos, viewPos, ambientColor, diffuseColor, specularColor; };

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
} // bmp's color is bgra order

bool LoadBmp (Texture &texture, std::string file) {
	std::ifstream is (file, std::ios_base::binary);
    if (!is) return false;
	unsigned char buf[54];
	is.read ((char *)buf, sizeof (buf));
	texture.width = *(int *)&buf[18], texture.height = abs (*(int *)&buf[22]);
    texture.smax = texture.width - 1.5f, texture.tmax = texture.height - 1.5f;
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
} // load bmp into texture

struct Model {
	Material material;
	std::vector<Vector4> posBuffer, normalBuffer, uvBuffer;
	std::vector<Index> indexBuffer;
	Matrix4 worldMat;

	Model (std::string name, const Vector4 &pos, Material m) : material (m), posBuffer (1, { 0 }), normalBuffer (1, { 0 }), uvBuffer (1, { 0 }) {
		worldMat = CreateModelMatrix (pos);
		LoadObj (name + ".obj");
		if (uvBuffer.size () > 1) LoadBmp (material.texture, name + ".bmp");
    }

	void LoadObj (std::string str) {
		float x, y, z;
		char dummy;
		std::ifstream is (str);
		while (std::getline (is, str)) {
			if (str.length () < 2) continue;
			std::istringstream iss (str);
			std::string token;
			if (str[1] == 't' && str[0] == 'v') { // load uv. "vt -0.05 0.0972793"
				iss >> token >> x >> y;
				uvBuffer.push_back ({ x, y });
			} else if (str[1] == 'n' && str[0] == 'v') { // load normal. "vn -0.981591 -0.162468 0.100411"
				iss >> token >> x >> y >> z;
				normalBuffer.push_back ({ x, y, z });
			} else if (str[0] == 'v') { // load postion. "v -0.983024 -0.156077 0.0964607"
				iss >> token >> x >> y >> z;
				posBuffer.push_back ({ x, y, z });
			} else if (str[0] == 'f') { // load index. keep in mind that uv/normal index are optional.
				Index index = { { 0 } };
				if (str.find ("//") != std::string::npos) { // pos//normal, no uv. "f 181//176 182//182 209//208"
					iss >> token >> index.pos[0] >> dummy >> dummy >> index.normal[0] >>
						index.pos[1] >> dummy >> dummy >> index.normal[1] >>
						index.pos[2] >> dummy >> dummy >> index.normal[2];
				} else {
					size_t count = 0, pos = str.find ('/');
					while (pos != std::string::npos) { count++; pos = str.find ('/', pos + 1); }
					if (count == 6) { // "f 181/292/176 182/250/182 209/210/208"
						iss >> token >> index.pos[0] >> dummy >> index.uv[0] >> dummy >> index.normal[0] >>
							index.pos[1] >> dummy >> index.uv[1] >> dummy >> index.normal[1] >>
							index.pos[2] >> dummy >> index.uv[2] >> dummy >> index.normal[2];
					} else if (count == 3) { // pos/uv, no normal. "f 181/176 182/182 209/208"
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
		} // deal with negative index
	} // obj is a text base model format
};

struct Renderer {
	int width, height;
	std::vector<Vector4> frameBuffer;
	std::vector<float> depthBuffer;
	Matrix4 projMat, viewMat, mvMat, mvpMat, nmvMat;
    Light light;

	Renderer (int w, int h) : width (w), height (h), frameBuffer (w * h, { 0, 0, 0.34f, 0 }), depthBuffer (w * h, std::numeric_limits<float>::max ()) {	}

	void SetFrustum (float hfov, float ratio, float n, float f) { projMat = CreateProjectionMatrix (hfov, ratio, n, f); }

    void SetCamera (const Vector4 &look, const Vector4 &at) { viewMat = CreateViewMatrix (look, at, { 0.0f, 1.0f, 0.0f }); }

	void SetLight (const Vector4 &pos, const Vector4 &ambi, const Vector4 &diff, const Vector4 &spec) {
		light.pos = pos; light.ambientColor = ambi; light.diffuseColor = diff;	light.specularColor = spec;
	} // we support one direction light right now.

	void DrawModel (Model &model, bool drawTex = true, bool drawWireFrame = false) {
		// again, using row-major order matrix, the calculation order is "pos * modelMat * viewMat * projMat".
		// if you are using column-major order matrix, it will be "projMat * viewMat * modelMat * pos".
		mvMat = model.worldMat * viewMat, mvpMat = mvMat * projMat, nmvMat = mvMat.InvertTranspose ();
		
		// we need this in pixel shader
		light.viewPos = TransformPoint (light.pos, mvMat);

		auto VertexShader = [this] (const Vector4 &pos, const Vector4 &normal, const Vector4 &uv, Vertex &outVertex) {
			outVertex.pos = TransformPoint (pos, mvpMat);
			outVertex.viewPos = TransformPoint (pos, mvMat);
            outVertex.normal = TransformDir(normal, nmvMat);
			outVertex.uv = uv;
		}; // note that transform point/dir/normal require different matrix.

		for (auto &index : model.indexBuffer) {
			Vertex outVertex[3];
			bool badTriangle = false;

			for (int i = 0; i < 3; i++) {
				// run vertex shader for every vertex
				VertexShader (model.posBuffer[index.pos[i]], model.normalBuffer[index.normal[i]], model.uvBuffer[index.uv[i]], outVertex[i]);

				if (outVertex[i].pos.z < 0.0f || outVertex[i].pos.z > 1.0f) {
					badTriangle = true;	break;
				} // check the vertex inside or outside the view frustum

				Ndc2Screen (outVertex[i].pos); // convert to screen coordinate
			} // travers all three vertex

			// skip triangles that are invisible
			if (badTriangle || BackFaceCulling (outVertex[0].viewPos, outVertex[1].viewPos, outVertex[2].viewPos)) continue;

			// texture mode drawing
            if (drawTex) FillTriangle (model, outVertex[0], outVertex[1], outVertex[2]);

			// wireframe mode drawing
			if (drawWireFrame) DrawTriangle (outVertex[0], outVertex[1], outVertex[2], { 0, 1.0f, 0, 0 });
		} // travers all triangles
	}

	inline void Ndc2Screen (Vector4 &pos) { pos.x = (pos.x + 1)* 0.5f * width; pos.y = (pos.y + 1)* 0.5f * height; pos.z = pos.w; pos.w = 1.0f / pos.w;
	} // convert from normalized device coordinate to screen coordinate

	static inline bool BackFaceCulling (const Vector4 &p0, const Vector4 &p1, const Vector4 &p2) { return (p0.Dot ((p1 - p0).Cross (p2 - p0)) >= 0); }

	void FillTriangle (Model &model, const Vertex &v0, const Vertex &v1, const Vertex &v2) {
		auto PixelShader = [&model, this] (Vertex &v) -> Vector4 {
			auto ldir = (light.viewPos - v.viewPos).Normalize ();
			auto vnormal = v.normal.Normalize ();
			auto lambertian = std::max (0.0f, ldir.Dot (vnormal));
			auto specular = 0.0f;
			if (lambertian > 0) {
				auto viewDir = (-v.viewPos).Normalize ();
				auto half = (ldir + viewDir).Normalize ();
				auto angle = std::max (0.0f, half.Dot (vnormal));
				specular = std::pow (angle, 16.0f);
			}
			//return (light.specularColor * specular * model.material.ks);
			return (TextureLookup (model.material.texture, v.uv.x, v.uv.y) * (light.ambientColor * model.material.ka + light.diffuseColor * lambertian * model.material.kd) + light.specularColor * specular * model.material.ks);
		}; // blinn-phong shading.

		Vector4 weight = { 0, 0, 0, EdgeFunc (v0.pos, v1.pos, v2.pos) };
        int x0 = std::max (0, (int)std::floor (std::min (v0.pos.x, std::min (v1.pos.x, v2.pos.x))));
        int y0 = std::max (0, (int)std::floor (std::min (v0.pos.y, std::min (v1.pos.y, v2.pos.y))));
        int x1 = std::min (width - 1, (int)std::floor (std::max (v0.pos.x, std::max (v1.pos.x, v2.pos.x))));
        int y1 = std::min (height - 1, (int)std::floor (std::max (v0.pos.y, std::max (v1.pos.y, v2.pos.y))));
        for (int y = y0; y <= y1; y++) { //       only check for points that are inside the screen
            for (int x = x0; x <= x1; x++) { //   and inside the triangle bounding box
                Vertex v = { { x + 0.5f, y + 0.5f, 0 } };

				// v is outside the triangle
				if (TriangleCheck (v0, v1, v2, v, weight)) continue;

				// perspective correct interpolation
				Interpolate (v0, v1, v2, v, weight);

				// z test
                if (v.pos.z >= depthBuffer[x + y * width]) continue;

				// pixel shader needs to be run for every fragment of the triangle
				// and then write the result to frame/depth buffer
                DrawPoint (x, y, PixelShader (v), v.pos.z);
            }
        } // use edge function to draw triangle.
	} // fill triangle with color

	static bool TriangleCheck (const Vertex &v0, const Vertex &v1, const Vertex &v2, Vertex &v, Vector4 &w) {
		w.x = EdgeFunc (v1.pos, v2.pos, v.pos) * v0.pos.w / w.w; // pos.w == 1 / pos.z . we did that in Ndc2Screen()
		w.y = EdgeFunc (v2.pos, v0.pos, v.pos) * v1.pos.w / w.w;
		w.z = EdgeFunc (v0.pos, v1.pos, v.pos) * v2.pos.w / w.w;
		return (w.x < 0 || w.y < 0 || w.z < 0);
	} // return true if v is outside the triangle

	static inline float EdgeFunc (const Vector4 &p0, const Vector4 &p1, const Vector4 &p2) {
		return ((p2.x - p0.x) * (p1.y - p0.y) - (p2.y - p0.y) * (p1.x - p0.x));
	} // note that the result of edge function could be represent as area as well.

    static inline void Interpolate (const Vertex &v0, const Vertex &v1, const Vertex &v2, Vertex &v,const Vector4 &w) {
        v.pos.z = 1.0f / (w.x + w.y + w.z); // keep in maind that in TriangleCheck() we already done the (w = w * 1/z) part
		v.viewPos = (v0.viewPos * w.x + v1.viewPos * w.y + v2.viewPos * w.z) * v.pos.z;
		v.normal = (v0.normal * w.x + v1.normal * w.y + v2.normal * w.z) * v.pos.z;
		v.color = (v0.color * w.x + v1.color * w.y + v2.color * w.z) * v.pos.z;
		v.uv = (v0.uv * w.x + v1.uv * w.y + v2.uv * w.z) * v.pos.z;
    } // yes we interpolate all variables, no matter they are going to be used or not.

    static inline Vector4 TextureLookup (const Texture &texture, float s, float t) {
        Vector4 color = { 0.87f, 0.87f, 0.87f, 0 }; // default color
        if (!texture.data.empty ()) {
            s = Saturate (s), t = Saturate (t); // texture wrap
            color = BilinearFiltering (texture, s * (texture.width - 1), t * (texture.height - 1));
        }
        return color;
    } // get pixel color from texture

	static inline float Saturate (float n) { return std::min (1.0f, std::max (0.0f, n));
	} // clamp n to range [0.0, 1.0]

    static inline Vector4 BilinearFiltering (const Texture &texture, float s, float t) {
        if (s <= 0.5f || s >= texture.smax) return LinearFilteringV (texture, s, t);
        if (t <= 0.5f || t >= texture.tmax) return LinearFilteringH (texture, s, t);
        float supper = s + 0.5f, fs = std::floor(supper), ws = supper - fs, tupper = t + 0.5f, ts = std::floor(tupper), wt = tupper - ts;
        return (NearestNeighbor (texture, fs, ts) * ws * wt +
                NearestNeighbor (texture, fs, ts - 1.0f) * ws * (1.0f - wt) +
                NearestNeighbor (texture, fs - 1.0f, ts) * (1.0f - ws) * wt +
                NearestNeighbor (texture, fs - 1.0f, ts - 1.0f) * (1.0f - ws) * (1.0f - wt));
    } // Texture filtering : Bilinear filtering

    static inline Vector4 LinearFilteringH (const Texture &texture, float s, float t) {
        if (s <= 0.5f || s >= texture.smax) return NearestNeighbor (texture, s, t);
        float supper = s + 0.5f, fs = std::floor(supper), ws = supper - fs;
        return (NearestNeighbor (texture, fs, t) * ws + NearestNeighbor (texture, fs - 1.0f, t) * (1.0f - ws));
    } // Texture filtering : horizontal linear filtering

    static inline Vector4 LinearFilteringV (const Texture &texture, float s, float t) {
        if (t <= 0.5f || t >= texture.tmax) return NearestNeighbor (texture, s, t);
        float tupper = t + 0.5f, ts = std::floor(tupper), wt = tupper - ts;
        return (NearestNeighbor (texture, s, ts) * wt + NearestNeighbor (texture, s, ts - 1.0f) * (1.0f - wt));
    } // texture filtering : vertical linear filtering

    static inline Vector4 NearestNeighbor (const Texture &texture, float s, float t) {
        return texture.data[(int)std::round (s) + (int)std::round (t) * texture.width];
    } // texture filtering : nearest-neighbor interpolation

	void DrawTriangle (const Vertex &v0, const Vertex &v1, const Vertex &v2, const Vector4 &color) {
		DrawLine (v0.pos, v1.pos, color); DrawLine (v1.pos, v2.pos, color); DrawLine (v0.pos, v2.pos, color);
	} // draw the edges of a triangle

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
    } // bresenham line algorithm
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
    } // still bresenham line algorithm

    void DrawPoint (int x, int y, const Vector4 &color, float z) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            frameBuffer[x + y * width] = color; // write frame buffer
            depthBuffer[x + y * width] = z; // write z buffer
        }
    } // need to check the range everytime, a little bit waste ha?
};

int main () {
	// renderer setup
	const int WIDTH = 1024, HEIGHT = 768;
	Renderer renderer (WIDTH, HEIGHT);
	renderer.SetFrustum ((float)M_PI_2, (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
	renderer.SetCamera ({ 0.0f, 3.0f, 5.0f }, { 0.0f, 0.0f, 0.0f });
	renderer.SetLight ({ -10.0f, 30.0f, 30.0f }, { 0.5f, 0.0f, 0.0f, 0 }, { 0.8f, 0.8f, 0.8f, 0 }, { 0.5f, 0.5f, 0.5f, 0 });

	// Model (filepath, position, material)
	Model sphere ("res/sphere", { 2.5f, 0.5f, 1.5f }, { 0.1f, 1.0f, 0.5f });
    renderer.DrawModel (sphere, true, false);
	Model bunny ("res/bunny", { 0.0f, 0.0f, 0.0f }, { 0.1f, 0.8f, 0.7f });
	renderer.DrawModel (bunny, true, false);
	Model cube ("res/cube", { -2.0f, 0.0f, 2.0f }, { 0.3f, 0.8f, 0.8f });
	renderer.DrawModel (cube, true, false);
	Model cubeFrame ("res/cube", { 4.0f, 1.8f, -2.2f }, { 0.5f, 0.8f, 0.8f });
	renderer.DrawModel (cubeFrame, false, true);

	// save the frame buffer to a bmp file
	SaveBmp (renderer.frameBuffer, WIDTH, HEIGHT, "screenshot.bmp");
	return 0;
}
