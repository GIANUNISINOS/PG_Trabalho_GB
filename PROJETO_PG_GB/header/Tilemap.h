class Tile {
public:
    bool isVisible;
    bool isSelected;
    bool isWalking;
    bool isMortal;
    int textureId;

    //left point
    float Ax, Ay;
    //top point
    float Bx, By;
    //bottom point
    float Cx, Cy;
    //right point
    float Dx, Dy;

    Tile(){
        isVisible = true;
        isSelected = false;
    }

    Tile(float x0,float y0, int textureId, bool isWalking, bool isMortal) {
        this->isVisible = true;
        this->isSelected = false;
        this->textureId = textureId;
        this->isWalking = isWalking;
        this->isMortal = isMortal;

        //left point
        Ax = x0;
        Ay = y0 + TILE_HEIGHT / 2.0f;
        //top point
        Bx = x0 + TILE_WIDTH / 2.0f;
        By = y0;
        //bottom point
        Cx = x0 + TILE_WIDTH / 2.0f;
        Cy = y0 + TILE_HEIGHT;

        //right point
        Dx = x0 + TILE_WIDTH;
        Dy = y0 + TILE_HEIGHT / 2.0f;
    }

    void getXandYReference(float &x0,float &y0){
        x0 = Ax;
        y0 = Ay - (TILE_HEIGHT / 2.0f);
    }
};

class Tilemap
{
public:
	int numRows, numCols;
    float tileWidth, tileHeight;
    float sum_tiles_heigth;
    glm::mat4 modelMatrix;

	SpriteSheet* tileset;
	VerticesObject* vertices;

    Tile *matrixTiles[ROWS][COLS] = {};

    Tilemap(float tileWidth, float tileHeight, int numRows, int numCols ){
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

        this->createMatrixTiles();
    }

    void setupVertices(float width, float height) {
        /*
            Comeca no zero canto 0 parte esquerda
        */

        float xt = (1.0f/((float)tileset->columns));
        float yt = (1.0f/((float)tileset->rows));

        float verticesCoordinates[] = {
            // positions						        // texture coords
            width / 2.0f,   0.0f,           0.0f,		xt/2.0f ,    1.00f-0.0f     ,  // TOP point - bot tex
            width ,         height / 2.0f,  0.0f,		xt      ,    1.00f-yt/2.0f,	// RIGHT point - left tex
            width  / 2.0f,  height,         0.0f,		xt/2.0f ,    1.00f-yt     ,  // BOTTOM point - top tex
            0.0f,           height / 2.0f,  0.0f,		0.0f    ,    1.00f-yt/2.0f     // LEFT point - right tex
        };

        vertices = new VerticesObject(verticesCoordinates, 20);

    }

    void calculoDesenhoDiamond(float &x, float &y, int row, int col) {
        x = row * (this->tileWidth / 2.0f) + col * (this->tileWidth / 2.0f);
        y = row * (this->tileHeight / 2.0f) - col * (this->tileHeight / 2.0f) + this->sum_tiles_heigth / 2.0f -
            (this->tileHeight / 2.0f);
    }

    void calculoDesenhoSlideMap(float &x, float &y, int row, int col) {
        x = ((float) col) * tileWidth + ((float) row) * (tileWidth / 2.0f);
        y = ((float) row) * tileHeight / 2.0f;
    }

    void calculoDesenhoLinear(float &x, float &y, int row, int col) {
        x = col*tileWidth;
        y = row*tileHeight;
    }

    void calculoCliqueDiamond(float xPos, float yPos, int &row, int &col) {
        double x = (double) xPos;
        double y = ((double) yPos) - (((double) sum_tiles_heigth) / 2.0);
        double tw = (double) tileWidth;
        double th = (double) tileHeight;
        double rowClick = (((2.0 * y / th) + (2.0 * x / tw))) / 2.0;
        double columnClick = (2.0 * x / tw) - rowClick;

        row = (int) rowClick;
        col = (int) columnClick;
    }

    void calculoCliqueSlideMap(float xPos, float yPos, int &row, int &col) {
        row = (int) (yPos / (tileHeight/2.0));
        col = (int) ((xPos - (row * (tileWidth/2.0)))/tileWidth);
    }

    void calculoCliqueLinear(float xPos, float yPos, int &row, int &col) {
        row = yPos/tileHeight;
        col = xPos/tileWidth;
    }

    void createMatrixTiles() {
        //matrix do mapa
        int mapa[ROWS][COLS] = {
                {36, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 39},
                {35, 20, 24, 20, 24, 24, 44, 24, 24, 45, 24, 24, 20, 9},
                {35, 68, 48, 48, 48, 23, 24, 48, 68, 48, 69, 68, 24, 9},
                {35, 24, 24, 24, 48, 48, 48, 48, 48, 48, 48, 68, 24, 37},
                {35, 20, 20, 24, 48, 27, 48, 68, 48, 23, 44, 48, 24, 69},
                {35, 20, 27, 20, 48, 27, 48, 68, 48, 44, 23, 48, 48, 48},
                {35, 27, 27, 27, 48, 27, 48, 68, 48, 22, 23, 48, 20, 68},
                {35, 68, 48, 48, 48, 27, 48, 48, 48, 23, 21, 48, 22, 20},
                {35, 48, 48, 48, 48, 68, 24, 48, 48, 22, 23, 48, 21, 22},
                {35, 48, 24, 24, 48, 24, 24, 69, 48, 48, 48, 48, 21, 39},
                {35, 48, 69, 24, 48, 24, 24, 24, 20, 48, 20, 20, 24, 9},
                {43, 48, 68, 24, 48, 48, 48, 48, 48, 48, 20, 24, 24, 9},
                {68, 48, 27, 27, 68, 24, 24, 24, 45, 24, 24, 24, 20, 9},
                {48, 48, 68, 24, 24, 24, 42, 32, 32, 32, 32, 32, 32, 12}
        };

        for (int row = 0; row < numRows; row++) {
            for (int col = 0; col < numCols; col++) {
                //define onde o tile é desenhado
                float x0,y0;
                this->calculoDesenhoDiamond(x0,y0,row,col);

                //define a textura de cada tile
                int idTex = mapa[row][col];

                //define tile caminhavel
                bool isWalking = (idTex==48);

                //cria o tile
                Tile *t = new Tile(x0,y0,idTex,isWalking,!isWalking);
                matrixTiles[row][col] = t;
            }
        }
    }

    void draw(Shader *shaderProgram) {

        // Define shaderProgram como o shader a ser utilizado
        shaderProgram->UseProgramShaders();

        // Define qual textura sera desenhada pelo shader
        tileset->texture->bind(shaderProgram);

        // Define em quais vertices sera desenhado pelo shader
        vertices->bind(shaderProgram);

        for (int row = 0; row < numRows; row++) {
            for (int col = 0; col < numCols; col++) {
                Tile *t = matrixTiles[row][col];

                float xi;
                float yi;
                t->getXandYReference(xi,yi);

                modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(xi, yi, 0.0));

                //define aonde desenhar
                glUniformMatrix4fv(
                   glGetUniformLocation(shaderProgram->Program, "matrix_OBJ"), 1,
                      GL_FALSE, glm::value_ptr(modelMatrix));

                //seleciona o tileset a ser desenhado
                tileset->setColumn((int)(t->textureId % tileset->columns));
                tileset->setRow((int)(t->textureId / tileset->columns));

                //passa o tileset selecionado para o shader
                tileset->passUniformsToShader(shaderProgram);

                // Define em quais vertices sera desenhado pelo shader
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            }
        }
    }

    ~Tilemap()
    {
        delete tileset;
    }
};