#pragma once
class Transformations
{
public:
	float xCenter;
	float yCenter;

    int tilePositionRow;
    int tilePositionCol;

	glm::mat4 matrix_translaction;
	glm::mat4 matrix_rotation;
	glm::mat4 matrix_scala;
	glm::mat4 transformations;

	Transformations(float initialPosX, float initialPosY, int tilePositionRow, int tilePositionCol) {
		this->xCenter = 0.0f;
        this->yCenter = 0.0f;

        this->tilePositionRow = tilePositionRow;
        this->tilePositionCol = tilePositionCol;

        this->matrix_translaction = glm::mat4(1);
        this->matrix_rotation = glm::mat4(1);
        this->matrix_scala = glm::mat4(1);

        this->move(initialPosX, initialPosY);
        this->transform();
	};

	void move(float x, float y) {
		matrix_translaction = glm::translate(matrix_translaction, glm::vec3(x, y, 0.0f));
		xCenter += x;
		yCenter += y;
		transform();
	}

	void rotate(float rad) {
		matrix_rotation = glm::rotate(matrix_rotation, glm::radians( rad ), glm::vec3(0, 0, 1));
		transform();
	}

	void scale(float s) {
		matrix_scala = glm::scale(matrix_scala, glm::vec3( s, s, s));
		transform();
	}

	void transform() {
		transformations = matrix_translaction * matrix_rotation * matrix_scala;
	}

	void getPositionTile(int &row, int &col){
	    row = tilePositionRow;
        col = tilePositionCol;
	}


	~Transformations() {};
};

