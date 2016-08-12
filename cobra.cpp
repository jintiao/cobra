#include <fstream>
#include <functional>
#include <sstream>
#include <string>
#include <vector>

struct Vector3 {
	float x, y, z;
	Vector3 operator- (const Vector3 &rhs) const { return{ x - rhs.x, y - rhs.y, z - rhs.z }; }
	Vector3 Cross (const Vector3 &rhs) const { return{ y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x }; }
	float Length () const { return sqrtf (x * x + y * y + z * z); }
	Vector3 Normalize () const { float rlen = 1.0f / Length (); return{ x * rlen, y * rlen, z * rlen }; };
};

struct Matrix4 {
	float m[4][4]; 
	Matrix4 () { memset (m, 0, sizeof (m)); };
	void Invert () {
		float tmp[12];
		Matrix4	temm = *this;
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
		float det = (temm.m[0][0]*m[0][0] + temm.m[1][0]*m[0][1] + temm.m[2][0]*m[0][2] + temm.m[3][0]*m[0][3]);
		float idet = 1.0f / det;
		m[0][0] *= idet; m[0][1] *= idet; m[0][2] *= idet; m[0][3] *= idet;
		m[1][0] *= idet; m[1][1] *= idet; m[1][2] *= idet; m[1][3] *= idet;
		m[2][0] *= idet; m[2][1] *= idet; m[2][2] *= idet; m[2][3] *= idet;
		m[3][0] *= idet; m[3][1] *= idet; m[3][2] *= idet; m[3][3] *= idet;
	}
	static Matrix4 LookAt (const Vector3 &eye, const Vector3 &at, const Vector3 &up) {
		Vector3 dir = (eye - at).Normalize ();
		Vector3 left = up.Cross (dir).Normalize ();
		Vector3 newUp = dir.Cross (left);
		Matrix4 t;
		t.m[0][0] = left.x; t.m[1][0] = left.y; t.m[2][0] = left.z; t.m[3][0] = 0.0f;
		t.m[0][1] = newUp.x; t.m[1][1] = newUp.y; t.m[2][1] = newUp.z; t.m[3][2] = 0.0f;
		t.m[0][2] = dir.x; t.m[1][2] = dir.y; t.m[2][2] = dir.z; t.m[3][2] = 0.0f;
		t.m[0][3] = eye.x; t.m[1][3] = eye.y; t.m[2][3] = eye.z; t.m[3][3] = 1.0f;
		t.Invert ();
		return t;
	}
};

struct Color { unsigned char b, g, r, a; };
struct Index { int pos, uv, normal; };
struct Index3 { Index index[3]; };
struct Vector2 { float u, v; };
struct Vertex { Vector3 pos, normal; Vector2 uv; };


struct Model {
	std::vector<Vector3> posBuffer;
	std::vector<Vector3> normalBuffer;
	std::vector<Vector2> uvBuffer;
	std::vector<Index3> indexBuffer;
	Model (std::string str, char dummy = '/') {
		float x, y, z;
		std::ifstream is (str);
		while (std::getline (is, str)) {
			if (str.length () < 2) continue;
			std::istringstream iss (str);
			std::string token;
			if (str[1] == 't' && str[0] == 'v') {
				iss >> token >> x >> y;
				uvBuffer.push_back ({ x, y });
			}
			else if (str[1] == 'n' && str[0] == 'v') {
				iss >> token >> x >> y >> z;
				normalBuffer.push_back ({ x, y, z });
			}
			else if (str[0] == 'v') {
				iss >> token >> x >> y >> z;
				posBuffer.push_back ({ x, y, z });
			}
			else if (str[0] == 'f') {
				Index3 indexes;
				iss >> token >> indexes.index[0].pos >> dummy >> indexes.index[0].uv >> dummy >> indexes.index[0].normal >>
					indexes.index[1].pos >> dummy >> indexes.index[1].uv >> dummy >> indexes.index[1].normal >>
					indexes.index[2].pos >> dummy >> indexes.index[2].uv >> dummy >> indexes.index[2].normal;
				indexBuffer.push_back (indexes);
			}
		}
	}
};

const int WIDTH = 1024, HEIGHT = 768;
struct Renderer {
	std::vector<Color> frameBuffer;
	std::vector<float> depthBuffer;
	Matrix4 camMat;
	Matrix4 projMat;
	Renderer (const Vector3 &eye, const Vector3 &at, const Vector3 &up) : frameBuffer (WIDTH * HEIGHT, { 255, 255, 255, 0 }), depthBuffer (WIDTH * HEIGHT, std::numeric_limits<float>::max ()) {
		camMat = Matrix4::LookAt (eye, at, up);
	}
	void DrawModel (Model &model) {
		auto VertexShader = [] (const Vector3 &pos, const Vector3 &normal, const Vector2 &uv, Vertex &outVertex) {
			outVertex.pos = pos;
			outVertex.normal = normal;
			outVertex.uv = uv;
		};
		for (auto &indexes : model.indexBuffer) {
			Vertex ov[3];
			for (int i = 0; i < 3; i++) {
				VertexShader (model.posBuffer[indexes.index[i].pos - 1], model.normalBuffer[indexes.index[i].normal - 1], model.uvBuffer[indexes.index[i].uv - 1], ov[i]);
				DrawTriangle (ov[0], ov[1], ov[2]);
			}
		}
	}
	void DrawTriangle (const Vertex &v0, const Vertex &v1, const Vertex &v2) {
		DrawPoint (v0);
		DrawPoint (v1);
		DrawPoint (v2);
	}
	void DrawPoint (const Vertex &v) {
		auto PixelShader = [] (const Vector2 &uv, Color &outColor) {
			outColor = { 255, 0, 0, 0 };
		};
		Color c;
		int x = (int)(v.pos.x * WIDTH / 2.0f), y = (int)(v.pos.y * HEIGHT / 2.0f);
		int p = x + y * WIDTH;
		if (x >= 0 && y >= 0 && p < frameBuffer.size ()) {
			PixelShader (v.uv, c);
			frameBuffer[p] = c;
		}
	}
};

void SaveBmp (std::vector<Color> &frameBuffer, int width, int height, std::string file) {
#define INT2CHAR_BIT(num, bit) (char)(((num) >> (bit)) & 0xff)
#define INT2CHAR(num) INT2CHAR_BIT((num),0), INT2CHAR_BIT((num),8), INT2CHAR_BIT((num),16), INT2CHAR_BIT((num),24)
	const char header[54] = { 'B', 'M', INT2CHAR(54+width*height*sizeof(Color)), INT2CHAR(0), INT2CHAR(54), INT2CHAR(40), INT2CHAR(width), INT2CHAR (height), 1, 0, 32, 0 };
	std::ofstream ofs (file, std::ios_base::out | std::ios_base::binary);
	ofs.write (header, sizeof(header));
	ofs.write (static_cast<const char *>(static_cast<const void *>(frameBuffer.data ())), width*height * sizeof (Color));
}

int main (int argc, char **argv) {
	Model model (argc > 1 ? argv[1] : "cube.obj");
	Renderer renderer ({ 4.0f, 3.0f, 3.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
	renderer.DrawModel (model);
	SaveBmp (renderer.frameBuffer, WIDTH, HEIGHT, "output.bmp");
	//Matrix4 mvp = CreateProjectMatrix (1.57f, 1024.0f / 768.0f, 0.1f, 1000.0f) * CreateViewMatrix ({4.0f, 3.0f, 3.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}) * CreateModelMatrix ();
	return 0;
}
