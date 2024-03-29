#ifdef __APPLE__
    #include "header/Configurations.h"
    #include "header/Includes.h"
    #include "header/Shader.h"
    #include "header/Texture.h"
    #include "header/SpriteSheet.h"
    #include "header/VerticesObject.h"
	#include "header/Transformations.h"
	#include "header/TileClass.h"
	#include "header/Tilemap.h"
	#include "header/GameObject.h"
    #include "header/Car.h"
#elif _WIN64
    #include "../header/Configurations.h"
	#include "../header/Includes.h"
	#include "../header/Shader.h"
	#include "../header/Texture.h"
	#include "../header/SpriteSheet.h"
	#include "../header/VerticesObject.h"
	#include "../header/Transformations.h"
	#include "../header/TileClass.h"
	#include "../header/Tilemap.h"
	#include "../header/GameObject.h"
	#include "../header/Car.h"
#endif

// definicao de alguns atributos globais
Shader *shaderProgram;
GLFWwindow *window;

Tilemap *tilemap;

SpriteSheet* spritesCar;
Car *car;

SpriteSheet* spritesExplosion;
GameObject *explosion;

SpriteSheet* spritesGameOver;
GameObject *gameOver;

SpriteSheet* spritesGameWin;
GameObject *gameWin;

vector<SpriteSheet*> spritesFlags;
vector<GameObject *> flags;

int catchedFlags;
int actualVisibleFlag;
bool gameIsRunning = true;
double lastTimeCatchFlag;


//Atributos janela
const int WIDTH = ROWS*TILE_WIDTH;
const int HEIGHT = COLS*TILE_HEIGHT;

//Atributos janela
int RESIZED_WIDTH = WIDTH;
int RESIZED_HEIGHT = HEIGHT;

//teclas pressionadas
int keys[1024];

//Define acoes do redimensionamento da tela
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    RESIZED_WIDTH = width;
    RESIZED_HEIGHT = height;
}

/*
Define acoes do mouse
*/
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {
        double xPos, yPos;
        bool isClickValid;
        int rowClick,columnClick;

        glfwGetCursorPos(window, &xPos, &yPos);

        //Realiza a proporcao do clique para projecao original
        xPos = WIDTH * xPos / RESIZED_WIDTH;
        yPos = HEIGHT * yPos / RESIZED_HEIGHT;

        tilemap->mouseMap(isClickValid, rowClick, columnClick, xPos, yPos);

        if(isClickValid)
            car->clickReaction(rowClick, columnClick);
    }
}

GLFWwindow* createWindow() {
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Try to Catch Flags!", NULL, NULL);
    if (window == NULL) {
        printf("%s", "Houve uma falha ao criar a janela GLFW");
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    return window;
}

void createExplosionObject(){
    //cria objeto sprites de explosao
    spritesExplosion = new SpriteSheet("resource/objects/fire_icon.png",true, 1, 1, (float) Z_OUT_OF_SCREEN);

    //cria objeto esplosao
    explosion = new GameObject(
            spritesExplosion,
            (float)(TILE_WIDTH/2),(float)(TILE_HEIGHT),
            0.0f,0.0f,
            0.0f, &gameIsRunning,
            0, 0
    );
}

void createGameOverObject() {
	//cria objeto sprites do gameOver
	spritesGameOver = new SpriteSheet("resource/game_status/GAMELOST.png", true, 1, 1, (float)Z_OUT_OF_SCREEN);

	//cria objeto gameOver
	gameOver = new GameObject(
		spritesGameOver,
		WIDTH, HEIGHT,
		WIDTH/2.0F, HEIGHT/2.0f,
		0.0f, &gameIsRunning,
		0, 0
	);
}
void createGameWinObject() {
	//cria objeto sprites do gameWin
	spritesGameWin = new SpriteSheet("resource/game_status/GAMEWON.png", true, 1, 1, (float)Z_OUT_OF_SCREEN);

	//cria objeto gameWin
	gameWin = new GameObject(
		spritesGameWin,
		WIDTH, HEIGHT,
		WIDTH / 2.0F, HEIGHT / 2.0f,
		0.0f, &gameIsRunning,
		0, 0
	);
}


void createCarObject(){
    // cria objeto sprites de carros
    spritesCar = new SpriteSheet("resource/cars/4cars.png", true, 2, 2, (float) Z_CAR);
    spritesCar->setColumn(1);
    spritesCar->setRow(0);

    //cria objeto carro
    float xCarInitial;
    float yCarInitial;
    int tilePositionCarRow = 13;
    int tilePositionCarCol = 0;
    tilemap->calculoDesenhoDiamond(xCarInitial,yCarInitial,tilePositionCarRow,tilePositionCarCol);

	car = new Car (
			tilemap, keys,
            spritesCar,
            (float)(TILE_WIDTH/2),(float)(TILE_HEIGHT),
            xCarInitial+(TILE_WIDTH/2),yCarInitial+(TILE_HEIGHT/2),
            1.0f, &gameIsRunning,
            tilePositionCarRow, tilePositionCarCol
    );
}

void createFlagsObjects(){
    spritesFlags = {};
    flags = {};

    int tilePositionFlagRow[] = {2,3,11,11,2,7,5,7,13};
    int tilePositionFlagCol[] = {2,10,9,4,7,6,13,2,1};

    for (int i =0; i < NUMBER_OF_FLAGS; i++){
        //cria objeto sprites de flag
        spritesFlags.push_back(
                new SpriteSheet("resource/objects/flag.png",true, 1, 1, Z_OUT_OF_SCREEN)
        );

        //cria objetos flags
        float xFlagInitial;
        float yFlagInitial;
        tilemap->calculoDesenhoDiamond(xFlagInitial,yFlagInitial,tilePositionFlagRow[i],tilePositionFlagCol[i]);
        flags.push_back(
            new GameObject(
                    spritesFlags[i],
                    (float)(TILE_WIDTH/2),(float)(TILE_HEIGHT),
                    xFlagInitial+(TILE_WIDTH/2),yFlagInitial+(TILE_HEIGHT/2),
                    0.5f, &gameIsRunning,
                    tilePositionFlagRow[i], tilePositionFlagCol[i]
            )
        );
    }
}

void changeVisibleFlag(){
    lastTimeCatchFlag = glfwGetTime();
    int id = (rand()%NUMBER_OF_FLAGS);
    while(actualVisibleFlag == id)
        id = (rand()%NUMBER_OF_FLAGS);
    actualVisibleFlag = id;
    spritesFlags[id]->z = Z_FLAG;
}

void drawAllFlags(){
    for (int i =0; i < NUMBER_OF_FLAGS; i++) {
        flags[i]->draw(shaderProgram);
    }
}

void testCarColisionWithObjects(){
    if(car->testCollisionWithAnotherObject(flags[actualVisibleFlag])) {
                catchedFlags++;
                printf("\nVoce pegou %d bandeiras!\n",catchedFlags);
                flags[actualVisibleFlag]->sprites->z = Z_OUT_OF_SCREEN;
                //testa se ganhou o jogo

                if (catchedFlags == CATCH_N_FLAGS) {
                    printf("\nYou Won!\n");
                    gameIsRunning = false;
					gameWin->sprites->z = Z_STATUS;
                } else{
					float speedBonus = PROC_TYPE == 0 ? 1.0f : 0.6F;
                    if(catchedFlags%2 == 0) car->speed+= speedBonus;
                    changeVisibleFlag();
                }
    }
}

void deleteCreatedPointers(){
    if(spritesCar != nullptr)       delete spritesCar;
    if(car != nullptr)              delete car;
    if(spritesExplosion != nullptr) delete spritesExplosion;
    if(explosion != nullptr)        delete explosion;
    if(spritesGameOver != nullptr)  delete spritesGameOver;
    if(gameOver != nullptr)         delete gameOver;
    if(spritesGameWin != nullptr)   delete spritesGameWin;
    if(gameWin != nullptr)          delete gameWin;
    if(spritesFlags.size()>0)
        for (int i =0; i < NUMBER_OF_FLAGS; i++)
            if(spritesFlags[i] != nullptr)  delete spritesFlags[i];

}

void rebootGame(){
    srand((unsigned) time(NULL));
    gameIsRunning = true;
    catchedFlags = 0;

    //deleta os objetos existentes
    deleteCreatedPointers();

    //cria o objeto carro
    createCarObject();

    //cria todas bandeiras
    createFlagsObjects();
    changeVisibleFlag();

    //cria explosao
    createExplosionObject();

	createGameOverObject();
	createGameWinObject();
}

void startGame(){
    //instancia do tilemap
    tilemap = new Tilemap();

    //cria objetos em suas posições iniciais
    rebootGame();
}

/*
	Controla que teclas estão pressionadas em um dado momento
*/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(action == GLFW_PRESS) keys[key] = 1;
    if(action == GLFW_RELEASE) keys[key] = 0;
    if(action == GLFW_PRESS && key == GLFW_KEY_ENTER) rebootGame();
    if(action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, true);
}


int main() {
	if (!glfwInit()) {
		fprintf(stderr, "ERRO: não é possivel iniciar GLFW3\n");
		return 1;
	}

	/* needs for Apple OS X */
    #ifdef __APPLE__
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #endif // !APPLE

	window = createWindow();

	// inicia manipulador da extensao GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	//criacao do shader
	shaderProgram = new Shader("shader/vertexShader.txt", "shader/fragmentShader.txt");

	// define shader para uso
	shaderProgram->UseProgramShaders();

	// habilita funcao de profundidade
	glEnable(GL_DEPTH_TEST);

	//Create SO callbacks
	// esta para quando clicar uma tecla
	glfwSetKeyCallback(window, key_callback);

	// esta para quando redimensionar a tela
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // esta para quando clicar com o mouse
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    //cria todos itens do jogo em suas posições iniciais
    startGame();

    double lastTimePrinted = glfwGetTime();

    // looping do main
	double startMoveTime = glfwGetTime();
	while (!glfwWindowShouldClose(window)) {

	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// glm projecao
		glm::mat4 projection = glm::ortho(0.0f, (float)WIDTH, (float)HEIGHT, 0.0f, -1.0f, 1.0f);
		
		glUniformMatrix4fv(
			glGetUniformLocation(shaderProgram->Program, "proj"), 1, GL_FALSE, glm::value_ptr(projection));

		//desenha tilemap
        tilemap->draw(shaderProgram);

        //desenha carro
        car->draw(shaderProgram);

        //desenha all flags
        drawAllFlags();

        double currentSeconds = glfwGetTime();

        if(gameIsRunning) {
            //testa colisao do carro com Objetos
            testCarColisionWithObjects();

            //testa colisao com o mapa
            if (car->isDead) {
                printf("\nYou Died!\n");
                explosion->transformations->move(car->transformations->xCenter, car->transformations->yCenter);
                explosion->sprites->z = Z_EXPLOSION;
				gameOver->sprites->z = Z_STATUS;
                gameIsRunning = false;
            }

            // testa se o tempo para pegar a proxima bandeira esgotou
            double elapsedSecondsFlag = currentSeconds - lastTimeCatchFlag;
            if (elapsedSecondsFlag > TIME_CATCH_FLAGS) {
                car->isDead = true;
            }

            double elapsedSecondsPrint = currentSeconds - lastTimePrinted;
            if(elapsedSecondsPrint>2){
                printf("\nFaltam %d segundos!",(int)round(TIME_CATCH_FLAGS-elapsedSecondsFlag));
                lastTimePrinted = currentSeconds;
            }

        } else {
            float difX = car->transformations->xCenter - explosion->transformations->xCenter;
            float difY = car->transformations->yCenter - explosion->transformations->yCenter;
            explosion->transformations->move(difX,difY);

			//desenha explosao
			explosion->draw(shaderProgram);

			//desenha gameOver
			gameOver->draw(shaderProgram);

			//desenha gameWin
			gameWin->draw(shaderProgram);
        }


		float t = PROC_TYPE == 0 ? 0.001f : 0.01f;
        float moveTime = t / car->speed;

        double elapsedSecondsMove = currentSeconds - startMoveTime;
        if (elapsedSecondsMove > moveTime){
            car->movementIteration();
            startMoveTime = currentSeconds;
        }

        //fila eventos 
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
    // encerra contexto GL e outros recursos da GLFW
    glfwTerminate();

	//deleta os objetos criados nesta classe
	delete tilemap;
	deleteCreatedPointers();
	return 0;
}