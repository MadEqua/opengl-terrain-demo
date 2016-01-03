#include "TerrainApplication.h"

int main() {
	
	TerrainApplication app;
	if (app.initGLContext(2, true)) {
		app.initialize();
		app.startLoop();
	}

	return 0;
}