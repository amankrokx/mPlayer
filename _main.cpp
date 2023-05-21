#include <GL/freeglut.h>
#include <iostream>
#include <vector>
#include <cmath>

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Target circle parameters
const int TARGET_RADIUS = 50;
const int INNER_RADIUS = 20;
const int TARGET_DISPLAY_TIME = 3000; // milliseconds

// Player parameters
const int MAX_SCORE = 100;
const int SCORE_STEP = 10;

// Game variables
bool gameActive = false;
bool gameEnded = false;
bool startScreen = true;
int score = 0;
int mouseX = 0;
int mouseY = 0;
int targetX = 0;
int targetY = 0;
bool targetHit = false;

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
    if (gameActive)
    {
        if (!targetHit)
        {
            // Player missed the target, end the game
            gameActive = false;
            gameEnded = true;
            return;
        }

        // Calculate the score based on the distance from the center of the target
        int distance = sqrt((mouseX - targetX) * (mouseX - targetX) + (mouseY - targetY) * (mouseY - targetY));
        score += std::max(0, TARGET_RADIUS - distance) * SCORE_STEP;

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
    else if (gameEnded && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        // Check if the replay button was clicked
        int buttonX = WINDOW_WIDTH / 2 - 50;
        int buttonY = WINDOW_HEIGHT / 2 - 25;
        int buttonWidth = 100;
        int buttonHeight = 50;

        if (x >= buttonX && x <= buttonX + buttonWidth && y >= buttonY && y <= buttonY + buttonHeight)
        {
            // Restart the game
            gameActive = true;
            gameEnded = false;
            score = 0;
            generateTargetPosition();
            glutTimerFunc(TARGET_DISPLAY_TIME, updateGame, 0);
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

        // Draw the inner circle
        glColor3f(0.0f, 0.0f, 0.0f); // Black color
        glTranslatef(targetX, targetY, 0.0f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0.0f, 0.0f); // Center of the circle
        for (int i = 0; i <= 360; i++)
        {
            float angle = i * 3.14159f / 180.0f;
            float x = INNER_RADIUS * cos(angle);
            float y = INNER_RADIUS * sin(angle);
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
    else if (gameEnded)
    {
        // Draw the game over screen
        glColor3f(1.0f, 1.0f, 1.0f); // White color
        glRasterPos2f(WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 + 50);
        std::string gameOverText = "Game Over!";
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char *)gameOverText.c_str());

        // Draw the final score
        glRasterPos2f(WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2);
        std::string finalScoreText = "Score: " + std::to_string(score);
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char *)finalScoreText.c_str());

        // Draw the replay button
        glColor3f(0.0f, 1.0f, 0.0f); // Green color
        int buttonX = WINDOW_WIDTH / 2 - 50;
        int buttonY = WINDOW_HEIGHT / 2 - 25;
        int buttonWidth = 100;
        int buttonHeight = 50;
        glRectf(buttonX, buttonY, buttonX + buttonWidth, buttonY + buttonHeight);

        // Draw the replay button text
        glColor3f(0.0f, 0.0f, 0.0f); // Black color
        glRasterPos2f(buttonX + 30, buttonY + 20);
        std::string replayText = "Replay";
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char *)replayText.c_str());
    }
    else
    {
        // Draw the start screen
        glColor3f(1.0f, 1.0f, 1.0f); // White color
        glRasterPos2f(WINDOW_WIDTH / 2 - 70, WINDOW_HEIGHT / 2 + 50);
        std::string startText = "Click Start to Play!";
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char *)startText.c_str());

        // Draw the start button
        glColor3f(0.0f, 1.0f, 0.0f); // Green color
        int buttonX = WINDOW_WIDTH / 2 - 50;
        int buttonY = WINDOW_HEIGHT / 2 - 25;
        int buttonWidth = 100;
        int buttonHeight = 50;
        glRectf(buttonX, buttonY, buttonX + buttonWidth, buttonY + buttonHeight);

        // Draw the start button text
        glColor3f(0.0f, 0.0f, 0.0f); // Black color
        glRasterPos2f(buttonX + 25, buttonY + 20);
        std::string startButton = "Start";
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char *)startButton.c_str());
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

// Function to handle the start button click event
void startButtonClick(int button, int state, int x, int y)
{
    if (startScreen && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        // Start the game
        startScreen = false;
        gameActive = true;
        generateTargetPosition();
        glutTimerFunc(TARGET_DISPLAY_TIME, updateGame, 0);
    }
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
    glutMouseFunc(startButtonClick);

    // Set up initial OpenGL settings
    initializeOpenGL();

    // Enter the FreeGLUT event loop
    glutMainLoop();

    return 0;
}
