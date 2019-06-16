class Tilemap
{
public:
	float sum_tiles_heigth;
    glm::mat4 modelMatrix;

	SpriteSheet* tileset;
	VerticesObject* vertices;

    Tile *matrixTiles[ROWS][COLS] = {};

    Tilemap(){
        this->tileset = new SpriteSheet("resource/mapa/seasons_tiles - resize.png",true, 8, 12, (float) Z_TILEMAP);

        this->sum_tiles_heigth = TILE_HEIGHT*COLS;
        this->modelMatrix = glm::mat4(1);

        this->setupVertices(TILE_WIDTH, TILE_HEIGHT);

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
        x = row * (TILE_WIDTH / 2.0f) + col * (TILE_WIDTH / 2.0f);
        y = row * (TILE_HEIGHT / 2.0f) - col * (TILE_HEIGHT / 2.0f) + this->sum_tiles_heigth / 2.0f -
            (TILE_HEIGHT / 2.0f);
    }

    void calculoDesenhoSlideMap(float &x, float &y, int row, int col) {
        x = ((float) col) * TILE_WIDTH + ((float) row) * (TILE_WIDTH / 2.0f);
        y = ((float) row) * TILE_HEIGHT / 2.0f;
    }

    void calculoDesenhoLinear(float &x, float &y, int row, int col) {
        x = col*TILE_WIDTH;
        y = row*TILE_HEIGHT;
    }

    void calculoCliqueDiamond(float xPos, float yPos, int &row, int &col) {
        double x = (double) xPos;
        double y = ((double) yPos) - (((double) sum_tiles_heigth) / 2.0);
        double tw = (double) TILE_WIDTH;
        double th = (double) TILE_HEIGHT;
        double rowClick = (((2.0 * y / th) + (2.0 * x / tw))) / 2.0;
        double columnClick = (2.0 * x / tw) - rowClick;

        row = (int) rowClick;
        col = (int) columnClick;
    }

    void calculoCliqueSlideMap(float xPos, float yPos, int &row, int &col) {
        row = (int) (yPos / (TILE_HEIGHT/2.0));
        col = (int) ((xPos - (row * (TILE_WIDTH/2.0)))/TILE_WIDTH);
    }

    void calculoCliqueLinear(float xPos, float yPos, int &row, int &col) {
        row = yPos/TILE_HEIGHT;
        col = xPos/TILE_WIDTH;
    }

    void createMatrixTiles() {
        //matrix do mapa
        int mapa[ROWS][COLS] = {
                {36, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 39},
                {35, 20, 24, 20, 24, 24, 20, 20, 24, 45, 24, 24, 20, 9},
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

        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLS; col++) {
                //define onde o tile é desenhado
                float x0,y0;
                this->calculoDesenhoDiamond(x0,y0,row,col);

                //define a textura de cada tile
                int idTex = mapa[row][col];

                //define tile caminhavel
				int walkingIds[5] = { 48, 69, 21, 23, 22 }; //ATEÑCAO -> Se mudar o tamanho aqui, mude tambem no for abaixo
				int mortalIds[6] = { 68,69,20,21,23,22 };

				bool isWalking = false;
				bool isMortal = false;

				for (int i = 0; i < 5; i++)		if (idTex == walkingIds[i])		isWalking = true;
				for (int i = 0; i < 6; i++)		if (idTex == mortalIds[i])		isMortal = true;

                //cria o tile
                Tile *t = new Tile(x0,y0,idTex,isWalking,isMortal);
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

        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLS; col++) {
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



    bool testPointCollision(float RefenceX,float RefenceY, float Bx,float By, float Cx,float Cy, float Px, float Py){
        float ABx = Bx-RefenceX;
        float ABy = By-RefenceY;
        float ABmodule = sqrt(pow(ABx,2)+pow(ABy,2));

        float normalABx = ABx / ABmodule;
        float normalABy = ABy / ABmodule;

        float ACx = Cx-RefenceX;
        float ACy = Cy-RefenceY;
        float ACmodule = sqrt(pow(ACx,2)+pow(ACy,2));

        float normalACx = ACx / ACmodule;
        float normalACy = ACy / ACmodule;

        float APx = Px-RefenceX;
        float APy = Py-RefenceY;
        float APmodule = sqrt(pow(APx,2)+pow(APy,2));

        float normalAPx = APx / APmodule;
        float normalAPy = APy / APmodule;

        float theta = acos(normalABx * normalAPx + normalABy * normalAPy);
        float alpha = acos(normalABx * normalACx + normalABy * normalACy);
        float betha = acos(normalACx * normalAPx + normalACy * normalAPy);

        // bool collide = alpha == (theta + betha);
        bool collide = 0.001>abs(alpha - (theta + betha));
        return collide;
    }

    void mouseMap(bool &isClickValid,int &rowClick, int &columnClick, double xPos,double yPos) {
        isClickValid = false;

        //projecao do click
        this->calculoCliqueDiamond(xPos, yPos, rowClick, columnClick);

        if (rowClick < 0 || columnClick < 0 || columnClick >= COLS || rowClick >= ROWS)
            return;

        Tile *tile = matrixTiles[rowClick][columnClick];

        if (testPointCollision(tile->Ax, tile->Ay, tile->Bx, tile->By, tile->Cx, tile->Cy, xPos, yPos))
            isClickValid = true;

        if(isClickValid==true){
            // caso debug ativo printa linha e coluna clicada
            if (DEBUG) {
                printf("\nxPos: %f", xPos);
                printf("\nyPos: %f", yPos);
                printf("\nRow: %d", rowClick);
                printf("\nColumn: %d\n", columnClick);
                printf("\nleftPoint x %f", tile->Ax);
                printf("\nleftPoint y %f\n", tile->Ay);
                printf("\ntopPointX x %f", tile->Bx);
                printf("\ntopPointY y %f\n", tile->By);
                printf("\nbottomPointX x %f", tile->Cx);
                printf("\nbottomPointY y %f\n", tile->Cy);
                printf("\nrightPointX x %f", tile->Dx);
                printf("\nrightPointY y %f\n", tile->Dy);
            }
            if(DEBUG) printf("\nValid Clicked Row: %d", rowClick);
            if(DEBUG) printf("\nValid Clicked Column: %d\n", columnClick);
        }
    }



    ~Tilemap()
    {
        delete tileset;
    }
};