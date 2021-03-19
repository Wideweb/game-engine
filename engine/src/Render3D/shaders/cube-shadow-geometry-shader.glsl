#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 18) out;

uniform mat4 CubeMatrices[6];
// переменная FragPos вычисляется в геометрическом шейдере
// и выдается для каждого вызова EmitVertex()
out vec4 FragPos;

void main() {
    for (int face = 0; face < 6; ++face) {
        // встроенная переменная, определяющая в какую
        // грань кубической карты идет рендер
        gl_Layer = face;
        for (int i = 0; i < 3; ++i) // цикл по всем вершинам треугольника
        {
            FragPos = gl_in[i].gl_Position;
            gl_Position = CubeMatrices[face] * FragPos;
            EmitVertex();
        }
        EndPrimitive();
    }
}