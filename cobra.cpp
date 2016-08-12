#include <fstream>
#include <functional>
#include <sstream>
#include <string>
#include <vector>

struct Color { unsigned char b, g, r, a; };
struct Vector2 { float u, v; };
struct Vector3 { float x, y, z; };
struct Index { int pos, uv, normal; };
struct Index3 { Index index[3]; };
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
	Renderer () : frameBuffer (WIDTH * HEIGHT, { 255, 255, 255, 0 }), depthBuffer (WIDTH * HEIGHT, std::numeric_limits<float>::max ()) { }
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
	Renderer renderer;
	renderer.DrawModel (model);
	SaveBmp (renderer.frameBuffer, WIDTH, HEIGHT, "output.bmp");
	//Matrix4 mvp = CreateProjectMatrix (1.57f, 1024.0f / 768.0f, 0.1f, 1000.0f) * CreateViewMatrix ({4.0f, 3.0f, 3.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}) * CreateModelMatrix ();
	return 0;
}
