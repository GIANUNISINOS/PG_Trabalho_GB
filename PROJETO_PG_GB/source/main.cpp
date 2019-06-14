#ifdef __APPLE__
    #include "header/Configurations.h";
    #include "header/Includes.h";
    #include "header/Shader.h";
    #include "header/Texture.h";
    #include "header/Layer.h";
    #include "header/SpriteSheet.h";
    #include "header/VerticesObject.h";
	#include "header/Transformations.h";
	#include "header/TileClass.h";
	#include "header/Tilemap.h";	
	#include "header/GameObject.h";
   
   
#elif _WIN64
    #include "../header/Configurations.h";
	#include "../header/Includes.h";
	#include "../header/Shader.h";
	#include "../header/Texture.h";
	#include "../header/Layer.h";
	#include "../header/SpriteSheet.h";
	#include "../header/VerticesObject.h";
	#include "../header/Transformations.h";
	#include "../header/TileClass.h";
	#include "../header/Tilemap.h";
	#include "../header/GameObject.h";
	#include "../header/Car.h";
	
#endif

// definicao de alguns atributos globais
Shader *shaderProgram;
GLFWwindow *window;
Tilemap *tilemap;
bool gameIsRunning = true;

SpriteSheet* spritesCar;
Car *car;

SpriteSheet* spritesFuel;
GameObject *fuel;

vector<SpriteSheet*> spritesFlags;
vector<GameObject *> flags;

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
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Try to survive thirty seconds!", NULL, NULL);
    if (window == NULL) {
        printf("%s", "Houve uma falha ao criar a janela GLFW");
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    return window;
}

void createCarObject(){
    //cria objeto sprites de carros
    spritesCar = new SpriteSheet("resource/cars/4cars.png",true, 2, 2, (float) Z_CAR);
    spritesCar->setColumn(1);
    spritesCar->setRow(0);

    //cria objeto carro
    float xCarInitial;
    float yCarInitial;
    int tilePositionCarRow = 13;
    int tilePositionCarCol = 0;
    tilemap->calculoDesenhoDiamond(xCarInitial,yCarInitial,tilePositionCarRow,tilePositionCarCol);

	car = new Car(
			tilemap, keys,
            spritesCar,
            (float)(TILE_WIDTH/2),(float)(TILE_HEIGHT),
            xCarInitial+(TILE_WIDTH/2),yCarInitial+(TILE_HEIGHT/2),
            0.5f, false,&gameIsRunning,
            tilePositionCarRow, tilePositionCarCol
    );
}

void createFuelObject(){
    //cria objeto sprites de fuel
    spritesFuel =new SpriteSheet("resource/objects/fuel.png",true, 1, 1, (float) Z_FUEL);

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
}

void createFlagsObjects(){
    int tilePositionFlagRow[] = {3,11,11,2,7};
    int tilePositionFlagCol[] = {10,9,4,7,6};

    for (int i =0; i < 5; i++){
        float z = (float) (Z_FLAG + (i * 0.01));

        //cria objeto sprites de flag
        spritesFlags.push_back(
                new SpriteSheet("resource/objects/flag.png",true, 1, 1, z)
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
                    0.5f, false, &gameIsRunning,
                    tilePositionFlagRow[i], tilePositionFlagCol[i]
            )
        );
    }
}

void drawAllFlags(){
    for (int i =0; i < 5; i++) {
        flags[i]->draw(shaderProgram);
    }
}

void testCarColisionWithObjects(){
    //testa se pegou o combustivel
    if(car->testCollisionWithAnotherObject(fuel)){
        fuel->sprites->z = Z_OUT_OF_SCREEN;
    }

    //testa se pegou flags
    for (int i = 0; i < 5; i++) {
        if(car->testCollisionWithAnotherObject(flags[i])) {
            flags[i]->sprites->z = Z_OUT_OF_SCREEN;
        }
    }
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

    //cria o objeto carro
    createCarObject();

    //cria objeto combustivel
    createFuelObject();

    //cria todas bandeiras
    createFlagsObjects();



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

        //desenha all flags
        drawAllFlags();

        //testa colisao do carro com Objetos
        testCarColisionWithObjects();

		//testa colisao com o mapa
		if (car->isDead) printf("\nIS DED\n");

		double currentSeconds = glfwGetTime();
		float speed = 0.05f;
		if (keys[GLFW_KEY_SPACE] == 1) speed = 0.01f;

		double elapsedSeconds = currentSeconds - previousFrameTime;
		if (elapsedSeconds > speed) {
			car->movementIteration();
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