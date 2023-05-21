#include <GL/freeglut.h>
#include <iostream>
#include <vector>
#include <cmath>

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Target circle parameters
const int TARGET_RADIUS = 50;
const int TARGET_DISPLAY_TIME = 1000; // milliseconds

// Player parameters
const int MAX_SCORE = 100;
const int SCORE_STEP = 10;

// Game variables
bool gameActive = false;
int score = 0;
int mouseX = 0;
int mouseY = 0;
int targetX = 0;
int targetY = 0;
bool targetHit = false;
bool gameEnded = false;

// Function to generate a random position for the target circle
void generateTargetPosition()
{
    targetX = rand() % (WINDOW_WIDTH - 2 * TARGET_RADIUS) + TARGET_RADIUS;
    targetY = rand() % (WINDOW_HEIGHT - 2 * TARGET_RADIUS) + TARGET_RADIUS;
}

// Function to check if the player clicked inside the target circle
bool checkTargetHit()
{
    int distanceSq = (mouseX - targetX) * (mouseX - targetX) + (mouseY - targetY) * (mouseY - targetY);
    return distanceSq <= TARGET_RADIUS * TARGET_RADIUS;
}

// Function to update the game state
void updateGame(int value)
{
    if (!gameActive)
    {
        gameActive = true;
        generateTargetPosition();
    }
    else
    {
        if (!targetHit)
        {
            // Player missed the target, end the game
            gameActive = false;
            gameEnded = true;
            std::cout << "Game over! Score: " << score << std::endl;
            return;
        }

        // Increment the score and limit it to the maximum score
        score += SCORE_STEP;
        if (score > MAX_SCORE)
            score = MAX_SCORE;

        targetHit = false;
    }

    generateTargetPosition();
    glutPostRedisplay();
    glutTimerFunc(TARGET_DISPLAY_TIME, updateGame, 0);
}

// Function to handle mouse click events
void mouseClick(int button, int state, int x, int y)
{
    if (gameActive && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        mouseX = x;
        mouseY = WINDOW_HEIGHT - y; // Invert the y-coordinate to match OpenGL coordinate system

        if (checkTargetHit())
        {
            targetHit = true;
        }
    }
}

// Function to draw the target circle and player's score
void drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (gameActive)
    {
        // Draw the target circle
        glColor3f(1.0f, 0.0f, 0.0f); // Red color
        glTranslatef(targetX, targetY, 0.0f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0.0f, 0.0f); // Center of the circle
        for (int i = 0; i <= 360; i++)
        {
            float angle = i * 3.14159f / 180.0f;
            float x = TARGET_RADIUS * cos(angle);
            float y = TARGET_RADIUS * sin(angle);
            glVertex2f(x, y);
        }
        glEnd();
        glTranslatef(-targetX, -targetY, 0.0f);

        // Draw the player's score
        glColor3f(1.0f, 1.0f, 1.0f); // White color
        glRasterPos2f(10.0f, WINDOW_HEIGHT - 30.0f);
        std::string scoreText = "Score: " + std::to_string(score);
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char *)scoreText.c_str());
    }

    glutSwapBuffers();
}

// Function to handle window resize events
void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
}

// Function to initialize OpenGL settings
void initializeOpenGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background color
}

int main(int argc, char **argv)
{
    // Initialize FreeGLUT and create the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Target Game");

    // Register callback functions
    glutDisplayFunc(drawScene);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouseClick);

    // Set up initial OpenGL settings
    initializeOpenGL();

    // Start the game
    updateGame(0);

    // Enter the FreeGLUT event loop
    glutMainLoop();

    return 0;
}
