#pragma once

class Car :
	public GameObject
{
public:
	Tilemap* tilemap;
	int* keys;
	bool isDead = false;

	Car(Tilemap* tilemap, int* keys, SpriteSheet* spritesParam, float width, float height, float initialPosX, float initialPosY, float speedParam, bool *gameIsRunning, int tilePositionRow, int tilePositionCol)
		: GameObject(spritesParam, width, height, initialPosX, initialPosY, speedParam, gameIsRunning, tilePositionRow, tilePositionCol) {
	
		this->tilemap = tilemap;
		this->keys = keys;
	};

	bool testCollisionWithAnotherObject(GameObject *another) {

		bool sameX = this->tileCol == another->tileCol;
		bool sameY = this->tileRow == another->tileRow;

		return sameX && sameY;
	}

	void movementIteration() {
		/*
			Se o carro estiver fora da sua posicao final apos a solicitacao de movimento
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

		if (onCorrectTile && *gameIsRunning) this->keyboardReaction();
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

	void keyboardReaction() {
		if (keys[GLFW_KEY_DOWN] == 1) {
			this->moveToTile(DIRECTION_SE);
		}
		else if (keys[GLFW_KEY_UP] == 1) {
			this->moveToTile(DIRECTION_NO);
		}
		else if (keys[GLFW_KEY_RIGHT] == 1) {
			this->moveToTile(DIRECTION_NE);
		}
		else if (keys[GLFW_KEY_LEFT] == 1) {
			this->moveToTile(DIRECTION_SO);
		}
	}

	void clickReaction(int rowCliked, int colCliked) {

		if (!this->onCorrectTile || !*gameIsRunning) return;

		int diffRows = rowCliked - this->tileRow;
		int diffCols = colCliked - this->tileCol;

		if (abs(diffRows) > 1 || abs(diffCols) > 1)
			return;

		if (diffRows == -1 && diffCols == 0) { //esquerda cima
			this->moveToTile(DIRECTION_NO);
		}
		else if (diffRows == 0 && diffCols == 1) { //direita cima
			this->moveToTile(DIRECTION_NE);
		}
		else if (diffRows == 0 && diffCols == -1) { //esq baixo
			this->moveToTile(DIRECTION_SO);
		}
		else if (diffRows == 1 && diffCols == 0) { //direita baixo
			this->moveToTile(DIRECTION_SE);
		}
	}


	void moveToTile(int direction) {

		switch (direction) {
		case DIRECTION_SE: // clicar para baixo
			if (tileRow < (ROWS - 1)) {
				if (tilemap->matrixTiles[tileRow + 1][tileCol]->isMortal) {
					this->isDead = true;
				}
				if (tilemap->matrixTiles[tileRow + 1][tileCol]->isWalking){
					this->tileRow++;
					this->sprites->setRow(1);
					this->sprites->setColumn(1);

					this->transformations->move(2.0f, 1.0f);
				}
				
			}
			break;
		case DIRECTION_NO:// clicar para cima
			if (tileRow > 0) {
				if (tilemap->matrixTiles[tileRow - 1][tileCol]->isMortal) {
					this->isDead = true;
				}
				if (tilemap->matrixTiles[tileRow - 1][tileCol]->isWalking) {
					this->tileRow--;
					this->sprites->setRow(0);
					this->sprites->setColumn(0);

					this->transformations->move(-2.0f, -1.0f);
				}

			}
			break;
		case DIRECTION_NE:// clicar para direita
			if (tileCol < (COLS - 1)) {
				if (tilemap->matrixTiles[tileRow][tileCol + 1]->isMortal) {
					this->isDead = true;
				}
				if (tilemap->matrixTiles[tileRow][tileCol + 1]->isWalking) {
					this->tileCol++;
					this->sprites->setRow(0);
					this->sprites->setColumn(1);

					this->transformations->move(2.0f, -1.0f);
				}

			}
			break;
		case DIRECTION_SO:// clicar para esquerda
			if (tileCol > 0) {
				if (tilemap->matrixTiles[tileRow][tileCol - 1]->isMortal) {
					this->isDead = true;
				}
				if (tilemap->matrixTiles[tileRow][tileCol - 1]->isWalking) {
					this->tileCol--;
					this->sprites->setRow(1);
					this->sprites->setColumn(0);

					this->transformations->move(-2.0f, 1.0f);
				}
			}
			break;
		}

	}
	~Car() {};
};
