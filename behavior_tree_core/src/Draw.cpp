#include <behavior_tree_core/Draw.h>

using namespace BT;
ControlNode *tree;
const float NODE_WIDTH = 0.03;
const float NODE_HEIGHT = 0.02;
const float WORD_OFFSET = 0.02;
const float CHAR_DIM = 0.0075;
const int MAX_STRING_CHAR = 8;
bool draw = false;

void drawString(void *font, char *s, float x, float y, float z) {
  unsigned int i;
  glRasterPos3f(x, y, z);

  for (i = 0; i < 2; i++)
    glutBitmapCharacter(font, s[i]);
}

// draw the node itself using a solid square (color coded)

void renderBitmapString(float x, float y, void *font, const char *string) {
  const char *c;
  glRasterPos2f(x, y);
  for (c = string; *c != '\0'; c++) {
    glutBitmapCharacter(font, *c);
  }
}

void drawBox(float x, float y, float width, float height)
{
	glBegin(GL_LINE_LOOP);
	glVertex3f((GLfloat)(x + width), (GLfloat)(y - height), (GLfloat)0.0);
  	glVertex3f((GLfloat)(x + width), (GLfloat)(y + height), (GLfloat)0.0);
	glVertex3f((GLfloat)(x - width), (GLfloat)(y + height), (GLfloat)0.0);
	glVertex3f((GLfloat)(x - width), (GLfloat)(y - height), (GLfloat)0.0);
	glColor3f(0.0, 0.0, 0.0);
	glEnd();
}

void draw_node(float x, float y, int node_type, const char *leafName,
               int status) {
  void *font = GLUT_BITMAP_8_BY_13;
  float width = NODE_WIDTH, height = NODE_HEIGHT;

  std::string st(leafName, 0, MAX_STRING_CHAR);

  switch (node_type) {
  case BT::SELECTORSTAR:
    drawString(font, "?*", (x - width + WORD_OFFSET), (y - height / 2), 0);
    break;
  case BT::SEQUENCESTAR:
    drawString(font, ">*", (x - width + WORD_OFFSET), (y - height / 2), 0);
    break;

  case BT::SELECTOR:
    drawString(font, "?", (x - width + WORD_OFFSET), (y - height / 2), 0);
    break;
  case BT::SEQUENCE:
    drawString(font, ">", (x - width + WORD_OFFSET), (y - height / 2), 0);
    break;
  case BT::PARALLEL:
    drawString(font, "=", (x - width + WORD_OFFSET), (y - height / 2), 0);
    break;
  case BT::DECORATOR:
    drawString(font, "D", (x - width + WORD_OFFSET), (y - height / 2), 0);
    break;
  case BT::ACTION:
	for (int i = 0; i < st.size(); i++)
	  width += CHAR_DIM;

	renderBitmapString((x - width + WORD_OFFSET), (y - height / 2), font,
				   st.c_str());
    glColor3f(0.2, 1.0, 0.2);
    break;
  case BT::CONDITION:
	for (int i = 0; i < st.size(); i++)
	  width += CHAR_DIM;

	renderBitmapString((x - width + WORD_OFFSET), (y - height / 2), font,
				   st.c_str());

    break;
  default:
    break;
  }

  switch (status) {
  case RUNNING:
    glColor3f(0.8, 0.8, 0.8);
    break;
  case SUCCESS:
    glColor3f(0.0, 1.0, 0.0);
    break;
  case FAILURE:
    glColor3f(1.0, 0.0, 0.0);
    break;
  case IDLE:
    glColor3f(0.0, 0.0, 0.0);
    break;
  default:
    break;
  }

  drawBox(x, y, width, height);

}

// draw the edge connecting one node to the other
void draw_edge(GLfloat parent_x, GLfloat parent_y, GLfloat parent_size,
               GLfloat child_x, GLfloat child_y, GLfloat child_size) {
  glLineWidth(1.5);
  glColor3f(0.0, 0.0, 0.0);
  glBegin(GL_LINES);
  glVertex3f(parent_x, parent_y - parent_size, 0.0);
  glVertex3f(child_x, child_y + child_size, 0);
  glEnd();
}

// Keyboard callback function ( called on keyboard event handling )
void keyboard(unsigned char key, int x, int y) {
  if (key == 'q' || key == 'Q')
    exit(EXIT_SUCCESS);
}

void resize(int width, int height) {
  // we ignore the params and do:
  glutReshapeWindow(1024, 768);
}

void drawCircle(float radius) {
  glBegin(GL_LINE_LOOP);

  for (int i = 0; i <= 360; i++) {

    float degInRad = i * 3.14142 / 180;
    glVertex2f(cos(degInRad) * radius, sin(degInRad) * radius);
  }

  glEnd();
}

float x = 0.0;
float y = 0.4;
float x_offset = 0.04;
float r_color = 1;
float g_color = 1;
float b_color = 1;

int depth;

void updateTree(TreeNode *tree, GLfloat x_pos, GLfloat y_pos, GLfloat x_offset,
                GLfloat y_offset) {
  ControlNode *d = dynamic_cast<ControlNode *>(tree);
  if (d == NULL) { // if it is a leaf node, draw it

    draw_node((GLfloat)x_pos, (GLfloat)y_pos, tree->GetType(),
              tree->Name.c_str(), tree->ReadColorState());
  } else { // if it is a control flow node, draw it and its children
    draw_node((GLfloat)x_pos, (GLfloat)y_pos, tree->GetType(),
              tree->Name.c_str(), tree->ReadColorState());
    std::vector<TreeNode *> children = d->GetChildren();
    int M = d->GetChildrenNumber();
    for (int i = M - 1; i >= 0; i--) {
      updateTree(children[i], x_pos - x_offset * (M - 1) + 2 * x_offset * (i),
                 y_pos - y_offset, x_offset / 2.0, y_offset);
      draw_edge(x_pos, y_pos, 0.02,
                x_pos - x_offset * (M - 1) + 2 * x_offset * (i),
                y_pos - y_offset, 0.02);
    }
  }
}

void display() {
  if (draw)
  {
    glClearColor(r_color, g_color, b_color, 0.1);

    // clear the draw buffer .
    glClear(GL_COLOR_BUFFER_BIT); // Erase everything
    updateTree(tree, x, y, x_offset * pow(4, depth - 1), 0.1);
    glutSwapBuffers();
    glutPostRedisplay();
  }
}

void processSpecialKeys(int key, int xx, int yy) {

  float fraction = 0.005f;

  switch (key) {
  case GLUT_KEY_UP:
    y += fraction;
    break;
  case GLUT_KEY_DOWN:
    y -= fraction;
    break;
  case GLUT_KEY_LEFT:
    x -= fraction;
    break;
  case GLUT_KEY_RIGHT:
    x += fraction;
    break;
  case GLUT_KEY_PAGE_UP:
    fraction = 0.0001f;
    x_offset += fraction;
    break;
  case GLUT_KEY_PAGE_DOWN:
    fraction = 0.0001f;
    if (x_offset > fraction)
      x_offset -= fraction; // Avoid negative offset
    break;
  case GLUT_KEY_F1:
    if (r_color < 1)
      r_color += fraction;
    break;
  case GLUT_KEY_F2:
    if (r_color > 0)
      r_color -= fraction;
    break;
  case GLUT_KEY_F3:
    if (g_color < 1)
      g_color += fraction;
    break;
  case GLUT_KEY_F4:
    if (g_color > 0)
      g_color -= fraction;
    break;
  case GLUT_KEY_F5:
    if (b_color < 1)
      b_color += fraction;
    break;
  case GLUT_KEY_F6:
    if (b_color > 0)
      b_color -= fraction;
    break;
  }
}

void drawTree(ControlNode *tree_) {
  //***************************BT VISUALIZATION****************************
  int argc = 1;
  char *argv[1] = {(char *)"Something"};
  glutInit(&argc, argv); // Initialize GLUT

  tree = tree_;
  depth = tree->GetDepth();

  glutInitWindowSize(1024, 1024);

  glutCreateWindow("Behavior Tree"); // Create a window
  glutReshapeFunc(resize);
  glClearColor(0, 0.71, 0.00, 0.1);
  glutDisplayFunc(display); // Register display callback

  glutKeyboardFunc(keyboard);          // Register keyboard callback
  glutSpecialFunc(processSpecialKeys); // Register keyboard arrow callback

  glutMainLoop(); // Enter main event loop

  //***************************ENDOF BT VISUALIZATION
  //****************************
}

void enableDrawing()
{
  draw = true;
}

void disableDrawing()
{
  draw = false;
}
