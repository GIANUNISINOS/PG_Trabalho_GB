#pragma once

class Tilemap
{
public:
	int numRows, numCols;
    float tileWidth, tileHeight;
    float sum_tiles_heigth;

    glm::mat4 modelMatrix;

	SpriteSheet* tileset;
	VerticesObject* vertices;

    Tilemap(float totalWidth, float totalHeight, int numRows, int numCols);
	void draw(Shader *shaderProgram);
	void setupVertices(float width, float height);
	~Tilemap();
	
};

Tilemap::Tilemap(float tileWidth, float tileHeight, int numRows, int numCols)
{
    this->tileset = new SpriteSheet("resource/mapa/seasons_tiles - resize.png",true, 8, 12, -0.10f);
	//this->tileset = new SpriteSheet("resource/tileset.png",true, 64, 48, -0.10f);
    //this->tileset = new SpriteSheet("resource/warrior.png",true, 8, 2, -0.10f);
    //this->tileset = new SpriteSheet("resource/Terrain_Tileset.png",true, 6, 1, -0.10f);
	this->numRows = numRows;
	this->numCols = numCols;

    this->tileWidth = tileWidth;
    this->tileHeight = tileHeight;

    this->sum_tiles_heigth = tileHeight*numCols;
	this->modelMatrix = glm::mat4(1);

	this->setupVertices(tileWidth, tileHeight);
}

void Tilemap::setupVertices(float width, float height) {
	/*
		Comeca no zero canto 0 parte esquerda
	*/

    float xt = (1.0f/((float)tileset->columns));
    float yt = (1.0f/((float)tileset->rows));

	float verticesCoordinates[] = {
		// positions						        // texture coords
		width / 2.0f,   0.0f,           0.0f,		xt/2.0f ,    yt      ,  // TOP
		width ,         height / 2.0f,  0.0f,		0.0f    ,    yt/2.0f ,	// RIGHT
		width  / 2.0f,  height,         0.0f,		xt/2.0f ,    0.0f    ,    // BOTTOM
		0.0f,           height / 2.0f,  0.0f,		xt      ,    yt/2.0f    // LEFT
	};

	vertices = new VerticesObject(verticesCoordinates, 20);

}


void Tilemap::draw(Shader *shaderProgram) {
   int mapa[ROWS][COLS] = {
       {39,  9,  9, 37, 69, 48, 68, 20, 22, 39,  9,  9,  9, 12},
       {34, 20, 24, 24, 24, 48, 20, 22, 21, 21, 24, 24, 20, 32},
       {34, 24, 68, 68, 48, 48, 48, 48, 48, 48, 20, 24, 24, 32},
       {34, 24, 69, 48, 44, 23, 23, 21, 23, 48, 20, 20, 24, 32},
       {34, 45, 48, 51, 23, 44, 22, 23, 22, 48, 48, 48, 24, 32},
       {34, 24, 68, 51, 48, 48, 48, 48, 48, 48, 20, 48, 45, 32},
       {34, 24, 48, 48, 68, 68, 68, 48, 48, 69, 24, 48, 24, 32},
       {34, 44, 24, 48, 48, 48, 48, 48, 24, 24, 24, 48, 24, 42},
       {34, 24, 23, 48, 27, 27, 27, 27, 68, 24, 24, 48, 24, 24},
       {34, 24, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 68, 24},
       {34, 20, 48, 24, 24, 20, 27, 48, 48, 24, 24, 24, 27, 24},
       {34, 24, 48, 24, 20, 27, 27, 48, 48, 24, 69, 68, 27, 68},
       {34, 20, 68, 24, 20, 20, 27, 68, 48, 48, 48, 48, 48, 48},
       {36, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 43, 68, 48}
   };

// matriz de cima tranposta
//    int mapa[ROWS][COLS] = {
//            {39, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 36},
//            {9,  20, 24, 24, 45, 24, 24, 44, 24, 24, 20, 24, 20, 35},
//            {9,  24, 68, 69, 48, 68, 48, 24, 23, 48, 48, 48, 68, 35},
//            {37, 24, 68, 48, 51, 51, 48, 48, 48, 48, 24, 24, 24, 35},
//            {69, 24, 48, 44, 23, 48, 68, 48, 27, 48, 24, 20, 20, 35},
//            {48, 48, 48, 23, 44, 48, 68, 48, 27, 48, 20, 27, 20, 35},
//            {68, 20, 48, 23, 22, 48, 68, 48, 27, 48, 27, 27, 27, 35},
//            {20, 22, 48, 21, 23, 48, 48, 48, 27, 48, 48, 48, 68, 35},
//            {22, 21, 48, 23, 22, 48, 48, 24, 68, 48, 48, 48, 48, 35},
//            {39, 21, 48, 48, 48, 48, 69, 24, 24, 48, 24, 24, 48, 35},
//            {9,  24, 20, 20, 48, 20, 24, 24, 24, 48, 24, 69, 48, 35},
//            {9,  24, 24, 20, 48, 48, 48, 48, 48, 48, 24, 68, 48, 43},
//            {9,  20, 24, 24, 24, 45, 24, 24, 24, 68, 27, 27, 48, 68},
//            {12, 32, 32, 32, 32, 32, 32, 42, 24, 24, 24, 68, 48, 48}
//    };

    // Define shaderProgram como o shader a ser utilizado
    shaderProgram->UseProgramShaders();

    // Define qual textura sera desenhada pelo shader
    tileset->texture->bind(shaderProgram);

    // Define em quais vertices sera desenhado pelo shader
    vertices->bind(shaderProgram);

    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numCols; col++) {
            //linear
            //float xi = col*tileWidth;
            //float yi = row*tileHeight;

            //slidemap
            //float xi = col*tileWidth  + row *(tileWidth/2.0f) ;
            //float yi = row*tileHeight/2.0f ;

            //diamond map
            float xi = row * (tileWidth/2.0f) + col * (tileWidth/2.0f);
            float yi = (row/*-1*/) * (tileHeight/2.0f) - col * (tileHeight/2.0f) + sum_tiles_heigth/2.0f;

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
