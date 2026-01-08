#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <regex>

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct Point { int x, y; };


void createDefaultSVG(const string& filename) {
    ofstream out(filename);
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    out << "<svg width=\"500\" height=\"500\" height_param=\"120\" angle=\"70\">\n";
    out << "  \n";
    out << "  <line x1=\"100\" y1=\"400\" x2=\"400\" y2=\"100\" />\n";
    out << "</svg>";
    out.close();
    cout << "Create svg " << filename << endl;
}

class Canvas {
    int w, h;
    vector<int> pixels;
public:
    Canvas(int width, int height) : w(width), h(height), pixels(width* height, 0) {}

    
    void drawLine(int x1, int y1, int x2, int y2) {
        int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
        int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
        int err = dx + dy, e2;
        while (true) {
            if (x1 >= 0 && x1 < w && y1 >= 0 && y1 < h) pixels[y1 * w + x1] = 1;
            if (x1 == x2 && y1 == y2) break;
            e2 = 2 * err;
            if (e2 >= dy) { err += dy; x1 += sx; }
            if (e2 <= dx) { err += dx; y1 += sy; }
        }
    }

    void savePBM(const string& filename) {
        ofstream out(filename);
        out << "P1\n" << w << " " << h << "\n";
        for (int i = 0; i < w * h; ++i) {
            out << pixels[i] << (i % w == w - 1 ? "\n" : " ");
        }
        cout << "Result in: " << filename << endl;
    }
};

int main() {
    string inputPath = "input.svg";

    
    ifstream checkFile(inputPath);
    if (!checkFile.good()) {
        createDefaultSVG(inputPath);
    }
    checkFile.close();

    
    ifstream in(inputPath);
    string content((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());

    smatch m;
    int x1, y1, x2, y2, height_p;
    double angle_deg;

    regex line_re("x1=\"(\\d+)\" y1=\"(\\d+)\" x2=\"(\\d+)\" y2=\"(\\d+)\"");
    regex h_re("height_param=\"(\\d+)\"");
    regex a_re("angle=\"(\\d+)\"");

    if (regex_search(content, m, line_re)) {
        x1 = stoi(m[1]); y1 = stoi(m[2]);
        x2 = stoi(m[3]); y2 = stoi(m[4]);
    }
    if (regex_search(content, m, h_re)) height_p = stoi(m[1]);
    if (regex_search(content, m, a_re)) angle_deg = stod(m[1]);

    
    double rad = angle_deg * M_PI / 180.0;
    int dx = (int)(height_p / tan(rad));

    Point v1 = { x1, y1 };
    Point v2 = { x1 + dx, y1 - height_p };
    Point v3 = { x2, y2 };
    Point v4 = { x2 - dx, y2 + height_p };

    
    Canvas scene(600, 600);
    scene.drawLine(v1.x, v1.y, v2.x, v2.y);
    scene.drawLine(v2.x, v2.y, v3.x, v3.y);
    scene.drawLine(v3.x, v3.y, v4.x, v4.y);
    scene.drawLine(v4.x, v4.y, v1.x, v1.y);

   
    scene.savePBM("output.pbm");

    return 0;
}