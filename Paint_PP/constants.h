#ifndef CONSTANTS_H
#define CONSTANTS_H


/** Valores por defecto del Lienzo y algunas herramientas*/
const int DEFAULT_IMG_WIDTH = 640;
const int DEFAULT_IMG_HEIGHT = 480;
const int DEFAULT_PEN_THICKNESS = 1;
const int DEFAULT_ERASER_THICKNESS = 10;

/** Rango de los Sliders para configurar el grosor de los trazos */
const int MIN_PEN_SIZE = 1;
const int MAX_PEN_SIZE = 50;
const int MIN_RECT_CURVE = 0;
const int MAX_RECT_CURVE = 100;

/**Rango de los SpinBox que definen el tamaño del lienzo*/
const int MIN_IMG_WIDTH = 1;
const int MAX_IMG_WIDTH = 2560; // 2560x1440 limit due to undo/redo limitations
const int MIN_IMG_HEIGHT = 1;
const int MAX_IMG_HEIGHT = 1440;

/** Maximo de comandos "undo" y "redo" permitidos */
const int UNDO_LIMIT = 100;

enum ToolType {pencil, pen, eraser, shapes_tool};
enum LineStyle {solid, dashed, dotted, dash_dotted, dash_dot_dotted};
enum DrawType {single, poly};
enum ShapeType {rectangle, ellipse, triangle};
enum FillColor {foreground, background, no_fill};
enum BoundaryType {miter_join, bevel_join, round_join};

#endif // CONSTANTS_H
