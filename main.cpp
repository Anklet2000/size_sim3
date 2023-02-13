#include<cstdlib>
#include<ctime>
#include"glut.h"
#include"size_sim3.h"

int main(int argc, char* argv[]) {
	srand(time(NULL));
	glutInitWindowSize(1960, 1080);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutCreateWindow(argv[0]);
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	GL_init();
	glutMainLoop();
}