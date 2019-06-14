#define DEBUG  true

// tamanho do mapa
#define ROWS 14
#define COLS 14

//tamanho dos tiles
#define TILE_WIDTH  80
#define TILE_HEIGHT 40

//define direcoes
#define DIRECTION_NO 1
#define DIRECTION_O 2
#define DIRECTION_SO 3
#define DIRECTION_N 4
#define DIRECTION_S 5
#define DIRECTION_NE 6
#define DIRECTION_E 7
#define DIRECTION_SE 8

#ifdef __APPLE__
    #include "header/Includes.h";
    #include "header/Shader.h";
    #include "header/Texture.h";
    #include "header/Layer.h";
    #include "header/SpriteSheet.h";
    #include "header/VerticesObject.h";
	#include "header/Transformations.h";
	#include "header/GameObject.h";
    #include "header/Tile.h";
    #include "header/Tilemap.h";
#elif _WIN64
	#include "../header/Includes.h";
	#include "../header/Shader.h";
	#include "../header/Texture.h";
	#include "../header/Layer.h";
	#include "../header/SpriteSheet.h";
	#include "../header/VerticesObject.h";
	#include "../header/Transformations.h";
	#include "../header/GameObject.h";
	#include "../header/Tile.h";
	#include "../header/Tilemap.h";
#endif

Shader *shaderProgram;
GLFWwindow *window;
Tilemap *tilemap;
GameObject *car;
bool carIsStop = true;
GameObject *fuel;

bool gameIsRunning = true;

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
	Controla que teclas estão pressionadas em um dado momento
*/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(action == GLFW_PRESS) keys[key] = 1;
	if(action == GLFW_RELEASE) keys[key] = 0;
}

void printValues(int rowCar, int colCar) {
	float tileX, tileY;
	tilemap->calculoDesenhoDiamond(tileX, tileY, rowCar, colCar);
	printf("\n------------------------------------------------------------------------------------\n");
	printf("tileX: %5.5f, tileY: %5.5f, rowcar: %d, colCar: %d\n", tileX, tileY, rowCar, colCar);
	printf("carX: %5.5f, carY: %5.5f\n", car->transformations->xCenter - TILE_WIDTH/2.0, car->transformations->yCenter - TILE_HEIGHT/2.0);
	printf("------------------------------------------------------------------------------------\n");
}

void startCarMove(int a) {
    int rowCar, colCar;
    car->transformations->getPositionTile(rowCar, colCar);

    switch (a) {
        case DIRECTION_SE: // clicar para baixo
            if (rowCar < (ROWS - 1) && tilemap->matrixTiles[rowCar+1][colCar]->isWalking ) {
                car->transformations->tilePositionRow++;
                car->sprites->setRow(1);
                car->sprites->setColumn(1);

				car->transformations->move(2.0f, 1.0f);

				if(DEBUG) printValues(rowCar + 1, colCar);
			}
            break;
        case DIRECTION_NO:// clicar para cima
            if (rowCar > 0 && tilemap->matrixTiles[rowCar-1][colCar]->isWalking ) {
                car->transformations->tilePositionRow--;
                car->sprites->setRow(0);
                car->sprites->setColumn(0);
                
				car->transformations->move(-2.0f, -1.0f);

				if (DEBUG) printValues(rowCar - 1, colCar);
            }
            break;
        case DIRECTION_NE:// clicar para direita
            if (colCar < (COLS - 1) &&  tilemap->matrixTiles[rowCar][colCar+1]->isWalking ) {
                car->transformations->tilePositionCol++;
                car->sprites->setRow(0);
                car->sprites->setColumn(1);
                
				car->transformations->move(2.0f, -1.0f);

				if (DEBUG) printValues(rowCar, colCar + 1);
            }
            break;
        case DIRECTION_SO:// clicar para esquerda
            if (colCar > 0 &&  tilemap->matrixTiles[rowCar][colCar-1]->isWalking ) {
                car->transformations->tilePositionCol--;
                car->sprites->setRow(1);
                car->sprites->setColumn(0);
                
				car->transformations->move(-2.0f, 1.0f);
				
				if (DEBUG) printValues(rowCar, colCar - 1);
			}
            break;
    }

}

void keboardReaction() {
    if (keys[GLFW_KEY_DOWN] == 1) {
        startCarMove(DIRECTION_SE);
    }
    else if (keys[GLFW_KEY_UP] == 1) {
        startCarMove(DIRECTION_NO);
    }
    else if (keys[GLFW_KEY_RIGHT] == 1) {
        startCarMove(DIRECTION_NE);
    }
    else if (keys[GLFW_KEY_LEFT] == 1) {
        startCarMove(DIRECTION_SO);
    }
}

void carReaction() {
	/*
		Se o carro estiver fora da sua posição
	*/
    int rowCar, colCar;
    car->transformations->getPositionTile(rowCar, colCar);

	float correctX, correctY;
	tilemap->calculoDesenhoDiamond(correctX, correctY, rowCar, colCar);
	
	float actualX = car->transformations->xCenter - TILE_WIDTH / 2.0;
	float actualY = car->transformations->yCenter - TILE_HEIGHT / 2.0;

	float differenceX = correctX - actualX;
	float differenceY = correctY - actualY;

	carIsStop=(differenceX == 0 && differenceY == 0);

	if (carIsStop)
        keboardReaction();
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
		} else if (differenceY < 0) {
			y = -1.0f;
		}
		car->transformations->move(x, y);
	}
}

void clickReaction(int rowCliked, int colCliked){
    int colCar = car->transformations->tilePositionCol;
    int rowCar = car->transformations->tilePositionRow;

    int diffRows = rowCliked - rowCar;
    int diffCols = colCliked - colCar;

    if (abs(diffRows) > 1 || abs(diffCols) > 1)
        return;

    if(diffRows == -1 && diffCols == 0){ //esquerda cima
        startCarMove(DIRECTION_NO);
    } else if(diffRows == 0 && diffCols == 1) { //direita cima
        startCarMove(DIRECTION_NE);
    } else if(diffRows == 0 && diffCols == -1) { //esq baixo
        startCarMove(DIRECTION_SO);
    } else if(diffRows == 1 && diffCols == 0) { //direita baixo
        startCarMove(DIRECTION_SE);
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

void mouseMap(double xPos,double yPos) {
    bool isClickValid = false;
    int rowClick, columnClick;
    tilemap->calculoCliqueDiamond(xPos, yPos, rowClick, columnClick);

    if (rowClick < 0 || columnClick < 0 || columnClick >= COLS || rowClick >= ROWS)
        return;

    Tile *tile = tilemap->matrixTiles[rowClick][columnClick];




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
        if(carIsStop)
            clickReaction(rowClick, columnClick);
    }
}

/*
Define acoes do mouse
*/
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);

        //Realiza a proporcao do clique para projecao original
        xPos = WIDTH * xPos / RESIZED_WIDTH;
        yPos = HEIGHT * yPos / RESIZED_HEIGHT;

        mouseMap(xPos, yPos);
    }
}

GLFWwindow* createWindow() {
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Try to survive thirty seconds!", NULL, NULL);
    if (window == NULL) {
        printf("%s", "Houve uma falha ao criar a janela GLFW");
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    return window;
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

    //instancia do tilemap
    tilemap = new Tilemap(TILE_WIDTH, TILE_HEIGHT, ROWS, COLS);

    //cria objeto sprites de carros
    SpriteSheet* spritesCar =new SpriteSheet("resource/cars/4cars.png",true, 2, 2, 0.94f);
    spritesCar->setColumn(1);
    spritesCar->setRow(0);

    //cria objeto carro
    float xCarInitial;
    float yCarInitial;
    int tilePositionCarRow = 13;
    int tilePositionCarCol = 0;
    tilemap->calculoDesenhoDiamond(xCarInitial,yCarInitial,tilePositionCarRow,tilePositionCarCol);

    car = new GameObject(
            spritesCar,
            (float)(TILE_WIDTH/2),(float)(TILE_HEIGHT),
            xCarInitial+(TILE_WIDTH/2),yCarInitial+(TILE_HEIGHT/2),
            0.5f, false,&gameIsRunning,
            tilePositionCarRow, tilePositionCarCol
            );


    //cria objeto sprites de fuel
    SpriteSheet* spritesFuel =new SpriteSheet("resource/objects/fuel.png",true, 1, 1, 0.93f);
    spritesFuel->setColumn(0);
    spritesFuel->setRow(0);

    //cria objeto combustivel
    float xFuelInitial;
    float yFuelInitial;
    int tilePositionFuelRow = 2;
    int tilePositionFuelCol = 2;
    tilemap->calculoDesenhoDiamond(xFuelInitial,yFuelInitial,tilePositionFuelRow,tilePositionFuelCol);

    fuel = new GameObject(
            spritesFuel,
            (float)(TILE_WIDTH/2),(float)(TILE_HEIGHT),
            xFuelInitial+(TILE_WIDTH/2),yFuelInitial+(TILE_HEIGHT/2),
            0.5f, false,&gameIsRunning,
            tilePositionFuelRow, tilePositionFuelCol
    );

    // looping do main
	double previousFrameTime = glfwGetTime();
	while (!glfwWindowShouldClose(window)) {

	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// glm projecao
		glm::mat4 projection = glm::ortho(0.0f, (float)WIDTH, (float)HEIGHT, 0.0f, -1.0f, 1.0f);
		
		glUniformMatrix4fv(
			glGetUniformLocation(shaderProgram->Program, "proj"), 1, GL_FALSE, glm::value_ptr(projection));

		//desenha tilemap
        tilemap->draw(shaderProgram);

        //desenha
        car->draw(shaderProgram);

        //desenha combustivel
        fuel->draw(shaderProgram);

        //testa se pegou o combustivel
        if(car->testCollisionWithAnotherObject(fuel)){
            fuel->sprites->z = 2.00f;
        }

		double currentSeconds = glfwGetTime();
		float speed = 0.05f;
		if (keys[GLFW_KEY_SPACE] == 1) speed = 0.01f;

		double elapsedSeconds = currentSeconds - previousFrameTime;
		if (elapsedSeconds > speed) {
            carReaction();
			previousFrameTime = currentSeconds;
		}
        
        //fila eventos 
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
    // encerra contexto GL e outros recursos da GLFW
    glfwTerminate();

	delete spritesCar;
	delete spritesFuel;

    return 0;
}