#include <GL/glut.h>
#include <string.h>
#include<cmath>
void drawText(const char* text, float x, float y) {
    glRasterPos2f(x, y);
    for (int i = 0; i < strlen(text); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
}

float blinkProgress = 0.0f;
const int NUM_POSITIONS = 6;  // Updated to 6 for the new position 
const int NUM_CIRCLES = 6;    // Number of circles for frosting
float flameSize = 0.02f;      // Initial size of the flame
bool increasing = true;       // To toggle flame size for animation
int clickCount = 0; // Counter to track the number of clicks


// Array of square positions, including the new 6th position
GLfloat positions[NUM_POSITIONS][4][2] = {
    {{-0.95f, -0.3f}, {-0.75f, -0.3f}, {-0.75f, -0.06f}, {-0.95f, -0.06f}},
    {{-0.65f, -0.3f}, {-0.45f, -0.3f}, {-0.45f, -0.06f}, {-0.65f, -0.06f}},
    {{-0.35f, -0.3f}, {-0.15f, -0.3f}, {-0.15f, -0.06f}, {-0.35f, -0.06f}},
    {{-0.05f, -0.3f}, {0.15f, -0.3f}, {0.15f, -0.06f}, {-0.05f, -0.06f}},
    {{0.25f, -0.3f}, {0.45f, -0.3f}, {0.45f, -0.06f}, {0.25f, -0.06f}},
    {{0.52f, -0.3f}, {0.75f, -0.3f}, {0.75f, -0.06f}, {0.52f, -0.06f}}  // New position
};

int currentPosition = 0;

void drawEye(float x1, float y1, float x2, float y2, float x3, float y3) {
    glBegin(GL_TRIANGLES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);
    glEnd();
}

void drawEyes() {
    float blinkFactor = 1.0f - blinkProgress;  // Adjusts the height of the eyes

    // Left Eye
    drawEye(-0.81f, 0.55f * blinkFactor, -0.79f, 0.53f * blinkFactor, -0.83f, 0.53f * blinkFactor);

    // Right Eye
    drawEye(-0.73f, 0.55f * blinkFactor, -0.71f, 0.53f * blinkFactor, -0.75f, 0.53f * blinkFactor);
}

// Function to draw a circle
void drawCircle(GLfloat centerX, GLfloat centerY, GLfloat radius) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(centerX, centerY); // Center of the circle
    for (int i = 0; i <= 360; i += 15) {
        GLfloat angle = i * 3.14159f / 180.0f;
        GLfloat x = centerX + radius * cos(angle);
        GLfloat y = centerY + radius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

// Function to draw a cherry
void drawCherry(GLfloat centerX, GLfloat topY, GLfloat radius) {
    // Draw the red cherry
    glColor3f(1.0f, 0.0f, 0.0f); // Red for the cherry
    drawCircle(centerX, topY, radius);

    // Draw the green stem with tilt
    glColor3f(0.0f, 1.0f, 0.0f); // Green for the stem
    glBegin(GL_QUADS);
    GLfloat stemWidth = 0.02f;
    GLfloat stemHeight = 0.0f;
    glVertex2f(centerX - stemWidth / 2.0f, topY + radius);
    glVertex2f(centerX + stemWidth / 2.0f, topY + radius);
    glVertex2f(centerX + stemWidth / 2.0f + stemHeight * 0.7f, topY + radius + stemHeight);
    glVertex2f(centerX - stemWidth / 2.0f + stemHeight * 0.7f, topY + radius + stemHeight);
    glEnd();
}

// Function to draw a candle
void drawCandle(GLfloat leftX, GLfloat bottomY, GLfloat width, GLfloat height) {
    glColor3f(0.0f, 0.0f, 1.0f); // Blue for the candle
    glBegin(GL_QUADS);
    glVertex2f(leftX, bottomY);
    glVertex2f(leftX + width, bottomY);
    glVertex2f(leftX + width, bottomY + height);
    glVertex2f(leftX, bottomY + height);
    glEnd();
}

// Function to draw the flame
void drawFlame(GLfloat centerX, GLfloat topY, GLfloat size) {
    glColor3f(1.0f, 0.5f, 0.0f); // Orange for the flame
    glBegin(GL_TRIANGLES);
    glVertex2f(centerX, topY + size);       // Top of the flame
    glVertex2f(centerX - size / 2.0f, topY); // Bottom left of the flame
    glVertex2f(centerX + size / 2.0f, topY); // Bottom right of the flame
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0f, 0.0f, 0.0f); // Black
    glBegin(GL_QUADS);
    glVertex2f(-0.6f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-0.6f, 1.0f);
    glEnd();

    // 1st machine (icing)
    float x, y, angle;
    int i;
    int triangleAmount = 20; // Number of triangles used to draw the circle
    float radius = 0.1f;
    float twicePi = 2.0f * 3.14159f;
    // Hexagon
    glColor3f(0.0f, 0.5f, 0.5f); // Dark teal 
    glBegin(GL_POLYGON);
    glVertex2f(-0.37, 0.75);
    glVertex2f(-0.25, 0.63);
    glVertex2f(-0.13, 0.75);
    glVertex2f(-0.19, 0.87);
    glVertex2f(-0.31, 0.87);
    glEnd();
    // Circle
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.3f, 0.3f, 0.3f); // Dark grey 
    glVertex2f(-0.25f, 0.9f); // Center of the circle
    for (i = 0; i <= triangleAmount; i++) {
        angle = i * twicePi / triangleAmount;
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex2f(x + -0.25f, y + 0.9f);
    }
    glEnd();
    //Square
    glColor3f(0.0f, 0.5f, 0.5f); //  dark teal 
    glBegin(GL_QUADS);
    glVertex2f(-0.32f, 0.55f);
    glVertex2f(-0.175f, 0.55f);
    glVertex2f(-0.175f, 0.35f);
    glVertex2f(-0.32f, 0.35f);
    glEnd();
    //2nd circle
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow color
    glVertex2f(-0.25f, 0.55f);
    for (i = 0; i <= triangleAmount; i++) {
        angle = i * twicePi / triangleAmount;
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex2f(x + -0.25f, y + 0.59f);
    }
    glEnd();

    // 2nd machine (sprinkle and cherry)
    float shiftX = -0.05f; // Shift everything left

    // Rectangles
    glColor3f(1, 0, 0); // Red
    glBegin(GL_QUADS);
    glVertex2f(0.03f + shiftX, 0.8f);
    glVertex2f(0.07f + shiftX, 0.8f);
    glVertex2f(0.07f + shiftX, 1.0f);
    glVertex2f(0.03f + shiftX, 1.0f);
    glEnd();

    glColor3f(1, 0.8f, 0); // Darker yellow
    glBegin(GL_QUADS);
    glVertex2f(0.07f + shiftX, 0.8f);
    glVertex2f(0.11f + shiftX, 0.8f);
    glVertex2f(0.11f + shiftX, 1.1f);
    glVertex2f(0.07f + shiftX, 1.1f);
    glEnd();

    glColor3f(1, 0, 0); // Red
    glBegin(GL_QUADS);
    glVertex2f(0.11f + shiftX, 0.8f);
    glVertex2f(0.14f + shiftX, 0.8f);
    glVertex2f(0.14f + shiftX, 1.1f);
    glVertex2f(0.11f + shiftX, 1.1f);
    glEnd();

    // Circle below the rectangles
    glColor3f(0.0f, 0.0f, 0.5f); // Navy Blue
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.09f + shiftX, 0.75f); // Center of the circle
    float circleRadius = 0.09f; // Radius of the circle
    int circleTriangleAmount = 25; // Number of triangles used to draw the circle
    for (int i = 0; i <= circleTriangleAmount; i++) {
        float angle = i * 2.0f * 3.14159f / circleTriangleAmount;
        float x = circleRadius * cos(angle);
        float y = circleRadius * sin(angle);
        glVertex2f(x + 0.09f + shiftX, y + 0.75f);
    }
    glEnd();

    float smallCircleRadius = circleRadius * 3.0f / 8.0f;
    float firstSmallCircleY = 0.75f - circleRadius - smallCircleRadius;
    float secondSmallCircleY = firstSmallCircleY - 2 * smallCircleRadius;
    float thirdSmallCircleY = secondSmallCircleY - 2 * smallCircleRadius;
    // First small circle
    glColor3f(1, 0, 0); // Red
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.09f + shiftX, firstSmallCircleY);
    for (int i = 0; i <= circleTriangleAmount; i++) {
        float angle = i * 2.0f * 3.14159f / circleTriangleAmount;
        float x = smallCircleRadius * cos(angle);
        float y = smallCircleRadius * sin(angle);
        glVertex2f(x + 0.09f + shiftX, y + firstSmallCircleY);
    }
    glEnd();

    // Second small circle
    glColor3f(1, 0, 0); // Red
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.09f + shiftX, secondSmallCircleY);
    for (int i = 0; i <= circleTriangleAmount; i++) {
        float angle = i * 2.0f * 3.14159f / circleTriangleAmount;
        float x = smallCircleRadius * cos(angle);
        float y = smallCircleRadius * sin(angle);
        glVertex2f(x + 0.09f + shiftX, y + secondSmallCircleY);
    }
    glEnd();

    // Third small circle
    glColor3f(1, 0, 0); // Red
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.09f + shiftX, thirdSmallCircleY);
    for (int i = 0; i <= circleTriangleAmount; i++) {
        float angle = i * 2.0f * 3.14159f / circleTriangleAmount;
        float x = smallCircleRadius * cos(angle);
        float y = smallCircleRadius * sin(angle);
        glVertex2f(x + 0.09f + shiftX, y + thirdSmallCircleY);
    }
    glEnd();


    // 3rd machine (Candles)
    // Rectangles
   // Top red rectangle
    glColor3f(0.5f, 0.0f, 0.0f); // Wine
    glBegin(GL_QUADS);
    glVertex2f(0.29f, 0.8f);
    glVertex2f(0.34f, 0.8f);
    glVertex2f(0.34f, 1.2f);
    glVertex2f(0.29f, 1.2f);
    glEnd();
    // 1st wine color rectangle 
    glColor3f(2.5f, 1.2f, 0.2f); // yellow
    glBegin(GL_QUADS);
    glVertex2f(0.22f, 0.7f);
    glVertex2f(0.42f, 0.7f);
    glVertex2f(0.42f, 0.8f);
    glVertex2f(0.22f, 0.8f);
    glEnd();
    // 2nd wine color rectangle 
    glColor3f(0.5f, 0.0f, 0.0f); // Wine
    glBegin(GL_QUADS);
    glVertex2f(0.25f, 0.6f);
    glVertex2f(0.39f, 0.6f);
    glVertex2f(0.39f, 0.7f);
    glVertex2f(0.25f, 0.7f);
    glEnd();
    // 3rd wine color rectangle 
    glColor3f(2.5f, 1.2f, 0.2f); // yellow
    glBegin(GL_QUADS);
    glVertex2f(0.27f, 0.5f);
    glVertex2f(0.37f, 0.5f);
    glVertex2f(0.37f, 0.6f);
    glVertex2f(0.27f, 0.6f);
    glEnd();
    // Inverted triangle below the last rectangle
    glColor3f(0.5f, 0.0f, 0.0f); // Wine
    glBegin(GL_TRIANGLES);
    glVertex2f(0.27f, 0.5f);
    glVertex2f(0.37f, 0.5f);
    glVertex2f(0.32f, 0.45f);
    glEnd();


    //  Maroon color Character 
    glColor3f(0.5f, 0.0f, 0.0f); // Maroon 
    glBegin(GL_QUADS);
    glVertex2f(-1.1f, 0.0f);
    glVertex2f(-0.5f, 0.0f);
    glVertex2f(-0.5f, 1.0f);
    glVertex2f(-1.1f, 1.0f);
    glEnd();
    //1st line
    glColor3f(1.0f, 0.75f, 0.8f); // Baby Pink 
    glBegin(GL_QUADS);
    glVertex2f(-1.1f, 0.8f);
    glVertex2f(-0.5f, 0.8f);
    glVertex2f(-0.5f, 0.9f);
    glVertex2f(-1.1f, 0.9f);
    glEnd();
    //2nd line
    glColor3f(1.0f, 0.75f, 0.8f); // Baby Pink 
    glBegin(GL_QUADS);
    glVertex2f(-1.1f, 0.6f);
    glVertex2f(-0.5f, 0.6f);
    glVertex2f(-0.5f, 0.7f);
    glVertex2f(-1.1f, 0.7f);
    glEnd();
    //3rd line
    glColor3f(1.0f, 0.75f, 0.8f); // Baby Pink 
    glBegin(GL_QUADS);
    glVertex2f(-1.1f, 0.4f);
    glVertex2f(-0.5f, 0.4f);
    glVertex2f(-0.5f, 0.5f);
    glVertex2f(-1.1f, 0.5f);
    glEnd();
    //4th line
    glColor3f(1.0f, 0.75f, 0.8f); // Baby Pink 
    glBegin(GL_QUADS);
    glVertex2f(-1.1f, 0.2f);
    glVertex2f(-0.5f, 0.2f);
    glVertex2f(-0.5f, 0.3f);
    glVertex2f(-1.1f, 0.3f);
    glEnd();
    //5th line
    glColor3f(1.0f, 0.75f, 0.8f); // Baby Pink 
    glBegin(GL_QUADS);
    glVertex2f(-1.1f, 0.0f);
    glVertex2f(-0.5f, 0.0f);
    glVertex2f(-0.5f, 0.1f);
    glVertex2f(-1.1f, 0.1f);
    glEnd();
    glLineWidth(3.0f); // Set line width to 3.0f
    glColor3f(0.0f, 0.0f, 0.0f); // Black
    glBegin(GL_LINES);
    glVertex2f(-0.5f, 1);
    glVertex2f(-0.5f, 0.0f);
    glEnd();
    //1st hanging bar
    glColor3f(0.3f, 0.3f, 0.3f); // Dark grey 
    glBegin(GL_QUADS);
    glVertex2f(-0.65f, 0.85);
    glVertex2f(-0.6f, 0.85f);
    glVertex2f(-0.6f, 01.0f);
    glVertex2f(-0.65f, 01.0f);
    glEnd();
    //2nd hanging bar
    glColor3f(0.3f, 0.3f, 0.3f); // Dark grey 
    glBegin(GL_QUADS);
    glVertex2f(-0.95f, 01.0f);
    glVertex2f(-0.9f, 01.0f);
    glVertex2f(-0.9f, 0.85f);
    glVertex2f(-0.95f, 0.85);
    glEnd();
    //1st box
    glColor3f(0.3f, 0.3f, 0.3f); // Dark grey 
    glBegin(GL_QUADS);
    glVertex2f(-0.97f, 0.2f);
    glVertex2f(-0.57f, 0.2f);
    glVertex2f(-0.57f, 0.9f);
    glVertex2f(-0.97f, 0.9f);
    glEnd();
    //2nd inner box
    glColor3f(1.0f, 1.0f, 1.f); // white
    glBegin(GL_QUADS);
    glVertex2f(-0.94f, 0.25f);
    glVertex2f(-0.6f, 0.25f);
    glVertex2f(-0.6f, 0.85f);
    glVertex2f(-0.94f, 0.85f);
    glEnd();

    // Face (Square)
    glColor3f(1.0f, 0.8f, 0.6f); // Light skin tone
    glBegin(GL_QUADS);
    glVertex2f(-0.85f, 0.4f);  // Bottom-left corner
    glVertex2f(-0.70f, 0.4f);  // Bottom-right corner
    glVertex2f(-0.70f, 0.60f);  // Top-right corner
    glVertex2f(-0.85f, 0.60f);  // Top-left corner
    glEnd();
    // Neck (Rectangle)
    glColor3f(1.0f, 0.8f, 0.6f); // Light skin tone
    glBegin(GL_QUADS);
    glVertex2f(-0.8f, 0.25f);
    glVertex2f(-0.75f, 0.25f);
    glVertex2f(-0.75f, 0.4f);
    glVertex2f(-0.8f, 0.4f);
    glEnd();
    // Nose (Triangle)
    glColor3f(1.0f, 0.6f, 0.4f); // Darker skin tone
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.77f, 0.55f); // Top of the nose
    glVertex2f(-0.80f, 0.50f);
    glVertex2f(-0.74f, 0.50f);
    glEnd();
    // Mustache (Rectangles)
    glColor3f(0.2f, 0.1f, 0.0f); // Dark brown color for the mustache
    // Left part of the mustache
    glBegin(GL_QUADS);
    glVertex2f(-0.81f, 0.50f);  // Top-left
    glVertex2f(-0.77f, 0.50f);  // Top-right
    glVertex2f(-0.77f, 0.48f);  // Bottom-right
    glVertex2f(-0.81f, 0.48f);  // Bottom-left
    glEnd();
    // Right part of the mustache
    glBegin(GL_QUADS);
    glVertex2f(-0.77f, 0.50f);  // Top-left
    glVertex2f(-0.73f, 0.50f);  // Top-right
    glVertex2f(-0.73f, 0.48f);  // Bottom-right
    glVertex2f(-0.77f, 0.48f);  // Bottom-left
    glEnd();
    // Mouth (Rectangle)
    glColor3f(0.8f, 0.0f, 0.0f); // Dark red
    glBegin(GL_QUADS);
    glVertex2f(-0.8f, 0.47f);
    glVertex2f(-0.75f, 0.47f);
    glVertex2f(-0.75f, 0.43f);
    glVertex2f(-0.8f, 0.43f);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f); // White color for the eyes
    // Left Eye
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.81f, 0.55f);  // Top vertex
    glVertex2f(-0.79f, 0.53f);  // Bottom-left vertex
    glVertex2f(-0.83f, 0.53f);  // Bottom-right vertex
    glEnd();
    // Right Eye
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.73f, 0.55f);  // Top vertex
    glVertex2f(-0.71f, 0.53f);  // Bottom-left vertex
    glVertex2f(-0.75f, 0.53f);  // Bottom-right vertex
    glEnd();
    // Hat (Rectangle)
    glColor3f(0.0f, 0.0f, 0.0f); // Black color for the hat
    glBegin(GL_QUADS);
    glVertex2f(-0.85f, 0.60f);  // Bottom-left corner
    glVertex2f(-0.70f, 0.60f);  // Bottom-right corner
    glVertex2f(-0.70f, 0.70f);  // Top-right corner
    glVertex2f(-0.85f, 0.70f);  // Top-left corner
    glEnd();

    // Black tiles bg
    glColor3f(0.0f, 0.0f, 0.0f); //black
    glBegin(GL_QUADS);
    glVertex2f(0.67f, -0.175f);
    glVertex2f(0.8f, -0.175f);
    glVertex2f(0.8f, 0.0f);
    glVertex2f(0.7f, 0.0f);
    //2nd Black tile
    glVertex2f(0.93f, -0.175f);
    glVertex2f(1.0f, -0.175f);
    glVertex2f(1.0f, 0.0f);
    glVertex2f(0.93f, 0.0f);
    //3nd Black tile
    glVertex2f(0.8f, -0.35f);
    glVertex2f(0.93f, -0.35f);
    glVertex2f(0.93f, -0.175f);
    glVertex2f(0.8f, -0.175f);
    //3nd Black tile
    glVertex2f(0.65f, -0.175f);
    glVertex2f(0.93f, -0.35f);
    glVertex2f(0.93f, -0.175f);
    glVertex2f(0.8f, -0.175f);
    //4th Black tile
    glVertex2f(0.65f, -0.525f);
    glVertex2f(0.8f, -0.525f);
    glVertex2f(0.8f, -0.35f);
    glVertex2f(0.65f, -0.35f);
    //5th Black tile
    glVertex2f(0.8f, -0.525f);
    glVertex2f(1.0f, -0.525f);
    glVertex2f(1.0f, -0.35f);
    glVertex2f(0.93f, -0.35f);
    //6th Black tile
    glVertex2f(0.8f, -0.7f);
    glVertex2f(0.93f, -0.7f);
    glVertex2f(0.93f, -0.525f);
    glVertex2f(0.8f, -0.525f);
    //7th Black tile
    glVertex2f(0.65f, -0.875f);
    glVertex2f(0.8f, -0.875f);
    glVertex2f(0.8f, -0.7f);
    glVertex2f(0.65f, -0.7f);
    //8th Black tile
    glVertex2f(0.93f, -0.875f);
    glVertex2f(1.0f, -0.875f);
    glVertex2f(1.0f, -0.7f);
    glVertex2f(0.93f, -0.7f);
    //9th Black tile
    glVertex2f(0.35f, -0.875f);
    glVertex2f(0.5f, -0.875f);
    glVertex2f(0.5f, -0.7f);
    glVertex2f(0.35f, -0.7f);
    //10th Black tile
    glVertex2f(0.05f, -0.875f);
    glVertex2f(0.2f, -0.875f);
    glVertex2f(0.2f, -0.7f);
    glVertex2f(0.05f, -0.7f);
    //11th Black tile
    glVertex2f(-0.25f, -0.875f);
    glVertex2f(-0.1f, -0.875f);
    glVertex2f(-0.1f, -0.7f);
    glVertex2f(-0.25f, -0.7f);
    //12th black tile
    glVertex2f(-0.55f, -0.875f);
    glVertex2f(-0.4f, -0.875f);
    glVertex2f(-0.4f, -0.7f);
    glVertex2f(-0.55f, -0.7f);
    // 13th black tile
    glVertex2f(-0.85f, -0.875f);
    glVertex2f(-0.7f, -0.875f);
    glVertex2f(-0.7f, -0.7f);
    glVertex2f(-0.85f, -0.7f);
    glEnd();

    // White tiles Bg 
    glColor3f(1.0f, 1.0f, 1.0f); //white
    glBegin(GL_QUADS);
    glVertex2f(0.8f, -0.175f);
    glVertex2f(0.93f, -0.175f);
    glVertex2f(0.93f, 0.0f);
    glVertex2f(0.8f, 0.0f);
    //2nd white tile
    glVertex2f(0.65f, -0.35f);
    glVertex2f(0.8f, -0.35f);
    glVertex2f(0.8f, -0.175f);
    glVertex2f(0.67f, -0.175f);
    //3rd white tile
    glVertex2f(0.93f, -0.35f);
    glVertex2f(1.0f, -0.35f);
    glVertex2f(1.0f, -0.175f);
    glVertex2f(0.93f, -0.175f);
    //4th white tile
    glVertex2f(0.8f, -0.525f);
    glVertex2f(0.93f, -0.525f);
    glVertex2f(0.93f, -0.35f);
    glVertex2f(0.8f, -0.35f);
    //5th white tile
    glVertex2f(-1.1f, -0.7f);
    glVertex2f(0.8f, -0.7f);
    glVertex2f(0.8f, -0.525f);
    glVertex2f(0.65f, -0.525f);
    //6th white tile
    glVertex2f(0.93f, -0.7f);
    glVertex2f(1.0f, -0.7f);
    glVertex2f(1.0f, -0.525f);
    glVertex2f(0.93f, -0.525f);
    //7th white tile
    glVertex2f(0.8f, -0.875f);
    glVertex2f(0.93f, -0.875f);
    glVertex2f(0.93f, -0.7f);
    glVertex2f(0.8f, -0.7f);
    //8th white tile
    glVertex2f(0.5f, -0.875f);
    glVertex2f(0.65f, -0.875f);
    glVertex2f(0.65f, -0.7f);
    glVertex2f(0.5f, -0.7f);
    //9th white tile
    glVertex2f(0.2f, -0.875f);
    glVertex2f(0.35f, -0.875f);
    glVertex2f(0.35f, -0.7f);
    glVertex2f(0.2f, -0.7f);
    //10th white tile
    glVertex2f(-0.1f, -0.875f);
    glVertex2f(0.05f, -0.875f);
    glVertex2f(0.05f, -0.7f);
    glVertex2f(-0.1f, -0.7f);
    //11th white tile
    glVertex2f(-0.4f, -0.875f);
    glVertex2f(-0.25f, -0.875f);
    glVertex2f(-0.25f, -0.7f);
    glVertex2f(-0.4f, -0.7f);
    // 12th white tile
    glVertex2f(-0.7f, -0.875f);
    glVertex2f(-0.55f, -0.875f);
    glVertex2f(-0.55f, -0.7f);
    glVertex2f(-0.7f, -0.7f);
    // 13th white tile
    glVertex2f(-1.0f, -0.875f);
    glVertex2f(-0.85f, -0.875f);
    glVertex2f(-0.85f, -0.7f);
    glVertex2f(-1.0f, -0.7f);
    glEnd();


    // Gray Elevator 
    glColor3f(0.3f, 0.3f, 0.3f); // Dark grey 
    glBegin(GL_QUADS);
    glVertex2f(-1.1f, -0.35f);
    glVertex2f(0.79, -0.35f);
    glVertex2f(0.79f, 0.0f);
    glVertex2f(-1.1f, 0.0f);
    glEnd();
    //line border
    glColor3f(0.0f, 0.0f, 0.0f); // Black
    glBegin(GL_LINES);
    glVertex2f(0.79f, 0.0f);
    glVertex2f(0.79f, -0.35f);
    glEnd();
    glLineWidth(3.0f); // Set line width to 3.0f
    glColor3f(0.0f, 0.0f, 0.0f); // Black
    glBegin(GL_LINES);
    glVertex2f(1.0f, 0.0f);
    glVertex2f(-1.1f, 0.0f);
    glLineWidth(3.0f); // Set line width to 3.0f
    glBegin(GL_LINES);
    glVertex2f(-1.1f, -0.35f);
    glVertex2f(0.65f, -0.35f);
    glEnd();
    glLineWidth(3.0f);

    //line 1
    glColor3f(0.0f, 0.0f, 0.0f); // Black
    glBegin(GL_LINES);
    glVertex2f(-0.7f, 0.0f);
    glVertex2f(-0.7f, -0.35f);
    glEnd();
    //line 2
    glColor3f(0.0f, 0.0f, 0.0f); // Black
    glBegin(GL_LINES);
    glVertex2f(-0.4f, 0.0f);
    glVertex2f(-0.4f, -0.35f);
    glEnd();
    //line 3
    glColor3f(0.0f, 0.0f, 0.0f); // Black
    glBegin(GL_LINES);
    glVertex2f(-0.1f, 0.0f);
    glVertex2f(-0.1f, -0.35f);
    glEnd();
    //line 4
    glLineWidth(3.0f);
    glColor3f(0.0f, 0.0f, 0.0f); // Black
    glBegin(GL_LINES);
    glVertex2f(0.2f, 0.0f);
    glVertex2f(0.2f, -0.35f);
    glEnd();
    //line 5
    glLineWidth(3.0f);
    glColor3f(0.0f, 0.0f, 0.0f); // Black
    glBegin(GL_LINES);
    glVertex2f(0.5f, 0.0f);
    glVertex2f(0.5f, -0.35f);
    glEnd();

    //  Light grey Counter 
    glColor3f(0.8f, 0.8f, 0.8f); // Lighter Grey 
    glBegin(GL_QUADS);
    glVertex2f(-1.1f, -0.7f);
    glVertex2f(0.79f, -0.7f);
    glVertex2f(0.79f, -0.35f);
    glVertex2f(-1.1f, -0.35f);
    glEnd();
    //slant line border
    glColor3f(0.0f, 0.0f, 0.0f); // Black
    glBegin(GL_LINES);
    glVertex2f(0.79f, -0.7f);
    glVertex2f(0.79f, -0.35f);
    glEnd();
    //Border
    glLineWidth(3.0f); // Set line width to 3.0f
    glBegin(GL_LINES);
    glVertex2f(0.79f, -0.35f);
    glVertex2f(-1.1f, -0.35f);
    glLineWidth(3.0f); // Set line width to 3.0f
    glBegin(GL_LINES);
    glVertex2f(-1.1f, -0.7f);
    glVertex2f(0.79f, -0.7f);
    glEnd();

    
    // Draw the cake
    glColor3f(0.6f, 0.3f, 0.1f); // Brown cake
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
        glVertex2f(positions[currentPosition][i][0], positions[currentPosition][i][1]);
    }

    glEnd();

    if (currentPosition >= 2 && currentPosition <= 5) {
        GLfloat topY = positions[currentPosition][2][1]; // Y-coordinate of the top edge
        glColor3f(1.0f, 1.0f, 1.0f); // White for frosting
        GLfloat startX = positions[currentPosition][0][0] + 0.02f; // Start position for circles
        GLfloat spacing = (positions[currentPosition][1][0] - positions[currentPosition][0][0] - 0.04f) / (NUM_CIRCLES - 1); // Space between circles
        GLfloat radius = 0.02f; // Radius of the circles

        for (int i = 0; i < NUM_CIRCLES; i++) {
            drawCircle(startX + i * spacing, topY + 0.02f, radius); // Adjusted Y position for frosting
        }
    }

    // Draw the cherry on the 3rd, 4th, 5th, and 6th clicks
    if (currentPosition >= 2) {
        GLfloat topY = positions[currentPosition][2][1];
        GLfloat centerX = (positions[currentPosition][0][0] + positions[currentPosition][1][0]) / 2.0f;
        GLfloat cherryRadius = 0.03f; // Radius of the cherry

        if (currentPosition != 2) { // No cherry on the 3rd position
            drawCherry(centerX, topY + 0.02f + cherryRadius, cherryRadius); // Position cherry above the frosting
        }
    }

    // Draw candles and flames on the 4th, 5th, and 6th positions
    if (currentPosition >= 4) {
        GLfloat leftX1 = positions[currentPosition][0][0] + 0.01f; // Position left candle
        GLfloat bottomY1 = positions[currentPosition][0][1] + 0.27f;
        GLfloat width1 = 0.02f;
        GLfloat height1 = 0.2f;

        GLfloat leftX2 = positions[currentPosition][1][0] - 0.03f; // Position right candle
        GLfloat bottomY2 = positions[currentPosition][1][1] + 0.27f;
        GLfloat width2 = 0.02f;
        GLfloat height2 = 0.2f;

        // Draw candles
        drawCandle(leftX1, bottomY1, width1, height1);
        drawCandle(leftX2, bottomY2, width2, height2);

        // Draw flames
        drawFlame(leftX1 + width1 / 2.0f, bottomY1 + height1, flameSize);
        drawFlame(leftX2 + width2 / 2.0f, bottomY2 + height2, flameSize);
    }
    drawEyes();

    if (clickCount >= 1) {
        // Box with text 1
        glColor3f(1, 1, 1); // white
        glBegin(GL_POLYGON);
        glVertex2f(0.57f, .7f);
        glVertex2f(1.2f, 0.7f);
        glVertex2f(1.2f, 0.8f);
        glVertex2f(0.6f, 0.8f);
        glEnd();
        // Text
        glColor3f(0, 0, 0); // Black
        drawText("Cake Batch 01", 0.66, .75);
    }

    if (clickCount >= 2) {
        // Box with text 2
        glColor3f(1, 1, 1); // white
        glBegin(GL_POLYGON);
        glVertex2f(0.57f, .5f);
        glVertex2f(1.2f, 0.5f);
        glVertex2f(1.2f, 0.6f);
        glVertex2f(0.6f, 0.6f);
        glEnd();
        // Text
        glColor3f(0, 0, 0); // Black
        drawText("Customer - Saumya", 0.66, .55);
    }

    if (clickCount >= 3) {
        // Box with text 3
        glColor3f(1, 1, 1); // white
        glBegin(GL_POLYGON);
        glVertex2f(0.57f, .3f);
        glVertex2f(1.2f, 0.3f);
        glVertex2f(1.2f, 0.4f);
        glVertex2f(0.6f, 0.4f);
        glEnd();
        // Text
        glColor3f(0, 0, 0); // Black
        drawText("Cake Id - 500106331", 0.65, .35);
    }

    if (clickCount >= 4) {
        // Box with text 4
        glColor3f(1, 1, 1); // white
        glBegin(GL_POLYGON);
        glVertex2f(0.57f, .1f);
        glVertex2f(1.2f, 0.1f);
        glVertex2f(1.2f, 0.2f);
        glVertex2f(0.6f, 0.2f);
        glEnd();
        // Text
        glColor3f(0, 0, 0); // Black
        drawText("Assignment 02", 0.65, .15);
    }

    glutSwapBuffers();
}


void animateFlame(int value) {
    if (increasing) {
        flameSize += 0.002f;
        if (flameSize > 0.03f) increasing = false;
    }
    else {
        flameSize -= 0.002f;
        if (flameSize < 0.02f) increasing = true;
    }
    glutPostRedisplay();
    glutTimerFunc(50, animateFlame, 0);
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        currentPosition = (currentPosition + 1) % NUM_POSITIONS;
        clickCount++; // Increment the counter on each click
        glutPostRedisplay();
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1000, 700);
    glutCreateWindow("Moving Square");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutTimerFunc(50, animateFlame, 0); // Initialize flame animation
    glutMainLoop();
    return 0;
}
