#include <fstream>
#include <sstream>
#include <string>
#include <vector>

//struct Color { unsigned char b, g, r, a; };
struct Vector2 { float u, v; };
struct Vector3 { float x, y, z; };
struct Index { int vertex, uv, normal; };
struct Index3 { Index i[3]; };
//struct Vector4 { float x, y, z, w; }
//struct Vertex { float x, y, z, u, v; };

//struct renderer {
//    renderer (int size) {
//        frameBuffer.resize (size);
//        for (auto &px : frameBuffer) px = {255, 255, 255, 0};
//        depthBuffer.resize (size);
//        for (auto &d : depthBuffer) d = std::numeric_limits<float>::max();
//    }
//    void Draw () {
//    }
//    std::vector<Color> frameBuffer;
//    std::vector<float> depthBuffer;
//};
//
//void VertexShader (const Matrix4 &mvp, const Vertex &vertex, Vector4 &outPos) {
//    Vector4 pos = mvp * Vector4{ vertex.x, vertex.y, vertex.z, 1.0f };
//}
//
//void PixelShader (Color &outColor) {
//}

struct Model {
	std::vector<Vector3> vertexBuffer;
	std::vector<Vector3> normalBuffer;
	std::vector<Vector2> uvBuffer;
	std::vector<Index3> indexBuffer;
	Model (std::string str) {
		float x, y, z;
		char c;
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
				vertexBuffer.push_back ({ x, y, z });
			}
			else if (str[0] == 'f') {
				Index3 index;
				iss >> token >> index.i[0].vertex >> c >> index.i[0].uv >> c >> index.i[0].normal >> 
					index.i[1].vertex >> c >> index.i[1].uv >> c >> index.i[1].normal >> 
					index.i[2].vertex >> c >> index.i[2].uv >> c >> index.i[2].normal;
				indexBuffer.push_back (index);
			}
		}
	}
};


int main (int argc, char **argv) {
	Model model (argc > 1 ? argv[1] : "cube.obj");
    //Renderer renderer (1024, 768);
    //Model model = LoadModel ();
    //renderer.Bind (model);
    //Matrix4 mvp = CreateProjectMatrix (1.57f, 1024.0f / 768.0f, 0.1f, 1000.0f) * CreateViewMatrix ({4.0f, 3.0f, 3.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}) * CreateModelMatrix ();
    //renderer.Draw (mvp, VertexShader, PixelShader);
    //Save (renderer.frameBuffer, "a.bmp");
	return 0;
}
