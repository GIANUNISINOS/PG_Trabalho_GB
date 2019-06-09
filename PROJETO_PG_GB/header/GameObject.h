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

		//poe na pos inicial
		transformations = new Transformations(initialPosX, initialPosY, tilePositionRow, tilePositionCol);
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

	virtual ~GameObject();
};

GameObject::~GameObject()
{
	delete vertices;
	delete transformations;
}

#endif //PROJETO_PG_GA_GAMEOBJECT_H