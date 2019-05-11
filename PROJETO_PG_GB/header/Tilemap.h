#pragma once
class Tilemap
{
public:
	int numRows;
	int numCols;
	int tilesetRows;
	int tilesetCols;
	Texture* tileset;
	VerticesObject* vertices;
	
	Tilemap(string tilesetAddr, int tilesetRows, int tilesetCols, int totalWidth, int totalHeight, int numRows, int numCols);
	void draw();
	void setupVertices(int frames, int actions);
	~Tilemap();
	
};

Tilemap::Tilemap(string tilesetAddr, int tilesetRows, int tilesetCols, int totalWidth, int totalHeight, int numRows, int numCols)
{
	this->tileset = new Texture(tilesetAddr, true);
	this->numRows = numRows;
	this->numCols = numCols;
	this->tilesetCols = tilesetCols;
	this->tilesetRows = tilesetRows;

	this->setupVertices(totalWidth / (float)tilesetCols, totalHeight / (float)tilesetRows);
}
void Tilemap::setupVertices(int width, int height) {
	/*
		Comeca centralizado no zero
	*/
	float verticesCoordinates[] = {
		// positions						// texture coords
		-width / 2, -height / 2, 0.0f,		0.0f,						1.0f,									// top left
		-width / 2, height / 2,  0.0f,		0.0f,						1.00f - (1.00f / (float)tilesetRows),	// bottom left
		width  / 2, height / 2,  0.0f,		1.00f / (float)tilesetCols, 1.00f - (1.00f / (float)tilesetRows),	// bottom right
		width  / 2, -height / 2, 0.0f,		1.00f / (float)tilesetCols, 1.0f,									// top right
	};

	vertices = new VerticesObject(verticesCoordinates, 20);

}


void Tilemap::draw() {

}

Tilemap::~Tilemap()
{
	delete tileset;
}
