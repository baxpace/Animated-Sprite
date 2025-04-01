
const int WALKING_ANIMATION_FRAMES = 4;
SDL_Rect gSpriteClipsDown[ WALKING_ANIMATION_FRAMES ];
SDL_Rect gSpriteClipsLeft[ WALKING_ANIMATION_FRAMES ];
SDL_Rect gSpriteClipsRight[ WALKING_ANIMATION_FRAMES ];
SDL_Rect gSpriteClipsUp[ WALKING_ANIMATION_FRAMES ];

void animateSriteDown();
void animateSriteLeft();
void animateSriteRight();
void animateSriteUp();

void animateSriteDown() {
    gSpriteClipsDown[ 0 ].x =   0;
    gSpriteClipsDown[ 0 ].y =   0;
    gSpriteClipsDown[ 0 ].w =  64;
    gSpriteClipsDown[ 0 ].h = 128;

    gSpriteClipsDown[ 1 ].x =  64;
    gSpriteClipsDown[ 1 ].y =   0;
    gSpriteClipsDown[ 1 ].w =  64;
    gSpriteClipsDown[ 1 ].h = 128;

    gSpriteClipsDown[ 2 ].x = 128;
    gSpriteClipsDown[ 2 ].y =   0;
    gSpriteClipsDown[ 2 ].w =  64;
    gSpriteClipsDown[ 2 ].h = 128;

    gSpriteClipsDown[ 3 ].x =  64;
    gSpriteClipsDown[ 3 ].y =   0;
    gSpriteClipsDown[ 3 ].w =  64;
    gSpriteClipsDown[ 3 ].h = 128;
}

void animateSriteLeft() {
    gSpriteClipsLeft[ 0 ].x =   0;
    gSpriteClipsLeft[ 0 ].y = 128;
    gSpriteClipsLeft[ 0 ].w =  64;
    gSpriteClipsLeft[ 0 ].h = 128;

    gSpriteClipsLeft[ 1 ].x =  64;
    gSpriteClipsLeft[ 1 ].y = 128;
    gSpriteClipsLeft[ 1 ].w =  64;
    gSpriteClipsLeft[ 1 ].h = 128;

    gSpriteClipsLeft[ 2 ].x = 128;
    gSpriteClipsLeft[ 2 ].y = 128;
    gSpriteClipsLeft[ 2 ].w =  64;
    gSpriteClipsLeft[ 2 ].h = 128;

    gSpriteClipsLeft[ 3 ].x =  64;
    gSpriteClipsLeft[ 3 ].y = 128;
    gSpriteClipsLeft[ 3 ].w =  64;
    gSpriteClipsLeft[ 3 ].h = 128;
}

void animateSriteRight() {
    gSpriteClipsRight[ 0 ].x =   0;
    gSpriteClipsRight[ 0 ].y = 256;
    gSpriteClipsRight[ 0 ].w =  64;
    gSpriteClipsRight[ 0 ].h = 128;

    gSpriteClipsRight[ 1 ].x =  64;
    gSpriteClipsRight[ 1 ].y = 256;
    gSpriteClipsRight[ 1 ].w =  64;
    gSpriteClipsRight[ 1 ].h = 128;

    gSpriteClipsRight[ 2 ].x = 128;
    gSpriteClipsRight[ 2 ].y = 256;
    gSpriteClipsRight[ 2 ].w =  64;
    gSpriteClipsRight[ 2 ].h = 128;

    gSpriteClipsRight[ 3 ].x =  64;
    gSpriteClipsRight[ 3 ].y = 256;
    gSpriteClipsRight[ 3 ].w =  64;
    gSpriteClipsRight[ 3 ].h = 128;
}

void animateSriteUp() {
    gSpriteClipsUp[ 0 ].x =   0;
    gSpriteClipsUp[ 0 ].y = 384;
    gSpriteClipsUp[ 0 ].w =  64;
    gSpriteClipsUp[ 0 ].h = 128;

    gSpriteClipsUp[ 1 ].x =  64;
    gSpriteClipsUp[ 1 ].y = 384;
    gSpriteClipsUp[ 1 ].w =  64;
    gSpriteClipsUp[ 1 ].h = 128;

    gSpriteClipsUp[ 2 ].x = 128;
    gSpriteClipsUp[ 2 ].y = 384;
    gSpriteClipsUp[ 2 ].w =  64;
    gSpriteClipsUp[ 2 ].h = 128;

    gSpriteClipsUp[ 3 ].x =  64;
    gSpriteClipsUp[ 3 ].y = 384;
    gSpriteClipsUp[ 3 ].w =  64;
    gSpriteClipsUp[ 3 ].h = 128;
}