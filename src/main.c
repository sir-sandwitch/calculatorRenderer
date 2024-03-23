#include <math.h>
#include <graphx.h>
#include <ti/getcsc.h>

#define PI 3.14159265
#define ORIGIN (-50)
#define DEG_TO_RAD(angle) (angle * PI / 180.0)
#define FOV_DEG (90.0)
#define ANG DEG_TO_RAD(FOV_DEG)
#define Z_MIN (-100)
#define Z_MAX (100)
#define X_MIN (-100)
#define X_MAX (100)
#define Y_MIN (-100)
#define Y_MAX (100)
#define RENDER_WIDTH (GFX_LCD_WIDTH)
#define RENDER_HEIGHT (GFX_LCD_HEIGHT)

// Define a point in 3D space
struct Point3D {
    double x, y, z;
};

// Define a face of a cube
struct Face {
    struct Point3D points[4];
    double avgZ;
};

// Define a cube
// Position: position of center of cube in 3D space
// Size: size of cube
// Rotation(x,y,z): rotation of cube in 3D space on each axis
// Faces: 6 faces of the cube
struct Cube {
    struct Point3D position;
    double size;
    double rotationx, rotationy, rotationz;
    struct Face faces[6];
    int color;
};

// Compare function for qsort
int compareFaces(const void* a, const void* b) {
    struct Face* faceA = (struct Face*) a;
    struct Face* faceB = (struct Face*) b;

    if (faceA->avgZ > faceB->avgZ) return -1;
    if (faceA->avgZ < faceB->avgZ) return 1;
    return 0;
}

// Render a cube using triangles
void renderCube(struct Cube *cube) {
    if(cube == NULL) return;
    if(cube->size <= 0) return;
    if(cube->color < 0 || cube->color > 255) return;
    if(cube->rotationx < 0 || cube->rotationx > 2 * PI) return;
    if(cube->rotationy < 0 || cube->rotationy > 2 * PI) return;
    if(cube->rotationz < 0 || cube->rotationz > 2 * PI) return;
    if(cube->position.x < X_MIN || cube->position.x > X_MAX) return;
    if(cube->position.y < Y_MIN || cube->position.y > Y_MAX) return;
    if(cube->position.z < Z_MIN || cube->position.z > Z_MAX) return;

    //normalize x and y position values to screen coordinates
    cube->position.x = (cube->position.x - X_MIN) * (RENDER_WIDTH / (X_MAX - X_MIN));
    cube->position.y = (cube->position.y - Y_MIN) * (RENDER_HEIGHT / (Y_MAX - Y_MIN));

    // Calculate the rotation matrices
    double rotationMatrixX[3][3] = {
        {1, 0, 0},
        {0, cos(cube->rotationx), -sin(cube->rotationx)},
        {0, sin(cube->rotationx), cos(cube->rotationx)}
    };

    double rotationMatrixY[3][3] = {
        {cos(cube->rotationy), 0, sin(cube->rotationy)},
        {0, 1, 0},
        {-sin(cube->rotationy), 0, cos(cube->rotationy)}
    };

    double rotationMatrixZ[3][3] = {
        {cos(cube->rotationz), -sin(cube->rotationz), 0},
        {sin(cube->rotationz), cos(cube->rotationz), 0},
        {0, 0, 1}
    };

    // Define the vertices of the cube
    cube->faces[0].points[0] = (struct Point3D){cube->position.x - cube->size / 2, cube->position.y - cube->size / 2, cube->position.z - cube->size / 2};
    cube->faces[0].points[1] = (struct Point3D){cube->position.x + cube->size / 2, cube->position.y - cube->size / 2, cube->position.z - cube->size / 2};
    cube->faces[0].points[2] = (struct Point3D){cube->position.x + cube->size / 2, cube->position.y + cube->size / 2, cube->position.z - cube->size / 2};
    cube->faces[0].points[3] = (struct Point3D){cube->position.x - cube->size / 2, cube->position.y + cube->size / 2, cube->position.z - cube->size / 2};
    cube->faces[1].points[0] = (struct Point3D){cube->position.x - cube->size / 2, cube->position.y - cube->size / 2, cube->position.z + cube->size / 2};
    cube->faces[1].points[1] = (struct Point3D){cube->position.x + cube->size / 2, cube->position.y - cube->size / 2, cube->position.z + cube->size / 2};
    cube->faces[1].points[2] = (struct Point3D){cube->position.x + cube->size / 2, cube->position.y + cube->size / 2, cube->position.z + cube->size / 2};
    cube->faces[1].points[3] = (struct Point3D){cube->position.x - cube->size / 2, cube->position.y + cube->size / 2, cube->position.z + cube->size / 2};
    cube->faces[2].points[0] = (struct Point3D){cube->position.x - cube->size / 2, cube->position.y - cube->size / 2, cube->position.z - cube->size / 2};
    cube->faces[2].points[1] = (struct Point3D){cube->position.x - cube->size / 2, cube->position.y - cube->size / 2, cube->position.z + cube->size / 2};
    cube->faces[2].points[2] = (struct Point3D){cube->position.x - cube->size / 2, cube->position.y + cube->size / 2, cube->position.z + cube->size / 2};
    cube->faces[2].points[3] = (struct Point3D){cube->position.x - cube->size / 2, cube->position.y + cube->size / 2, cube->position.z - cube->size / 2};
    cube->faces[3].points[0] = (struct Point3D){cube->position.x + cube->size / 2, cube->position.y - cube->size / 2, cube->position.z - cube->size / 2};
    cube->faces[3].points[1] = (struct Point3D){cube->position.x + cube->size / 2, cube->position.y - cube->size / 2, cube->position.z + cube->size / 2};
    cube->faces[3].points[2] = (struct Point3D){cube->position.x + cube->size / 2, cube->position.y + cube->size / 2, cube->position.z + cube->size / 2};
    cube->faces[3].points[3] = (struct Point3D){cube->position.x + cube->size / 2, cube->position.y + cube->size / 2, cube->position.z - cube->size / 2};
    cube->faces[4].points[0] = (struct Point3D){cube->position.x - cube->size / 2, cube->position.y + cube->size / 2, cube->position.z - cube->size / 2};
    cube->faces[4].points[1] = (struct Point3D){cube->position.x + cube->size / 2, cube->position.y + cube->size / 2, cube->position.z - cube->size / 2};
    cube->faces[4].points[2] = (struct Point3D){cube->position.x + cube->size / 2, cube->position.y + cube->size / 2, cube->position.z + cube->size / 2};
    cube->faces[4].points[3] = (struct Point3D){cube->position.x - cube->size / 2, cube->position.y + cube->size / 2, cube->position.z + cube->size / 2};
    cube->faces[5].points[0] = (struct Point3D){cube->position.x - cube->size / 2, cube->position.y - cube->size / 2, cube->position.z - cube->size / 2};
    cube->faces[5].points[1] = (struct Point3D){cube->position.x + cube->size / 2, cube->position.y - cube->size / 2, cube->position.z - cube->size / 2};
    cube->faces[5].points[2] = (struct Point3D){cube->position.x + cube->size / 2, cube->position.y - cube->size / 2, cube->position.z + cube->size / 2};
    cube->faces[5].points[3] = (struct Point3D){cube->position.x - cube->size / 2, cube->position.y - cube->size / 2, cube->position.z + cube->size / 2};
    
    // Transform and project vertices
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 4; j++) {
            // Apply rotation matrices
            double x = cube->faces[i].points[j].x;
            double y = cube->faces[i].points[j].y;
            double z = cube->faces[i].points[j].z;
            cube->faces[i].points[j].x = rotationMatrixX[0][0] * x + rotationMatrixX[0][1] * y + rotationMatrixX[0][2] * z;
            cube->faces[i].points[j].y = rotationMatrixX[1][0] * x + rotationMatrixX[1][1] * y + rotationMatrixX[1][2] * z;
            cube->faces[i].points[j].z = rotationMatrixX[2][0] * x + rotationMatrixX[2][1] * y + rotationMatrixX[2][2] * z;
            x = cube->faces[i].points[j].x;
            y = cube->faces[i].points[j].y;
            z = cube->faces[i].points[j].z;
            cube->faces[i].points[j].x = rotationMatrixY[0][0] * x + rotationMatrixY[0][1] * y + rotationMatrixY[0][2] * z;
            cube->faces[i].points[j].y = rotationMatrixY[1][0] * x + rotationMatrixY[1][1] * y + rotationMatrixY[1][2] * z;
            cube->faces[i].points[j].z = rotationMatrixY[2][0] * x + rotationMatrixY[2][1] * y + rotationMatrixY[2][2] * z;
            x = cube->faces[i].points[j].x;
            y = cube->faces[i].points[j].y;
            z = cube->faces[i].points[j].z;
            cube->faces[i].points[j].x = rotationMatrixZ[0][0] * x + rotationMatrixZ[0][1] * y + rotationMatrixZ[0][2] * z;
            cube->faces[i].points[j].y = rotationMatrixZ[1][0] * x + rotationMatrixZ[1][1] * y + rotationMatrixZ[1][2] * z;
            cube->faces[i].points[j].z = rotationMatrixZ[2][0] * x + rotationMatrixZ[2][1] * y + rotationMatrixZ[2][2] * z;

            // Apply translation
            cube->faces[i].points[j].x += cube->position.x;
            cube->faces[i].points[j].y += cube->position.y;
            cube->faces[i].points[j].z += cube->position.z;

            // Project vertices
            double scale = 1 / (cube->faces[i].points[j].z / 100 + 1);
            cube->faces[i].points[j].x *= scale;
            cube->faces[i].points[j].y *= scale;
        
            // Translate to screen coordinates
            cube->faces[i].points[j].x += RENDER_WIDTH / 2;
            cube->faces[i].points[j].y += RENDER_HEIGHT / 2;

        }
    }

    // Calculate average Z value for each face
    for (int i = 0; i < 6; i++) {
        double avgZ = 0;
        for (int j = 0; j < 4; j++) {
            avgZ += cube->faces[i].points[j].z;
        }
        avgZ /= 4;
        cube->faces[i].avgZ = avgZ;
    }

    // Sort faces by average Z value
    qsort(cube->faces, 6, sizeof(struct Face), compareFaces);
    
    // Draw faces in sorted order
    for (int i = 0; i < 6; i++) {
        struct Face face = cube->faces[i];
        gfx_SetColor(cube->color);
        gfx_FillTriangle(face.points[0].x, face.points[0].y, face.points[1].x, face.points[1].y, face.points[2].x, face.points[2].y);
        gfx_FillTriangle(face.points[0].x, face.points[0].y, face.points[2].x, face.points[2].y, face.points[3].x, face.points[3].y);
    }
}

int main(){
    // Initialize the graphics
    gfx_Begin();
    gfx_SetDrawBuffer();

    // Create a cube
    struct Cube cube = {
        .position = {0, 0, 0},
        .size = 50,
        .rotationx = 0,
        .rotationy = 0,
        .rotationz = 0,
        .faces = {
            {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, 0},
            {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, 0},
            {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, 0},
            {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, 0},
            {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, 0},
            {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, 0}
        },
        .color = 7
    };

    // Main loop
    while (!os_GetCSC()) {
        // Rotate the cube
        cube.rotationx += 0.01;
        cube.rotationy += 0.01;
        cube.rotationz += 0.01;

        // Clear the screen
        gfx_FillScreen(0);

        // Render the cube
        renderCube(&cube);

        // Swap the buffer with the screen
        gfx_SwapDraw();
    }

    // End graphics drawing
    gfx_End();

    return 0;
}