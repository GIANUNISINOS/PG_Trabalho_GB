#ifndef PROJETO_PG_GB_CONFIGURATIONS_H
#define PROJETO_PG_GB_CONFIGURATIONS_H

#define DEBUG  true

// tamanho do mapa
#define ROWS 14
#define COLS 14

//tamanho dos tiles
#define TILE_WIDTH  80
#define TILE_HEIGHT 40

//quantidade de bandeiras no jogo
#define NUMBER_OF_FLAGS 9
#define CATCH_N_FLAGS 8
#define TIME_CATCH_FLAGS 11

//define direcoes
#define DIRECTION_NO 1
#define DIRECTION_O 2
#define DIRECTION_SO 3
#define DIRECTION_N 4
#define DIRECTION_S 5
#define DIRECTION_NE 6
#define DIRECTION_E 7
#define DIRECTION_SE 8

//Camada de profundidade
#define Z_TILEMAP 0.50
#define Z_FLAG 0.51
#define Z_CAR 0.52
#define Z_STATUS 0.53
#define Z_EXPLOSION 0.54
#define Z_OUT_OF_SCREEN 2.00

#endif //PROJETO_PG_GB_CONFIGURATIONS_H
