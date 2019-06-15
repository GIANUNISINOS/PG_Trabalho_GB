#pragma once

class GameObject
{
public:
	SpriteSheet* sprites;
	VerticesObject* vertices;
	Transformations* transformations;
    bool *gameIsRunning;

    float width;
    float height;
    float speed;

	int tileCol;
	int tileRow;
    bool onCorrectTile;

	GameObject(SpriteSheet* spritesParam,float width, float height, float initialPosX, float initialPosY, float speedParam, bool *gameIsRunning,int tilePositionRow, int tilePositionCol) {
		sprites = spritesParam;
		speed = speedParam;

        this->width = width;
        this->height = height;
        this->gameIsRunning = gameIsRunning;

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

        vertices = new VerticesObject(verticesCoordinates, 20);
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

	virtual ~GameObject();
};

GameObject::~GameObject()
{
	delete vertices;
	delete transformations;
}
