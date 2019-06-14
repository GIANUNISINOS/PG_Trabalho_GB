class Tile {
public:
    bool isVisible;
    bool isSelected;
    bool isWalking;
    bool isMortal;
    int textureId;

    //left point
    float Ax, Ay;
    //top point
    float Bx, By;
    //bottom point
    float Cx, Cy;
    //right point
    float Dx, Dy;

    Tile(){
        isVisible = true;
        isSelected = false;
    }

    Tile(float x0,float y0, int textureId, bool isWalking, bool isMortal) {
        this->isVisible = true;
        this->isSelected = false;
        this->textureId = textureId;
        this->isWalking = isWalking;
        this->isMortal = isMortal;

        //left point
        Ax = x0;
        Ay = y0 + TILE_HEIGHT / 2.0f;
        //top point
        Bx = x0 + TILE_WIDTH / 2.0f;
        By = y0;
        //bottom point
        Cx = x0 + TILE_WIDTH / 2.0f;
        Cy = y0 + TILE_HEIGHT;

        //right point
        Dx = x0 + TILE_WIDTH;
        Dy = y0 + TILE_HEIGHT / 2.0f;
    }

    void getXandYReference(float &x0,float &y0){
        x0 = Ax;
        y0 = Ay - (TILE_HEIGHT / 2.0f);
    }
};