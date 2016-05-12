#include <iostream>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <random>
#include <vector>
using namespace std;

auto super_max = 0.0001;
auto super_min = -0.0001;

class Triangle {
       private:
	vector<vector<double>> points = {
	    {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}};

       public:
	Triangle();
	void render(const float ratio,
		    const tuple<vector<double>, vector<double>>);

	tuple<vector<double>, vector<double>> chooseNewOriginPoints();
	void setNewOriginPoints(const tuple<vector<double>, vector<double>>);
};

Triangle::Triangle(void) {}

void Triangle::render(
    const float ratio,
    const tuple<vector<double>, vector<double>> originPoints) {
	setNewOriginPoints(originPoints);

	random_device seed;
	mt19937_64 gen(seed());

	super_min =
	    min(min(min(*min_element(points[0].begin(), points[0].end()),
			*min_element(points[1].begin(), points[1].end())),
		    *min_element(points[2].begin(), points[2].end())),
		super_min);

	super_max =
	    max(max(max(*max_element(points[0].begin(), points[0].end()),
			*max_element(points[1].begin(), points[1].end())),
		    *max_element(points[2].begin(), points[2].end())),
		super_max);

	//  cout << super_min << " - " << super_max << endl;

	uniform_real_distribution<> dist(max(super_min - 0.001, -1.0),
					 min(super_max + 0.001, 1.0));
	points[2] = {dist(gen), dist(gen), dist(gen)};

	// cout << points[2][0] << "," << points[2][1] << "," << points[2][2] <<
	// endl;

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// glRotatef(glfwGetTime() * 50.f, 0.f, 0.f, 1.f);

	glBegin(GL_TRIANGLES);

	glVertex3f(points[0][0], points[0][1], points[0][2]);
	glVertex3f(points[1][0], points[1][1], points[1][2]);
	glVertex3f(points[2][0], points[2][1], points[2][2]);

	glVertex3f(points[2][0] + 0.1, points[2][1] + 0.1, points[2][2] + 0.1);

	glEnd();
}

tuple<vector<double>, vector<double>> Triangle::chooseNewOriginPoints() {
	random_device seed;
	mt19937_64 gen(seed());
	uniform_int_distribution<int> dist(0, 2);

	return make_tuple(points[dist(gen)], points[dist(gen)]);
}

void Triangle::setNewOriginPoints(
    const tuple<vector<double>, vector<double>> newOriginPoints) {
	points[0] = get<0>(newOriginPoints);
	points[1] = get<1>(newOriginPoints);
}

static void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action,
			 int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(void) {
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) exit(EXIT_FAILURE);
	window = glfwCreateWindow(1900, 1080, "Simple example", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, key_callback);

	Triangle triangles[99999];

	tuple<vector<double>, vector<double>> newOriginPoints{{0.0, 0.0, 0.0},
							      {0.0, 0.0, 0.0}};

	auto drawn = false;

	float rgb[3] = {0.0, 0.0, 0.0};

	glClear(GL_COLOR_BUFFER_BIT);

	auto width = 0;
	auto height = 0;
	glfwGetFramebufferSize(window, &width, &height);
	auto ratio = width / height;
	glViewport(0, 0, width, height);

	random_device seed;
	mt19937_64 gen(seed());
	uniform_real_distribution<> dist(0.00001, 0.1);

	while (!glfwWindowShouldClose(window)) {
		if (!drawn) {
			for (Triangle t : triangles) {
				glColor3f(rgb[0], rgb[1], rgb[2]);

				t.render(ratio, newOriginPoints);

				glfwSwapBuffers(window);
				glfwPollEvents();

				newOriginPoints = t.chooseNewOriginPoints();

				if (rgb[0] < 1) {
					rgb[0] += dist(gen);
				} else {
					rgb[0] = 0.0;
				}
				if (rgb[1] < 1) {
					rgb[1] += dist(gen);
				} else {
					rgb[1] = 0.0;
				}
				if (rgb[2] < 1) {
					rgb[2] += dist(gen);
				} else {
					rgb[2] = 0.0;
				}
			}

			drawn = true;
		}
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
