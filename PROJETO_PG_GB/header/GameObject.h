#ifndef PROJETO_PG_GA_GAMEOBJECT_H
	#define PROJETO_PG_GA_GAMEOBJECT_H

#pragma once
class GameObject
{
public:
	SpriteSheet* sprites;
	VerticesObject* vertices;
	Transformations* transformations;
	double previousFrameTime;

	float frameChangeSpeed;
	float speed;

    bool invertTextureX;

    float width;
    float height;
    bool *gameIsRunning;

	bool onCorrectTile;
	int tileCol;
	int tileRow;

	GameObject(SpriteSheet* spritesParam,float width, float height, float initialPosX, float initialPosY, float speedParam, bool invertX, bool *gameIsRunning,int tilePositionRow, int tilePositionCol) {
		sprites = spritesParam;
		previousFrameTime = glfwGetTime();

		speed = speedParam;

		frameChangeSpeed = 0.1;
        invertTextureX = invertX;
        this->width = width;
        this->height = height;
        this->gameIsRunning =gameIsRunning;

		setupVertices(sprites->columns, sprites->rows);


		this->tileCol = tilePositionCol;
		this->tileRow = tilePositionRow;
		//poe na pos inicial
		transformations = new Transformations(initialPosX, initialPosY);
	}

	/*
		rows e o numero de linhas da imagem de sprites
		columns  e o numero de colunas da imagem de sprites
	*/
	void setupVertices(int frames, int actions) {
		/*
			Comeca centralizado no zero
		*/
		float verticesCoordinates[] = {
			// positions						// texture coords
			-width/2,  -height/2, 0.0f,			0.0f,                1.0f,							 // top left
			-width/2,  height/2,  0.0f,			0.0f,	             1.00f - (1.00f/(float)actions), // bottom left
			width/2,   height/2,  0.0f,			1.00f/(float)frames, 1.00f - (1.00f/(float)actions), // bottom right
			width/2, - height/2,  0.0f,			1.00f/(float)frames, 1.0f,							 // top right
		};

        float verticesCoordinatesInvertedX[] = {
                // positions						// texture coords
                -width/2,  -height/2, 0.0f,			1.0f/(float)frames,   1.0f,							 // top left
                -width/2,  height/2,  0.0f,			1.00f/(float)frames, 1.00f - (1.00f/(float)actions), // bottom left
                width/2,   height/2,  0.0f,			0.0f,	             1.00f - (1.00f/(float)actions), // bottom right
                width/2, - height/2,  0.0f,			0.00f,               1.0f,							 // top right
        };


        if(invertTextureX){
            vertices = new VerticesObject(verticesCoordinatesInvertedX, 20);
        } else{
            vertices = new VerticesObject(verticesCoordinates, 20);
        }

	}

	void draw(Shader* shaderProgram) {
		// Define shaderProgram como o shader a ser utilizado
		shaderProgram->UseProgramShaders();

		glUniformMatrix4fv(
			glGetUniformLocation(shaderProgram->Program, "matrix_OBJ"), 1,
			GL_FALSE, glm::value_ptr(transformations->transformations));

		// Passa os offsets para o shader
		sprites->passUniformsToShader(shaderProgram);

		// Define qual textura sera desenhada pelo shader
		sprites->texture->bind(shaderProgram);

		// Define em quais vertices sera desenhado pelo shader
		vertices->bind(shaderProgram);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void changeFrame() {
		//Troca o sprite a 10 FPS
		double currentSeconds = glfwGetTime();
		double elapsedSeconds = currentSeconds - previousFrameTime;
		if (elapsedSeconds > frameChangeSpeed) {
			if (*gameIsRunning)
				sprites->nextColumn();
			previousFrameTime = currentSeconds;
		}
	}

    bool testCollisionWithAnotherObject(GameObject *another){
		
		bool sameX = this->tileCol == another->tileCol;
		bool sameY = this->tileRow == another->tileRow;

		return sameX && sameY;
    }

	void movementIteration(Tilemap* tilemap, int keys[1024]) {
		/*
			Se o carro estiver fora da sua posição final apos a solicitação de movimento
			move esse pouco a pouco
		*/
		//int correctRow, correctCol;
		//this->transformations->getPositionTile(correctRow, correctCol);

		float correctX, correctY;
		tilemap->calculoDesenhoDiamond(correctX, correctY, tileRow, tileCol);

		float actualX = this->transformations->xCenter - TILE_WIDTH / 2.0;
		float actualY = this->transformations->yCenter - TILE_HEIGHT / 2.0;

		float differenceX = correctX - actualX;
		float differenceY = correctY - actualY;

		onCorrectTile = (differenceX == 0 && differenceY == 0);

		if (onCorrectTile) this->keyboardReaction(tilemap, keys);
		else {
			float x = 0.0f, y = 0.0f;
			if (differenceX > 0) {
				x = 2.0f;
			}
			else if (differenceX < 0) {
				x = -2.0f;
			}
			if (differenceY > 0) {
				y = 1.0f;
			}
			else if (differenceY < 0) {
				y = -1.0f;
			}
			this->transformations->move(x, y);
		}
	}

	void keyboardReaction(Tilemap* tilemap, int* keys) {
		if (keys[GLFW_KEY_DOWN] == 1) {
			this->moveToTile(DIRECTION_SE, tilemap);
		}
		else if (keys[GLFW_KEY_UP] == 1) {
			this->moveToTile(DIRECTION_NO, tilemap);
		}
		else if (keys[GLFW_KEY_RIGHT] == 1) {
			this->moveToTile(DIRECTION_NE, tilemap);
		}
		else if (keys[GLFW_KEY_LEFT] == 1) {
			this->moveToTile(DIRECTION_SO, tilemap);
		}
	}

	void clickReaction(int rowCliked, int colCliked, Tilemap* tilemap) {
		
		if (this->onCorrectTile == false) return;

		int diffRows = rowCliked - this->tileRow;
		int diffCols = colCliked - this->tileCol;

		if (abs(diffRows) > 1 || abs(diffCols) > 1)
			return;

		if (diffRows == -1 && diffCols == 0) { //esquerda cima
			this->moveToTile(DIRECTION_NO, tilemap);
		}
		else if (diffRows == 0 && diffCols == 1) { //direita cima
			this->moveToTile(DIRECTION_NE, tilemap);
		}
		else if (diffRows == 0 && diffCols == -1) { //esq baixo
			this->moveToTile(DIRECTION_SO, tilemap);
		}
		else if (diffRows == 1 && diffCols == 0) { //direita baixo
			this->moveToTile(DIRECTION_SE, tilemap);
		}
	}


	void moveToTile(int direction, Tilemap* tilemap) {

		switch (direction) {
		case DIRECTION_SE: // clicar para baixo
			if (tileRow < (ROWS - 1) && tilemap->matrixTiles[tileRow + 1][tileCol]->isWalking) {
				this->tileRow++;
				this->sprites->setRow(1);
				this->sprites->setColumn(1);

				this->transformations->move(2.0f, 1.0f);

			
			}
			break;
		case DIRECTION_NO:// clicar para cima
			if (tileRow > 0 && tilemap->matrixTiles[tileRow - 1][tileCol]->isWalking) {
				this->tileRow--;
				this->sprites->setRow(0);
				this->sprites->setColumn(0);

				this->transformations->move(-2.0f, -1.0f);

		
			}
			break;
		case DIRECTION_NE:// clicar para direita
			if (tileCol < (COLS - 1) && tilemap->matrixTiles[tileRow][tileCol + 1]->isWalking) {
				this->tileCol++;
				this->sprites->setRow(0);
				this->sprites->setColumn(1);

				this->transformations->move(2.0f, -1.0f);

		
			}
			break;
		case DIRECTION_SO:// clicar para esquerda
			if (tileCol > 0 && tilemap->matrixTiles[tileRow][tileCol - 1]->isWalking) {
				this->tileCol--;
				this->sprites->setRow(1);
				this->sprites->setColumn(0);

				this->transformations->move(-2.0f, 1.0f);

			}
			break;
		}

	}

	virtual ~GameObject();
};

GameObject::~GameObject()
{
	delete vertices;
	delete transformations;
}

#endif //PROJETO_PG_GA_GAMEOBJECT_H