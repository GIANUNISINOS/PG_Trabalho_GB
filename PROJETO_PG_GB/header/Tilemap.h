#pragma once
class Tilemap
{
public:
	int numRows, numCols;
    float tileWidth, tileHeight;

    glm::mat4 modelMatrix;

	SpriteSheet* tileset;
	VerticesObject* vertices;

    Tilemap(float totalWidth, float totalHeight, int numRows, int numCols);
	void draw(Shader *shaderProgram);
	void setupVertices(float width, float height);
	~Tilemap();
	
};

Tilemap::Tilemap(float totalWidth, float totalHeight, int numRows, int numCols)
{
	//this->tileset = new SpriteSheet("resource/tileset.png",true, 64, 48, -0.10f);
    this->tileset = new SpriteSheet("resource/warrior.png",true, 8, 2, -0.10f);
	this->numRows = numRows;
	this->numCols = numCols;

	this->tileWidth = totalWidth / (float)numRows;
	this->tileHeight = totalHeight / (float)numCols;

	this->modelMatrix = glm::mat4(1);

	this->setupVertices(tileWidth, tileHeight);
}

void Tilemap::setupVertices(float width, float height) {
	/*
		Comeca centralizado no zero
	*/
	float verticesCoordinates[] = {
		// positions						// texture coords
		-width / 2.0f, -height / 2.0f, 0.0f,		0.0f,						1.0f,									// top left
		-width / 2.0f, height / 2.0f,  0.0f,		0.0f,						1.00f - (1.00f / (float)tileset->rows),	// bottom left
		width  / 2.0f, height / 2.0f,  0.0f,		1.00f / (float)tileset->columns, 1.00f - (1.00f / (float)tileset->rows),	// bottom right
		width  / 2.0f, -height / 2.0f, 0.0f,		1.00f / (float)tileset->columns, 1.0f,									// top right
	};

	vertices = new VerticesObject(verticesCoordinates, 20);

}


void Tilemap::draw(Shader *shaderProgram) {
    int mapa[10][10] ={
            {0 ,1 ,2 ,3 ,4, 5, 0, 0, 0, 0},
            {0 ,0 ,0 ,0 ,0, 0, 0, 0, 0, 0},
            {0 ,0 ,0 ,0 ,0, 0, 0, 0, 0, 0},
            {0 ,0 ,0 ,1 ,1, 0, 0, 0, 0, 0},
            {0 ,0 ,0 ,0 ,0, 0, 0, 0, 0, 0},
            {0 ,0 ,0 ,0 ,0, 0, 0, 0, 0, 0},
            {0 ,0 ,0 ,0 ,0, 0, 0, 0, 0, 0},
            {6 ,7 ,8 ,9 ,0, 0, 0, 0, 0, 0},
            {0 ,0 ,0 ,0 ,0, 0, 0, 0, 0, 0},
            {0 ,0 ,0 ,0 ,0, 0, 0, 0, 0, 0}
    };

    // Define shaderProgram como o shader a ser utilizado
    shaderProgram->UseProgramShaders();

    // Define qual textura sera desenhada pelo shader
    tileset->texture->bind(shaderProgram);

    // Define em quais vertices sera desenhado pelo shader
    vertices->bind(shaderProgram);

    for (int row = 0; row < 10; row++) {
        for (int col = 0; col < 10; col++) {

            float xi = col*tileWidth  + tileWidth/2;
            float yi = row*tileHeight + tileHeight/2;

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(xi, yi, 0.0));

            //define aonde desenhar
            glUniformMatrix4fv(
               glGetUniformLocation(shaderProgram->Program, "matrix_OBJ"), 1,
                  GL_FALSE, glm::value_ptr(modelMatrix));

            //seleciona o tileset a ser desenhado
            int id_tile = mapa[row][col];
            tileset->setColumn((int)(id_tile % tileset->columns));
            tileset->setRow((int)(id_tile / tileset->columns));

            //passa o tileset selecionado para o shader
            tileset->passUniformsToShader(shaderProgram);

            // Define em quais vertices sera desenhado pelo shader
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
    }
}

Tilemap::~Tilemap()
{
	delete tileset;
}
